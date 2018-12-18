#ifdef _MSC_VER
    #define DEBUGBREAK DebugBreak();
#else
    #include <csignal>
    #define DEBUGBREAK raise(SIGINT);
#endif
