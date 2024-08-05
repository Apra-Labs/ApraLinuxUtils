/*
 * PWM.cpp
 *
 *  Created on: 20-Oct-2023
 *      Author: developer
 */

#include <stddef.h>
#include <stdio.h>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <utils/PWM.h>
#include <inttypes.h>
#include "ApraUtils.h"

#define PWM_TOUT_USEC 10000
#define PWM_EXPORT "export"
#define PWM_UNEXPORT "unexport"
#define PWM_PERIOD "period"
#define PWM_DUTYCYCLE "duty_cycle"
#define PWM_ENABLE "enable"

using namespace std;
namespace apra
{

PWM::PWM(uint32_t pwmChipNo, uint32_t pwmPinNo, bool shouldPrint) :
		m_shouldPrint(shouldPrint), m_chipNo(pwmChipNo), m_pinNo(pwmPinNo), m_nSecPeriod(
				0), m_nSecDutyCycle(0), m_chipPath(SYS_PWM_PATH), m_isSetupComplete(
				false), m_isPWMRunning(false)
{
	// TODO Auto-generated constructor stub
	m_chipPath += "/pwmchip" + to_string(m_chipNo);
	if (!Utils::directoryExists(m_chipPath))
	{
		string error = "PWM Chip number is invalid: " + to_string(m_chipNo);
		printf("%s\n", error.c_str());
		throw std::invalid_argument(error);
	}
}

PWM::~PWM()
{
	// TODO Auto-generated destructor stub
	destroy();
}

bool PWM::setup(uint64_t nSecPeriod, uint64_t nSecDutyCycle)
{
	if (nSecDutyCycle > nSecPeriod)
	{
		throw std::invalid_argument(
				"Duty cycle time cannot be more than period");
		return false;
	}
	m_nSecPeriod = nSecPeriod;
	m_nSecDutyCycle = nSecDutyCycle;
	bool didFail = false;
	if (m_isPWMRunning)
	{
		didFail = stop();
		if (m_shouldPrint && didFail)
		{
			printf("stop failed\n");
		}
	}
	if (m_isSetupComplete && !didFail)
	{
		didFail = !updatePWMParams(0, PWM_DUTYCYCLE);
		if (m_shouldPrint && didFail)
		{
			printf("set duty cycle failed\n");
		}
		if (!didFail)
		{
			didFail = !updatePWMParams(0, PWM_PERIOD);
			if (m_shouldPrint && didFail)
			{
				printf("set period failed\n");
			}
		}
	}
	if (!m_isSetupComplete && !didFail)
	{
		didFail = !Export();
		if (m_shouldPrint && didFail)
		{
			printf("export failed\n");
		}
	}
	if (!didFail)
	{
		didFail = !updatePWMParams(nSecPeriod, PWM_PERIOD);
		if (m_shouldPrint && didFail)
		{
			printf("set period failed\n");
		}
	}
	if (!didFail)
	{
		didFail = !updatePWMParams(nSecDutyCycle, PWM_DUTYCYCLE);
		if (m_shouldPrint && didFail)
		{
			printf("set duty cycle failed\n");
		}
	}
	m_isSetupComplete = !didFail;
	if (!m_isSetupComplete)
	{
		bool didSucceed = UnExport();
		if (!didSucceed && m_shouldPrint)
		{
			printf("unexport failed\n");
		}
	}
	return !didFail;
}

bool PWM::destroy()
{
	bool didFail = false;
	if (m_isPWMRunning)
	{
		didFail = !stop();
		m_isPWMRunning = didFail;
	}
	if (m_isSetupComplete)
	{
		didFail = !UnExport();
		m_isSetupComplete = didFail;
	}
	return !didFail;
}

bool PWM::start()
{
	bool didFail = false;
	if (m_isSetupComplete)
	{
		didFail = !updatePWMParams(1, PWM_ENABLE);
		m_isPWMRunning = !didFail;
	}
	return !didFail;
}

bool PWM::stop()
{
	bool didFail = false;
	if (m_isSetupComplete && m_isPWMRunning)
	{
		didFail = !updatePWMParams(0, PWM_ENABLE);
		m_isPWMRunning = didFail;
	}
	return !didFail;
}

bool PWM::updateDutyCycle(uint64_t nSecDutyCycle)
{
	m_nSecDutyCycle = nSecDutyCycle;
	m_percentDutyCycle = (m_nSecDutyCycle / m_nSecPeriod) * 100;
	return updatePWMParams(nSecDutyCycle, PWM_DUTYCYCLE);
}

bool PWM::changeDutyCycle(uint32_t percent)
{
	uint64_t dutyCycle = (m_nSecPeriod * percent) / 100;
	m_nSecDutyCycle = dutyCycle;
	return updatePWMParams(dutyCycle, PWM_DUTYCYCLE);
}

uint32_t PWM::getDutyCyclePercent()
{
	return m_percentDutyCycle;
}

uint64_t PWM::getDutyCycleInNSec()
{
	return m_nSecDutyCycle;
}

std::string PWM::getPWMPinPath()
{
	return m_chipPath + "/pwm" + to_string(m_pinNo);
}

bool PWM::updatePWMParams(uint64_t value, std::string params)
{
	string pwmPinPath = getPWMPinPath();
	if (!Utils::directoryExists(pwmPinPath))
	{
		if (m_shouldPrint)
		{
			printf("%s doesnt exist\n", pwmPinPath.c_str());
		}
		string log(
				"Seems like the pwm(" + to_string(m_pinNo)
						+ ") setup is not done properly.");
		printf("%s\n", log.c_str());
		return false;
	}
	int fd, length;
	char buff[64] = { 0 };

	fd = open((pwmPinPath + "/" + params).c_str(), O_WRONLY);
	if (fd < 0)
	{
		printf("unable to open %s to set %" PRIu64 "\n", params.c_str(), value);
		return false;
	}

	length = snprintf(buff, sizeof(buff), "%" PRIu64 "", value);
	if (write(fd, buff, length) != length)
	{
		close(fd);
		printf("unable to write %" PRIu64 " on %s\n", value, params.c_str());
		return false;
	}
	close(fd);
	usleep(PWM_TOUT_USEC);
	return true;
}

bool PWM::Export()
{
	int fd, length;
	char buff[64] = { 0 };
	string pwmPath = m_chipPath + "/" + PWM_EXPORT;
	if (m_shouldPrint)
	{
		printf("export path %s\n", pwmPath.c_str());
	}
	if (!Utils::fileExists(pwmPath))
	{
		printf("%s not found\n", pwmPath.c_str());
		return false;
	}
	fd = open(pwmPath.c_str(), O_WRONLY);
	if (fd < 0)
	{
		printf("unable to open %s for pwm %" PRIu32 "\n", pwmPath.c_str(),
				m_pinNo);
		return false;
	}

	length = snprintf(buff, sizeof(buff), "%" PRIu32 "", m_pinNo);
	if (m_shouldPrint)
	{
		printf("write on export %s length %d\n", buff, length);
	}
	if (write(fd, buff, length) != length)
	{
		close(fd);
		printf("unable to init pwm %" PRIu32 "\n", m_pinNo);
		return false;
	}
	close(fd);
	usleep(PWM_TOUT_USEC);
	if (m_shouldPrint)
	{
		printf("export write succeed\n");
	}
	if (!Utils::directoryExists(getPWMPinPath()))
	{
		printf("unable to access pwm %" PRIu32 "\n", m_pinNo);
		return false;
	}
	return true;
}

bool PWM::UnExport()
{
	int fd, length;
	char buff[64] = { 0 };

	fd = open((m_chipPath + "/" + PWM_UNEXPORT).c_str(), O_WRONLY);
	if (fd < 0)
	{
		printf("unable to open pwm %" PRIu32 " for %s\n", m_pinNo,
		PWM_UNEXPORT);
		return false;
	}

	length = snprintf(buff, sizeof(buff), "%" PRIu32 "", m_pinNo);
	if (write(fd, buff, length) != length)
	{
		close(fd);
		printf("unable to %s gpio %" PRIu32 "\n", PWM_UNEXPORT, m_pinNo);
		return false;
	}
	close(fd);
	if (Utils::directoryExists(getPWMPinPath()))
	{
		printf("unable to close pwm %" PRIu32 "\n", m_pinNo);
		return false;
	}
	return true;
}

} /* namespace apra */
