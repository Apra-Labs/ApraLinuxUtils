/*
 * StorageUSB.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */
#include <fcntl.h>
#include <inttypes.h>
#include <libudev.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <err.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/statvfs.h>
#include "utils/Utils.h"
#include <utils/StorageUSB.h>
#include "utils/Macro.h"
#include <algorithm>
#include <iostream>
#include <algorithm>
#include <iostream>

#define MAX_BUF_LEN 1024

using namespace std;
namespace apra
{

StorageUSB::StorageUSB(string mountPath, vector<STORAGE_TYPE> supportedTypes,
		bool shouldPrint, bool skipMount) :
		m_shouldPrint(shouldPrint), m_supportedTypes(supportedTypes), m_mountPoint(
				mountPath), m_deviceNode(), m_partitionNode(), m_skipMount(
				skipMount), m_state(STORAGE_SAFE_EJECT), m_manualPath(mountPath), m_retryCount(3)
{
	string error;
	if (m_supportedTypes.empty())
	{
		error = "Supported filesytem type(supportedTypes) parameter is empty!";
		throw std::invalid_argument(error.c_str());
	}
	if (!skipMount)
	{
		if (m_mountPoint.empty())
		{
			error = "Path to mount(mountPath) parameter is empty!";
			throw std::invalid_argument(error.c_str());
		}
	}
}

StorageUSB::~StorageUSB()
{
}

STORAGE_STATE StorageUSB::getStatus()
{
	return m_state;
}

std::string StorageUSB::getMountPath()
{
	string mountPath;
	if (m_state == STORAGE_MOUNTED)
	{
		mountPath = m_mountPoint;
	}
	return mountPath;
}

struct udev_device* StorageUSB::getChildDevice(struct udev *udev,
		struct udev_device *parent, const char *subsystem)
{
	struct udev_device *child = NULL;
	struct udev_enumerate *enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_parent(enumerate, parent);
	udev_enumerate_add_match_subsystem(enumerate, subsystem);
	udev_enumerate_scan_devices(enumerate);
	struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry *entry;
	udev_list_entry_foreach(entry, devices)
	{
		const char *path = udev_list_entry_get_name(entry);
		child = udev_device_new_from_syspath(udev, path);
		break;
	}
	udev_enumerate_unref(enumerate);
	return child;
}

string StorageUSB::enumerateDevices(struct udev *udev)
{
	string deviceID;
	struct udev_enumerate *enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "scsi");
	udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
	udev_enumerate_scan_devices(enumerate);
	struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry *entry;

	udev_list_entry_foreach(entry, devices)
	{
		if (deviceID.empty())
		{
			const char *path = udev_list_entry_get_name(entry);
			struct udev_device *scsi = udev_device_new_from_syspath(udev, path);
			struct udev_device *block = getChildDevice(udev, scsi, "block");
			struct udev_device *scsiDisk = getChildDevice(udev, scsi,
					"scsi_disk");

			struct udev_device *usb =
					udev_device_get_parent_with_subsystem_devtype(scsi, "usb",
							"usb_device");
			if (block && scsiDisk && usb)
			{
				deviceID.assign(udev_device_get_devnode(block));
			}
			if (block)
			{
				udev_device_unref(block);
			}
			if (scsiDisk)
			{
				udev_device_unref(scsiDisk);
			}
			udev_device_unref(scsi);
		}
	}
	udev_enumerate_unref(enumerate);
	return deviceID;
}

string StorageUSB::insertCheck()
{
	if (m_state == STORAGE_UNSAFE_EJECT || m_state == STORAGE_SAFE_EJECT)
	{
		struct udev *udev = udev_new();
		string devicePath = enumerateDevices(udev);
		udev_unref(udev);
		if (devicePath.length())
		{
			m_state = STORAGE_INSERTED;
			m_deviceNode = devicePath;
		}
		if (m_shouldPrint)
		{
			std::cout << "Device Node is ==========>>>" << m_deviceNode << std::endl;
		}
		return devicePath;
	}
	return m_deviceNode;
}

string StorageUSB::mountDevice()
{
	string usbMountPath;
	string devNode = insertCheck();
	bool isSupportedFs = false;
	if (m_skipMount)
	{
		StorageMinimalInfo highPartition = getHighCapacityPartition(devNode);
		if (highPartition.m_size > 0)
		{
			usbMountPath = findMountDeviceBylsblk(highPartition.m_partition);
			m_mountPoint = usbMountPath;
			m_partitionNode = highPartition.m_partition;
		}
		STORAGE_TYPE type = STORAGE_TYPE_STRING::getEnum(highPartition.m_fsType);
		auto it = std::find(m_supportedTypes.begin(),
						m_supportedTypes.end(),
						type);
		isSupportedFs = (it != m_supportedTypes.end());
		m_retryCount--;				
	}
	else
	{
		if (mountDeviceNode(devNode))
		{
			usbMountPath = m_mountPoint;
		}
	}
	if (!isSupportedFs)
	{
		m_state = STORAGE_INSERTED_UNMOUNTED;
	}
	else if (!usbMountPath.empty())
	{
		m_state = STORAGE_MOUNTED;
	}
	else if (!isSupportedFs && !m_retryCount)
	{
		m_state = STORAGE_INSERTED_UNMOUNTED;
		m_retryCount = 0;
	}
	else 
	{
		if (m_shouldPrint)
		{
			printf("USB mount path is empty\n");
		}
	}
	return usbMountPath;
}

bool StorageUSB::isUnsafeEject()
{
	if (m_state == STORAGE_UNSAFE_EJECT)
	{
		return true;
	}
	if (m_state != STORAGE_MOUNTED)
	{
		return false;
	}
	/*	struct statvfs stat;
	 int64_t status = statvfs(m_mountPoint.c_str(), &stat);
	 if (m_shouldPrint || true)
	 {
	 printf("statvfs response = %" PRId64 " id=%lu\n", status, stat.f_fsid);
	 }
	 if (status != 0)
	 {
	 m_state = UNSAFE_EJECT;
	 return true;
	 }*/
	if (findMountDeviceBylsblk(m_partitionNode).empty())
	{
		m_state = STORAGE_UNSAFE_EJECT;
		m_deviceNode.clear();
		m_retryCount = 3;
		return true;
	}
	return false;

}

StorageMinimalInfo StorageUSB::getHighCapacityPartition(std::string deviceNode)
{
	StorageMinimalInfo highCapacityPartition;
	if (!deviceNode.empty())
	{
		vector<StorageMinimalInfo> partitions = getPartitions(deviceNode);
		for (uint32_t count = 0; count < partitions.size(); count++)
		{
			if (highCapacityPartition.m_size < partitions[count].m_size)
			{
				highCapacityPartition = partitions[count];
			}
		}
	}
	if (m_shouldPrint)
	{
		std::cout << "High Capacity Partition is " << highCapacityPartition.m_partition << " == " << highCapacityPartition.m_fsType << " === " << highCapacityPartition.m_size << std::endl;
	}
	return highCapacityPartition;
}

bool StorageUSB::isDeviceNodeConnected()
{
	int fd = open(m_mountPoint.c_str(), O_RDONLY);

	if (fd == -1)
	{
		// Error occurred, USB device is likely not connected
		return false;
	}

	close(fd);
	return true;
}

vector<StorageMinimalInfo> StorageUSB::getPartitions(string devpath)
{
	std::vector<StorageMinimalInfo> partitions;

	string listPartitionCommand =
			"lsblk " + devpath
					+ " -b --noheadings --raw -o NAME,SIZE,FSTYPE | awk '$1~/.*[0-9]+$/ && $7==\"\"'";
	string cmdResponse;
	try
	{
		cmdResponse = apra::Utils::exec(listPartitionCommand, m_shouldPrint);
	} catch (std::runtime_error &e)
	{
		printf("list partition error: %s\n", e.what());
		return partitions;
	}
	if (cmdResponse.empty())
	{
		return partitions;
	}
	if (Utils::caseInsensitiveSearch(cmdResponse, "not a block device"))
	{
		return partitions;
	}

	istringstream issResponse(cmdResponse);
	string line;
	while (getline(issResponse, line))
	{
		char name[MAX_BUF_LEN] = { 0 }, size[MAX_BUF_LEN] = { 0 },
				fstype[MAX_BUF_LEN] = { 0 };
		int ret = sscanf(line.c_str(), "%s %s %s", name, size, fstype);
		if (ret != 3)
		{
			continue;
		}
		// Add partition to the vector
		partitions.push_back(
				StorageMinimalInfo("/dev/" + string(name), std::stoull(size),
						fstype));
	}

	return partitions;
}

bool StorageUSB::mountUSBDevice(StorageMinimalInfo storageDevice,
		uint8_t retryLimit)
{
	string storageMountCheck = findMountDeviceBylsblk(
			storageDevice.m_partition);
	if (storageMountCheck.length())
	{
		m_mountPoint = storageMountCheck;
		m_partitionNode = storageDevice.m_partition;
		unMountUSBDevice();
	}

	bool mountStatus = mountWithPrivilege(storageDevice, retryLimit);
	if (mountStatus)
	{
		m_mountPoint = m_manualPath;
	}
	else
	{
		if (m_shouldPrint)
		{
			printf("Privilege mount did not work\n");
		}
		mountStatus = mountWithoutPrivilege(storageDevice);
		if (mountStatus)
		{
			m_mountPoint = findMountDeviceBylsblk(storageDevice.m_partition);
		}
		else
		{
			if (m_shouldPrint)
			{
				printf("Non-Privilege mount did not work\n");
			}
		}
	}
	return mountStatus;
}

bool StorageUSB::mountDeviceNode(string deviceNode)
{
	bool ret = false;
	StorageMinimalInfo highStorage;
	if (deviceNode.length())
	{
		highStorage = getHighCapacityPartition(deviceNode);
	}
	if (highStorage.m_partition.length() > 0)
	{
		ret = mountUSBDevice(highStorage);
	}
	else
	{
		if (m_shouldPrint)
		{
			printf("No Partitions found to mount\n");
		}
	}
	if (ret)
	{
		m_deviceNode = deviceNode;
		m_partitionNode = highStorage.m_partition;
	}
	return ret;
}

bool StorageUSB::mountWithPrivilege(StorageMinimalInfo storageDevice,
		uint8_t retryLimit)
{
	struct stat st = { 0 };
	if (stat(m_mountPoint.c_str(), &st) == -1)
	{
		int64_t retVal = mkdir(m_mountPoint.c_str(), 0777);
		if (retVal == -1 && m_shouldPrint)
		{
			perror("directory cannot be created\n");
			return false;
		}
	}
	int64_t mountStatus = mount(storageDevice.m_partition.c_str(),
			m_mountPoint.c_str(), storageDevice.m_fsType.c_str(), MS_NOATIME,
			NULL);
	if (mountStatus == 0)
	{
		if (m_shouldPrint)
		{
			printf("Mount successful\n");
		}
		return true;
	}
	else
	{
		if (m_shouldPrint)
		{
			printf("mount unsuccessful(%" PRId64 ") %s -> %s\n", mountStatus,
					storageDevice.m_partition.c_str(), m_mountPoint.c_str());
		}
	}
	string error = "";
	if (mountStatus == EBUSY)
	{
		error = "Mountpoint busy. Let's retry in next " + to_string(retryLimit)
				+ " iteration";
		unMountUSBDevice();
	}
	else
	{
		error = "Mount error: " + to_string(errno);
	}
	if (m_shouldPrint)
	{
		printf("%s\n", error.c_str());
	}
	if (retryLimit-- == 0)
	{
		return false;
	}
	if (m_shouldPrint)
	{
		printf("mount retry left %" PRIu8 "\n", retryLimit);
	}
	usleep(1000);
	return mountWithPrivilege(storageDevice, retryLimit);
}

bool StorageUSB::mountWithoutPrivilege(StorageMinimalInfo storageDevice)
{
	try
	{
		string cmdResponse = Utils::exec(
				"udisksctl mount --no-user-interaction -b "
						+ storageDevice.m_partition, m_shouldPrint);
		if (m_shouldPrint)
		{
			printf("cmdResponse %s\n", cmdResponse.c_str());
		}
		if (apra::Utils::caseInsensitiveSearch(cmdResponse, "mounted"))
		{
			return true;
		}
	} catch (std::runtime_error &e)
	{
		printf("mount error: %s\n", e.what());
	}
	return false;
}

bool StorageUSB::unMountWithPrivilege()
{
	if (umount2(m_mountPoint.c_str(), MNT_FORCE))
	{
		string error;
		if (errno == EBUSY)
		{
			error = "Unmount busy";
		}
		else
		{
			error = "Unmount error: " + to_string(errno);
		}
		if (m_shouldPrint)
		{
			printf("%s\n", error.c_str());
		}
		return false;
	}
	if (m_shouldPrint)
	{
		printf("Unmount successful\n");
	}
	return true;
}

bool StorageUSB::unMountWithoutPrivilege()
{
	try
	{
		string cmdResponse = Utils::exec(
				"udisksctl unmount -f -b " + m_partitionNode, m_shouldPrint);
		if (!apra::Utils::caseInsensitiveSearch(cmdResponse, "error"))
		{
			return true;
		}
	} catch (std::runtime_error &e)
	{
		printf("unmount error: %s\n", e.what());
	}
	return false;
}

bool StorageUSB::unMountUSBDevice()
{
	if (!unMountWithPrivilege())
	{
		return unMountWithoutPrivilege();
	}
	return true;
}

bool StorageUSB::ejectDevice()
{
	if (unMountUSBDevice())
	{
		string ejectCommand = "";
		m_state = STORAGE_SAFE_EJECT;
		m_retryCount = 3;
		m_mountPoint.clear();
		m_deviceNode.clear();
		m_partitionNode.clear();
		return true;
	}
	return false;
}

string StorageUSB::findMountDeviceBylsblk(string devicePartitionNode)
{
	try
	{
		string cmdResponse = Utils::exec(
				"lsblk " + devicePartitionNode + " --noheadings -o MOUNTPOINT",
				m_shouldPrint);
		string mountPath = Utils::trim(cmdResponse);
		if (m_shouldPrint)
		{
			std::cout<<"\t mountPath ->" <<mountPath.c_str()<<std::endl;
		}
		if (mountPath.length()
				&& Utils::caseInsensitiveSearch(mountPath,
						"not a block device"))
		{
			mountPath.clear();
		}
		return mountPath;
	} catch (std::runtime_error &e)
	{
		printf("lsblk error: %s\n", e.what());
	}
	return "";
}

string StorageUSB::findMountedDevice(string devicePartitionNode)
{
	std::string deviceName = devicePartitionNode.substr(
			devicePartitionNode.rfind("/") + 1);
	std::ifstream mountsFile("/proc/mounts");
	if (!mountsFile)
	{
		perror("Failed to open mounts file");
		return "";
	}
	std::string line;
	while (std::getline(mountsFile, line))
	{
		std::istringstream iss(line);
		std::string dev_name, dir_name, type;
		if (iss >> dev_name >> dir_name >> type)
		{
			if (dev_name == devicePartitionNode)
			{
				return dir_name;
			}
		}
	}
	return "";
}

bool StorageUSB::getStorageInfo(uint64_t &freeSpaceInMB,
		uint64_t &totalCapacityInMB)
{
	struct statvfs stat;
	if (m_shouldPrint)
	{
		printf("path to stat %s\n", m_mountPoint.c_str());
	}
	if (statvfs(m_mountPoint.c_str(), &stat) != 0)
	{
		return false;
	}
	if (m_shouldPrint)
	{
		printf("stat.f_bsize=%" PRIu64 "\n stat.f_frsize=%" PRIu64 "\n stat.f_bfree=%" PRIu64 "\n "
				"stat.f_blocks=%" PRIu64 "\n", stat.f_bsize, stat.f_frsize,
				stat.f_bfree, stat.f_blocks);
	}
	uint64_t sz = stat.f_bsize;
	if (m_shouldPrint)
	{
		printf("int sz = stat.f_bsize; === %" PRIu64 " = %" PRIu64 "\n", sz, stat.f_bsize);
		printf("sz *= stat.f_bfree === %" PRIu64 " *= %" PRIu64 "\n", sz, stat.f_bfree);
	}
	sz *= stat.f_bfree;
	freeSpaceInMB = sz >> 20;
	if (m_shouldPrint)
	{
		printf("int freeSpaceInMB = sz >> 20 === int %" PRId64 " = %" PRId64 " >> 20\n",
				freeSpaceInMB, sz);
	}
	sz = stat.f_frsize;

	if (m_shouldPrint)
	{
		printf("sz = stat.f_bsize; === %" PRId64 " = %" PRId64 ";\n", sz, stat.f_bsize);
	}

	sz *= stat.f_blocks;

	if (m_shouldPrint)
	{
		printf("sz *= stat.f_blocks; === %" PRIu64 " *= %" PRIu64 ";\n", sz,
				stat.f_blocks);
	}

	totalCapacityInMB = sz >> 20;

	if (m_shouldPrint)
	{
		printf("int totalSpaceInMB = sz >> 20; === int %" PRIu64 " = "
		"%" PRIu64 " >> 20;\n", totalCapacityInMB, sz);
	}
	return true;
}

void StorageUSB::checkDeviceNode()
{
	struct stat buffer;
	string path = m_deviceNode;
	if (path.length() && stat(path.c_str(), &buffer) == 0)
	{
		if (S_ISBLK(buffer.st_mode))
		{
			return;
		}
	}
	m_state = STORAGE_SAFE_EJECT;
	m_deviceNode.clear();
	m_retryCount = 3;
}

}
/* namespace apra */

