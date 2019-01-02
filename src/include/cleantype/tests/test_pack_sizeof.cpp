// Based on the number of args in the parameter
// packs, the return type will be *different*
// (using these macros for clarity)
#define EMPTY_ARGS 42u
#define NONEMPTY_ARGS false

///////////////////////////////////////
// C++ 17 version, using constexpr if
///////////////////////////////////////
template<typename... Args>
constexpr auto one_cpp17()
{
    if constexpr((sizeof...(Args) > 0))
        return NONEMPTY_ARGS;
    else
        return EMPTY_ARGS;
}

template<typename... Args, typename T>
constexpr auto two_cpp17(T t)
{
    if constexpr((sizeof...(Args) > 0))
        return NONEMPTY_ARGS;
    else
        return EMPTY_ARGS;
}

void test_cpp17()
{
    static_assert( one_cpp17<int>() == NONEMPTY_ARGS, "" );
    static_assert( one_cpp17<>() == EMPTY_ARGS, "" );
    static_assert( one_cpp17() == EMPTY_ARGS, "" );

    static_assert( two_cpp17(42) == EMPTY_ARGS, "" );
    static_assert( two_cpp17<>(42) == EMPTY_ARGS, "" );
    static_assert( two_cpp17<int, int>(42) == NONEMPTY_ARGS, "" );

    // Yikes ! when using only one arg (supposedly T), sizeof...(Args) is still > 0  !!!
    // => As as consequence, the static_assert below passes:
    // static_assert( two_cpp17<int>(42) == NONEMPTY_ARGS, "" );
    // Although, I would have thought otherwise, i.e this :
    // static_assert( two_cpp17<int>(42) == EMPTY_ARGS, "" );
}


///////////////////////////////////////
// C++ 14 version, using overloads
///////////////////////////////////////
template<typename... Args>
constexpr auto one_cpp14()
{
    return NONEMPTY_ARGS;
}

template<>
constexpr auto one_cpp14()
{
    return EMPTY_ARGS;
}

//template<typename T, typename... Args>
//template <typename T, typename First, typename... Rest>
template <typename First,typename Second, typename... Rest, typename T>
constexpr auto two_cpp14(T t)
{
    return NONEMPTY_ARGS;
}

template<typename T>
constexpr auto two_cpp14(T t)
{
    return EMPTY_ARGS;
}


void test_cpp14()
{
    static_assert( one_cpp14<int>() == NONEMPTY_ARGS, "" );
    static_assert( one_cpp14<>() == EMPTY_ARGS, "" );
    static_assert( one_cpp14() == EMPTY_ARGS, "" );

    static_assert( two_cpp14(42) == EMPTY_ARGS, "" );
    static_assert( two_cpp14<>(42) == EMPTY_ARGS, "" );
    static_assert( two_cpp14<int>(42) == EMPTY_ARGS, "" );
    static_assert( two_cpp14<int, int>(42) == NONEMPTY_ARGS, "" );
    static_assert( two_cpp14<int, int, int>(42) == NONEMPTY_ARGS, "" );
}
