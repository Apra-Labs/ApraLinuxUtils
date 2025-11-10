/*
 * GPIO.h
 *
 *  Created on: Jul 29, 2020
 *      Author: developer
 */

#ifndef INCLUDES_APRA_GPIO_H_
#define INCLUDES_APRA_GPIO_H_

#include <sys/types.h>
#include <string>

#define SYS_GPIO_PATH "/sys/class/gpio"

enum GPIO_EDGES
{
	NONE, RISING, FALLING, BOTH
};

namespace apra
{
class GPIO
{
public:
	GPIO(uint gpioNo);
	virtual ~GPIO();
	bool Init(bool isRead);
	bool Init4EdgeInterrupt(bool isRead, GPIO_EDGES edge);
	bool UnInit();
	int Open();
	bool Close();
	int Read();
	int ReadWithInterrupt(unsigned long uSecTout);
	bool Write(bool makeHigh);
	int& GetGPIODescriptor();
	static int ReadOnce(uint gpioNo);
	static bool WriteOnce(uint gpioNo, bool makeHigh);
private:
	bool Export();
	bool SetDirection(bool isRead);
	bool SetGPIOEdgeEvent(GPIO_EDGES edge);
	bool UnExport();
	std::string GPIO_EDGES_STR(GPIO_EDGES edge);

	int m_fd;
	uint m_gpio;
	bool m_isRead;
};
}

#endif /* INCLUDES_APRA_GPIO_H_ */
