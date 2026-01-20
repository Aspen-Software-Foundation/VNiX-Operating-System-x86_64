/*

            The AMPOS Operating System copyright under the Aspen Software Foundation (And the file's corresponding developers).
            
            This project is licensed under the GNU Public License v2;
            For more information, visit "https://www.gnu.org/licenses/gpl-2.0.en.html"
            OR see to the "LICENSE" file.

*/

#include "util/math.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t roundf(float number) {
    return (number >= 0.0f) ? (int)(number + 0.5f) : (int)(number - 0.5f);
}

double fabs(double x) {
    return x < 0.0 ? -x : x;
}

double fmod(double x, double y) {
    double result;
    asm (
        "1:\n\t"       // loop label
        "fprem1;"      // perform floating-point remainder operation
        "fnstsw %%ax;" // store the FPU status word in AX
        "sahf;"        // transfer status flags from AH to CPU flags
        "jp 1b;"       // if parity flag is set, jump back to label 1
        : "=t" (result)    // output: top of FPU stack goes into result
        : "0" (x), "u" (y) // inputs: 'x' is in the top of the FPU stack and 'y' in the other register
        : "ax", "cc"       // clobbered registers: AX and condition codes
    );
    return result;
}

float fmodf(float x, float y) {
    float result;
    asm (
        "1:\n\t"       // loop label
        "fprem1;"      // perform floating-point remainder operation
        "fnstsw %%ax;" // store the FPU status word in AX
        "sahf;"        // transfer status flags from AH to CPU flags
        "jp 1b;"       // if parity flag is set, jump back to label 1
        : "=t" (result)    // output: top of FPU stack goes into result
        : "0" (x), "u" (y) // inputs: 'x' is in the top of the FPU stack and 'y' in the other register
        : "ax", "cc"       // clobbered registers: AX and condition codes
    );
    return result;
}



double sin(double x) {
    double result;
    asm (
        "fldl %1;"    // Load x onto the FPU stack
        "fsin;"      // Compute sine of ST(0)
        "fstpl %0;"   // Store the result into result and pop the FPU stack
        : "=m" (result)
        : "m" (x)
    );
    return result;
}


double cos(double x) {
    double result;
    asm (
        "fldl %1;"    // Load x onto the FPU stack
        "fcos;"      // Compute cosine of ST(0)
        "fstpl %0;"   // Store the result into result and pop the FPU stack
        : "=m" (result)
        : "m" (x)
    );
    return result;
}

float sinf(float x) {
    float result;
    asm (
        "flds %1;"    // Load x onto the FPU stack
        "fsin;"      // Compute sine of ST(0)
        "fstps %0;"   // Store the result into result and pop the FPU stack
        : "=m" (result)
        : "m" (x)
    );
    return result;
}


float cosf(float x) {
    float result;
    asm (
        "flds %1;"    // Load x onto the FPU stack
        "fcos;"      // Compute cosine of ST(0)
        "fstps %0;"   // Store the result into result and pop the FPU stack
        : "=m" (result)
        : "m" (x)
    );
    return result;
}

double tan(double x) {
    x = fmod(x + PI, 2 * PI);
    if (x < 0)
        x += 2 * PI;
    // Shift to range -PI to PI
    x = x - PI;
    double cos_x = cos(x);

    // Define a small threshold to check for values close to zero
    const double epsilon = 1e-10;

    if (fabs(cos_x) < epsilon) {
        // Handle the case where cosine is too close to zero
        if (sin(x) > 0)
            return 1000000;
        else
            return -1000000;
    }

    return sin(x) / cos_x;
}

float tanf(float x) {
    x = fmod(x + PI, 2 * PI);
    if (x < 0)
        x += 2 * PI;
    // Shift to range -PI to PI
    x = x - PI;
    float cos_x = cosf(x);

    // Define a small threshold to check for values close to zero
    const float epsilon = 1e-10;

    if (fabs(cos_x) < epsilon) {
        // Handle the case where cosine is too close to zero
        if (sinf(x) > 0)
            return 1000000;
        else
            return -1000000;
    }

    return sinf(x) / cos_x;
}


// black magic
double pow(double x, double y) {
    double out;
    asm(
            "fyl2x;"
            "fld %%st;"
            "frndint;"
            "fsub %%st,%%st(1);"
            "fxch;"
            "fchs;"
            "f2xm1;"
            "fld1;"
            "faddp;"
            "fxch;"
            "fld1;"
            "fscale;"
            "fstp %%st(1);"
            "fmulp;" : "=t"(out) : "0"(x),"u"(y) : "st(1)" );
    return out;
}



uint64_t __udivdi3(uint64_t n, uint32_t d) {
    if (d == 0) {INTERRUPT(0); return 0;} // avoid division by zero
    if (n < d) return 0;

    uint64_t q = 0;
    uint64_t r = n;
    int shift = 0;

    while ((d << shift) <= r && shift < 32) shift++;

    for (int i = shift - 1; i >= 0; i--) {
        if (r >= ((uint64_t)d << i)) {
            r -= ((uint64_t)d << i);
            q |= ((uint64_t)1 << i);
        }
    }

    return q;
}


uint64_t __umoddi3(uint64_t n, uint32_t d) {
    if (d == 0) {INTERRUPT(0); return 0;} // avoid division by zero
    if (n < d) return n;

    uint64_t r = n;
    int shift = 0;

    while ((d << shift) <= r && shift < 32) shift++;

    for (int i = shift - 1; i >= 0; i--) {
        if (r >= ((uint64_t)d << i)) {
            r -= ((uint64_t)d << i);
        }
    }

    return r;
}