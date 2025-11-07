/*
 * StorageUSB.h
 *
 *  Created on: 17-Apr-2023
 *      Author: developer
 */

#ifndef INCLUDES_APRA_UTILS_STORAGEUSB_H_
#define INCLUDES_APRA_UTILS_STORAGEUSB_H_
#include <string>
#include <vector>
#include <constants/StorageType.h>
#include <models/StorageMinimalInfo.h>
#include <constants/StorageState.h>

namespace apra
{

class StorageUSB
{
public:
	StorageUSB(std::string mountPath, std::vector<STORAGE_TYPE> supportedTypes,
			bool shouldPrint, bool skipMount = false);
	virtual ~StorageUSB();
	virtual string insertCheck();
	virtual string mountDevice();
	virtual bool ejectDevice();
	virtual bool isUnsafeEject();
	bool getStorageInfo(uint64_t &freeSpaceInMB, uint64_t &totalCapacityInMB);
	STORAGE_STATE getStatus();
	std::string getMountPath();
	struct udev_device* getChildDevice(struct udev *udev,
			struct udev_device *parent, const char *subsystem);
protected:
	string findMountDeviceBylsblk(string devicePartitionNode);
	string findMountedDevice(string devicePartitionNode);
	std::string enumerateDevices(struct udev *udev);
	std::vector<StorageMinimalInfo> getPartitions(std::string devpath);
	bool mountDeviceNode(string deviceNode);
	bool mountUSBDevice(StorageMinimalInfo storageDevice,
			uint8_t retryLimit = 3);
	bool unMountUSBDevice();
	bool unMountWithPrivilege();
	bool unMountWithoutPrivilege();
	bool mountWithPrivilege(StorageMinimalInfo storageDevice,
			uint8_t retryLimit = 3);
	bool mountWithoutPrivilege(StorageMinimalInfo storageDevice);
	StorageMinimalInfo getHighCapacityPartition(std::string deviceNode);
	bool isDeviceNodeConnected();
	void checkDeviceNode();

	bool m_shouldPrint;
	std::vector<STORAGE_TYPE> m_supportedTypes;
	std::string m_mountPoint;
	std::string m_deviceNode;
	std::string m_partitionNode;
	bool m_skipMount;
	STORAGE_STATE m_state;
	std::string m_manualPath;
};

} /* namespace apra */

#endif /* INCLUDES_APRA_UTILS_STORAGEUSB_H_ */
