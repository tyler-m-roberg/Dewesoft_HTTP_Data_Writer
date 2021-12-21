#include "StdAfx.h"
#include "Resource.h"
#include <initguid.h>

#include "TypeLib_h.h"
#include "interface/plugin_impl.h"
#include "plugin_reg.h"

ATL::CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_Plugin, Plugin)
END_OBJECT_MAP()

// Common Controls initialization helper
inline BOOL AtlInitCommonControls(DWORD dwFlags)
{
    INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
    BOOL bRet = ::InitCommonControlsEx(&iccx);
    ATLASSERT(bRet);
    return bRet;
}

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        PluginRegistrar::hInst = hInstance;
        _Module.Init(ObjectMap, hInstance, &LIB_GUID(_LIB_NAME));
        AtlInitCommonControls(ICC_WIN95_CLASSES);
        DisableThreadLibraryCalls(hInstance);

        //DewesoftBridge::DLLModule = hInstance;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
	PluginRegistrar::guid = Plugin::getGuid();
	PluginRegistrar::keyName = Plugin::keyName;

	PluginRegistrar::description = Plugin::description;
	PluginRegistrar::name = Plugin::name;
	PluginRegistrar::keyName = Plugin::keyName;
	PluginRegistrar::perUser = _AtlRegisterPerUser;
	PluginRegistrar::vendor = Plugin::vendor;
	PluginRegistrar::icon = Plugin::icon;
	PluginRegistrar::version = Plugin::version;
	PluginRegistrar::registerPlugin();

	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	PluginRegistrar reg;
	reg.guid = Plugin::getGuid();
	reg.keyName = Plugin::keyName;

	reg.unregisterPlugin();

	return _Module.UnregisterServer(TRUE);
}

STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";
	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}
	if (bInstall)
	{
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}
	return hr;
}
