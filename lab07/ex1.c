#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "ex1.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            __m128i chunk = _mm_loadu_si128((__m128i *)(vals + i));
            __m128i mask = _mm_cmpgt_epi32(chunk, _127);
            __m128i result_vector = _mm_and_si128(chunk, mask);
            int temp[4];
            _mm_storeu_si128((__m128i *)temp, result_vector);
            result += temp[0] + temp[1] + temp[2] + temp[3];
            }
        /* Hint: you'll need a tail case. */
        for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        /* Copy your sum_simd() implementation here, and unroll it */
        for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
            __m128i chunk1 = _mm_loadu_si128((__m128i *)(vals + i));
            __m128i chunk2 = _mm_loadu_si128((__m128i *)(vals + i + 4));
            __m128i chunk3 = _mm_loadu_si128((__m128i *)(vals + i + 8));
            __m128i chunk4 = _mm_loadu_si128((__m128i *)(vals + i + 12));
            __m128i mask1 = _mm_cmpgt_epi32(chunk1, _127);
            __m128i mask2 = _mm_cmpgt_epi32(chunk2, _127);
            __m128i mask3 = _mm_cmpgt_epi32(chunk3, _127);
            __m128i mask4 = _mm_cmpgt_epi32(chunk4, _127);
            __m128i result_vector1 = _mm_and_si128(chunk1, mask1);
            __m128i result_vector2 = _mm_and_si128(chunk2, mask2);
            __m128i result_vector3 = _mm_and_si128(chunk3, mask3);
            __m128i result_vector4 = _mm_and_si128(chunk4, mask4);
            int temp[16];
            _mm_storeu_si128((__m128i *)temp, result_vector1);
            _mm_storeu_si128((__m128i *)(temp + 4), result_vector2);
            _mm_storeu_si128((__m128i *)(temp + 8), result_vector3);
            _mm_storeu_si128((__m128i *)(temp + 12), result_vector4);
            result += temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7] + temp[8] + temp[9] + temp[10] + temp[11] + temp[12] + temp[13] + temp[14] + temp[15];
            }
        /* Hint: you'll need 1 or maybe 2 tail cases here. */
        for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
