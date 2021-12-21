#pragma once
#include "TypeLib_h.h"
#include <string>
#include "dewesoft_bridge.h"
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
#include <dcomlib/dcom_output_channel/output_factory_impl.h>
#include <dcomlib/dcom_utils/plugin_types.h>

/////////////////////////////////////////////////////////////////////////////
// Plugin

#define LIB_GUID_1(b) LIBID_##b##Lib
#define LIB_GUID(b) LIB_GUID_1(b)

class Plugin : public IDispatchImpl<IPlugin3, &IID_IPlugin3, &LIB_GUID(_LIB_NAME)>,
               public IDispatchImpl<IPlugin4, &IID_IPlugin4, &LIB_GUID(_LIB_NAME)>,
               public CComObjectRoot,
               public CComCoClass<Plugin, &CLSID_Plugin>
{
public:
    using NodePtr = Dewesoft::Utils::Serialization::NodePtr;

    static const std::wstring name;
    static const std::wstring keyName;
    static const std::wstring vendor;
    static const std::wstring version;
    static const std::wstring description;
    static const std::wstring icon;
    static const std::wstring dewesoftIcon;

    Plugin();

    BEGIN_COM_MAP(Plugin)
    COM_INTERFACE_ENTRY2(IDispatch, IPlugin3)
    COM_INTERFACE_ENTRY(IPlugin2)
    COM_INTERFACE_ENTRY(IPlugin3)
    COM_INTERFACE_ENTRY(IPlugin4)
    END_COM_MAP()

    static std::wstring getGuid();
    static HRESULT WINAPI UpdateRegistry(_In_ BOOL bRegister) throw();

    // IPlugin
private:
    IAppPtr app;
    Dewesoft::Utils::Dcom::InputChannel::InputManagerImpl inputManager;
    Dewesoft::Utils::Dcom::OutputChannel::OutputFactoryImpl outputFactory;
    DewesoftBridge bridge;

    bool isRecalculationRequired() const;
    IPluginGroup* getPluginGroup() const;

    STDMETHODIMP checkMuiSupportedDewesoftVersion() const;
    STDMETHODIMP checkPluginSupportedDewesoftVersion() const;

    STDMETHODIMP onShowSetupMUI(Dewesoft::MUI::IWindow* window);
    STDMETHODIMP onHideSetupMUI();

    STDMETHODIMP onShowSettingsMUI(Dewesoft::MUI::IWindow* window);
    STDMETHODIMP onHideSettingsMUI();

    STDMETHODIMP onEnterPluginSetupScreen();
    STDMETHODIMP onLeavePluginSetupScreen();

    STDMETHODIMP onEnterPluginAnalysisSetupScreen();
    STDMETHODIMP onLeavePluginAnalysisSetupScreen();

    STDMETHODIMP onEnterHardwareSetup();
    STDMETHODIMP onInitiateHardware();

    STDMETHODIMP onEstablishConnections();
    STDMETHODIMP onChannelRemoved(IChannelPtr channel);

    STDMETHODIMP onClearChannelInstances();

    STDMETHODIMP onUpdateDwHwXml(NodePtr node);
    STDMETHODIMP onUpdateDwXml(NodePtr node, bool dataFile);
    STDMETHODIMP onClearSetup();
    STDMETHODIMP onNewSetup();
    STDMETHODIMP onSaveSetup(NodePtr node, bool dataFile);
    STDMETHODIMP onLoadSetup(NodePtr node, bool dataFile);

    STDMETHODIMP onPreInitiate();

    STDMETHODIMP onStartData();
    STDMETHODIMP onGetData();
    STDMETHODIMP onStopData();

    STDMETHODIMP onStartStoring();
    STDMETHODIMP onStopStoring();

    STDMETHODIMP onStartAnalysis();
    STDMETHODIMP onPrepareAnalysis(IChannelListExPtr channelList);
    STDMETHODIMP onCalculateAnalysis();

public:
    STDMETHOD(raw_Initiate)(IApp* deweApp);

    STDMETHOD(raw_OnStartAcq)();
    STDMETHOD(raw_OnStopAcq)();

    STDMETHOD(raw_OnStartStoring)();
    STDMETHOD(raw_OnStopStoring)();

    STDMETHOD(raw_OnGetData)();
    STDMETHOD(raw_OnTrigger)(DOUBLE time)
    {
        return S_OK;
    }

    STDMETHOD(raw_Configure)()
    {
        return S_OK;
    }

    STDMETHOD(get_Id)(BSTR* value)
    {
        return S_OK;
    }

    STDMETHOD(raw_SaveSetup)(VARIANT* data)
    {
        return S_OK;
    }

    STDMETHOD(raw_LoadSetup)(VARIANT data);
    STDMETHOD(raw_NewSetup)();
    STDMETHOD(raw_ClearChannelsInstance)();
    STDMETHOD(raw_UpdateFrame)()
    {
        return S_OK;
    }

    STDMETHODIMP raw_ShowFrame(LONG parent, VARIANT_BOOL* value)
    {
        *value = true;
        return S_OK;
    }

    STDMETHODIMP raw_OnResizeFrame(long width, long height)
    {
        return S_OK;
    }

    STDMETHODIMP raw_HideFrame()
    {
        return S_OK;
    }

    STDMETHOD(get_Used)(VARIANT_BOOL* value)
    {
        *value = true;
        return S_OK;
    }

    STDMETHOD(put_Used)(VARIANT_BOOL value)
    {
        return S_OK;
    }

    STDMETHOD(raw_OnOleMsg)(LONG msg, LONG param)
    {
        return S_OK;
    }

    // IPlugin3

    STDMETHOD(raw_OnStartSetup());
    STDMETHOD(raw_OnStopSetup());

    STDMETHOD(raw_OnBeforeStartAcq(VARIANT_BOOL* allowStart))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnAfterStartAcq())
    {
        return S_OK;
    }

    STDMETHOD(raw_OnBeforeStopAcq(VARIANT_BOOL* allowStop))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnAfterStopAcq())
    {
        return S_OK;
    }

    STDMETHOD(raw_OnRepaintFrame())
    {
        return S_OK;
    }

    STDMETHOD(raw_OnTriggerStop(double time, double trigDuration))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnAfterCalcMath())
    {
        return S_OK;
    }

    STDMETHOD(raw_GetDWTypeLibVersion(long* value))
    {
        *value = 7;
        return S_OK;
    }

    STDMETHOD(raw_OnGetSetupData());

    STDMETHOD(raw_ProvidesClock(VARIANT_BOOL* Value))
    {
        *Value = FALSE;
        return S_OK;
    }

    STDMETHOD(raw_OnGetClock(long* clockLow, long* clockHigh))
    {
        return S_OK;
    }

    STDMETHOD(raw_SetCANPort(long port))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnAlarm(long condIndex, VARIANT_BOOL status))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnBigListLoad(BSTR textSetup))
    {
        return S_OK;
    }

    STDMETHOD(raw_OnExit())
    {
        return S_OK;
    }

    STDMETHODIMP raw_OnShowHWFrame(long parent, VARIANT_BOOL* value)
    {
        *value = true;
        return S_OK;
    }

    STDMETHODIMP raw_OnHideHWFrame()
    {
        return S_OK;
    }

    STDMETHODIMP eventOnResizeHWFrame(int width, int height)
    {
        return S_OK;
    }

    // IPlugin4
    STDMETHOD(raw_OnEvent(enum EventIDs eventID, VARIANT inParam, VARIANT* outParam));
};
