#ifdef _MSC_VER
    #include <windows.h>
    #define DEBUGBREAK DebugBreak();
#else
    #include <csignal>
    #define DEBUGBREAK raise(SIGINT);
#endif
