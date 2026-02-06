#define _KERNEL //for PAGE_SIZE in <limits.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "mm/includes/pmm.h"


int safe_add(int a, int b, int *result)
{
    if (a > 0 && b > INT_MAX - a) {
        return -1;
    }
    if (a < 0 && b < INT_MIN - a) {
        return -1;
    }
    
    *result = a + b;
    return 0; 
}

int safe_multiply(int a, int b, int *result)
{

    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return -1;
        } else {
            if (b < INT_MIN / a) return -1;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return -1;
        } else {
            if (a != 0 && b < INT_MAX / a) return -1;
        }
    }
    
    *result = a * b;
    return 0;
}

int validate_path(const char *path)
{
#ifdef PATH_MAX
    size_t len = 0;
    while (path[len] != '\0') {
        len++;
        if (len >= PATH_MAX) {
            return -1; 
        }
    }
    return 0; 
#else
    return 0; 
#endif
}



int validate_filename(const char *name)
{
#ifdef NAME_MAX
    size_t len = 0;
    while (name[len] != '\0') {
        len++;
        if (len > NAME_MAX) {
            return -1; 
        }
    }
    return 0; 
#else
    return 0; 
#endif
}


int clamp_to_char(int value)
{
    if (value < SCHAR_MIN)
        return SCHAR_MIN;
    if (value > SCHAR_MAX)
        return SCHAR_MAX;
    return value;
}



int unsigned_to_signed(unsigned int val, int *result)
{
    if (val > INT_MAX) {
        return -1; 
    }
    *result = (int)val;
    return 0;
}

void* safe_buffer_alloc(size_t total)
{
    size_t max_alloc_size = pmm_get_free_pages() * PAGE_SIZE;

    if (total > max_alloc_size) {
        return NULL;
    }

    void* ptr = malloc(total);
    if (!ptr) {
        return NULL;
    }

    return ptr;
}

int validate_pid(int pid)
{
#ifdef PID_MAX
    if (pid <= 0 || pid > PID_MAX) {
        return -1;
    }
#else
    if (pid <= 0) {
        return -1; 
    }
#endif
    return 0;
}


#include "assert.h"


static_assert(sizeof(char) == 1, "char must be 1 byte");
static_assert(sizeof(short) == 2, "short must be 2 bytes");
static_assert(sizeof(int) == 4, "int must be 4 bytes");


static_assert(CHAR_BIT == 8, "byte must be 8 bits");
static_assert(SCHAR_MAX == 127, "signed char max must be 127");
static_assert(INT_MAX == 2147483647, "int max must be 2^31-1");

static_assert(UCHAR_MAX > 0, "unsigned char max must be positive");
static_assert(UINT_MAX > 0, "unsigned int max must be positive");


unsigned int reverse_bits(unsigned int value)
{
    unsigned int result = 0;
    int bits = sizeof(unsigned int) * CHAR_BIT;
    
    for (int i = 0; i < bits; i++) {
        result <<= 1;
        result |= (value & 1);
        value >>= 1;
    }
    
    return result;
}

int bits_needed(unsigned int value)
{
    if (value == 0)
        return 1;
    
    int bits = 0;
    while (value > 0) {
        bits++;
        value >>= 1;
    }
    
    return bits;
}