#include "StdAfx.h"
#include "plugin_reg.h"

std::wstring PluginRegistrar::pluginRegPath;
std::wstring PluginRegistrar::regCOMPath;
std::wstring PluginRegistrar::keyName;
std::wstring PluginRegistrar::name;
std::wstring PluginRegistrar::version;
std::wstring PluginRegistrar::vendor;
std::wstring PluginRegistrar::description;
std::wstring PluginRegistrar::guid;
std::wstring PluginRegistrar::icon;
bool PluginRegistrar::perUser = true;
bool PluginRegistrar::supportsAnalysis = true;
HINSTANCE PluginRegistrar::hInst = nullptr;
HKEY PluginRegistrar::regRoot = nullptr;

HRESULT PluginRegistrar::registerPlugin()
{
    HKEY hKey;
    DWORD disposition;

    init();

    if (RegCreateKeyEx(regRoot, pluginRegPath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, &disposition) == ERROR_SUCCESS)
    {
        const TCHAR* RegName = name.c_str();
        const TCHAR* RegVendor = vendor.c_str();
        const TCHAR* RegVersion = version.c_str();
        const TCHAR* RegDescription = description.c_str();
        const TCHAR* RegIcon = icon.c_str();
        const TCHAR* RegGuid = guid.c_str();
        const DWORD RegAnalysis = supportsAnalysis ? 1 : 0;

        RegSetValueEx(hKey, _T("Name"), 0, REG_SZ, (BYTE*)RegName, (DWORD)_tcslen(RegName) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("Vendor"), 0, REG_SZ, (BYTE*)RegVendor, (DWORD)_tcslen(RegVendor) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("Version"), 0, REG_SZ, (BYTE*)RegVersion, (DWORD)_tcslen(RegVersion) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("Description"), 0, REG_SZ, (BYTE*)RegDescription, (DWORD)_tcslen(RegDescription) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("GUID"), 0, REG_SZ, (BYTE*)RegGuid, (DWORD)_tcslen(RegGuid) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("Icon"), 0, REG_SZ, (BYTE*)RegIcon, (DWORD)_tcslen(RegIcon) * sizeof(TCHAR));
        RegSetValueEx(hKey, _T("SupportsAnalyse"), 0, REG_DWORD, (BYTE*)&RegAnalysis, sizeof(RegAnalysis));

        RegCloseKey(hKey);
    }

    if (RegCreateKeyEx(regRoot, regCOMPath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, &disposition) == ERROR_SUCCESS)
    {
        const TCHAR RegPluginClass[] = _T("Plugin class");
        RegSetValueEx(hKey, nullptr, 0, REG_SZ, (BYTE*)RegPluginClass, (DWORD)_tcslen(RegPluginClass) * sizeof(TCHAR));

        HKEY hSubKey;
        if (RegCreateKeyEx(hKey, _T("InprocServer32"), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hSubKey, &disposition) == ERROR_SUCCESS)
        {
            TCHAR moduleFileName[2048];
            GetModuleFileNameW(hInst, moduleFileName, 2048);

            RegSetValueEx(hSubKey, nullptr, 0, REG_SZ, (BYTE*)moduleFileName, (DWORD)_tcslen(moduleFileName) * sizeof(TCHAR));
            static std::wstring ThreadingModelApartment = _T("Apartment");
            RegSetValueEx(hSubKey, _T("ThreadingModel"), 0, REG_SZ, (BYTE*)ThreadingModelApartment.c_str(), (DWORD)_tcslen(ThreadingModelApartment.c_str()) * sizeof(TCHAR));

            RegCloseKey(hSubKey);
        }

        RegCloseKey(hKey);
    }

    return ERROR_SUCCESS;
}

HRESULT PluginRegistrar::unregisterPlugin()
{
    init();

    RegDeleteKey(regRoot, pluginRegPath.c_str());
    RegDeleteKey(regRoot, regCOMPath.c_str());

    return ERROR_SUCCESS;
}

void PluginRegistrar::init()
{
    pluginRegPath = _T("SOFTWARE\\Dewesoft\\Plugins\\") + keyName;

    if (perUser)
        regRoot = HKEY_CURRENT_USER;
    else
        regRoot = HKEY_LOCAL_MACHINE;

    regCOMPath = _T("SOFTWARE\\Classes\\CLSID\\") + guid;
}
