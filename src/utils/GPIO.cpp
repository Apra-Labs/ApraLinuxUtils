/*
 * GPIO.cpp
 *
 *  Created on: Jul 29, 2020
 *      Author: developer
 */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <linux/input.h>
#include "utils/GPIO.h"
#include <poll.h>

#define GPIO_TOUT_USEC 100000
using namespace apra;

GPIO::GPIO(uint gpioNo) :
		m_fd(-1), m_gpio(gpioNo), m_isRead(false)
{
	// TODO Auto-generated constructor stub
}

GPIO::~GPIO()
{
	// TODO Auto-generated destructor stub
}

std::string GPIO::GPIO_EDGES_STR(GPIO_EDGES edge)
{
	switch (edge)
	{
	case NONE:
		return "none";
	case RISING:
		return "rising";
	case FALLING:
		return "falling";
	case BOTH:
		return "both";
	default:
		break;
	}
	return "";
}

bool GPIO::Init(bool isRead)
{
	m_isRead = isRead;
	if (!Export())
	{
		//		return false;
	}
	return SetDirection(isRead);
}

bool GPIO::Init4EdgeInterrupt(bool isRead, GPIO_EDGES edge)
{
	m_isRead = isRead;
	if (Init(isRead))
	{
		return SetGPIOEdgeEvent(edge);
	}
	return false;
}

bool GPIO::UnInit()
{
	return UnExport();
}

int GPIO::Open()
{
	char buff[64] = { 0 };
	snprintf(buff, sizeof(buff), SYS_GPIO_PATH "/gpio%d/value", m_gpio);

	m_fd = open(buff, (m_isRead ? (O_RDONLY | O_NONBLOCK) : O_WRONLY));
	if (m_fd < 0)
	{
		printf("Error opening gpio");
		return -1;
	}
	return m_fd;
}

bool GPIO::Close()
{
	if (m_fd)
		return close(m_fd);
	return true;
}

bool GPIO::Export()
{
	int fd, length;
	char buff[64] = { 0 };

	fd = open(SYS_GPIO_PATH "/export", O_WRONLY);
	if (fd < 0)
	{
		printf("unable to open export for gpio %d\n", m_gpio);
		return false;
	}

	length = snprintf(buff, sizeof(buff), "%d", m_gpio);
	if (write(fd, buff, length) != length)
	{
		close(fd);
		printf("unable to init gpio %d\n", m_gpio);
		return false;
	}
	close(fd);
	usleep(GPIO_TOUT_USEC);
	return true;
}

bool GPIO::UnExport()
{
	int fd, length;
	char buff[64] = { 0 };

	fd = open(SYS_GPIO_PATH "/unexport", O_WRONLY);
	if (fd < 0)
	{
		printf("unable to open gpio %d for unexport\n", m_gpio);
		return false;
	}

	length = snprintf(buff, sizeof(buff), "%d", m_gpio);
	if (write(fd, buff, length) != length)
	{
		close(fd);
		printf("unable to unexport gpio %d\n", m_gpio);
		return false;
	}
	close(fd);
	return true;
}

bool GPIO::SetDirection(bool isRead)
{
	int fd;
	char buff[64] = { 0 };

	snprintf(buff, sizeof(buff),
	SYS_GPIO_PATH "/gpio%d/direction", m_gpio);

	fd = open(buff, O_WRONLY);
	if (fd < 0)
	{
		printf("SetDirection unable to open gpio%d", m_gpio);
		return false;
	}

	if (!isRead)
	{
		if (write(fd, "out", 4) != 4)
		{
			close(fd);
			printf("unable to make gpio%d as output\n", m_gpio);
			return false;
		}
	}
	else
	{
		if (write(fd, "in", 3) != 3)
		{
			close(fd);
			printf("unable to make gpio%d as input\n", m_gpio);
			return false;
		}
	}
	close(fd);
	return true;
}
bool GPIO::SetGPIOEdgeEvent(GPIO_EDGES edge)
{
	int fd;
	char buff[64] = { 0 };

	snprintf(buff, sizeof(buff), SYS_GPIO_PATH "/gpio%d/edge", m_gpio);

	fd = open(buff, O_WRONLY);
	if (fd < 0)
	{
		printf("Unable to open gpio%d/edge\n", m_gpio);
		return false;
	}
	std::string edgeStr = GPIO_EDGES_STR(edge);
	if (write(fd, edgeStr.c_str(), edgeStr.length() + 1)
			!= ((int) (edgeStr.length() + 1)))
	{
		printf("Error setting edge to \"%s\"\n", edgeStr.c_str());
		return false;
	}
	close(fd);
	return true;
}

int GPIO::Read()
{
	if (!m_fd)
	{
		return -1;
	}
	char ch;
	lseek(m_fd, 0, SEEK_SET);
	if (read(m_fd, &ch, 1) != 1)
	{
		printf("Error fetching GPIO value\n");
		return -1;
	}

	if (ch != '0')
	{
		return 1;
	}
	return 0;
}

bool GPIO::Write(bool makeHigh)
{
	if (!m_fd)
	{
		return false;
	}
	std::string state = makeHigh ? "1" : "0";
	if (write(m_fd, state.c_str(), 2) != 2)
	{
		printf("Error setting GPIO state %s\n", state.c_str());
		return false;
	}
	return true;
}

int GPIO::ReadOnce(uint gpioNo)
{
	GPIO gpio(gpioNo);
	if (!gpio.Init(true))
	{
		return -1;
	}
	gpio.Open();
	int data = gpio.Read();
	gpio.Close();
	gpio.UnInit();
	return data;
}
bool GPIO::WriteOnce(uint gpioNo, bool makeHigh)
{
	GPIO gpio(gpioNo);
	if (!gpio.Init(true))
	{
		return -1;
	}
	gpio.Open();
	bool response = gpio.Write(makeHigh);
	gpio.Close();
	gpio.UnInit();
	return response;
}
int& GPIO::GetGPIODescriptor()
{
	return m_fd;
}

int GPIO::ReadWithInterrupt(unsigned long uSecTout)
{
	struct pollfd fdset;
	fdset.fd = m_fd;
	fdset.events = POLLPRI;

	auto rc = poll(&fdset, 1, uSecTout);
	if (rc < 0)
	{
		printf("\npoll() failed!\n");
		return -1;
	}
	else if (rc == 0)
	{
		printf("timeout \n");
		return -1;
	}

	if (fdset.revents & POLLPRI)
	{
		return Read();
	}

	return -1;
}
