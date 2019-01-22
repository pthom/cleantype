#ifdef _MSC_VER
#define DEBUGBREAK DebugBreak();
#else
#include <csignal>
#define DEBUGBREAK raise(SIGINT);
#endif

#define LOG(...) std::cout << __VA_ARGS__ << "\n";
#define LOG_VAR(...) std::cout << #__VA_ARGS__ << "\n" << __VA_ARGS__ << "\n";
