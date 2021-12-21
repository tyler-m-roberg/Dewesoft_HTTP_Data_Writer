#include "StdAfx.h"
#include "settings_window.h"

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

SettingsWindow::SettingsWindow(WindowPtr ui, DewesoftBridge& bridge) : BaseSettingsWindow(ui, bridge, "ui/settings_window.xml")
{
}
