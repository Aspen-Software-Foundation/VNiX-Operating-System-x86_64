

#ifndef _ASSERT_H
#define _ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif


void __assert_fail(const char *expr, const char *file, unsigned int line, const char *func) __attribute__((noreturn));

#if defined(__GNUC__) || defined(__clang__)
    #define __ASSERT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define __ASSERT_FUNCTION __func__
#else
    #define __ASSERT_FUNCTION ((const char *)0)
#endif

#ifdef NDEBUG
    #define assert(expr) ((void)0)
#else
    #define assert(expr) \
        ((expr) ? (void)0 : __assert_fail(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define static_assert _Static_assert
#elif defined(__cplusplus) && __cplusplus >= 201103L
    //do nothing, since C 11+ have assert built in.
#else
    //Fallback for older C standards, just in case ;)
    #define __STATIC_ASSERT_CONCAT_(a, b) a##b
    #define __STATIC_ASSERT_CONCAT(a, b) __STATIC_ASSERT_CONCAT_(a, b)
    #define static_assert(expr, msg) \
        typedef char __STATIC_ASSERT_CONCAT(static_assert_, __LINE__)[(expr) ? 1 : -1]
#endif


#ifdef NDEBUG
    #define assert_not_reached() __builtin_unreachable()
#else
    #define assert_not_reached() \
        __assert_fail("Code should not be reached", __FILE__, __LINE__, __ASSERT_FUNCTION)
#endif

//assert, but with a twist (a custom mesg)
#ifdef NDEBUG
    #define assert_msg(expr, msg) ((void)0)
#else
    #define assert_msg(expr, msg) \
        ((expr) ? (void)0 : __assert_fail(msg, __FILE__, __LINE__, __ASSERT_FUNCTION))
#endif

#ifdef NDEBUG
    #define assert_not_null(ptr) ((void)0)
#else
    #define assert_not_null(ptr) \
        assert_msg((ptr) != NULL, "Null pointer: " #ptr)
#endif

#ifdef NDEBUG
    #define assert_equal(a, b) ((void)0)
#else
    #define assert_equal(a, b) \
        assert_msg((a) == (b), #a " != " #b)
#endif

#ifdef NDEBUG
    #define assert_in_range(val, min, max) ((void)0)
#else
    #define assert_in_range(val, min, max) \
        assert_msg(((val) >= (min)) && ((val) <= (max)), \
                   #val " out of range [" #min ", " #max "]")
#endif

#ifdef NDEBUG
    #define verify(expr) ((void)(expr))
#else
    #define verify(expr) assert(expr)
#endif

#define assert_size(type, size) \
    static_assert(sizeof(type) == (size), "Size of " #type " must be " #size " bytes")

#define assert_offset(type, member, offset) \
    static_assert(__builtin_offsetof(type, member) == (offset), \
                  "Offset of " #type "." #member " must be " #offset)

#ifdef __cplusplus
}
#endif

#endif