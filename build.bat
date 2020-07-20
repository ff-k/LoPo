@echo off

pushd bin
pushd x86
set KDBLibPath="..\..\lib\x86"

if "%Platform%" == "X64" (
    popd
    pushd x64
    set KDBLibPath="..\..\lib\x64"
)

REM /nologo      : Suppress startup banner
REM /Gm-         : Disable minimal rebuild
REM /GR-         : Disable C++ RTTI
REM /EHa-        : Disable C++ exception handling with SEH exceptions
REM /WX          : Treat warnings as errors
REM /Wx          : Warning level=x
REM /wdxxxx      : Disable warning xxxx
REM /FC          : Use full pathnames
REM /Z7          : Enable old-style debug info
REM /GS          : Enable security checks
REM /sdl         : Enable additional security features and warnings
REM set DisabledWarnings=/wd4100 /wd4127 /wd4189 /wd4201 /wd4312 /wd4505 /wd4530 /wd4577 /wd4800
set DisabledWarnings=/wd4100 /wd4127 /wd4201 /wd4189 /wd4800
set CommonCompilerFlags=/nologo /D_SCL_SECURE_NO_WARNINGS /Gm- /GR- /EHa- /WX /W4 /EHsc %DisabledWarnings% /FC /Z7 /GS /sdl -I "..\..\include" /Wv:18
REM /incremental : Incremental linking [:no disables it]
set CommonLinkerFlags=/subsystem:windows user32.lib gdi32.lib opengl32.lib /entry:mainCRTStartup /incremental:no /NODEFAULTLIB:libc.lib /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:libcd.lib /NODEFAULTLIB:msvcrtd.lib /LIBPATH:%KDBLibPath% /OUT:"kdb.exe"

REM /Od          : Disable optimizations 
REM /MTd         : Link with libcmtd.lib
set DebugCompilerFlag=/DKDBDevelopmentBuild=1 %CommonCompilerFlags% /Od /MTd
REM /opt:noref   : Do not remove functions that are never referenced
set DebugLinkerFlag=%CommonLinkerFlags% /opt:noref /NODEFAULTLIB:libcmt.lib

REM /O2          : Maximize speed
REM /MT          : Link with libcmt.lib
REM /Zo          : Generate richer debugging information for optimized code
REM TODO(furkan): Set KDBDevelopmentBuild to 0 for release build
set ReleaseCompilerFlag=/DKDBDevelopmentBuild=1 %CommonCompilerFlags% /O2 /MT /Zo
REM /opt:ref     : Remove functions that are never referenced
set ReleaseLinkerFlag=%CommonLinkerFlags% /opt:ref /NODEFAULTLIB:libcmtd.lib 

set SourceFiles=..\..\src\kdb_main.cpp ^
                ..\..\src\kdb_asset.cpp ^
                ..\..\src\kdb_bvh.cpp ^
                ..\..\src\kdb_camera.cpp ^
                ..\..\src\kdb_input.cpp ^
                ..\..\src\kdb_io.cpp ^
                ..\..\src\kdb_math.cpp ^
                ..\..\src\kdb_platform_win32.cpp ^
                ..\..\src\kdb_physics.cpp ^
                ..\..\src\kdb_ray.cpp ^
                ..\..\src\kdb_renderer.cpp ^
                ..\..\src\kdb_renderer_opengl.cpp ^
                ..\..\src\kdb_renderer_opengl_win32.cpp ^
                ..\..\src\kdb_scene.cpp ^
                ..\..\src\kdb_string.cpp ^
                ..\..\src\kdb_window.cpp

if "%1%" == "debug" (
    REM Debug build
    cl %DebugCompilerFlag% %SourceFiles% /link %DebugLinkerFlag%
) else (
    REM Release build
    cl %ReleaseCompilerFlag% %SourceFiles% /link %ReleaseLinkerFlag%
)

popd
popd
