#include "Win32Application.h"
#include <SetupAPI.h>

#if defined(_DEBUG) || defined(DEBUG)

const wchar_t* readableWinMsg(uint32_t msg)
{
    switch (msg)
    {
    case 0x0000:  return  L"WM_NULL";
    case 0x0001:  return  L"WM_CREATE";
    case 0x0002:  return  L"WM_DESTROY";
    case 0x0003:  return  L"WM_MOVE";
    case 0x0005:  return  L"WM_SIZE";
    case 0x0006:  return  L"WM_ACTIVATE";
    case 0x0007:  return  L"WM_SETFOCUS";
    case 0x0008:  return  L"WM_KILLFOCUS";
    case 0x000A:  return  L"WM_ENABLE";
    case 0x000B:  return  L"WM_SETREDRAW";
    case 0x000C:  return  L"WM_SETTEXT";
    case 0x000D:  return  L"WM_GETTEXT";
    case 0x000E:  return  L"WM_GETTEXTLENGTH";
    case 0x000F:  return  L"WM_PAINT";
    case 0x0010:  return  L"WM_CLOSE";
    case 0x0011:  return  L"WM_QUERYENDSESSION";
    case 0x0013:  return  L"WM_QUERYOPEN";
    case 0x0016:  return  L"WM_ENDSESSION";
    case 0x0012:  return  L"WM_QUIT";
    case 0x0014:  return  L"WM_ERASEBKGND";
    case 0x0015:  return  L"WM_SYSCOLORCHANGE";
    case 0x0018:  return  L"WM_SHOWWINDOW";
    case 0x001A:  return  L"WM_WININICHANGE | WM_SETTINGCHANGE";
    case 0x001B:  return  L"WM_DEVMODECHANGE";
    case 0x001C:  return  L"WM_ACTIVATEAPP";
    case 0x001D:  return  L"WM_FONTCHANGE";
    case 0x001E:  return  L"WM_TIMECHANGE";
    case 0x001F:  return  L"WM_CANCELMODE";
    case 0x0020:  return  L"WM_SETCURSOR";
    case 0x0021:  return  L"WM_MOUSEACTIVATE";
    case 0x0022:  return  L"WM_CHILDACTIVATE";
    case 0x0023:  return  L"WM_QUEUESYNC";
    case 0x0024:  return  L"WM_GETMINMAXINFO";
    case 0x0026:  return  L"WM_PAINTICON";
    case 0x0027:  return  L"WM_ICONERASEBKGND";
    case 0x0028:  return  L"WM_NEXTDLGCTL";
    case 0x002A:  return  L"WM_SPOOLERSTATUS";
    case 0x002B:  return  L"WM_DRAWITEM";
    case 0x002C:  return  L"WM_MEASUREITEM";
    case 0x002D:  return  L"WM_DELETEITEM";
    case 0x002E:  return  L"WM_VKEYTOITEM";
    case 0x002F:  return  L"WM_CHARTOITEM";
    case 0x0030:  return  L"WM_SETFONT";
    case 0x0031:  return  L"WM_GETFONT";
    case 0x0032:  return  L"WM_SETHOTKEY";
    case 0x0033:  return  L"WM_GETHOTKEY";
    case 0x0037:  return  L"WM_QUERYDRAGICON";
    case 0x0039:  return  L"WM_COMPAREITEM";
    case 0x003D:  return  L"WM_GETOBJECT";
    case 0x0041:  return  L"WM_COMPACTING";
    case 0x0044:  return  L"WM_COMMNOTIFY";
    case 0x0046:  return  L"WM_WINDOWPOSCHANGING";
    case 0x0047:  return  L"WM_WINDOWPOSCHANGED";
    case 0x0048:  return  L"WM_POWER";
    case 0x004A:  return  L"WM_COPYDATA";
    case 0x004B:  return  L"WM_CANCELJOURNAL";
    case 0x004E:  return  L"WM_NOTIFY";
    case 0x0050:  return  L"WM_INPUTLANGCHANGEREQUEST";
    case 0x0051:  return  L"WM_INPUTLANGCHANGE";
    case 0x0052:  return  L"WM_TCARD";
    case 0x0053:  return  L"WM_HELP";
    case 0x0054:  return  L"WM_USERCHANGED";
    case 0x0055:  return  L"WM_NOTIFYFORMAT";
    case 0x007B:  return  L"WM_CONTEXTMENU";
    case 0x007C:  return  L"WM_STYLECHANGING";
    case 0x007D:  return  L"WM_STYLECHANGED";
    case 0x007E:  return  L"WM_DISPLAYCHANGE";
    case 0x007F:  return  L"WM_GETICON";
    case 0x0080:  return  L"WM_SETICON";
    case 0x0081:  return  L"WM_NCCREATE";
    case 0x0082:  return  L"WM_NCDESTROY";
    case 0x0083:  return  L"WM_NCCALCSIZE";
    case 0x0084:  return  L"WM_NCHITTEST";
    case 0x0085:  return  L"WM_NCPAINT";
    case 0x0086:  return  L"WM_NCACTIVATE";
    case 0x0087:  return  L"WM_GETDLGCODE";
    case 0x0088:  return  L"WM_SYNCPAINT";
    case 0x00A0:  return  L"WM_NCMOUSEMOVE";
    case 0x00A1:  return  L"WM_NCLBUTTONDOWN";
    case 0x00A2:  return  L"WM_NCLBUTTONUP";
    case 0x00A3:  return  L"WM_NCLBUTTONDBLCLK";
    case 0x00A4:  return  L"WM_NCRBUTTONDOWN";
    case 0x00A5:  return  L"WM_NCRBUTTONUP";
    case 0x00A6:  return  L"WM_NCRBUTTONDBLCLK";
    case 0x00A7:  return  L"WM_NCMBUTTONDOWN";
    case 0x00A8:  return  L"WM_NCMBUTTONUP";
    case 0x00A9:  return  L"WM_NCMBUTTONDBLCLK";
    case 0x00AB:  return  L"WM_NCXBUTTONDOWN";
    case 0x00AC:  return  L"WM_NCXBUTTONUP";
    case 0x00AD:  return  L"WM_NCXBUTTONDBLCLK";
    case 0x00FF:  return  L"WM_INPUT";
    case 0x0100:  return  L"WM_KEYDOWN | WM_KEYFIRST";
    case 0x0101:  return  L"WM_KEYUP";
    case 0x0102:  return  L"WM_CHAR";
    case 0x0103:  return  L"WM_DEADCHAR";
    case 0x0104:  return  L"WM_SYSKEYDOWN";
    case 0x0105:  return  L"WM_SYSKEYUP";
    case 0x0106:  return  L"WM_SYSCHAR";
    case 0x0107:  return  L"WM_SYSDEADCHAR";
    case 0x0109:  return  L"WM_KEYLAST | WM_UNICHAR";
    case 0x010D:  return  L"WM_IME_STARTCOMPOSITION";
    case 0x010E:  return  L"WM_IME_ENDCOMPOSITION";
    case 0x010F:  return  L"WM_IME_COMPOSITION | WM_IME_KEYLAST";
    case 0x0110:  return  L"WM_INITDIALOG";
    case 0x0111:  return  L"WM_COMMAND";
    case 0x0112:  return  L"WM_SYSCOMMAND";
    case 0x0113:  return  L"WM_TIMER";
    case 0x0114:  return  L"WM_HSCROLL";
    case 0x0115:  return  L"WM_VSCROLL";
    case 0x0116:  return  L"WM_INITMENU";
    case 0x0117:  return  L"WM_INITMENUPOPUP";
    case 0x011F:  return  L"WM_MENUSELECT";
    case 0x0120:  return  L"WM_MENUCHAR";
    case 0x0121:  return  L"WM_ENTERIDLE";
    case 0x0122:  return  L"WM_MENURBUTTONUP";
    case 0x0123:  return  L"WM_MENUDRAG";
    case 0x0124:  return  L"WM_MENUGETOBJECT";
    case 0x0125:  return  L"WM_UNINITMENUPOPUP";
    case 0x0126:  return  L"WM_MENUCOMMAND";
    case 0x0127:  return  L"WM_CHANGEUISTATE";
    case 0x0128:  return  L"WM_UPDATEUISTATE";
    case 0x0129:  return  L"WM_QUERYUISTATE";
    case 0x0132:  return  L"WM_CTLCOLORMSGBOX";
    case 0x0133:  return  L"WM_CTLCOLOREDIT";
    case 0x0134:  return  L"WM_CTLCOLORLISTBOX";
    case 0x0135:  return  L"WM_CTLCOLORBTN";
    case 0x0136:  return  L"WM_CTLCOLORDLG";
    case 0x0137:  return  L"WM_CTLCOLORSCROLLBAR";
    case 0x0138:  return  L"WM_CTLCOLORSTATIC";
    case 0x0200:  return  L"WM_MOUSEFIRST | WM_MOUSEMOVE";
    case 0x0201:  return  L"WM_LBUTTONDOWN";
    case 0x0202:  return  L"WM_LBUTTONUP";
    case 0x0203:  return  L"WM_LBUTTONDBLCLK";
    case 0x0204:  return  L"WM_RBUTTONDOWN";
    case 0x0205:  return  L"WM_RBUTTONUP";
    case 0x0206:  return  L"WM_RBUTTONDBLCLK";
    case 0x0207:  return  L"WM_MBUTTONDOWN";
    case 0x0208:  return  L"WM_MBUTTONUP";
    case 0x0209:  return  L"WM_MBUTTONDBLCLK";
    case 0x020A:  return  L"WM_MOUSEWHEEL";
    case 0x020B:  return  L"WM_XBUTTONDOWN";
    case 0x020C:  return  L"WM_XBUTTONUP";
    case 0x020D:  return  L"WM_XBUTTONDBLCLK";
    case 0x0210:  return  L"WM_PARENTNOTIFY";
    case 0x0211:  return  L"WM_ENTERMENULOOP";
    case 0x0212:  return  L"WM_EXITMENULOOP";
    case 0x0213:  return  L"WM_NEXTMENU";
    case 0x0214:  return  L"WM_SIZING";
    case 0x0215:  return  L"WM_CAPTURECHANGED";
    case 0x0216:  return  L"WM_MOVING";
    case 0x0218:  return  L"WM_POWERBROADCAST";
    case 0x0219:  return  L"WM_DEVICECHANGE";
    case 0x0220:  return  L"WM_MDICREATE";
    case 0x0221:  return  L"WM_MDIDESTROY";
    case 0x0222:  return  L"WM_MDIACTIVATE";
    case 0x0223:  return  L"WM_MDIRESTORE";
    case 0x0224:  return  L"WM_MDINEXT";
    case 0x0225:  return  L"WM_MDIMAXIMIZE";
    case 0x0226:  return  L"WM_MDITILE";
    case 0x0227:  return  L"WM_MDICASCADE";
    case 0x0228:  return  L"WM_MDIICONARRANGE";
    case 0x0229:  return  L"WM_MDIGETACTIVE";
    case 0x0230:  return  L"WM_MDISETMENU";
    case 0x0231:  return  L"WM_ENTERSIZEMOVE";
    case 0x0232:  return  L"WM_EXITSIZEMOVE";
    case 0x0233:  return  L"WM_DROPFILES";
    case 0x0234:  return  L"WM_MDIREFRESHMENU";
    case 0x0281:  return  L"WM_IME_SETCONTEXT";
    case 0x0282:  return  L"WM_IME_NOTIFY";
    case 0x0283:  return  L"WM_IME_CONTROL";
    case 0x0284:  return  L"WM_IME_COMPOSITIONFULL";
    case 0x0285:  return  L"WM_IME_SELECT";
    case 0x0286:  return  L"WM_IME_CHAR";
    case 0x0288:  return  L"WM_IME_REQUEST";
    case 0x0290:  return  L"WM_IME_KEYDOWN";
    case 0x0291:  return  L"WM_IME_KEYUP";
    case 0x02A1:  return  L"WM_MOUSEHOVER";
    case 0x02A3:  return  L"WM_MOUSELEAVE";
    case 0x02A0:  return  L"WM_NCMOUSEHOVER";
    case 0x02A2:  return  L"WM_NCMOUSELEAVE";
    case 0x02B1:  return  L"WM_WTSSESSION_CHANGE";
    case 0x02C0:  return  L"WM_TABLET_FIRST";
    case 0x02DF:  return  L"WM_TABLET_LAST";
    case 0x0300:  return  L"WM_CUT";
    case 0x0301:  return  L"WM_COPY";
    case 0x0302:  return  L"WM_PASTE";
    case 0x0303:  return  L"WM_CLEAR";
    case 0x0304:  return  L"WM_UNDO";
    case 0x0305:  return  L"WM_RENDERFORMAT";
    case 0x0306:  return  L"WM_RENDERALLFORMATS";
    case 0x0307:  return  L"WM_DESTROYCLIPBOARD";
    case 0x0308:  return  L"WM_DRAWCLIPBOARD";
    case 0x0309:  return  L"WM_PAINTCLIPBOARD";
    case 0x030A:  return  L"WM_VSCROLLCLIPBOARD";
    case 0x030B:  return  L"WM_SIZECLIPBOARD";
    case 0x030C:  return  L"WM_ASKCBFORMATNAME";
    case 0x030D:  return  L"WM_CHANGECBCHAIN";
    case 0x030E:  return  L"WM_HSCROLLCLIPBOARD";
    case 0x030F:  return  L"WM_QUERYNEWPALETTE";
    case 0x0310:  return  L"WM_PALETTEISCHANGING";
    case 0x0311:  return  L"WM_PALETTECHANGED";
    case 0x0312:  return  L"WM_HOTKEY";
    case 0x0317:  return  L"WM_PRINT";
    case 0x0318:  return  L"WM_PRINTCLIENT";
    case 0x0319:  return  L"WM_APPCOMMAND";
    case 0x031A:  return  L"WM_THEMECHANGED";
    case 0x0358:  return  L"WM_HANDHELDFIRST";
    case 0x035F:  return  L"WM_HANDHELDLAST";
    case 0x0360:  return  L"WM_AFXFIRST";
    case 0x037F:  return  L"WM_AFXLAST";
    case 0x0380:  return  L"WM_PENWINFIRST";
    case 0x038F:  return  L"WM_PENWINLAST";
    case 0x0400:  return  L"WM_USER";
    case 0x8000:  return  L"WM_APP";

    default:
        break;
    }
    return L"???";
}

#endif

/**
* Extracts EDID data from the given registry key and reads out native display with and height
* @param hDevRegKey - Registry key where EDID is stored
* @param OutWidth - Reference to output variable for monitor native width
* @param OutHeight - Reference to output variable for monitor native height
* @returns 'true' if data was extracted successfully, 'false' otherwise
**/
static bool getMonitorSizeFromEDID(const HKEY hDevRegKey, int32_t& OutWidth, int32_t& OutHeight)
{
    static const uint32_t NameSize = 512;
    static TCHAR ValueName[NameSize];
    static std::wstring strEDID(L"EDID");

    DWORD Type;
    DWORD ActualValueNameLength = NameSize;

    BYTE EDIDData[1024];
    DWORD EDIDSize = sizeof(EDIDData);

    for (LONG i = 0, RetValue = ERROR_SUCCESS; RetValue != ERROR_NO_MORE_ITEMS; ++i)
    {
        RetValue = RegEnumValue(hDevRegKey,
            i,
            &ValueName[0],
            &ActualValueNameLength, NULL, &Type,
            EDIDData,
            &EDIDSize);

        //if (RetValue != ERROR_SUCCESS || (FCString::Strcmp(ValueName, TEXT("EDID")) != 0))
        if (RetValue != ERROR_SUCCESS || strEDID.compare(ValueName) != 0)
        {
            continue;
        }

        // EDID data format documented here:  http://en.wikipedia.org/wiki/EDID

        int DetailTimingDescriptorStartIndex = 54;
        OutWidth = ((EDIDData[DetailTimingDescriptorStartIndex + 4] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 2];
        OutHeight = ((EDIDData[DetailTimingDescriptorStartIndex + 7] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 5];

        return true; // valid EDID found
    }

    return false; // EDID not found
}

/**
* Locate registry record for the given display device ID and extract native size information
* @param TargetDevID - Name of taret device
* @praam OutWidth - Reference to output variable for monitor native width
* @praam OutHeight - Reference to output variable for monitor native height
* @returns TRUE if data was extracted successfully, FALSE otherwise
**/
inline bool getSizeForDevID(const std::wstring& TargetDevID, int32_t& Width, int32_t& Height)
{
    static const GUID ClassMonitorGuid = { 0x4d36e96e, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };

    HDEVINFO DevInfo = SetupDiGetClassDevsEx(
        &ClassMonitorGuid, //class GUID
        NULL,
        NULL,
        DIGCF_PRESENT,
        NULL,
        NULL,
        NULL);

    if (NULL == DevInfo)
    {
        return false;
    }

    bool bRes = false;

    for (ULONG MonitorIndex = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++MonitorIndex)
    {
        SP_DEVINFO_DATA DevInfoData;
        ZeroMemory(&DevInfoData, sizeof(DevInfoData));
        DevInfoData.cbSize = sizeof(DevInfoData);

        if (SetupDiEnumDeviceInfo(DevInfo, MonitorIndex, &DevInfoData) == TRUE)
        {
            HKEY hDevRegKey = SetupDiOpenDevRegKey(DevInfo, &DevInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

            if (!hDevRegKey || (hDevRegKey == INVALID_HANDLE_VALUE))
            {
                continue;
            }

            bRes = getMonitorSizeFromEDID(hDevRegKey, Width, Height);

            RegCloseKey(hDevRegKey);
        }
    }

    if (SetupDiDestroyDeviceInfoList(DevInfo) == FALSE)
    {
        bRes = false;
    }

    return bRes;
}

/**
* Extract hardware information about connect monitors
* @param OutMonitorInfo - Reference to an array for holding records about each detected monitor
**/
void getMonitorInfo(std::vector<PhysicalMonitorInfo>& outMonitorInfo)
{
    ::DISPLAY_DEVICE DisplayDevice;
    DisplayDevice.cb = sizeof(DisplayDevice);
    DWORD DeviceIndex = 0; // device index

    outMonitorInfo.clear();
    outMonitorInfo.reserve(2);// Reserve two slots, as that will be the most common maximum

    std::wstring DeviceID;
    while (EnumDisplayDevices(0, DeviceIndex, &DisplayDevice, 0))
    {
        if ((DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) > 0)
        {
            DISPLAY_DEVICE Monitor;
            ZeroMemory(&Monitor, sizeof(Monitor));
            Monitor.cb = sizeof(Monitor);
            DWORD MonitorIndex = 0;

            while (EnumDisplayDevices(DisplayDevice.DeviceName, MonitorIndex, &Monitor, 0))
            {
                if (Monitor.StateFlags & DISPLAY_DEVICE_ACTIVE &&
                    !(Monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
                {
                    PhysicalMonitorInfo Info;

                    //Info.ID = FString::Printf(TEXT("%s"), Monitor.DeviceID);
                    Info.ID = Monitor.DeviceID;
                    //Info.Name = Info.ID.Mid(8, Info.ID.Find(TEXT("\\"), ESearchCase::IgnoreCase, ESearchDir::FromStart, 9) - 8);

                    if (getSizeForDevID(Info.Name, Info.NativeWidth, Info.NativeHeight))
                    {
                        Info.ID = Monitor.DeviceID;
                        Info.IsPrimary = (DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;
                        outMonitorInfo.push_back(Info);
                    }
                }
                MonitorIndex++;

                ZeroMemory(&Monitor, sizeof(Monitor));
                Monitor.cb = sizeof(Monitor);
            }
        }

        ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
        DisplayDevice.cb = sizeof(DisplayDevice);
        DeviceIndex++;
    }
}

Win32Application* Win32Application::mAppInstance = nullptr;

Win32Application::Win32Application(MessageHandler* msgHandler, const HINSTANCE instanceHandle, const HICON hIcon) : 
    mInstanceHandle(instanceHandle), 
    mIconHandle(hIcon),
    mMsgHandler(msgHandler),
    mIsInModalSizingLoop(false)
{
    registerWin32Window(mInstanceHandle, mIconHandle);
    getDisplayMetrics(mInitialDisplayMetrics);
}

Win32Application::~Win32Application()
{

}

Win32Window* Win32Application::createWin32Window()
{
    return Win32Window::createUninitialized();
}

void Win32Application::addWin32Window(const Win32WindowDefinition& definition, Win32Window* wnd, Win32Window* parent)
{
    // Window must be added to that array before the call to initialize(), which raises some win32 messages
    // that we might want to handle in application's wndProc function.
    // Sidenote: messages sent from ::CreateWindowEx() are not of our interest thus routed to DefWndProc().
    mWin32Windows.push_back(wnd);
    wnd->initialize(this, definition, mInstanceHandle, parent );
}

bool Win32Application::pumpMsg()
{
    bool hasMsg = false;
    MSG msg = { 0 };

    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        hasMsg = true;
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    return hasMsg;
}


void Win32Application::registerWin32Window(const HINSTANCE hInstance, const HICON hIcon)
{
    WNDCLASS wc = { 0 };

    wc.style         = CS_DBLCLKS; // We want to receive double clicks
    wc.lpfnWndProc   = wndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = hIcon;
    wc.hCursor       = NULL;	// We manage the cursor ourselves
    wc.hbrBackground = NULL; // Transparent
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = Win32Window::ClassName;

    ::RegisterClass(&wc);
}

LRESULT CALLBACK Win32Application::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return Win32Application::getInstance()->processMsg(hwnd, uMsg, wParam, lParam);
}

int32_t Win32Application::processMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

#if defined(_DEBUG) || defined(DEBUG)
    const wchar_t* readable_win_msg = readableWinMsg(uMsg);
#endif


    Win32Window* currWnd = getWin32WindowByHwnd(hwnd);
    // Make sure any message from CreateWindow() is forwarded to DefWindowProc(), as currentWindow is not valid 
    // at this point of time.
    //if (!mWin32Windows.empty() && currWnd != nullptr)
    if (currWnd != nullptr && currWnd->getHwnd() != nullptr)
    {
        // return DefWindowProc(hwnd, uMsg, wParam, lParam);

        // This effectively disables a window without actually disabling it natively with the OS.
        // This allows us to continue receiving messages for it.
        if (!mMsgHandler->shouldProcessUserInputMessages(currWnd) && isInputMessage(uMsg))
        {
            return 0;   // consume input messages
        }

        switch (uMsg)
        {
/************************************************************************
Quick Reminder
- return 1
    WndDefProc() should furthur handle the msg
- return 0
    msg gets intercepted/handled here
- break
    msg passed through to OS::DefWindowProc()
************************************************************************/ 

/************************************************************************/
/* Keyboard Input                                                       */
/************************************************************************/

        case WM_CHAR:
        {
            //
            // Character code is stored in WPARAM
            const wchar_t character = wParam;

            // LPARAM bit 30 will be ZERO for new presses, or ONE if this is a repeat
            const bool bIsRepeat = (lParam & 0x40000000) != 0;

            mMsgHandler->onKeyChar(character, bIsRepeat);

            // Note: always return 0 to handle the message.  Win32 beeps if WM_CHAR is not handled...
            return 0;
        }
            break;

        case WM_SYSCHAR:
        {
            // Do not handle Alt+Space so that it passes through and opens the window system menu
            if ((HIWORD(lParam) & 0x2000) != 0 && wParam == VK_SPACE)
               break;
            else
               return 0;
        }
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            const int32_t key = wParam;

            // Alt-F4 or Alt+Space was pressed. 
            // Allow alt+f4 to close the window and alt+space to open the window menu
            if ( uMsg == WM_SYSKEYDOWN && (key == VK_F4 || key== VK_SPACE) )
            { 
                break;
            }
            else
            {
                bool bIsRepeat = (lParam & 0x40000000) != 0;
                if ( uMsg == WM_SYSKEYDOWN && (key == VK_MENU || key ==  VK_CONTROL || key == VK_SHIFT) )
                {
                    std::pair<std::set<int32_t>::iterator, bool> ret = mPressedModifierKeys.insert(key);
                    bIsRepeat = !ret.second;
                }

                uint32_t charCode = ::MapVirtualKey(key, MAPVK_VK_TO_CHAR);
                mMsgHandler->onKeyDown(key, charCode, bIsRepeat);
                return 0;
            }
        }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            int32_t key = wParam;
            if (key == VK_SHIFT || key == VK_CONTROL || key == VK_MENU)
            {
                mPressedModifierKeys.erase(key);
            }
            uint32_t CharCode = ::MapVirtualKey(key, MAPVK_VK_TO_CHAR);
            mMsgHandler->onKeyUp(key, CharCode, false);
            return 0;
        }
            break;

/************************************************************************/
/* Mouse Input                                                       */
/************************************************************************/

        case WM_LBUTTONDOWN:
            mMsgHandler->onMouseDown(currWnd, EMouseButtons::Left);
            return 0;

        case WM_LBUTTONDBLCLK:
            mMsgHandler->onMouseDoubleClick(currWnd, EMouseButtons::Left);
            return 0;

        case WM_MBUTTONDOWN:
            mMsgHandler->onMouseDown(currWnd, EMouseButtons::Middle);
            return 0;

        case WM_MBUTTONDBLCLK:
            mMsgHandler->onMouseDoubleClick(currWnd, EMouseButtons::Middle);
            return 0;

        case WM_RBUTTONDOWN:
            mMsgHandler->onMouseDown(currWnd, EMouseButtons::Right);
            return 0;

        case WM_RBUTTONDBLCLK:
            mMsgHandler->onMouseDoubleClick(currWnd, EMouseButtons::Right);
            return 0;

        case WM_LBUTTONUP:
            mMsgHandler->onMouseUp(EMouseButtons::Left);
            return 0;

        case WM_MBUTTONUP:
            mMsgHandler->onMouseUp(EMouseButtons::Middle);
            return 0;

        case WM_RBUTTONUP:
            mMsgHandler->onMouseUp(EMouseButtons::Right);
            return 0;

        case WM_NCMOUSEMOVE:
        case WM_MOUSEMOVE:
            mMsgHandler->onMouseMove();
            return 0;

        case WM_MOUSEWHEEL:
        {
          const float SpinFactor = 1 / 120.0f;
          const SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

          mMsgHandler->onMouseWheel(static_cast<float>(WheelDelta)* SpinFactor);
        }
          return 0;

            /*
            Sent to a window if the mouse causes the cursor to move within a window and mouse input is not captured.

            wParam
                A handle to the window that contains the cursor.
            lParam
                The low-order word of lParam specifies the hit-test code.
                The high-order word of lParam specifies the identifier of the mouse message.
            */
        case WM_SETCURSOR:
            mMsgHandler->onCursorSet();
            return 0;

            // Sent to the window that is getting raw input.
        case WM_INPUT:
            // TODO: support raw input
            break;

/************************************************************************/
/* Application -- Activation                                           */
/************************************************************************/

        case WM_ACTIVATE:
        {
            /* Sent to both the window being activated and the window being deactivated. If the windows use 
            the same input queue, the message is sent synchronously, first to the window procedure of the 
            top-level window being deactivated, then to the window procedure of the top-level window being 
            activated. If the windows use different input queues, the message is sent asynchronously, so 
            the window is activated immediately.  */

            EWindowActivation ActivationType;

            if (LOWORD(wParam) & WA_ACTIVE)
            {
                ActivationType = EWindowActivation::Activate;
            }
            else if (LOWORD(wParam) & WA_CLICKACTIVE)
            {
                ActivationType = EWindowActivation::ActivateByMouse;
            }
            else
            {
                ActivationType = EWindowActivation::Deactivate;
            }

            mMsgHandler->onWindowActivationChanged(currWnd, ActivationType);
        }
            break;

        case WM_ACTIVATEAPP:
        {
            /* Sent when a window belonging to a different application than the active window is about to be 
            activated. The message is sent to the application whose window is being activated and to the 
            application whose window is being deactivated.  */

            // When window activation changes we are not in a modal size loop
            mIsInModalSizingLoop = false;

            mMsgHandler->onApplicationActivationChanged(!!wParam);
        }
            break;


        case WM_NCACTIVATE:
            /*Sent to a window when its nonclient area needs to be changed to indicate an active or inactive state.*/
            if (!currWnd->getDefinition().HasOSWindowBorder)
            {
                // Unless using the OS window border, intercept calls to prevent non-client area drawing a border 
                // upon activation or deactivation

                // Return true to ensure standard activation happens
                return true;
            }
            break;

/************************************************************************/
/* Window -- Resize                                                     */
/************************************************************************/

        case WM_NCCALCSIZE:
            // Let windows absorb this message if using the standard border
            if (wParam && !currWnd->getDefinition().HasOSWindowBorder)
            {
                return 0;
            }
            break;

        case WM_SIZE: /*Sent to a window after its size has changed.  */
            {
                // @todo Fullscreen - Perform deferred resize
                // Note WM_SIZE provides the client dimension which is not equal to the window dimension if 
                // there is a windows border 
                const int32_t NewWidth = (int)(short)(LOWORD(lParam));
                const int32_t NewHeight = (int)(short)(HIWORD(lParam));

                const Win32WindowDefinition& Definition = currWnd->getDefinition();
                if (Definition.IsRegularWindow && !Definition.HasOSWindowBorder)
                {
                    currWnd->setWindowRegion(NewWidth, NewHeight);
                }

                const bool bWasMinimized = (wParam == SIZE_MINIMIZED);

                mMsgHandler->onSizeChanged(currWnd, NewWidth, NewHeight, bWasMinimized);
            }
#ifdef PASS_THROUGH
            break;
#else
            return 0;
#endif

        case WM_SIZING: 
            /* Sent to a window that the user is resizing. By processing this message, an application can 
            monitor the size and position of the drag rectangle and, if needed, change its size or position.*/
            mMsgHandler->onResizingWindow(currWnd);
            break;

        case WM_ENTERSIZEMOVE: 
            /* Sent one time to a window after it enters the moving or sizing modal loop. The window enters the moving
            or sizing modal loop when the user clicks the window's title bar or sizing border, or when the window passes 
            the WM_SYSCOMMAND message to the DefWindowProc function and the wParam parameter of the message specifies the 
            SC_MOVE or SC_SIZE value. The operation is complete when DefWindowProc returns.  */
            mIsInModalSizingLoop = true;
            mMsgHandler->beginReshapingWindow(currWnd);
            break;

        case WM_EXITSIZEMOVE: 
            mIsInModalSizingLoop = false;
            mMsgHandler->finishedReshapingWindow(currWnd);
            break;

/************************************************************************/
/* Window -- Move                                                       */
/************************************************************************/
        case WM_MOVE: /*Sent after a window has been moved.*/
        {
            // client area position
            const int32_t NewX = (int)(short)(LOWORD(lParam));
            const int32_t NewY = (int)(short)(HIWORD(lParam));

            // Only cache the screen position if its not minimized
            mMsgHandler->onMovedWindow(currWnd, NewX, NewY);
        }
            return 0;

/************************************************************************/
/* Window -- HitTest                                                    */
/************************************************************************/

        case WM_NCHITTEST:
        {
         /* Sent to a window in order to determine what part of the window corresponds to a particular screen coordinate. 
         This can happen, for example, when the cursor moves, when a mouse button is pressed or released, or in response 
         to a call to a function such as WindowFromPoint. If the mouse is not captured, the message is sent to the window 
         beneath the cursor. Otherwise, the message is sent to the window that has captured the mouse.  */

            // Only needed if not using the OS window border as this is determined automatically
            if (!currWnd->getDefinition().HasOSWindowBorder)
            { 
                RECT rcWindow; 
                ::GetWindowRect(hwnd, &rcWindow);

                const int32_t LocalMouseX = (int)(short)(LOWORD(lParam)) - rcWindow.left;
                const int32_t LocalMouseY = (int)(short)(HIWORD(lParam)) - rcWindow.top;
                if (currWnd->isRegularWindow())
                {
                    EWindowZone Zone;
                    {
                        Zone = mMsgHandler->getWindowZoneForPoint(currWnd, LocalMouseX, LocalMouseY);
                    }

                    static const LRESULT Results[] = { HTNOWHERE, HTTOPLEFT, HTTOP, HTTOPRIGHT, HTLEFT, HTCLIENT,
                        HTRIGHT, HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT,
                        HTCAPTION, HTMINBUTTON, HTMAXBUTTON, HTCLOSE, HTSYSMENU };

                    return Results[Zone];
                }
            }
        }
            break;

/************************************************************************/
/* Window -- Paint                                                    */
/************************************************************************/
        case WM_PAINT:
            if (mIsInModalSizingLoop)
            {
                mMsgHandler->onOSPaint(currWnd);
            }
            break;

        case WM_ERASEBKGND:
            // Intercept background erasing to eliminate flicker.
            // Return non-zero to indicate that we'll handle the erasing ourselves
            return 1;

        case WM_NCPAINT:
            // Unless using the OS window border, intercept calls to draw the non-client area - we do this ourselves
            if (!currWnd->getDefinition().HasOSWindowBorder)
            {
                return 0;
            }
            break;

/************************************************************************/
/* Window -- Life Cycle                                                    */
/************************************************************************/

        case WM_CREATE: 
            return 0;

        case WM_CLOSE:
            // Sent when the OS close button is pressed
            mMsgHandler->onWindowClose(currWnd);
            return 0;

        case WM_DESTROY:
            removeWin32Window(currWnd);
            return 0;

/************************************************************************/
/* Misc                                                                     */
/************************************************************************/

        case WM_SYSCOMMAND:
        {
              switch (wParam & 0xfff0)
              {
              case SC_RESTORE:
                  // Checks to see if the window is minimized.
                  if (IsIconic(hwnd))
                  {
                      // This is required for restoring a minimized fullscreen window
                      ::ShowWindow(hwnd, SW_RESTORE);
                      return 0;
                  }
                  else
                  {
                      if (!mMsgHandler->onWindowAction(currWnd, EWindowAction::Restore))
                      {
                          return 1;
                      }
                  }
                  break;

              case SC_MAXIMIZE:
              {
                  if (!mMsgHandler->onWindowAction(currWnd, EWindowAction::Maximize))
                  {
                      return 1;
                  }
              }
                  break;

              default:
                  if (!(mMsgHandler->shouldProcessUserInputMessages(currWnd) && isInputMessage(uMsg)))
                  {
                      return 0;
                  }
                  break;
              }
        }
            break;

        case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:
        {
             switch (wParam)
             {
             case HTMINBUTTON:
             case HTMAXBUTTON:
             case HTCLOSE:
             case HTCAPTION:
             {
                 if (!mMsgHandler->onWindowAction(currWnd, EWindowAction::ClickedNonClientArea))
                 {
                     return 1;
                 }
             }
                 break;
             }
        }
            break;

        case WM_GETDLGCODE:
        {
              // Slate wants all keys and messages.
              return DLGC_WANTALLKEYS;
        }
            break;

        default:
            break;
        }
        //return currWnd->handleMsg(uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);;
}

Win32Window* Win32Application::getWin32WindowByHwnd(HWND hwnd)
{
    for (auto wnd : mWin32Windows)
    {
        if (wnd->getHwnd() == hwnd)
        {
            return wnd;
        }
    }
    return nullptr;
}

void Win32Application::setCapture(const Win32Window* win32Window)
{
    if (win32Window)
        ::SetCapture(win32Window->getHwnd());
    else
        ::SetCapture(NULL);
}

void* Win32Application::getCapture()
{
    return ::GetCapture();
}

ModifierKeysState Win32Application::getModifierKeys() const
{
    const bool bIsLeftShiftDown    = (::GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0;
    const bool bIsRightShiftDown   = (::GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
    const bool bIsLeftControlDown  = (::GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
    const bool bIsRightControlDown = (::GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
    const bool bIsLeftAltDown      = (::GetAsyncKeyState(VK_LMENU) & 0x8000) != 0;
    const bool bIsRightAltDown     = (::GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;

    return ModifierKeysState(bIsLeftShiftDown, bIsRightShiftDown, bIsLeftControlDown, bIsRightControlDown, bIsLeftAltDown, bIsRightAltDown);
}


void Win32Application::getDisplayMetrics(DisplayMetrics& outDisplayMetrics) const
{
    // Total screen size of the primary monitor
    outDisplayMetrics.PrimaryDisplayWidth  = ::GetSystemMetrics(SM_CXSCREEN);
    outDisplayMetrics.PrimaryDisplayHeight = ::GetSystemMetrics(SM_CYSCREEN);

    // Get the screen rect of the primary monitor, excluding taskbar etc.
    RECT WorkAreaRect;
    WorkAreaRect.top = WorkAreaRect.bottom = WorkAreaRect.left = WorkAreaRect.right = 0;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
    outDisplayMetrics.PrimaryDisplayWorkAreaRect.Left   = WorkAreaRect.left;
    outDisplayMetrics.PrimaryDisplayWorkAreaRect.Top    = WorkAreaRect.top;
    outDisplayMetrics.PrimaryDisplayWorkAreaRect.Right  = 1 + WorkAreaRect.right;
    outDisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom = 1 + WorkAreaRect.bottom;

    // Virtual desktop area
    outDisplayMetrics.VirtualDisplayRect.Left   = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
    outDisplayMetrics.VirtualDisplayRect.Top    = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
    outDisplayMetrics.VirtualDisplayRect.Right  = 1 + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
    outDisplayMetrics.VirtualDisplayRect.Bottom = 1 + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // Get connected monitor information
    getMonitorInfo(outDisplayMetrics.MonitorInfo);
}

void Win32Application::getInitialDisplayMetrics(DisplayMetrics& outDisplayMetrics) const
{
    outDisplayMetrics = mInitialDisplayMetrics;
}

bool Win32Application::removeWin32Window(const Win32Window* wnd)
{
    bool erased = false;
    for (auto iter = mWin32Windows.begin(); iter != mWin32Windows.end(); ++iter)
    {
        if (*iter == wnd)
        {
            mWin32Windows.erase(iter);
            erased = true;
            break;
        }
    }
    return erased;
}

bool Win32Application::isInputMessage(uint32_t msg)
{
    switch (msg)
    {
        // Keyboard input notification messages...
    case WM_CHAR:
    case WM_SYSCHAR:
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYUP:
    case WM_SYSCOMMAND:
        // Mouse input notification messages...
    case WM_MOUSEHWHEEL:
    case WM_MOUSEWHEEL:
    case WM_MOUSEHOVER:
    case WM_MOUSELEAVE:
    case WM_MOUSEMOVE:
    case WM_NCMOUSEHOVER:
    case WM_NCMOUSELEAVE:
    case WM_NCMOUSEMOVE:
    case WM_NCMBUTTONDBLCLK:
    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP:
    case WM_NCRBUTTONDBLCLK:
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCXBUTTONDBLCLK:
    case WM_NCXBUTTONDOWN:
    case WM_NCXBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_XBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        // Raw input notification messages...
    case WM_INPUT:
    case WM_INPUT_DEVICE_CHANGE:
        return true;
    }
    return false;
}







