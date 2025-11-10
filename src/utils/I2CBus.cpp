/*
 * I2CBus.cpp
 *
 *  Created on: 05-Feb-2023
 *      Author: developer
 */

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include "models/I2CError.h"
#include "utils/Utils.h"
#include "utils/Macro.h"

#include "utils/I2CBus.h"
using namespace apra;

I2C_Bus::I2C_Bus(string i2cPath, bool shouldPrint) :
		m_i2cPath(i2cPath), m_shouldPrint(shouldPrint), m_i2cFileDescriptor(-1), m_registerSize(
				1), m_dataSize(1), m_lastI2COperationTs(0)
{
	// TODO Auto-generated constructor stub

}

I2C_Bus::~I2C_Bus()
{
	// TODO Auto-generated destructor stub
}

bool I2C_Bus::isI2CExecRecommended()
{
	MONOCURRTIME(timeNow);
	return !((timeNow - m_lastI2COperationTs) > CONSEQUENT_I2C_TIME_LIMIT_US);
}

void I2C_Bus::setSize(uint8_t registerSize, uint8_t dataSize)
{
	m_registerSize = registerSize;
	m_dataSize = dataSize;
}

I2CError I2C_Bus::openBus()
{
	I2CError error;
#if defined(__arm__) || defined(__aarch64__)
	m_i2cFileDescriptor = open(m_i2cPath.c_str(), O_RDWR);
	if (m_i2cFileDescriptor < 0)
	{
		m_i2cFileDescriptor = -1;
		if (m_shouldPrint)
		{
			char err[200];
			sprintf(err, "open('%s') in i2c_init", m_i2cPath.c_str());
			error = I2CError(err, OPEN_BUS_ERROR);
			perror(err);
		}
	}
#endif
	return error;
}

void I2C_Bus::closeBus()
{
#if defined(__arm__) || defined(__aarch64__)
	if (m_i2cFileDescriptor > -1)
	{
		close(m_i2cFileDescriptor);
		m_i2cFileDescriptor = -1;
	}
#endif
}

I2CError I2C_Bus::genericWrite(uint8_t chipAddress,
		vector<uint8_t> registerAddress, vector<uint8_t> data)
{
	I2CError error;
#if defined(__arm__) || defined(__aarch64__)
	if (m_i2cFileDescriptor > -1)
	{
		vector<uint8_t> i2cBytes = registerAddress;
		i2cBytes.insert(i2cBytes.end(), data.begin(), data.end());
		struct i2c_msg msgs[1];
		struct i2c_rdwr_ioctl_data msgset[1];
		msgs[0].addr = chipAddress;
		msgs[0].flags = 0;
		msgs[0].len = i2cBytes.size();
		msgs[0].buf = i2cBytes.data();
		msgset[0].msgs = msgs;
		msgset[0].nmsgs = 1;
		string debugString(__func__);
		debugString += " , 0x";
		for (uint32_t count = 0; count < registerAddress.size(); count++)
		{
			char regCh[5] =
			{	0};
			sprintf(regCh, "%02x", registerAddress[count]);
			debugString += string(regCh);
		}
		debugString += " <--> 0x";
		for (uint32_t count = 0; count < data.size(); count++)
		{
			char dataCh[5] =
			{	0};
			sprintf(dataCh, "%02x", data[count]);
			debugString += string(dataCh);
		}
		debugString += "\n";
		if (m_shouldPrint)
		{
			printf("%s", debugString.c_str());
		}
		if (ioctl(m_i2cFileDescriptor, I2C_RDWR, &msgset) < 0)
		{
			error = I2CError("ioctl(I2C_RDWR) in i2c_write", debugString,
					WRITE_ERROR);
			if(m_shouldPrint)
			{
				perror(error.getMessage().c_str());}
		}
		else
		{
			MONOTIMEUS(m_lastI2COperationTs);
		}
	}
	else
	{
		error = I2CError("I2C bus is not opened yet");
	}
#endif
	return error;

}
I2CError I2C_Bus::genericRead(uint8_t chipAddress,
		vector<uint8_t> registerAddress, vector<uint8_t> &readData)
{
	I2CError error;
	string debugString(__func__);
#if defined(__arm__) || defined(__aarch64__)
	uint8_t *readBytes = (uint8_t*) calloc((m_dataSize + 1), 1);
	if (readBytes == NULL)
	{
		error = I2CError("Unable to allocate memory for i2c read");
		if (m_shouldPrint)
		{
			printf("%s\n", error.getMessage().c_str());
		}
	}
	else
	{
		vector<uint8_t> i2cBytes = registerAddress;
		struct i2c_msg msgs[2];
		struct i2c_rdwr_ioctl_data msgset[1];
		msgs[0].addr = chipAddress;
		msgs[0].flags = 0;
		msgs[0].len = i2cBytes.size();
		msgs[0].buf = i2cBytes.data();
		msgs[1].addr = chipAddress;
		msgs[1].flags = I2C_M_RD;
		msgs[1].len = m_dataSize;
		msgs[1].buf = readBytes;
		msgset[0].msgs = msgs;
		msgset[0].nmsgs = 2;
		debugString += " , 0x";
		for (uint32_t count = 0; count < registerAddress.size(); count++)
		{
			char regCh[5] =
			{	0};
			sprintf(regCh, "%02x", registerAddress[count]);
			debugString += string(regCh);
		}
		if (ioctl(m_i2cFileDescriptor, I2C_RDWR, &msgset) < 0 )
		{
			debugString += "\n";
			error = I2CError("ioctl(I2C_RDWR) in i2c_read", debugString,
					READ_ERROR);
			if(m_shouldPrint)
			{
				perror(error.getMessage().c_str());}
		}
		else
		{
			MONOTIMEUS(m_lastI2COperationTs);
			readData.clear();
			for (uint16_t count = 0; count < m_dataSize; ++count)
			{
				readData.push_back(readBytes[count]);
			}
		}
	}
	if (readBytes != NULL)
	{
		free(readBytes);
		readBytes = NULL;
	}

	if (m_shouldPrint)
	{
		printf("%s <--> 0x", debugString.c_str());
		for (uint32_t count = 0; count < readData.size(); count++)
		{
			printf("%02x", readData[count]);
		}
		printf("\n");
	}
#endif
	return error;
}

I2CError I2C_Bus::writeOnce(uint8_t chipAddress, uint64_t registerAddress,
		uint64_t data)
{
	I2CError error = openBus();
	if (!error.isError())
	{
		error = apra::I2C_Bus::writeOnI2C(chipAddress, registerAddress, data);
		closeBus();
	}
	return error;
}

I2CError I2C_Bus::readOnce(uint8_t chipAddress, uint64_t registerAddress,
		uint64_t &data)
{
	I2CError error = openBus();
	if (!error.isError())
	{
		error = apra::I2C_Bus::readOnI2C(chipAddress, registerAddress, data);
		closeBus();
	}
	return error;
}

I2CError I2C_Bus::writeOnI2C(uint8_t chipAddress, uint64_t registerAddress,
		uint64_t data)
{
	I2CError error;
	vector<uint8_t> registerArray = Utils::extractBytes(registerAddress,
			m_registerSize);
	vector<uint8_t> dataArray = Utils::extractBytes(data, m_dataSize);
#if defined(__arm__) || defined(__aarch64__)
	if (m_i2cFileDescriptor > -1)
	{
		error = genericWrite(chipAddress, registerArray, dataArray);
	}
	else
	{
		error = I2CError("I2C bus is not opened yet", BUS_UNOPENED);
	}
#endif
	return error;
}

I2CError I2C_Bus::readOnI2C(uint8_t chipAddress, uint64_t registerAddress,
		uint64_t &data)
{
	I2CError error;
	vector<uint8_t> registerArray = Utils::extractBytes(registerAddress,
			m_registerSize);
	vector<uint8_t> dataArray;
#if defined(__arm__) || defined(__aarch64__)
	if (m_i2cFileDescriptor > -1)
	{
		error = genericRead(chipAddress, registerArray, dataArray);
		if (!error.isError())
		{
			data = Utils::combineBytes(dataArray);
		}
	}
	else
	{
		error = I2CError("I2C bus is not opened yet", BUS_UNOPENED);
	}
#endif
	return error;
}
