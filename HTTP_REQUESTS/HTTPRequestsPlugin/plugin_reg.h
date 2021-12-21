#pragma once

#include "StdAfx.h"
#include <string>

class PluginRegistrar
{
public:
    static std::wstring pluginRegPath;
    static std::wstring regCOMPath;
    static std::wstring keyName;
    static std::wstring name;
    static std::wstring version;
    static std::wstring vendor;
    static std::wstring description;
    static std::wstring guid;
    static std::wstring icon;
    static bool perUser;
	static bool supportsAnalysis;
    static HINSTANCE hInst;
    static HKEY regRoot;

    static HRESULT registerPlugin();
    static HRESULT unregisterPlugin();
private:
    static void init();
};
