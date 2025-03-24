#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <usbiodef.h>
#include <iostream>
#include <devguid.h>
#include <cfgmgr32.h>

#pragma comment(lib, "SetupAPI.lib")

void GetDeviceRegistryProperty(HDEVINFO hDevInfo, SP_DEVINFO_DATA devInfoData) {
    DWORD DataT;
    TCHAR buffer[1024];
    DWORD buffersize = sizeof(buffer);

    if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buffer, buffersize, NULL)) {
        std::wcout << L"  Device Name: " << buffer << std::endl;
        return;
    }

    if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_DEVICEDESC, &DataT, (PBYTE)buffer, buffersize, NULL)) {
        std::wcout << L"  Device Description: " << buffer << std::endl;
        return;
    }

    std::cerr << "  Could not retrieve any information about the device.\n";
}

void ListUSBDevices() {
    HDEVINFO hDevInfo;
    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
    SP_DEVINFO_DATA devInfoData;
    DWORD i;

    hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USB_DEVICE, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "Error retrieving USB devices.\n";
        return;
    }

    DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_USB_DEVICE, i, &DeviceInterfaceData); i++) {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &DeviceInterfaceData, NULL, 0, &requiredSize, NULL);
        PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        if (!DeviceInterfaceDetailData) {
            std::cerr << "Memory allocation error.\n";
            continue;
        }

        DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &DeviceInterfaceData, DeviceInterfaceDetailData, requiredSize, NULL, &devInfoData)) {
            std::wcout << L"USB Device: " << DeviceInterfaceDetailData->DevicePath << std::endl;
            GetDeviceRegistryProperty(hDevInfo, devInfoData);
        }

        free(DeviceInterfaceDetailData);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
}

int main() {
    ListUSBDevices();
    return 0;
}
