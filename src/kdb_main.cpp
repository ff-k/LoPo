#include "kdb_common.h"
#include "kdb_util.h"
#include "kdb_string.h"

#include "kdb_asset.h"
#include "kdb_renderer.h"

#include <stdio.h>

struct preferences {
    char **MeshFilePaths;

    u32 WindowWidth;
    u32 WindowHeight;

    u32 MeshCount;

    kadabra::swizzle_order MeshSwizzling;

    kadabra::renderer_polygon_mode PolygonMode;
    
    preferences(){
        MeshFilePaths = 0;
        
        WindowWidth  = 1280;
        WindowHeight = 720;
        
        MeshCount = 0;
        MeshSwizzling = kadabra::SwizzleOrder_XYZ;
        
        PolygonMode = kadabra::RendererPolygonMode_Fill;
    }
    
    b32 ParseArguments(char **Args, u32 ArgCount);
};

b32 
preferences::ParseArguments(char **Args, u32 ArgCount){
    b32 Success = false;
    
    u32 ArgIndex;
    for(ArgIndex=1; ArgIndex<ArgCount; ArgIndex++){
        if(!strncmp(Args[ArgIndex], "--meshes", 8)){
            if((ArgIndex+1) < ArgCount){
                ArgIndex++;
                MeshCount = kadabra::StringToU32(Args[ArgIndex]);
                if((ArgIndex+1) < ArgCount){
                    ArgIndex++;
                    MeshFilePaths = Args+ArgIndex;
                    ArgIndex += (MeshCount - 1);
                    Success = true;
                }
            }
            
            if(!Success){
                Error("Input meshes are not specified.");
            }
        } else if(!strncmp(Args[ArgIndex], "-m", 2) || 
                  !strncmp(Args[ArgIndex], "--mesh", 6)){
            if((ArgIndex+1) < ArgCount){
                ArgIndex++;
                MeshFilePaths = Args+ArgIndex;
                MeshCount = 1;
                Success = true;
            } else {
                Error("Input mesh is not specified.");
            }
        } else if(!strncmp(Args[ArgIndex], "--xyz", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_XYZ;
        } else if(!strncmp(Args[ArgIndex], "--xzy", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_XZY;
        } else if(!strncmp(Args[ArgIndex], "--yxz", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_YXZ;
        } else if(!strncmp(Args[ArgIndex], "--yzx", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_YZX;
        } else if(!strncmp(Args[ArgIndex], "--zxy", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_ZXY;
        } else if(!strncmp(Args[ArgIndex], "--zyx", 5)){
            MeshSwizzling = kadabra::SwizzleOrder_ZYX;
        } else {
            Error("Argument cannot be recognized : %s.", Args[ArgIndex]);
        }
    }
    
    return Success;
}

#if KDBDevelopmentBuild

inline b32 
ValidateDataTypeSize(){
    b32 Valid = true;

    if(sizeof(s64) != 8){ Valid = false; Error("s64 is not 64 bits length!"); }
    if(sizeof(s32) != 4){ Valid = false; Error("s32 is not 32 bits length!"); }
    if(sizeof(s16) != 2){ Valid = false; Error("s16 is not 16 bits length!"); }
    if(sizeof(s8)  != 1){ Valid = false; Error("s8  is not  8 bits length!"); }
    
    if(sizeof(u64) != 8){ Valid = false; Error("u64 is not 64 bits length!"); }
    if(sizeof(u32) != 4){ Valid = false; Error("u32 is not 32 bits length!"); }
    if(sizeof(u16) != 2){ Valid = false; Error("u16 is not 16 bits length!"); }
    if(sizeof(u8)  != 1){ Valid = false; Error("u8  is not  8 bits length!"); }
    
    if(sizeof(f64) != 8){ Valid = false; Error("f64 is not 64 bits length!"); }
    if(sizeof(f32) != 4){ Valid = false; Error("f32 is not 32 bits length!"); }

    return Valid;
}

int main(int ArgCount, char ** Args){
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  
    if(!ValidateDataTypeSize()){
        Error("Data type size validation failed. Terminating.");
        return -1;
    }
    Log("Development build");
#else
int main(int ArgCount, char ** Args){
#endif 

    preferences Prefs;
    b32 ParseResult = Prefs.ParseArguments(Args, ArgCount);
    if(ParseResult){
        kadabra::window Window;
        kadabra::timer_freq TimerFrequency;
        kadabra::system_properties SystemProps;
        if(kadabra::platform::QueryTimerFrequency(&TimerFrequency) && 
           kadabra::platform::GetSystemProperties(&SystemProps) && 
           Window.Initialise("MMI541 Game Physics", 
                             kadabra::platform::MainWindowCallback,
                             kadabra::platform::GetDefaultCursor(),
                             {0}, 0, 0,
                             Prefs.WindowWidth, 
                             Prefs.WindowHeight, true)){
    
            kadabra::asset_manager AssetManager;
            if(AssetManager.Initialise(Prefs.MeshCount, 
                                       Prefs.MeshFilePaths,
                                       Prefs.MeshSwizzling)){

                kadabra::renderer Renderer;
                if(Renderer.Initialise(kadabra::Renderer_OpenGL, &Window, 
                                       Prefs.PolygonMode)){

                    kadabra::scene Scene;
                    Scene.Initialise(&AssetManager, &Window);
                    
                    kadabra::input Input;
                    if(kadabra::platform::InitialiseInput(&Input, &Window, 
                                                          &SystemProps)){ 
                        
                        Input.DeltaTime = 1.0f/(f32)SystemProps.PrimaryMonitorRefreshRate;

                        while(true){
                            kadabra::timer FrameBegin;
                            kadabra::platform::QueryTimer(&FrameBegin);
                            
                            if(kadabra::platform::ProcessPlatformMessages(&Input, 
                                                                          &Window,
                                                                          &SystemProps)){
    
                                kadabra::window_event_tracker *Events = 
                                                         &Window.EventTracker;

                                b32 IsPaused = Events->IsSizingOrMoving ||
                                               !Events->ApplicationHasFocus;
                                if(Window.QuitRequested){
                                    break;
                                } else if(IsPaused){
                                    continue;
                                }
    
                                if(Window.EventTracker.Resized){
                                    kadabra::vec2i NewSize = Window.EventTracker.NewSize;
                                    if(Window.OnResize(NewSize)){
                                        Window.EventTracker.Resized = false;
                                        Renderer.OnWindowResize(&Window);
                                    }
                                }

                                if(!Scene.Update(&AssetManager, 
                                                 &Input, &Window,
                                                 &Renderer)){
                                    Warning("UpdateScene failed");
                                }
                                
                                if(!Renderer.RenderScene(&Scene, 
                                                         &AssetManager, 
                                                         &Window)){
                                    Warning("RenderScene failed");
                                }

                                if(!Renderer.SwapBuffers(&Window)){
                                    Warning("SwapBuffers failed");
                                }
                            } else {
                                break;
                            }
                            
                            kadabra::timer FrameEnd;
                            kadabra::platform::QueryTimer(&FrameEnd);
                            
                            u64 FrameCycles = FrameEnd.Counter-FrameBegin.Counter;
                            f64 FrameTime = (f64)FrameCycles/(f64)TimerFrequency.Value;
                            
                            char FrameTimeBuf[32];
                            snprintf(FrameTimeBuf, sizeof(FrameTimeBuf), "%f", FrameTime);
                            Window.SetTitle(FrameTimeBuf);
                        }
                    }
                    
                    if(!Renderer.Finalise()){
                        Warning("FinaliseRenderer failed");
                    }
                }
    
                if(!AssetManager.Finalise()){
                    Warning("FinaliseAssetManager failed");
                }
            }
        }
    } else {
        Error("Invalid set of arguments, terminating...");
        return -1;
    }
    
    return 0;
}
