

#ifndef _LIMITS_H
#define _LIMITS_H
#define CHAR_BIT    8
#define SCHAR_MIN   (-128)
#define SCHAR_MAX   127
#define UCHAR_MAX   255

#ifdef __CHAR_UNSIGNED__
    #define CHAR_MIN    0
    #define CHAR_MAX    UCHAR_MAX
#else
    #define CHAR_MIN    SCHAR_MIN
    #define CHAR_MAX    SCHAR_MAX
#endif

#define SHRT_MIN    (-32768)
#define SHRT_MAX    32767
#define USHRT_MAX   65535

#define INT_MIN     (-2147483647 - 1)
#define INT_MAX     2147483647
#define UINT_MAX    4294967295U

#if defined(__LP64__) || defined(__x86_64__) || defined(__aarch64__) || defined(_WIN64)
    #ifdef _WIN64
        #define LONG_MIN    (-2147483647L - 1)
        #define LONG_MAX    2147483647L
        #define ULONG_MAX   4294967295UL
    #else
        #define LONG_MIN    (-9223372036854775807L - 1)
        #define LONG_MAX    9223372036854775807L
        #define ULONG_MAX   18446744073709551615UL
    #endif
#else
    #define LONG_MIN    (-2147483647L - 1)
    #define LONG_MAX    2147483647L
    #define ULONG_MAX   4294967295UL
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define LLONG_MIN   (-9223372036854775807LL - 1)
    #define LLONG_MAX   9223372036854775807LL
    #define ULLONG_MAX  18446744073709551615ULL
#endif

#define MB_LEN_MAX  16


#ifdef _POSIX_SOURCE

#define ARG_MAX         131072  /* Max bytes for arguments + environment */
#define CHILD_MAX       25      /* Max simultaneous processes per user */
#define LINK_MAX        127     /* Max links to a single file */
#define MAX_CANON       255     /* Max bytes in terminal canonical input line */
#define MAX_INPUT       255     /* Max bytes in terminal input queue */
#define NAME_MAX        255     /* Max bytes in a filename */
#define NGROUPS_MAX     16      /* Max supplemental group IDs */
#define OPEN_MAX        256     /* Max open files per process */
#define PATH_MAX        4096    /* Max bytes in pathname */
#define PIPE_BUF        512     /* Max bytes for atomic pipe writes */
#define STREAM_MAX      256     // Max number of open stdio streams */
#define TZNAME_MAX      6       /* Max bytes in timezone name */

#endif

#ifdef _KERNEL

#ifndef PAGE_SIZE
#define PAGE_SIZE       4096
#endif

#define PID_MAX         32768   
#define TASK_COMM_LEN   16     

/* File system limits */
#define MAXPATHLEN      PATH_MAX
#define MAXNAMLEN       NAME_MAX

#define HOST_NAME_MAX   64     

#endif

#ifdef __GNUC__

    _Static_assert(CHAR_BIT == __CHAR_BIT__, "CHAR_BIT mismatch");
    _Static_assert(SCHAR_MAX == __SCHAR_MAX__, "SCHAR_MAX mismatch");
    _Static_assert(SHRT_MAX == __SHRT_MAX__, "SHRT_MAX mismatch");
    _Static_assert(INT_MAX == __INT_MAX__, "INT_MAX mismatch");
    _Static_assert(LONG_MAX == __LONG_MAX__, "LONG_MAX mismatch");
    
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        _Static_assert(LLONG_MAX == __LONG_LONG_MAX__, "LLONG_MAX mismatch");
    #endif
#endif

#endif 