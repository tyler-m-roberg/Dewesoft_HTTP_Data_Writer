#include "stdafx.h"
#include "plugin_impl.h"

#define CONC(a, b) a##b
#define STRINGIFY_TEMP_W(s) CONC(L, #s)
#define STRINGIFY_W(s) STRINGIFY_TEMP_W(s)

#define MAKE_VERSION_DOT(major, minor, release) major.minor.release

const std::wstring Plugin::name = STRINGIFY_W(_PLUGIN_DISPLAY_NAME);
const std::wstring Plugin::vendor = STRINGIFY_W(_PLUGIN_VENDOR);
const std::wstring Plugin::version = STRINGIFY_W(MAKE_VERSION_DOT(_PLUGIN_VERSION_MAJOR, _PLUGIN_VERSION_MINOR, _PLUGIN_VERSION_RELEASE));
const std::wstring Plugin::description = STRINGIFY_W(_PLUGIN_DESCRIPTION);
const std::wstring Plugin::icon = L"Icon";
const std::wstring Plugin::keyName = STRINGIFY_W(_PLUGIN_NAME);
