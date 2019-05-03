#include "kdb_common.h"
#include "kdb_util.h"
#include "kdb_math.h"
#include "kdb_platform.h"

namespace kadabra {

    void
    Win32LogErrorString(DWORD ErrCode){
        char *MessageBuffer = 0;
        DWORD FormatResult = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    0,
                    ErrCode,
                    MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                    MessageBuffer,
                    0, 0);
        if(FormatResult){
            Error("%s", MessageBuffer);
        } else {
            Error("FormatMessage failed!");
        }
        if(MessageBuffer){
            LocalFree(MessageBuffer);
        }
    }
    
    void
    Win32LogErrorString(){
        DWORD ErrCode = GetLastError();
        Win32LogErrorString(ErrCode);
    }

    LRESULT CALLBACK 
    Win32MainWindowCallback(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam) {
        LRESULT Result = 0;
    
        window_event_tracker *EventTracker = (window_event_tracker *)
                                            GetWindowLongPtr(WindowHandle,
                                                             GWLP_USERDATA);
        
        switch (Message) {
            case WM_CLOSE:{
                // NOTE(furkan) : Remarks section for WM_CLOSE in MSDN 
                // is as follows : 
                // "An application can prompt the user for confirmation, prior to 
                // destroying a window, by processing the WM_CLOSE message and 
                // calling the DestroyWindow function only if the user confirms 
                // the choice." 
                DestroyWindow(WindowHandle);
                
                Log("WM_CLOSE");
            } break;
            case WM_DESTROY:{
                PostQuitMessage(0);
                
                Log("WM_DESTROY");
            } break;
            case WM_QUIT:{
                // NOTE(furkan) : This message should be handled in 
                // ProcessPlatformMessages.
                // This branch is practically unreachable.
            
                Log("WM_QUIT");
            } break;
            case WM_MOVE:{
                // S16 X =  LParam & 0x0000FFFF;
                // S16 Y = (LParam & 0xFFFF0000) >> 16;
                // 
                // Log("WM_MOVE | X : %hd, Y : %hd", X, Y);
            } break;
            case WM_SIZE:{
                // u32 Type = (u32)WParam;
                // switch(Type){
                //     case SIZE_RESTORED:{
                //     } break;
                //     case SIZE_MINIMIZED:{
                //     } break;
                //     case SIZE_MAXIMIZED:{
                //     } break;
                //     case SIZE_MAXSHOW:{
                //     } break;
                //     case SIZE_MAXHIDE:{
                //     } break;
                //     default:{
                //         Warning("WM_SIZE | Unexpected window resize type : %u", 
                //                                                          Type);
                //     } break;
                // }
                // 
                // u16 Width  =  LParam & 0x0000FFFF;
                // u16 Height = (LParam & 0xFFFF0000) >> 16;
                // 
                // Log("WM_SIZE | W : %hu, H : %hu, Type : %u", Width, Height, Type);
            } break;
            case WM_ACTIVATE:{
                u16 ActivationState = WParam & 0x0000FFFF;
                // b8 IsMinimised = ((WParam & 0xFFFF0000) >> 16) != 0;
                // HWND Handle = (HWND)LParam;
                
                switch(ActivationState){
                    case WA_INACTIVE:{
                        // Paused = true;
                    } break;
                    case WA_ACTIVE:
                    case WA_CLICKACTIVE:{
                        // Paused = false;
                    } break;
                    default:{
                        Warning("WM_ACTIVATE | Unexpected activation state : %hu", 
                                                                ActivationState);
                    } break;
                }
                
                Log("WM_ACTIVATE | Activation state : %hu", ActivationState);
            } break;
            // NOTE(furkan) : Background color that becomes visible when 
            // resizing can be set in WM_ERASEBKGND message.
            case WM_ERASEBKGND:{
                HDC DeviceContext = (HDC) WParam;
                if(DeviceContext){
                    RECT Rect;
                    if(GetClientRect(WindowHandle, &Rect)){
                    HBRUSH BackgroundBrush = CreateSolidBrush(RGB(0, 0, 0));
                        Result = FillRect(DeviceContext, &Rect, BackgroundBrush);
                        if(!Result){
                            Warning("FillRect failed!");
                        }
                    }
                    
                    ReleaseDC(WindowHandle, DeviceContext);
                }
                Log("WM_ERASEBKGND");
            } break;
            case WM_ACTIVATEAPP:{
                if(EventTracker){
                    EventTracker->ApplicationHasFocus = (b32)WParam;
                }
                
                Log("WM_ACTIVATEAPP | %s", ((WParam)?"Activated":"Deactivated"));
            } break;
            // NOTE(furkan) : WM_WINDOWPOSCHANGING and WM_WINDOWPOSCHANGED 
            // messages block WM_MOVE and WM_SIZE
            case WM_WINDOWPOSCHANGING:{
                WINDOWPOS * WindowPos = (WINDOWPOS *) LParam;
                
                if(!(WindowPos->flags & SWP_NOMOVE)){
                    // u32 WindowTopLeftX = WindowPos->x;
                    // u32 WindowTopLeftY = WindowPos->y;
                }
                
                if(!(WindowPos->flags & SWP_NOSIZE)){
                    // u32 WindowWidth = WindowPos->cx;
                    // u32 WindowHeight = WindowPos->cy;
                }
                
                Log("WM_WINDOWPOSCHANGING | W:%d, H:%d, X:%d, Y:%d", 
                                            WindowPos->cx, WindowPos->cy, 
                                            WindowPos->x, WindowPos->y);
            } break;
            case WM_WINDOWPOSCHANGED:{
                WINDOWPOS * WindowPos = (WINDOWPOS *) LParam;
                
                if(!(WindowPos->flags & SWP_NOMOVE)){
                    // s32 WindowTopLeftX = WindowPos->x;
                    // s32 WindowTopLeftY = WindowPos->y;
                }
                
                if(!(WindowPos->flags & SWP_NOSIZE)){
                    s32 WindowWidth = WindowPos->cx;
                    s32 WindowHeight = WindowPos->cy;
    
                    if(EventTracker){
                        EventTracker->Resized = true;
                        EventTracker->NewSize = Vec2i(WindowWidth, WindowHeight);
                    }
                }
                
                Log("WM_WINDOWPOSCHANGED | W:%d, H:%d, X:%d, Y:%d", 
                                        WindowPos->cx, WindowPos->cy, 
                                        WindowPos->x, WindowPos->y);
            } break;
    #if 0
            // NOTE(furkan) : WM_NCCALCSIZE can be used in order to create 
            // a window without borders
            case WM_NCCALCSIZE:{
                b32 IndicationNeeded = (b32) WParam;
                if(IndicationNeeded){
                    // NCCALCSIZE_PARAMS * Params = (NCCALCSIZE_PARAMS *) LParam;
                } else {
                    // RECT * Params = (RECT *) LParam;
                }
                Log("WM_NCCALCSIZE");
            } break;
    #endif
            // NOTE(furkan) : WM_SIZING and WM_MOVING return frame+client 
            // area unlike WM_SIZE and WM_MOVE which return just client area. 
            // Use WM_SIZE and WM_MOVE instead of WM_SIZING and WM_MOVING.
            case WM_SIZING:{
                // TODO(furkan) : Force aspect ratio using this message
                // u32 Edge = (u32) WParam;
                // RECT * Rect = (RECT *) LParam;
                // 
                // s32 Top = Rect->top;
                // s32 Left = Rect->left;
                // s32 Bottom = Rect->bottom;
                // s32 Right = Rect->right;
                // 
                // s32 Width = Right - Left;
                // s32 Height = Bottom - Top;
                // 
                // Log("WM_SIZING | W:%d, H:%d, L:%d, T:%d, R:%d, B:%d, Edge: %x", 
                //                  Width, Height, Left, Top, Right, Bottom, Edge);
                
                Result = true;
            } break;
            case WM_MOVING:{
                // RECT * Rect = (RECT *) LParam;
                // 
                // s32 Top = Rect->top;
                // s32 Left = Rect->left;
                // s32 Bottom = Rect->bottom;
                // s32 Right = Rect->right;
                // 
                // s32 Width = Right - Left;
                // s32 Height = Bottom - Top;
                // 
                // Log("WM_MOVING | W:%d, H:%d, L:%d, T:%d, R:%d, B:%d", 
                //                  Width, Height, Left, Top, Right, Bottom);
                
                Result = true;
            } break;
            case WM_POWERBROADCAST:{
                // TODO(furkan) : WM_POWERBROADCAST case should be tested on 
                // laptop. Currently, it this case block is not tested and prone 
                // to fail.
                
                u32 Event = (u32) WParam;
                switch(Event){
                    case PBT_APMSUSPEND:{
                        // NOTE(furkan) : Notifies applications that the computer
                        // is about to enter a suspended state. This event is 
                        // typically broadcast when all applications and installable 
                        // drivers have returned TRUE to a previous 
                        // PBT_APMQUERYSUSPEND event.
                        Log("WM_POWERBROADCAST | PBT_APMSUSPEND");
                    } break;
                    case PBT_APMRESUMESUSPEND:{
                        // NOTE(furkan) : Notifies applications that the system has 
                        // resumed operation after being suspended.
                        Log("WM_POWERBROADCAST | PBT_APMRESUMESUSPEND");
                    } break;
                    case PBT_APMPOWERSTATUSCHANGE:{
                        // NOTE(furkan) : Notifies applications of a change in the 
                        // power status of the computer, such as a switch from 
                        // battery power to A/C. The system also broadcasts this 
                        // event when remaining battery power slips below the 
                        // threshold specified by the user or if the battery power 
                        // changes by a specified percentage.
                        Log("WM_POWERBROADCAST | PBT_APMPOWERSTATUSCHANGE");
                    } break;
                    case PBT_APMRESUMEAUTOMATIC:{
                        // NOTE(furkan) : Notifies applications that the system is 
                        // resuming from sleep or hibernation. This event is 
                        // delivered every time the system resumes and does not 
                        // indicate whether a user is present. 
                        Log("WM_POWERBROADCAST | PBT_APMRESUMEAUTOMATIC");
                    } break;
                    case PBT_POWERSETTINGCHANGE:{
                        // NOTE(furkan) : Detailed information about power setting 
                        // GUIDs can be found on MSDN : 
                        // https://msdn.microsoft.com/en-us/library/windows/desktop/hh448380(v=vs.85).aspx
                        POWERBROADCAST_SETTING * Settings = 
                                                (POWERBROADCAST_SETTING *) LParam;
                        GUID PowerSetting = Settings->PowerSetting;
                        
                        if(IsEqualGUID(PowerSetting, GUID_ACDC_POWER_SOURCE)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            SYSTEM_POWER_CONDITION PowerSource = 
                                        *(SYSTEM_POWER_CONDITION*) &Settings->Data;
                            switch(PowerSource){
                                case PoAc:{
                                    // NOTE(furkan) : The computer is powered by 
                                    // an AC power source (or similar, such as a 
                                    // laptop powered by a 12V automotive adapter).
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_ACDC_POWER_SOURCE | PoAc");
                                } break;
                                case PoDc:{
                                    // NOTE(furkan) : The system is receiving power
                                    // from built-in batteries.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_ACDC_POWER_SOURCE | PoDc");
                                } break;
                                case PoHot:{
                                    // NOTE(furkan) : The computer is powered by a 
                                    // short-term power source such as a UPS 
                                    // device.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_ACDC_POWER_SOURCE | PoHot");
                                } break;
                                default:{
                                    Warning("Unexpected power source");
                                } break;
                            }
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_BATTERY_PERCENTAGE_REMAINING)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 RemaniningBatteryPercentage = 
                                                *(s32*) &Settings->Data;
                            Assert(RemaniningBatteryPercentage >= 0 && 
                                RemaniningBatteryPercentage <= 100);
                            Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_BATTERY_PERCENTAGE_REMAINING");
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_CONSOLE_DISPLAY_STATE)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 DisplayState = *(s32*) &Settings->Data;
                            switch(DisplayState){
                                case 0x0:{
                                    // NOTE(furkan) : The display is off.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_CONSOLE_DISPLAY_STATE | 0x0");
                                } break;
                                case 0x1:{
                                    // NOTE(furkan) : The display is on.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_CONSOLE_DISPLAY_STATE | 0x1");
                                } break;
                                case 0x2:{
                                    // NOTE(furkan) : The display is dimmed.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_CONSOLE_DISPLAY_STATE | 0x2");
                                } break;
                                default:{
                                    Warning("Unexpected display state");
                                } break;
                            }
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_GLOBAL_USER_PRESENCE)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 Presence = *(s32*) &Settings->Data;
                            switch(Presence){
                                case 0x0:{
                                    // NOTE(furkan) : The user is present in any 
                                    // local or remote session on the system.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_GLOBAL_USER_PRESENCE | 0x0");
                                } break;
                                case 0x2:{
                                    // NOTE(furkan) : The user is not present in 
                                    // any local or remote session on the system.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_GLOBAL_USER_PRESENCE | 0x2");
                                } break;
                                default:{
                                    Warning("Unexpected presence value");
                                } break;
                            }
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_IDLE_BACKGROUND_TASK)){
                            // NOTE(furkan) : There is no notification when the 
                            // system is able to move into an idle state. The idle 
                            // background task notification does not indicate 
                            // whether a user is present at the computer. The Data 
                            // member has no information and can be ignored.
                            Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_IDLE_BACKGROUND_TASK");
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_MONITOR_POWER_ON)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 MonitorPower = *(s32*) &Settings->Data;
                            switch(MonitorPower){
                                case 0x0:{
                                    // NOTE(furkan) : The monitor is off.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_MONITOR_POWER_ON | 0x0");
                                } break;
                                case 0x1:{
                                    // NOTE(furkan) : The monitor is on.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_MONITOR_POWER_ON | 0x1");
                                } break;
                                default:{
                                    Warning("Unexpected monitor power state");
                                } break;
                            }
                        // // NOTE(furkan) : GUID_POWER_SAVING_STATUS is a new 
                        // // GUID which is available on Windows 10 and later.
                        // } else if(IsEqualGUID(PowerSetting, 
                        //                       GUID_POWER_SAVING_STATUS)){
                        //     Assert(Settings->DataLength == sizeof(DWORD));
                        //     s32 BatterySaverState = *(s32*) &Settings->Data;
                        //     switch(BatterySaverState){
                        //         case 0x0:{
                        //             // NOTE(furkan) : Battery saver is off.
                        //         } break;
                        //         case 0x1:{
                        //             // NOTE(furkan) : Battery saver is on. Save 
                        //             // energy where possible.
                        //         } break;
                        //         default:{
                        //             Warning("Unexpected battery saver state");
                        //         } break;
                        //     }
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_POWERSCHEME_PERSONALITY)){
                            Assert(Settings->DataLength == sizeof(GUID));
                            GUID ActiveScheme = *(GUID*) &Settings->Data;
                            if(IsEqualGUID(ActiveScheme, GUID_MIN_POWER_SAVINGS)){
                                // NOTE(furkan) : High Performance - The scheme is 
                                // designed to deliver maximum performance at the 
                                // expense of power consumption savings.
                                Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_POWERSCHEME_PERSONALITY | GUID_MIN_POWER_SAVINGS");
                            } else if(IsEqualGUID(ActiveScheme, 
                                                GUID_MAX_POWER_SAVINGS)){
                                // NOTE(furkan) : Power Saver - The scheme is 
                                // designed to deliver maximum power consumption 
                                // savings at the expense of system performance 
                                // and responsiveness.
                                Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_POWERSCHEME_PERSONALITY | GUID_MAX_POWER_SAVINGS");
                            } else if(IsEqualGUID(ActiveScheme, 
                                                GUID_TYPICAL_POWER_SAVINGS)){
                                // NOTE(furkan) : Automatic - The scheme is 
                                // designed to automatically balance performance 
                                // and power consumption savings.
                                Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_POWERSCHEME_PERSONALITY | GUID_TYPICAL_POWER_SAVINGS");
                            } else {
                                Warning("Unexpected power scheme GUID");
                            }
                        } else if(IsEqualGUID(PowerSetting, 
                                            GUID_SESSION_DISPLAY_STATUS)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 DisplayState = *(s32*) &Settings->Data;
                            switch(DisplayState){
                                case 0x0:{
                                    // NOTE(furkan) : The display is off.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SESSION_DISPLAY_STATUS | 0x0");
                                } break;
                                case 0x1:{
                                    // NOTE(furkan) : The display is on.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SESSION_DISPLAY_STATUS | 0x1");
                                } break;
                                case 0x2:{
                                    // NOTE(furkan) : The display is dimmed.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SESSION_DISPLAY_STATUS | 0x2");
                                } break;
                                default:{
                                    Warning("Unexpected session display status");
                                } break;
                            }
                        } else if(IsEqualGUID(PowerSetting, GUID_SESSION_USER_PRESENCE)){
                            // NOTE(furkan) : All applications that run in an 
                            // interactive user-mode session should use this 
                            // setting. When kernel mode applications register 
                            // for monitor status they should use 
                            // GUID_CONSOLE_DISPLAY_STATUS instead.
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 Presence = *(s32*) &Settings->Data;
                            switch(Presence){
                                case 0x0:{
                                    // NOTE(furkan) : The user is providing input 
                                    // to the session.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SESSION_USER_PRESENCE | 0x0");
                                } break;
                                case 0x2:{
                                    // NOTE(furkan) : The user activity timeout 
                                    // has elapsed with no interaction from the 
                                    // user.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SESSION_USER_PRESENCE | 0x2");
                                } break;
                                default:{
                                    Warning("Unexpected presence value");
                                } break;
                            }
                        } else if(IsEqualGUID(PowerSetting, GUID_SYSTEM_AWAYMODE)){
                            Assert(Settings->DataLength == sizeof(DWORD));
                            s32 AwayMode = *(s32*) &Settings->Data;
                            switch(AwayMode){
                                case 0x0:{
                                    // NOTE(furkan) : The computer is exiting 
                                    // away mode.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SYSTEM_AWAYMODE | 0x0");
                                } break;
                                case 0x1:{
                                    // NOTE(furkan) : The computer is entering 
                                    // away mode.
                                    Log("WM_POWERBROADCAST | PBT_POWERSETTINGCHANGE | GUID_SYSTEM_AWAYMODE | 0x1");
                                } break;
                                default:{
                                    Warning("Unexpected away mode");
                                } break;
                            }
                        } else {
                            Warning("Unexpected power setting GUID");
                        }
                    } break;
                    default:{
                        Warning("Unexpected power management event : %u", Event);
                    } break;
                }
                
                Result = true;
            } break;
    #if 0
            case WM_DEVICECHANGE:{
                // TODO(furkan) : A function like, 
                //      GetDeviceIdentification(DEV_BROADCAST_HDR * DeviceHeader);
                // should be implemented to get more information about the 
                // changed device. For further details :
                // https://msdn.microsoft.com/en-us/library/windows/desktop/aa363246(v=vs.85).aspx
                u32 Event = (u32) WParam;
                switch(Event){
                    case DBT_DEVNODES_CHANGED:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVNODES_CHANGED device event when a device has 
                        // been added to or removed from the system. Applications 
                        // that maintain lists of devices in the system should 
                        // refresh their lists.
                        Log("WM_DEVICECHANGE | DBT_DEVNODES_CHANGED");
                    } break;
                    case DBT_QUERYCHANGECONFIG:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_QUERYCHANGECONFIG device event to request 
                        // permission to change the current configuration (dock or 
                        // undock). Any application can deny this request and 
                        // cancel the change.
                        Log("WM_DEVICECHANGE | DBT_QUERYCHANGECONFIG");
                    } break;
                    case DBT_CONFIGCHANGED:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_CONFIGCHANGED device event to indicate that the 
                        // current configuration has changed, due to a dock or 
                        // undock. An application or driver that stores data in 
                        // the registry under the HKEY_CURRENT_CONFIG key should 
                        // update the data.
                        Log("WM_DEVICECHANGE | DBT_CONFIGCHANGED");
                    } break;
                    case DBT_CONFIGCHANGECANCELED:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_CONFIGCHANGECANCELED device event when a request to 
                        // change the current configuration (dock or undock) has 
                        // been canceled.
                        Log("WM_DEVICECHANGE | DBT_CONFIGCHANGECANCELED");
                    } break;
                    case DBT_DEVICEARRIVAL:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICEARRIVAL device event when a device or piece 
                        // of media has been inserted and becomes available.
                        DEV_BROADCAST_HDR * DeviceHeader = (DEV_BROADCAST_HDR *) LParam;
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICEARRIVAL");
                    } break;
                    case DBT_DEVICEQUERYREMOVE:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICEQUERYREMOVE device event to request 
                        // permission to remove a device or piece of media. This 
                        // message is the last chance for applications and drivers 
                        // to prepare for this removal. However, any application 
                        // can deny this request and cancel the operation.
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICEQUERYREMOVE");
                    } break;
                    case DBT_DEVICEQUERYREMOVEFAILED:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICEQUERYREMOVEFAILED device event when a request 
                        // to remove a device or piece of media has been canceled.
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICEQUERYREMOVEFAILED");
                    } break;
                    case DBT_DEVICEREMOVEPENDING:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICEREMOVEPENDING device event when a device or 
                        // piece of media is being removed and is no longer 
                        // available for use.
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICEREMOVEPENDING");
                    } break;
                    case DBT_DEVICEREMOVECOMPLETE:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICEREMOVECOMPLETE device event when a device or 
                        // piece of media has been physically removed.
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICEREMOVECOMPLETE");
                    } break;
                    case DBT_DEVICETYPESPECIFIC:{
                        // NOTE(furkan) : The system broadcasts the 
                        // DBT_DEVICETYPESPECIFIC device event when a 
                        // device-specific event occurs.
                        
                        Log("WM_DEVICECHANGE | DBT_DEVICETYPESPECIFIC");
                    } break;
                    case DBT_CUSTOMEVENT:{
                        // NOTE(furkan) : The system sends the DBT_CUSTOMEVENT 
                        // device event when a driver-defined custom event has 
                        // occurred.
                        
                        Log("WM_DEVICECHANGE | DBT_CUSTOMEVENT");
                    } break;
                    case DBT_USERDEFINED:{
                        // NOTE(furkan) : The DBT_USERDEFINED device event 
                        // identifies a user-defined event.
                        
                        _DEV_BROADCAST_USERDEFINED * DeviceHeader = 
                                    (_DEV_BROADCAST_USERDEFINED *) LParam;
                        // TODO(furkan) : A function like, 
                        //   GetUserDefinedDeviceIdentification(
                        //                               _DEV_BROADCAST_USERDEFINED 
                        //                               * DeviceHeader);
                        // should be implemented to get more information about the 
                        // changed device. For further details :
                        // https://msdn.microsoft.com/en-us/library/windows/desktop/aa363481(v=vs.85).aspx
                        
                        Log("WM_DEVICECHANGE | DBT_USERDEFINED");
                    } break;
                    default:{
                        Warning("Unexpected device change event : %u", Event);
                    } break;
                }
                
                Result = true;
            } break;
    #endif        
            case WM_ENTERSIZEMOVE:{
                if(EventTracker){
                    EventTracker->IsSizingOrMoving = true;
                }
                Log("WM_ENTERSIZEMOVE");
            } break;
            case WM_EXITSIZEMOVE:{
                if(EventTracker){
                    EventTracker->IsSizingOrMoving = false;
                }
                Log("WM_EXITSIZEMOVE");
            } break;
            default: {
                Log("DefaultProc: %u - %llu", Message, WParam);
                
                Result = DefWindowProc(WindowHandle, Message, WParam, LParam);
            } break;
        }
    
        return Result;
    }
    
    b32
    Win32MemoryAllocate(void **Ptr, size_t AllocSize){
        b32 Success = false;
    
        if(Ptr && AllocSize > 0){
            *Ptr = HeapAlloc(GetProcessHeap(), 0, AllocSize);
            if(*Ptr){
                Success = true;
                ZeroMemory(*Ptr, AllocSize);
            } else {
                Error("HeapAlloc error");
                // NOTE(furkan): According to MSDN, HeapAlloc does not call 
                // SetLastError() which is pretty weird.
            }
        }
        
        return Success;
    }
    
    b32
    Win32MemoryFree(void **Ptr){
        b32 Success = false;
        
        if(Ptr){
            if(*Ptr){
            if(HeapFree(GetProcessHeap(), 0, *Ptr)){
                    Success = true;
                    *Ptr = 0;
            } else {
                    Error("HeapFree error: %d", GetLastError());
                    Win32LogErrorString();
            }
            }    
        }
        
        return Success;
    }
    
    b32
    Win32MemoryCopy(void *Dest, size_t DestSize, void *Src, size_t SrcSize){
        b32 Success = true;
    
        size_t MinSize = MinOf(DestSize, SrcSize);
        if(MinSize != SrcSize){
            Success = false;
            Warning("Destination memory is not large enough");
        }
    
        CopyMemory(Dest, Src, MinSize);
    
        return Success;
    }
    
    b32
    Win32OpenFile(file_handle *Handle, char *Path, u32 FileAccessMode, u32 FileShareMode, file_creation_attrib FileCreationAttrib){
        b32 Success = false;
        
        if(Handle){
            if(Path){
                u32 FileAccess = 0;
                if(FileAccessMode & FileAccessMode_Read){
                    FileAccess = FileAccess | GENERIC_READ;
                }
                
                if(FileAccessMode & FileAccessMode_Write){
                    FileAccess = FileAccess | GENERIC_WRITE;
                }
                
                u32 FileShare = 0;
                if(FileShareMode & FileShareMode_Read){
                    FileShare = FileShare | FILE_SHARE_READ;
                }
                
                if(FileShareMode & FileShareMode_Write){
                    FileShare = FileShare | FILE_SHARE_WRITE;
                }
                
                if(FileShareMode & FileShareMode_Delete){
                    FileShare = FileShare | FILE_SHARE_DELETE;
                }
                
                u32 FileCreation = 0;
                switch(FileCreationAttrib){
                    case FileCreationAttrib_CreateAlways:{
                        FileCreation = CREATE_ALWAYS;
                    } break;
                    case FileCreationAttrib_CreateIfNotExist:{
                        FileCreation = CREATE_NEW;
                    } break;
                    case FileCreationAttrib_OpenAlways:{
                        FileCreation = OPEN_ALWAYS;
                    } break;
                    case FileCreationAttrib_OpenIfExist:{
                        FileCreation = OPEN_EXISTING;
                    } break;
                    case FileCreationAttrib_OpenAndTruncate:{
                        FileCreation = TRUNCATE_EXISTING;
                    } break;
                    default:{
                        Error("Unexpected file creation attrib! (%u)", FileCreationAttrib);
                    } break;
                }
                
                if(FileCreation){
                    Handle->Ptr = CreateFileA(Path, FileAccess, FileShare, 0, FileCreation, 0, 0);
                    if(Handle->Ptr && Handle->Ptr != INVALID_HANDLE_VALUE){
                        Success = true;
                    } else {
                        Error("CreateFile error: %d", GetLastError());
                        Win32LogErrorString();
                    }
                }
            }
        }
        
        return Success;
    }
    
    b32
    Win32GetFileSize(file_handle FileHandle, u64 *FileSize){
        b32 Success = false;
    
        if(FileSize){
            LARGE_INTEGER Size;
            if(GetFileSizeEx(FileHandle.Ptr, &Size)){
                *FileSize = Size.QuadPart;
                Success = true;
            } else {
                Error("GetFileSizeEx error: %d", GetLastError());
                Win32LogErrorString();
            }    
        }
        
        return Success;
    }
    
    b32
    Win32ReadFile32(file_handle File, void *MemoryPtr, u32 BytesToRead, u32 *BytesRead){
        b32 Success = false;
        
        if(File.Ptr && File.Ptr != INVALID_HANDLE_VALUE){
            if(MemoryPtr){
                if(BytesToRead > 0){
                    Success = ReadFile(File.Ptr, MemoryPtr, BytesToRead, (LPDWORD)BytesRead, 0);
                    if(!Success){
                        Error("ReadFile error: %d", GetLastError());
                        Win32LogErrorString();
                    }
                }
            }
        }
        
        return Success;
    }
    
    b32
    Win32SetFilePointer(file_handle File, s64 SeekDistance, file_seek_pivot Pivot){
        b32 Success = false;
        
        u32 MoveMethod = u32_Max;
        switch(Pivot){
            case FileSeekPivot_Begin:{
                MoveMethod = FILE_BEGIN;
            } break;
            case FileSeekPivot_Current:{
                MoveMethod = FILE_CURRENT;
            } break;
            case FileSeekPivot_End:{
                MoveMethod = FILE_END;
            } break;
            default:{
                Error("Unexpected file seek pivot! (%u)", Pivot);
            } break;
        }
        
        if(MoveMethod != u32_Max){
            LARGE_INTEGER Seek;
            Seek.QuadPart = SeekDistance;
            if(SetFilePointerEx(File.Ptr, Seek, 0, MoveMethod)){
                Success = true;
            } else {
                Error("SetFilePointerEx error: %d", GetLastError());
                Win32LogErrorString();
            }
        }
        
        return Success;
    }
    
    b32
    Win32CloseFileHandle(file_handle Handle){
        b32 Success = false;
        
        if(CloseHandle(Handle.Ptr)){
            Success = true;
        } else {
            Error("CloseHandle error: %d", GetLastError());
            Win32LogErrorString();
        }
        
        return Success;
    }
    
    b32
    Win32GetProcessorCoreCount(u32 *Result){
        b32 Success = false;
        
        if(Result){
            SYSTEM_INFO SysInfo;
            GetNativeSystemInfo(&SysInfo);
            
            *Result = SysInfo.dwNumberOfProcessors;
            Success = true;
        }
        
        return Success;
    }
    
    b32
    Win32GetAvailableMemory(u64 *Result){
        b32 Success = false;
        
        if(Result){
            MEMORYSTATUSEX MemoryStatus;
            MemoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
            if(GlobalMemoryStatusEx(&MemoryStatus)){
                *Result = MemoryStatus.ullAvailPhys;
                Success = true;
            } else {
                Error("GlobalMemoryStatusEx failed! (Error: %d)", GetLastError());
                Win32LogErrorString();
            }
        }
        
        return Success;
    }
    
    b32
    Win32QueryTimer(timer *Timer){
        b32 Success = false;
        
        if(Timer){
            LARGE_INTEGER Counter;
            if(QueryPerformanceCounter(&Counter)){
                Timer->Counter = Counter.QuadPart;
                Success = true;
            } else {
                Error("QueryPerformanceCounter failed! (Error: %d)", GetLastError());
                Win32LogErrorString();
            }
        }
        
        return Success;
    }
    
    b32
    Win32QueryTimerFrequency(timer_freq *Freq){
        b32 Success = false;
        
        if(Freq){
            LARGE_INTEGER QueryData;
            if(QueryPerformanceFrequency(&QueryData)){
                Freq->Value = QueryData.QuadPart;
                Success = true;
            } else {
                Error("QueryPerformanceFrequency failed! (Error: %d)", GetLastError());
                Win32LogErrorString();
            }
        }
        
        return Success;
    }
    
    b32
    Win32CreateThread(thread_handle *Handle, platform_thread_proc *ThreadProc, void *Params, u32 *ThreadID, size_t StackSize=0){
        b32 Success = false;
        
        if(Handle){
            Handle->Ptr = CreateThread(0 , StackSize, (LPTHREAD_START_ROUTINE)ThreadProc, Params, 0, (LPDWORD)ThreadID);
            if(Handle->Ptr){
                Success = true;
            } else {
                Error("CreateThread error: %d", GetLastError());
                Win32LogErrorString();
            }
        }
        
        return Success;
    }
    
    b32
    Win32WaitForThreads(u32 ThreadCount, thread_handle *ThreadHandles, b32 WaitAll, b32 WaitInfinitely, u32 WaitTimeLimit=0){
        b32 Success = false;
        
        if(WaitInfinitely){
            WaitTimeLimit = INFINITE;
        }
        
        u32 Result = WaitForMultipleObjects(ThreadCount, (HANDLE *)ThreadHandles, WaitAll, WaitTimeLimit);
        if(Result != WAIT_FAILED){
            Success = true;
        } else {
            Error("WaitForMultipleObjects error: %d", GetLastError());
            Win32LogErrorString();
        }
        
        return Success;
    }
    
    b32
    Win32CloseThreadHandle(thread_handle Handle){
        b32 Success = false;
        
        if(CloseHandle(Handle.Ptr)){
            Success = true;
        } else {
            Error("CloseHandle error: %d", GetLastError());
            Win32LogErrorString();
        }
        
        return Success;
    }
    
    b32
    Win32GetMonitorRefreshRate(u32 *RefreshRate){
        b32 Success = false;
    
        DEVMODE DeviceSettings;
        if(EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &DeviceSettings)){
            if(DeviceSettings.dmFields & DM_DISPLAYFREQUENCY){
                *RefreshRate = DeviceSettings.dmDisplayFrequency;
                Success = true;
            } else {
                Warning("Display frequency is not initialised by driver");
            }
        } else {
            Error("EnumDisplaySettings failed!");
            Win32LogErrorString();
        }
    
        return Success;
    }
    
    cursor_handle
    Win32GetDefaultCursor(){
        cursor_handle Result;
    
        Result.Ptr =  (HCURSOR)LoadImage(0, 
                                        MAKEINTRESOURCE(OCR_NORMAL), 
                                        IMAGE_CURSOR, 0, 0, 
                                        LR_DEFAULTSIZE | LR_SHARED);
        if(!Result.Ptr){
            Win32LogErrorString();
        }
    
        return Result;
    }
    
    b32
    Win32CreateWindow(window_handle *Window, char *WindowTitle, 
                    char *ClassName, platform_window_callback *Callback, 
                    cursor_handle CursorHandle, icon_handle IconHandle, 
                    s32 AtX, s32 AtY, s32 Width, s32 Height){
        b32 Success = false;
    
        WNDCLASSEX WindowClass = { 0 };
        
        WindowClass.cbSize = sizeof(WNDCLASSEX);
        /* TODO(furkan) : What is the purpose of CS_HREDRAW and CS_VREDRAW ? */
        WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        WindowClass.lpfnWndProc = Callback ? Callback : DefWindowProc;
        WindowClass.hInstance = GetModuleHandle(0);
        WindowClass.hIconSm = WindowClass.hIcon = IconHandle.Ptr;
        WindowClass.hCursor = CursorHandle.Ptr;
        WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        
        /* TODO(furkan) : What is the purpose of lpszClassName ? */
        WindowClass.lpszClassName = ClassName;
    
        if (RegisterClassEx(&WindowClass)) {
            Window->Ptr = CreateWindowEx(0, WindowClass.lpszClassName,
                WindowTitle,
                WS_OVERLAPPED | 
                WS_CAPTION | 
                WS_SYSMENU | 
                WS_THICKFRAME | 
                WS_MINIMIZEBOX | 
                WS_MAXIMIZEBOX | 
                WS_EX_LEFT | 
                WS_EX_LTRREADING | 
                WS_EX_RIGHTSCROLLBAR,
                AtX, AtY,
                Width, Height,
                0, 0,
                WindowClass.hInstance,
                0);
            if(Window->Ptr){
                Success = true;
            } else {
                Error("CreateWindowEx failed");
                Win32LogErrorString();
            }
        } else {
            Error("RegisterClassEx failed");
            Win32LogErrorString();
        }
    
        return Success;
    }
    
    b32
    Win32InitialiseWindowEventTracker(kadabra::window * const Window){
        b32 Success = true;
        
        DWORD TempLastError = GetLastError();
        SetLastError(0);
    
        LONG_PTR PrevOffset = SetWindowLongPtr(Window->Handle->Ptr, 
                                            GWLP_USERDATA, 
                                            (LONG_PTR)&Window->EventTracker);
        if(!PrevOffset){
            DWORD LastError = GetLastError();
            if(LastError){
                Error("SetWindowLongPtr failed");
                Win32LogErrorString(LastError);
                Success = false;
            }
        }
    
        SetLastError(TempLastError);
    
        return Success;
    }
    
    b32
    Win32GetWindowClientSize(window_handle *Window, vec2i *Size){
        b32 Success = false;
        
        RECT ClientRect;
        if(GetClientRect(Window->Ptr, &ClientRect)){
            Size->x = ClientRect.right - ClientRect.left;
            Size->y = ClientRect.bottom - ClientRect.top;
            Success = true;
        } else {
            Error("GetClientRect failed");
            Win32LogErrorString();
        }
        
        return Success;
    }
    
    b32
    Win32SetWindowTitle(window_handle *Window, char *Title){
        b32 Success = false;
        
        if(SetWindowTextA(Window->Ptr, Title)){
            Success = true;
        } else {
            Error("SetWindowTitle failed");
            Win32LogErrorString();
        }
        
        return Success;
    }
    
    b32
    Win32ShowWindow(window_handle *Window){
        b32 Success = false;
        
        if(!ShowWindow(Window->Ptr, SW_SHOW)){
            Success = true;
        } else {
            Warning("An already visible window is tried to be visible again");
        }
        
        return Success;
    }
    
    b32
    Win32ProcessMouseInput(input *Input, f32 MouseWheelInput, 
                        window *Window, system_properties *SysProps){
        b32 Success = true;
    
        s32 Win32ButtonMapping[InputMouseButton_Count*2] = {
            InputMouseButton_Left,      VK_LBUTTON,     // Left mouse button
            InputMouseButton_Right,     VK_RBUTTON,     // Right mouse button
            InputMouseButton_Middle,    VK_MBUTTON,     // Middle mouse button
            InputMouseButton_Extra1,    VK_XBUTTON1,    // X1 mouse button
            InputMouseButton_Extra2,    VK_XBUTTON2,    // X2 mouse button
        };
        
        input_buffer *FrontBuffer = Input->InputFrontBuffer();
        
        for(u32 InputMouseButtonIndex=0; 
                InputMouseButtonIndex<InputMouseButton_Count; 
                InputMouseButtonIndex++){
            s32 Win32KeyCode   = Win32ButtonMapping[InputMouseButtonIndex*2+1];
            s32 KeyCode = Win32ButtonMapping[InputMouseButtonIndex*2];
            
            Assert(KeyCode < InputMouseButton_Count);
            
            u16 KeyState = GetKeyState(Win32KeyCode);
            
            FrontBuffer->MouseButtons[KeyCode].IsDown = (KeyState >> 7) & 0x01;
        }
        
        // TODO(furkan) : Multi-screen support
        vec2 MonitorSize = Vec2((f32)SysProps->PrimaryMonitorWidth, 
                                (f32)SysProps->PrimaryMonitorHeight);
    
        POINT MousePosition;
        GetCursorPos(&MousePosition);
        FrontBuffer->MousePositionScreen = Vec2((f32)MousePosition.x, 
                                                (f32)(MousePosition.y));
        FrontBuffer->MousePositionScreen.y = MonitorSize.y - 1.0f -
                                            FrontBuffer->MousePositionScreen.y;
    
        ScreenToClient(Window->Handle->Ptr, &MousePosition);    
        FrontBuffer->MousePositionClient = Vec2((f32)MousePosition.x, 
                                                (f32)(MousePosition.y));
        FrontBuffer->MousePositionClient.y = Window->ClientSize.y - 1.0f -
                                            FrontBuffer->MousePositionClient.y;
    
        FrontBuffer->NormalisedMousePositionScreen.x = 
                        FrontBuffer->MousePositionScreen.x/(MonitorSize.x-1.0f);
        FrontBuffer->NormalisedMousePositionScreen.y = 
                        FrontBuffer->MousePositionScreen.y/(MonitorSize.y-1.0f);
    
        FrontBuffer->NormalisedMousePositionClient.x = 
            FrontBuffer->MousePositionClient.x/(f32)(Window->ClientSize.x-1.0f);
        FrontBuffer->NormalisedMousePositionClient.y = 
            FrontBuffer->MousePositionClient.y/(f32)(Window->ClientSize.y-1.0f);
    
        FrontBuffer->MouseWheel = MouseWheelInput;
    
        return Success;
    }
    
    b32
    Win32ProcessKeyboardInput(input *Input){
        b32 Success = true;
    
        s32 Win32KeyMapping[InputKey_Count*2] = {
            InputKey_Backspace,         VK_BACK,        // BACKSPACE key
            InputKey_Tab,               VK_TAB,         // TAB key
            InputKey_Enter,             VK_RETURN,      // ENTER key
            InputKey_Shift,             VK_SHIFT,       // SHIFT key
            InputKey_Ctrl,              VK_CONTROL,     // CTRL key
            InputKey_Alt,               VK_MENU,        // ALT key
            InputKey_Pause,             VK_PAUSE,       // PAUSE key
            InputKey_CapsLock,          VK_CAPITAL,     // CAPS LOCK key
            InputKey_ESC,               VK_ESCAPE,      // ESC key
            InputKey_Space,             VK_SPACE,       // SPACEBAR
            InputKey_PageUp,            VK_PRIOR,       // PAGE UP key
            InputKey_PageDown,          VK_NEXT,        // PAGE DOWN key
            InputKey_End,               VK_END,         // END key
            InputKey_Home,              VK_HOME,        // HOME key
            InputKey_ArrowLeft,         VK_LEFT,        // LEFT ARROW key
            InputKey_ArrowUp,           VK_UP,          // UP ARROW key
            InputKey_ArrowRight,        VK_RIGHT,       // RIGHT ARROW key
            InputKey_ArrowDown,         VK_DOWN,        // DOWN ARROW key
            InputKey_PrintScreen,       VK_SNAPSHOT,    // PRINT SCREEN key
            InputKey_Insert,            VK_INSERT,      // INS key
            InputKey_Delete,            VK_DELETE,      // DEL key
            InputKey_Help,              VK_HELP,        // HELP key
            InputKey_0,                 0x30,           // 0 key
            InputKey_1,                 0x31,           // 1 key
            InputKey_2,                 0x32,           // 2 key
            InputKey_3,                 0x33,           // 3 key
            InputKey_4,                 0x34,           // 4 key
            InputKey_5,                 0x35,           // 5 key
            InputKey_6,                 0x36,           // 6 key
            InputKey_7,                 0x37,           // 7 key
            InputKey_8,                 0x38,           // 8 key
            InputKey_9,                 0x39,           // 9 key
            InputKey_A,                 0x41,           // A key
            InputKey_B,                 0x42,           // B key
            InputKey_C,                 0x43,           // C key
            InputKey_D,                 0x44,           // D key
            InputKey_E,                 0x45,           // E key
            InputKey_F,                 0x46,           // F key
            InputKey_G,                 0x47,           // G key
            InputKey_H,                 0x48,           // H key
            InputKey_I,                 0x49,           // I key
            InputKey_J,                 0x4A,           // J key
            InputKey_K,                 0x4B,           // K key
            InputKey_L,                 0x4C,           // L key
            InputKey_M,                 0x4D,           // M key
            InputKey_N,                 0x4E,           // N key
            InputKey_O,                 0x4F,           // O key
            InputKey_P,                 0x50,           // P key
            InputKey_Q,                 0x51,           // Q key
            InputKey_R,                 0x52,           // R key
            InputKey_S,                 0x53,           // S key
            InputKey_T,                 0x54,           // T key
            InputKey_U,                 0x55,           // U key
            InputKey_V,                 0x56,           // V key
            InputKey_W,                 0x57,           // W key
            InputKey_X,                 0x58,           // X key
            InputKey_Y,                 0x59,           // Y key
            InputKey_Z,                 0x5A,           // Z key
            InputKey_Sleep,             VK_SLEEP,       // Computer Sleep key
            InputKey_Numpad0,           VK_NUMPAD0,     // Numeric keypad 0 key
            InputKey_Numpad1,           VK_NUMPAD1,     // Numeric keypad 1 key
            InputKey_Numpad2,           VK_NUMPAD2,     // Numeric keypad 2 key
            InputKey_Numpad3,           VK_NUMPAD3,     // Numeric keypad 3 key
            InputKey_Numpad4,           VK_NUMPAD4,     // Numeric keypad 4 key
            InputKey_Numpad5,           VK_NUMPAD5,     // Numeric keypad 5 key
            InputKey_Numpad6,           VK_NUMPAD6,     // Numeric keypad 6 key
            InputKey_Numpad7,           VK_NUMPAD7,     // Numeric keypad 7 key
            InputKey_Numpad8,           VK_NUMPAD8,     // Numeric keypad 8 key
            InputKey_Numpad9,           VK_NUMPAD9,     // Numeric keypad 9 key
            InputKey_NumpadStar,        VK_MULTIPLY,    // Multiply key
            InputKey_NumpadPlus,        VK_ADD,         // Add key
            InputKey_NumpadMinus,       VK_SUBTRACT,    // Subtract key
            InputKey_NumpadDot,         VK_DECIMAL,     // Decimal key
            InputKey_NumpadBackslash,   VK_DIVIDE,      // Divide key
            InputKey_F1,                VK_F1,          // F1 key
            InputKey_F2,                VK_F2,          // F2 key
            InputKey_F3,                VK_F3,          // F3 key
            InputKey_F4,                VK_F4,          // F4 key
            InputKey_F5,                VK_F5,          // F5 key
            InputKey_F6,                VK_F6,          // F6 key
            InputKey_F7,                VK_F7,          // F7 key
            InputKey_F8,                VK_F8,          // F8 key
            InputKey_F9,                VK_F9,          // F9 key
            InputKey_F10,               VK_F10,         // F10 key
            InputKey_F11,               VK_F11,         // F11 key
            InputKey_F12,               VK_F12,         // F12 key
            InputKey_NumLock,           VK_NUMLOCK,     // NUM LOCK key
            InputKey_ScrollLock,        VK_SCROLL,      // SCROLL LOCK key
            InputKey_LeftShift,         VK_LSHIFT,      // Left SHIFT key
            InputKey_RightShift,        VK_RSHIFT,      // Right SHIFT key
            InputKey_LeftCtrl,          VK_LCONTROL,    // Left CONTROL key
            InputKey_RightCtrl,         VK_RCONTROL,    // Right CONTROL key
            InputKey_LeftAlt,           VK_LMENU,       // Left MENU key
            InputKey_RightAlt,          VK_RMENU,       // Right MENU key
            InputKey_Mute,              VK_VOLUME_MUTE, // Volume Mute key
            InputKey_VolumeDown,        VK_VOLUME_DOWN, // Volume Down key
            InputKey_VolumeUp,          VK_VOLUME_UP,   // Volume Up key
        };
        
        input_buffer *FrontBuffer = Input->InputFrontBuffer();
        
        for(u32 InputKeyIndex=0; InputKeyIndex<InputKey_Count; InputKeyIndex++){
            s32 Win32KeyCode   = Win32KeyMapping[InputKeyIndex*2+1];
            s32 KeyCode = Win32KeyMapping[InputKeyIndex*2];
            
            Assert(KeyCode < InputKey_Count);
            
            u16 KeyState = GetKeyState(Win32KeyCode);
            
            FrontBuffer->Keys[KeyCode].IsDown = (KeyState >> 7) & 0x01;
        }
    
        return Success;
    }
    
    b32
    Win32InitialiseInput(input *Input, window *Window, 
                        system_properties *SysProps){
        b32 Success = true;
        
        // NOTE(furkan) : Fill both of input buffers
        Input->FrontBufferIndex = 0;
        Win32ProcessKeyboardInput(Input);
        Win32ProcessMouseInput(Input, 0.0f, Window, SysProps);
        
        Input->FrontBufferIndex ^= 0x01;
        Win32ProcessKeyboardInput(Input);
        Win32ProcessMouseInput(Input, 0.0f, Window, SysProps);
        
        return Success;
    }
    
    b32
    Win32ProcessPlatformMessages(input *Input, window *Window, 
                                 system_properties *SysProps){
        b32 Success = true;
    
        f32 TotalMouseWheelChange = 0;
        MSG Message = {0};
        BOOL PeekReturn;
        while ((PeekReturn = PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) > 0) {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
    
            if(Message.message == WM_MOUSEWHEEL){
                s16 WheelDelta = (Message.wParam & 0xFFFF0000) >> 16;
                f32 WheelRatio = ((f32)WheelDelta/((f32)WHEEL_DELTA));
                TotalMouseWheelChange += WheelRatio;
            } else if(Message.message == WM_QUIT){
                Window->QuitRequested = true;
            }
        }
    
        if(PeekReturn != 0){
            Warning("Peek return is %d", PeekReturn);
            Success = false;
        }
        
        Win32ProcessMouseInput(Input, TotalMouseWheelChange, Window, SysProps);
        Win32ProcessKeyboardInput(Input);
        // TODO(furkan) : Controller input (using both xinput and dinput)
        Input->FrontBufferIndex ^= 0x01;
        Assert(Input->FrontBufferIndex == 0 || Input->FrontBufferIndex == 1);
    
        return Success;
    }
    
    b32
    Win32GetSystemProperties(system_properties *SysProps){
        b32 Success = true;
    
        SysProps->PrimaryMonitorWidth = GetSystemMetrics(SM_CXSCREEN);
        SysProps->PrimaryMonitorHeight = GetSystemMetrics(SM_CYSCREEN);   
        if(!Win32GetMonitorRefreshRate(&SysProps->PrimaryMonitorRefreshRate)){
            Warning("Primary monitor's refresh rate could not be detected! 60Hz is set by default.");
            SysProps->PrimaryMonitorRefreshRate = 60;
        }
    
        return Success;
    }

}
