/*
 * PWM.h
 *
 *  Created on: 20-Oct-2023
 *      Author: developer
 */

#ifndef INCLUDES_APRA_UTILS_PWM_H_
#define INCLUDES_APRA_UTILS_PWM_H_

#include <stdint.h>
#include <string>

#define SYS_PWM_PATH "/sys/class/pwm"

namespace apra
{

class PWM
{
public:
	PWM(uint32_t pwmChipNo, uint32_t pwmPinNo, bool shouldPrint);
	virtual ~PWM();
	bool setup(uint64_t nSecPeriod, uint64_t nSecDutyCycle);
	bool destroy();
	bool start();
	bool stop();
	bool updateDutyCycle(uint64_t nSecDutyCycle);
	bool changeDutyCycle(uint32_t percent);
	uint32_t getDutyCyclePercent();
	uint64_t getDutyCycleInNSec();
private:
	bool Export();
	bool UnExport();
	bool updatePWMParams(uint64_t value, std::string params);
	std::string getPWMPinPath();

	bool m_shouldPrint;
	uint32_t m_chipNo;
	uint32_t m_pinNo;
	uint64_t m_nSecPeriod;
	uint64_t m_nSecDutyCycle;
	std::string m_chipPath;
	bool m_isSetupComplete;
	bool m_isPWMRunning;
	uint32_t m_percentDutyCycle;
};

} /* namespace apra */

#endif /* INCLUDES_APRA_UTILS_PWM_H_ */
