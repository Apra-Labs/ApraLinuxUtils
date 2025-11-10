/*
 * usb_storage_example.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

/*
 * USB Storage Detection and Monitoring Example
 *
 * This example demonstrates how to use the ApraUtils StorageUSB interface to:
 * 1. Detect USB storage devices (flash drives, external HDDs, SD cards)
 * 2. Monitor for USB insertion and removal events
 * 3. Get storage device information (size, capacity, filesystem)
 * 4. Handle mounting and unmounting operations
 * 5. Check storage status and handle safe/unsafe eject scenarios
 *
 * Hardware Requirements:
 * ---------------------
 * - Linux-based embedded system with USB host support
 * - USB flash drive or external storage device
 * - Sufficient permissions to mount/unmount devices
 *
 * System Requirements:
 * --------------------
 * - udev library (libudev) for device enumeration
 * - Mount point directory (e.g., /mnt/usb)
 * - User must be in appropriate groups (plugdev, disk) or run as root
 *
 * Supported Filesystems:
 * ----------------------
 * - FAT32 (vfat)
 * - NTFS
 * - EXT4
 *
 * Compilation:
 * ------------
 * g++ -std=c++14 usb_storage_example.cpp -o usb_storage_example -lApraUtils -ludev
 *
 * Execution:
 * ----------
 * sudo ./usb_storage_example
 * (sudo required for mount/unmount operations)
 *
 * Notes:
 * ------
 * - Create mount point before running: sudo mkdir -p /mnt/usb
 * - Ensure mount point has appropriate permissions
 * - For auto-mounting, run without root using skipMount=true option
 */

#include <ApraUtils.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>

using namespace apra;
using namespace std;

// Configuration
#define USB_MOUNT_PATH "/mnt/usb"
#define POLL_INTERVAL_SECONDS 2

// Control flag
volatile bool g_keepRunning = true;

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << endl;
    g_keepRunning = false;
}

// Function to convert storage state enum to readable string
string storageStateToString(STORAGE_STATE state) {
    switch (state) {
        case STORAGE_INSERTED:
            return "INSERTED (not mounted)";
        case STORAGE_MOUNTED:
            return "MOUNTED (ready for use)";
        case STORAGE_UNSAFE_EJECT:
            return "UNSAFE_EJECT (still in use)";
        case STORAGE_SAFE_EJECT:
            return "SAFE_EJECT (can be removed)";
        default:
            return "UNKNOWN";
    }
}

// Function to format bytes into human-readable size
string formatBytes(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    // Format with 2 decimal places
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f %s", size, units[unitIndex]);
    return string(buffer);
}

// Function to display storage information
void displayStorageInfo(StorageUSB& storage) {
    cout << "\n--- Storage Device Information ---" << endl;

    // Get status
    STORAGE_STATE state = storage.getStatus();
    cout << "Status: " << storageStateToString(state) << endl;

    // Get mount path
    string mountPath = storage.getMountPath();
    cout << "Mount path: " << mountPath << endl;

    // If mounted, get storage info
    if (state == STORAGE_MOUNTED) {
        uint64_t freeSpaceMB = 0;
        uint64_t totalCapacityMB = 0;

        if (storage.getStorageInfo(freeSpaceMB, totalCapacityMB)) {
            uint64_t usedSpaceMB = totalCapacityMB - freeSpaceMB;
            double usedPercentage = (totalCapacityMB > 0) ?
                (static_cast<double>(usedSpaceMB) / totalCapacityMB * 100.0) : 0.0;

            cout << "\nCapacity Information:" << endl;
            cout << "  Total capacity: " << formatBytes(totalCapacityMB * 1024 * 1024)
                 << " (" << totalCapacityMB << " MB)" << endl;
            cout << "  Used space:     " << formatBytes(usedSpaceMB * 1024 * 1024)
                 << " (" << usedSpaceMB << " MB)" << endl;
            cout << "  Free space:     " << formatBytes(freeSpaceMB * 1024 * 1024)
                 << " (" << freeSpaceMB << " MB)" << endl;
            cout << "  Usage:          " << fixed << setprecision(1)
                 << usedPercentage << "%" << endl;
        } else {
            cout << "Unable to retrieve storage capacity information." << endl;
        }
    }

    cout << "-----------------------------------" << endl;
}

// Function to demonstrate basic USB detection and mounting
void basicDetectionExample() {
    cout << "\n==================================================" << endl;
    cout << "Basic USB Storage Detection Example" << endl;
    cout << "==================================================" << endl;

    // Create StorageUSB instance
    // Parameters: mount_path, supported_types, should_print, skip_mount
    vector<STORAGE_TYPE> supportedTypes = {FAT32, NTFS, EXT4};
    StorageUSB storage(USB_MOUNT_PATH, supportedTypes, true, false);

    cout << "Checking for USB storage devices..." << endl;

    // Check if USB device is inserted
    string devicePath = storage.insertCheck();

    if (devicePath.empty()) {
        cout << "No USB storage device detected." << endl;
        cout << "Please insert a USB flash drive." << endl;
        return;
    }

    cout << "USB storage device detected: " << devicePath << endl;

    // Display initial status
    displayStorageInfo(storage);

    // If not mounted, try to mount
    if (storage.getStatus() == STORAGE_INSERTED) {
        cout << "\nAttempting to mount device..." << endl;
        string mountedPath = storage.mountDevice();

        if (!mountedPath.empty()) {
            cout << "Successfully mounted at: " << mountedPath << endl;
            displayStorageInfo(storage);
        } else {
            cerr << "Failed to mount device." << endl;
            cerr << "Possible reasons:" << endl;
            cerr << "  - Insufficient permissions (try running as root)" << endl;
            cerr << "  - Mount point doesn't exist or not accessible" << endl;
            cerr << "  - Unsupported filesystem" << endl;
            cerr << "  - Device is already mounted elsewhere" << endl;
        }
    }

    // Wait before cleanup
    cout << "\nPress Enter to unmount and cleanup...";
    cin.get();

    // Eject device
    if (storage.getStatus() == STORAGE_MOUNTED) {
        cout << "Ejecting device..." << endl;

        if (storage.ejectDevice()) {
            cout << "Device ejected successfully." << endl;

            // Check if it was safe eject
            if (storage.isUnsafeEject()) {
                cout << "Warning: Device was in use during eject (unsafe)." << endl;
            } else {
                cout << "Device ejected safely. You can now remove it." << endl;
            }
        } else {
            cerr << "Failed to eject device." << endl;
        }
    }
}

// Function to demonstrate continuous USB monitoring
void continuousMonitoringExample() {
    cout << "\n==================================================" << endl;
    cout << "Continuous USB Storage Monitoring Example" << endl;
    cout << "==================================================" << endl;
    cout << "This will monitor for USB insertion and removal." << endl;
    cout << "Press Ctrl+C to stop monitoring." << endl;
    cout << "==================================================" << endl;

    // Setup signal handler
    signal(SIGINT, signalHandler);

    // Create StorageUSB instance with auto-mount enabled
    vector<STORAGE_TYPE> supportedTypes = {FAT32, NTFS, EXT4};
    StorageUSB storage(USB_MOUNT_PATH, supportedTypes, true, false);

    STORAGE_STATE previousState = STORAGE_SAFE_EJECT;
    bool devicePresent = false;

    cout << "\nMonitoring started. Waiting for USB device..." << endl;

    while (g_keepRunning) {
        // Check for device insertion
        string devicePath = storage.insertCheck();
        STORAGE_STATE currentState = storage.getStatus();

        // Detect state changes
        if (!devicePresent && !devicePath.empty()) {
            // Device inserted
            devicePresent = true;
            cout << "\n*** USB DEVICE INSERTED ***" << endl;
            cout << "Device: " << devicePath << endl;

            // Attempt to mount
            cout << "Attempting to mount..." << endl;
            string mountedPath = storage.mountDevice();

            if (!mountedPath.empty()) {
                cout << "Successfully mounted!" << endl;
                displayStorageInfo(storage);
            } else {
                cerr << "Failed to mount device." << endl;
            }
        }
        else if (devicePresent && devicePath.empty()) {
            // Device removed
            devicePresent = false;
            cout << "\n*** USB DEVICE REMOVED ***" << endl;

            // Check if it was a safe removal
            if (previousState == STORAGE_MOUNTED) {
                cout << "Warning: Device removed while mounted (unsafe removal)!" << endl;
                cout << "Data loss may have occurred." << endl;
            } else {
                cout << "Device was properly ejected before removal." << endl;
            }
        }
        else if (devicePresent && currentState != previousState) {
            // State changed while device present
            cout << "\nStorage state changed: "
                 << storageStateToString(previousState)
                 << " -> "
                 << storageStateToString(currentState)
                 << endl;

            if (currentState == STORAGE_MOUNTED) {
                displayStorageInfo(storage);
            }
        }

        previousState = currentState;

        // Poll every few seconds
        sleep(POLL_INTERVAL_SECONDS);
    }

    // Cleanup on exit
    cout << "\n\nStopping monitoring..." << endl;

    if (storage.getStatus() == STORAGE_MOUNTED) {
        cout << "Unmounting device..." << endl;
        storage.ejectDevice();
    }

    cout << "Monitoring stopped." << endl;
}

// Function to demonstrate detection without auto-mounting
void detectionOnlyExample() {
    cout << "\n==================================================" << endl;
    cout << "USB Detection Without Auto-Mount Example" << endl;
    cout << "==================================================" << endl;
    cout << "This mode only detects USB devices without mounting." << endl;
    cout << "Useful when system auto-mounts or manual mount needed." << endl;
    cout << "==================================================" << endl;

    // Create StorageUSB with skipMount=true
    vector<STORAGE_TYPE> supportedTypes = {FAT32, NTFS, EXT4};
    StorageUSB storage(USB_MOUNT_PATH, supportedTypes, true, true);

    cout << "\nChecking for USB devices..." << endl;

    for (int i = 0; i < 10 && g_keepRunning; i++) {
        string devicePath = storage.insertCheck();

        if (!devicePath.empty()) {
            cout << "\nUSB device detected: " << devicePath << endl;
            cout << "Status: " << storageStateToString(storage.getStatus()) << endl;

            // In skip mount mode, you can still get info if system auto-mounted
            uint64_t freeMB = 0, totalMB = 0;
            if (storage.getStorageInfo(freeMB, totalMB)) {
                cout << "Device is mounted and accessible." << endl;
                cout << "Free: " << freeMB << " MB, Total: " << totalMB << " MB" << endl;
            } else {
                cout << "Device detected but not mounted." << endl;
                cout << "Manual mount command:" << endl;
                cout << "  sudo mount " << devicePath << " " << USB_MOUNT_PATH << endl;
            }
        } else {
            cout << "." << flush;
        }

        sleep(2);
    }

    cout << "\nDetection-only example completed." << endl;
}

// Main function
int main() {
    cout << "==================================================" << endl;
    cout << "ApraUtils USB Storage Detection Example" << endl;
    cout << "==================================================" << endl;
    cout << "This example demonstrates:" << endl;
    cout << "  1. Basic USB detection and mounting" << endl;
    cout << "  2. Continuous monitoring for USB events" << endl;
    cout << "  3. Detection without auto-mounting" << endl;
    cout << "\nRequirements:" << endl;
    cout << "  - Mount point created: " << USB_MOUNT_PATH << endl;
    cout << "  - Sufficient permissions (run as root if needed)" << endl;
    cout << "  - USB storage device for testing" << endl;
    cout << "==================================================" << endl;

    // Check command line arguments
    if (system("test -d " USB_MOUNT_PATH) != 0) {
        cerr << "\nWarning: Mount point " << USB_MOUNT_PATH
             << " does not exist." << endl;
        cerr << "Create it with: sudo mkdir -p " << USB_MOUNT_PATH << endl;
        cerr << "\nContinuing anyway..." << endl;
    }

    // Menu for selecting example
    cout << "\nSelect example to run:" << endl;
    cout << "  1. Basic detection and mounting" << endl;
    cout << "  2. Continuous monitoring (recommended)" << endl;
    cout << "  3. Detection only (no auto-mount)" << endl;
    cout << "  q. Quit" << endl;
    cout << "\nEnter choice: ";

    char choice;
    cin >> choice;
    cin.ignore(); // Clear newline

    switch (choice) {
        case '1':
            basicDetectionExample();
            break;
        case '2':
            continuousMonitoringExample();
            break;
        case '3':
            detectionOnlyExample();
            break;
        case 'q':
        case 'Q':
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Exiting..." << endl;
            break;
    }

    cout << "\n==================================================" << endl;
    cout << "USB Storage example completed." << endl;
    cout << "==================================================" << endl;

    return 0;
}
