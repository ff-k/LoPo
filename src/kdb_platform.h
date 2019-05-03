#ifndef KDB_HEADER_PLATFORM
#define KDB_HEADER_PLATFORM

#include "kdb_common.h"

#if _WIN32
    #include "kdb_platform_win32.h"
#elif
    #error "Platform layer is not implemented for this platform!"
#endif

#include "kdb_io.h"
#include "kdb_input.h"
#include "kdb_window.h"

#define DeclarePlatformAPI(PlatformPrefix);                                                              \
        extern platform_window_callback                 PlatformPrefix##MainWindowCallback;          \
        extern platform_open_file                       PlatformPrefix##OpenFile;                    \
        extern platform_get_file_size                   PlatformPrefix##GetFileSize;                 \
        extern platform_read_file_32                    PlatformPrefix##ReadFile32;                  \
        extern platform_set_file_pointer                PlatformPrefix##SetFilePointer;              \
        extern platform_close_file_handle               PlatformPrefix##CloseFileHandle;             \
        extern platform_get_processor_core_count        PlatformPrefix##GetProcessorCoreCount;       \
        extern platform_get_available_memory            PlatformPrefix##GetAvailableMemory;          \
        extern platform_query_timer                     PlatformPrefix##QueryTimer;                  \
        extern platform_query_timer_frequency           PlatformPrefix##QueryTimerFrequency;         \
        extern platform_memory_allocate                 PlatformPrefix##MemoryAllocate;              \
        extern platform_memory_free                     PlatformPrefix##MemoryFree;                  \
        extern platform_create_thread                   PlatformPrefix##CreateThread;                \
        extern platform_wait_for_threads                PlatformPrefix##WaitForThreads;              \
        extern platform_close_thread_handle             PlatformPrefix##CloseThreadHandle;           \
        extern platform_get_monitor_refresh_rate        PlatformPrefix##GetMonitorRefreshRate;       \
        extern platform_create_window                   PlatformPrefix##CreateWindow;                \
        extern platform_initialise_window_event_tracker PlatformPrefix##InitialiseWindowEventTracker;\
        extern platform_get_window_client_size          PlatformPrefix##GetWindowClientSize;         \
        extern platform_show_window                     PlatformPrefix##ShowWindow;                  \
        extern platform_process_mouse_input             PlatformPrefix##ProcessMouseInput;           \
        extern platform_process_keyboard_input          PlatformPrefix##ProcessKeyboardInput;        \
        extern platform_initialise_input                PlatformPrefix##InitialiseInput;             \
        extern platform_process_platform_messages       PlatformPrefix##ProcessPlatformMessages;     \
        extern platform_get_system_properties           PlatformPrefix##GetSystemProperties;         \
        extern platform_set_window_title                PlatformPrefix##SetWindowTitle;              \
        extern platform_memory_copy                     PlatformPrefix##MemoryCopy;                  \
        extern platform_get_default_cursor              PlatformPrefix##GetDefaultCursor;

#define SetPlatformAPI(PlatformPrefix);                                                                                                         \
        static constexpr platform_window_callback                 *MainWindowCallback           = &PlatformPrefix##MainWindowCallback;          \
        static constexpr platform_open_file                       *OpenFile                     = &PlatformPrefix##OpenFile;                    \
        static constexpr platform_get_file_size                   *GetFileSize                  = &PlatformPrefix##GetFileSize;                 \
        static constexpr platform_read_file_32                    *ReadFile32                   = &PlatformPrefix##ReadFile32;                  \
        static constexpr platform_set_file_pointer                *SetFilePointer               = &PlatformPrefix##SetFilePointer;              \
        static constexpr platform_close_file_handle               *CloseFileHandle              = &PlatformPrefix##CloseFileHandle;             \
        static constexpr platform_get_processor_core_count        *GetProcessorCoreCount        = &PlatformPrefix##GetProcessorCoreCount;       \
        static constexpr platform_get_available_memory            *GetAvailableMemory           = &PlatformPrefix##GetAvailableMemory;          \
        static constexpr platform_query_timer                     *QueryTimer                   = &PlatformPrefix##QueryTimer;                  \
        static constexpr platform_query_timer_frequency           *QueryTimerFrequency          = &PlatformPrefix##QueryTimerFrequency;         \
        static constexpr platform_memory_allocate                 *MemoryAllocate               = &PlatformPrefix##MemoryAllocate;              \
        static constexpr platform_memory_free                     *MemoryFree                   = &PlatformPrefix##MemoryFree;                  \
        static constexpr platform_create_thread                   *CreateThread                 = &PlatformPrefix##CreateThread;                \
        static constexpr platform_wait_for_threads                *WaitForThreads               = &PlatformPrefix##WaitForThreads;              \
        static constexpr platform_close_thread_handle             *CloseThreadHandle            = &PlatformPrefix##CloseThreadHandle;           \
        static constexpr platform_get_monitor_refresh_rate        *GetMonitorRefreshRate        = &PlatformPrefix##GetMonitorRefreshRate;       \
        static constexpr platform_create_window                   *CreateWindow                 = &PlatformPrefix##CreateWindow;                \
        static constexpr platform_initialise_window_event_tracker *InitialiseWindowEventTracker = &PlatformPrefix##InitialiseWindowEventTracker;\
        static constexpr platform_get_window_client_size          *GetWindowClientSize          = &PlatformPrefix##GetWindowClientSize;         \
        static constexpr platform_show_window                     *ShowWindow                   = &PlatformPrefix##ShowWindow;                  \
        static constexpr platform_process_mouse_input             *ProcessMouseInput            = &PlatformPrefix##ProcessMouseInput;           \
        static constexpr platform_process_keyboard_input          *ProcessKeyboardInput         = &PlatformPrefix##ProcessKeyboardInput;        \
        static constexpr platform_initialise_input                *InitialiseInput              = &PlatformPrefix##InitialiseInput;             \
        static constexpr platform_process_platform_messages       *ProcessPlatformMessages      = &PlatformPrefix##ProcessPlatformMessages;     \
        static constexpr platform_get_system_properties           *GetSystemProperties          = &PlatformPrefix##GetSystemProperties;         \
        static constexpr platform_set_window_title                *SetWindowTitle               = &PlatformPrefix##SetWindowTitle;              \
        static constexpr platform_memory_copy                     *MemoryCopy                   = &PlatformPrefix##MemoryCopy;                  \
        static constexpr platform_get_default_cursor              *GetDefaultCursor             = &PlatformPrefix##GetDefaultCursor;

namespace kadabra {
    
    class window;
    
    struct system_properties {
        s32 PrimaryMonitorWidth;
        s32 PrimaryMonitorHeight;
        u32 PrimaryMonitorRefreshRate;
    };
    
    typedef b32 platform_open_file(file_handle *Handle, char *Path, 
                                   u32 FileAccessMode, u32 FileShareMode, 
                                   file_creation_attrib FileCreationAttrib);
    typedef b32 platform_get_file_size(file_handle FileHandle, u64 *FileSize);
    typedef b32 platform_read_file_32(file_handle File, void *MemoryPtr, 
                                      u32 BytesToRead, u32 *BytesRead);
    typedef b32 platform_set_file_pointer(file_handle File, s64 SeekDistance, 
                                          file_seek_pivot Pivot);
    typedef b32 platform_close_file_handle(file_handle Handle);
    typedef b32 platform_get_processor_core_count(u32 *Result);
    typedef b32 platform_get_available_memory(u64 *Result);
    typedef b32 platform_query_timer(timer *Timer);
    typedef b32 platform_query_timer_frequency(timer_freq *Freq);
    typedef b32 platform_memory_allocate(void **Ptr, size_t AllocSize);
    typedef b32 platform_memory_free(void **Ptr);
    typedef b32 platform_memory_copy(void *Dest, size_t DestSize, 
                                     void *Src, size_t SrcSize);
    typedef b32 platform_create_thread(thread_handle *Handle, 
                                       platform_thread_proc *ThreadProc, 
                                       void *Params, u32 *ThreadID, 
                                       size_t StackSize);
    typedef b32 platform_wait_for_threads(u32 ThreadCount, 
                                          thread_handle *ThreadHandles, 
                                          b32 WaitAll, b32 WaitInfinitely, 
                                          u32 WaitTimeLimit);
    typedef b32 platform_close_thread_handle(thread_handle Handle);
    typedef b32 platform_get_monitor_refresh_rate(u32 *RefreshRate);
    typedef cursor_handle platform_get_default_cursor();
    typedef b32 platform_create_window(window_handle *Window, 
                                       char *WindowTitle,
                                       char *WindowClass,
                                       platform_window_callback *Callback, 
                                       cursor_handle CursorHandle, 
                                       icon_handle IconHandle, 
                                       s32 AtX, s32 AtY, s32 Width, s32 Height);
    typedef b32 platform_initialise_window_event_tracker(window * const Window);
    typedef b32 platform_get_window_client_size(window_handle *Window, 
                                                vec2i *Size);
    typedef b32 platform_show_window(window_handle *Window);
    typedef b32 platform_process_mouse_input(input *Input, f32 MouseWheelInput, 
                                             window *Window, 
                                             system_properties *SysProps);
    typedef b32 platform_process_keyboard_input(input *Input);
    typedef b32 platform_initialise_input(input *Input, window *Window, 
                                          system_properties *SysProps);
    typedef b32 platform_process_platform_messages(input *Input, window *Window, 
                                                   system_properties *SysProps);
    typedef b32 platform_get_system_properties(system_properties *SysProps);
    typedef b32 platform_set_window_title(window_handle *Window, char *Title);
    
    DeclarePlatformAPI(Win32);
    
    class platform {
        public:
        
        #if _WIN32
            SetPlatformAPI(Win32);
        #elif
            #error "Platform layer is not implemented for this platform!"
        #endif
    };
    
}

#endif