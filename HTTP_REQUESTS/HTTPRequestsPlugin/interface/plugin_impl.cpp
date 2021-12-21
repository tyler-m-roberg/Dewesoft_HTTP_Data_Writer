#include "StdAfx.h"
#include "plugin_impl.h"
#include <propvarutil.h>
#include "Windows.h"
#include <dcomlib/dcom_utils/enums.h>

using namespace Dewesoft::MUI;
using namespace Dewesoft::Utils::Dcom::Utils;
using namespace Dewesoft::Utils::Dcom::InputChannel;

Plugin::Plugin()
    : app(nullptr)
    , bridge(inputManager, outputFactory)
{
}

// static
HRESULT WINAPI Plugin::UpdateRegistry(_In_ BOOL bRegister) throw()
{
    return ERROR_SUCCESS;
}

// static
std::wstring Plugin::getGuid()
{
    LPOLESTR lpolestrguid;
    StringFromCLSID(CLSID_Plugin, (LPOLESTR*)&lpolestrguid);
    std::wstring g = lpolestrguid;
    CoTaskMemFree(lpolestrguid);
    return g;
}

bool Plugin::isRecalculationRequired() const
{
    return inputManager.isRecalculationRequired() || outputFactory.isRecalculationRequired() || bridge.isRecalculationRequired();
}

IPluginGroup* Plugin::getPluginGroup() const
{
    IPluginGroup* pluginGroup;
    IChannelGroupPtr channelGroup = app->Data->Groups->GetItem((long)GroupType::Plugins);

    channelGroup->QueryInterface(IID_IPluginGroup, (void **)&pluginGroup);
    return pluginGroup;
}

STDMETHODIMP Plugin::checkMuiSupportedDewesoftVersion() const
{
    long super = -1;
    long xVersion = -1;
    long spVersion = -1;
    long buildVersion = -1;
    unsigned char releaseChannelId;

    app->GetDewesoftVersion(&super, &xVersion, &spVersion, &buildVersion, (char*)&releaseChannelId);

    if (xVersion < 3 || (xVersion <= 3 && spVersion < 4))
    {
        std::wstringstream ss;
        ss << "MUI3 Plugin \"" << name.c_str() << "\" needs DEWESoft X3 SP4 or newer.";

        app->WriteErrorMessage(_bstr_t(ss.str().c_str()));
        return E_ABORT;
    }
    return S_OK;
}

STDMETHODIMP Plugin::checkPluginSupportedDewesoftVersion() const
{
    std::string errorMessage;
    if (!bridge.checkSupportedDewesoftVersion(errorMessage))
    {
        app->WriteErrorMessage(_bstr_t(errorMessage.c_str()));
        return E_ABORT;
    }
    return S_OK;
}

STDMETHODIMP Plugin::onShowSetupMUI(IWindow* window)
{
    if (!bridge.showSetup(window))
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP Plugin::onHideSetupMUI()
{
    bridge.hideSetup();
    return S_OK;
}

STDMETHODIMP Plugin::onShowSettingsMUI(IWindow* window)
{
    if (!bridge.showSettings(window))
        return E_FAIL;

    return S_OK;
}

STDMETHODIMP Plugin::onHideSettingsMUI()
{
    bridge.hideSettings();
    return S_OK;
}

STDMETHODIMP Plugin::onEnterPluginSetupScreen()
{
    inputManager.setupEnter();
    bridge.onSetupEnter(false);
    return S_OK;
}

STDMETHODIMP Plugin::onLeavePluginSetupScreen()
{
    bridge.onSetupLeave(false);
    inputManager.setupLeave();
    return S_OK;
}

STDMETHODIMP Plugin::onEnterPluginAnalysisSetupScreen()
{
    inputManager.setupEnter();
    bridge.onSetupEnter(true);
    return S_OK;
}

STDMETHODIMP Plugin::onLeavePluginAnalysisSetupScreen()
{
    bridge.onSetupLeave(true);
    inputManager.analysisSetupLeave();
    outputFactory.analysisSetupLeave(isRecalculationRequired());
    return S_OK;
}

STDMETHODIMP Plugin::onEnterHardwareSetup()
{
    bridge.onEnterHardwareSetup();
    return S_OK;
}

STDMETHODIMP Plugin::onInitiateHardware()
{
    bridge.onInitiateHardware();
    return S_OK;
}

STDMETHODIMP Plugin::onEstablishConnections()
{
    inputManager.establishConnections();
    return S_OK;
}

STDMETHODIMP Plugin::onChannelRemoved(IChannelPtr channel)
{
    inputManager.channelRemoved(channel);
    return S_OK;
}

STDMETHODIMP Plugin::onClearChannelInstances()
{
    inputManager.clearChannelsInstances();
    outputFactory.clearChannelsInstances();
    return S_OK;
}

STDMETHODIMP Plugin::onUpdateDwHwXml(NodePtr node)
{
    if (!node || !node->isInitialized())
        return S_FALSE;

    bridge.onUpdateSettings(node);
    return S_OK;
}

STDMETHODIMP Plugin::onUpdateDwXml(NodePtr node, const bool dataFile)
{
    if (node->isInitialized() && node->isWrite())
        return onSaveSetup(node, dataFile);

    if (!node->isWrite())
    {
        const HRESULT clearOk = onClearSetup();
        if (!SUCCEEDED(clearOk))
            return clearOk;

        if (node->isInitialized())
            return onLoadSetup(node, dataFile);

        if (dataFile)
            return S_OK;

        return onNewSetup();
    }
    return S_FALSE;
}

STDMETHODIMP Plugin::onClearSetup()
{
    bridge.onClearSetup();
    inputManager.clearSetup();
    outputFactory.clearSetup();
    return S_OK;
}

STDMETHODIMP Plugin::onNewSetup()
{
    outputFactory.beginLoadSetup();
    bridge.onNewSetup();
    outputFactory.endLoadSetup();
    return S_OK;
}

STDMETHODIMP Plugin::onSaveSetup(NodePtr node, const bool dataFile)
{
    inputManager.saveSetup(node);
    outputFactory.saveSetup(node);
    bridge.onSaveSetup(node, dataFile);
    return S_OK;
}

STDMETHODIMP Plugin::onLoadSetup(NodePtr node, const bool dataFile)
{
    outputFactory.beginLoadSetup();
    inputManager.loadSetup(node);
    outputFactory.loadSetup(node);
    bridge.onLoadSetup(node, dataFile);
    outputFactory.endLoadSetup();
    return S_OK;
}

STDMETHODIMP Plugin::onPreInitiate()
{
    bridge.onPreInitiate();
    return S_OK;
}

STDMETHODIMP Plugin::onStartData()
{
    inputManager.startData();
    outputFactory.startData();
    bridge.onStartData();
    return S_OK;
}

STDMETHODIMP Plugin::onGetData()
{
    const AcquiredDataInfo acquiredDataInfo = inputManager.getData();
    bridge.onGetData(acquiredDataInfo);
    outputFactory.getDataAfterWrite();
    return S_OK;
}

STDMETHODIMP Plugin::onStopData()
{
    bridge.onStopData();
    return S_OK;
}

STDMETHODIMP Plugin::onStartStoring()
{
    inputManager.startStoring();
    bridge.onStartStoring();
    return S_OK;
}

STDMETHODIMP Plugin::onStopStoring()
{
    bridge.onStopStoring();
    return S_OK;
}

STDMETHODIMP Plugin::onStartAnalysis()
{
    if (!isRecalculationRequired())
        return S_OK;

    inputManager.startAnalysis();
    outputFactory.startAnalysis();
    bridge.onStartAnalysis();
    return S_OK;
}

STDMETHODIMP Plugin::onPrepareAnalysis(IChannelListExPtr channelList)
{
    if (!isRecalculationRequired())
        return S_OK;

    outputFactory.prepareAnalysis();
    inputManager.prepareAnalysis(channelList);
    bridge.onPrepareAnalysis();
    return S_OK;
}

STDMETHODIMP Plugin::onCalculateAnalysis()
{
    if (outputFactory.isRecalculationRequired())
        onGetData();

    return S_OK;
}

STDMETHODIMP Plugin::raw_Initiate(IApp* deweApp)
{
    app = deweApp;
    inputManager.setPluginProps(app, getGuid().c_str());
    outputFactory.setPluginProps(app, getGuid().c_str(), getPluginGroup());
    bridge.setPluginProps(app, getGuid().c_str(), getPluginGroup());

    const HRESULT muiDewesoftVersionOk = checkMuiSupportedDewesoftVersion();
    if (!SUCCEEDED(muiDewesoftVersionOk))
        return muiDewesoftVersionOk;

    return checkPluginSupportedDewesoftVersion();
}

STDMETHODIMP Plugin::raw_OnEvent(enum EventIDs eventID, VARIANT inParam, VARIANT* outParam)
{
    using ErrCode = Dewesoft::RT::Core::ErrCode;

    HRESULT returnValue = S_FALSE;

    switch (eventID)
    {
        case evHasMUI:
            outParam->boolVal = true;
            returnValue = S_OK;
            break;
        case evMUIVersion:
            outParam->intVal = 3;
            returnValue = S_OK;
            break;
        case evUseDebugMUI:
            outParam->vt = VT_BOOL;
#if defined(_DEBUG)
            outParam->boolVal = true;
#else
            outParam->boolVal = false;
#endif
            returnValue = S_OK;
            break;
        case evShowSetupMUI:
            returnValue = onShowSetupMUI(reinterpret_cast<IWindow*>(inParam.punkVal));
            break;
        case evHideSetupMUI:
            returnValue = onHideSetupMUI();
            break;
        case evShowSettingsMUI:
            returnValue = onShowSettingsMUI(reinterpret_cast<IWindow*>(inParam.punkVal));
            break;
        case evHideSettingsMUI:
            returnValue = onHideSettingsMUI();
            break;
        case evOnEnterPluginSetupScreen:
            returnValue = onEnterPluginSetupScreen();
            break;
        case evOnLeavePluginSetupScreen:
            returnValue = onLeavePluginSetupScreen();
            break;
        /* ANALYSIS START*/
        case evShowAnalysisMUI:
            returnValue = onShowSetupMUI(reinterpret_cast<IWindow*>(inParam.punkVal));
            break;
        case evHideAnalysisMUI:
        {
            HRESULT leaveAnalysisSetupScreen = onLeavePluginAnalysisSetupScreen();
            HRESULT hideAnalysis = onHideSetupMUI();
            returnValue = leaveAnalysisSetupScreen && hideAnalysis;
        }
        break;
        case evOnEnterAnalysisMode:
            break;
        case evStartAnalysis:
            returnValue = onStartAnalysis();
            break;
        case evPrepareAnalysis:
        {
            IChannelListExPtr channelList;
            HRESULT success = inParam.punkVal->QueryInterface(IID_IChannelListEx, (void**)&channelList);
            if (SUCCEEDED(success))
                returnValue = onPrepareAnalysis(channelList);
        }
        break;
        case evCalculateAnalysis:
            returnValue = onCalculateAnalysis();
            break;
        case evUpdateAnalysisFrame:
            returnValue = onEnterPluginAnalysisSetupScreen();
            break;
        case evStopAnalysis:
            break;
        case evShowAnalysisFrame:
            outParam->boolVal = VARIANT_TRUE;
            returnValue = S_OK;
            break;
        case evHideAnalysisFrame:
            returnValue = S_OK;
            break;
        case evOnEnterAnalysisPluginSetupScreen:
            returnValue = S_OK;
            break;
        case evOnLeaveAnalysisPluginSetupScreen:
            returnValue = S_OK;
            break;
        /* ANALYSIS END */
        case evEstablishConnections:
            returnValue = onEstablishConnections();
            break;
        case evChannelRemoved:
        {
            IChannelPtr channel(inParam.pdispVal);
            returnValue = onChannelRemoved(channel);
        }
        break;
        case evOnUpdateDwHWXML:
        {
            CComSafeArray<VARIANT> sa(inParam.parray);
            IDwXMLDocumentPtr internalDocument(sa[0].pdispVal);
            IDwXMLNodePtr internalNode(sa[1].pdispVal);
            bool write = sa[2].boolVal == VARIANT_TRUE;
            NodePtr node = std::make_shared<DewesoftDCOMNode>(internalDocument, internalNode);
            node->updateOperation(write);
            returnValue = onUpdateDwHwXml(node);
            InitVariantFromUInt32(1, outParam);
            returnValue = S_OK;
        }
        break;
        case evOnUpdateDwXML:
        {
            CComSafeArray<VARIANT> sa(inParam.parray);
            IDwXMLDocumentPtr internalDocument(sa[0].pdispVal);
            IDwXMLNodePtr internalNode(sa[1].pdispVal);
            bool write = sa[2].boolVal == VARIANT_TRUE;
            bool dataFile = sa[3].boolVal == VARIANT_TRUE;
            NodePtr node = std::make_shared<DewesoftDCOMNode>(internalDocument, internalNode);
            node->updateOperation(write);
            returnValue = onUpdateDwXml(node, dataFile);
            InitVariantFromUInt32(1, outParam);
            returnValue = S_OK;
        }
        break;
        case evResizeHWFrame:
        {
            CComSafeArray<VARIANT> sa(inParam.parray);
            int width = sa[0].intVal;
            int height = sa[0].intVal;
            returnValue = eventOnResizeHWFrame(width, height);
            break;
        }
        case evOnInitiateHardware:
            returnValue = onInitiateHardware();
            break;
        case evEnterHardwareSetup:
            returnValue = onEnterHardwareSetup();
            break;
        case evPreInitiate:
            returnValue = onPreInitiate();
            break;
        default:
            break;
    }

    return returnValue;
}

STDMETHODIMP Plugin::raw_LoadSetup(VARIANT data)
{
    return S_OK;
}

STDMETHODIMP Plugin::raw_NewSetup()
{
    const HRESULT clearOk = onClearSetup();
    if (!SUCCEEDED(clearOk))
        return clearOk;

    return onNewSetup();
}

STDMETHODIMP Plugin::raw_ClearChannelsInstance()
{
    return onClearChannelInstances();
}

STDMETHODIMP Plugin::raw_OnStartAcq()
{
    return onStartData();
}

STDMETHODIMP Plugin::raw_OnStartSetup()
{
    return onStartData();
}

STDMETHODIMP Plugin::raw_OnGetSetupData()
{
    return onGetData();
}

STDMETHODIMP Plugin::raw_OnGetData()
{
    return onGetData();
}

STDMETHODIMP Plugin::raw_OnStopAcq()
{
    return onStopData();
}

STDMETHODIMP Plugin::raw_OnStopSetup()
{
    return onStopData();
}

STDMETHODIMP Plugin::raw_OnStartStoring()
{
    return onStartStoring();
}

STDMETHODIMP Plugin::raw_OnStopStoring()
{
    return onStopStoring();
}
