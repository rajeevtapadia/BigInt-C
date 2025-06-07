/*
 * This is a stb style header file for handling
 * arbitrarily large integers in c
 *
 * How to use?
 *    as header file
 *      #include "BigInt.h"
 *    as source file
 *      #define BIG_INT_IMPLEMENTATION
 *      #include "BigInt.h"
 */

#ifndef BIG_INT_H

#define BIG_INT_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define BIGINT_GUARD(n) (n)->buf[(n)->size - 1]

typedef struct {
    uint32_t *buf;   // array to store numbers with base 2^32
    size_t size;     // used memeory
    size_t capacity; // total allcated memory
} BigInt;

BigInt bigint_alloc();
void bigint_clear(BigInt *bigint);
void bigint_free(BigInt *bigint);
void bigint_set(BigInt *num, char *arr);
void bigint_expand(BigInt *num);
void bigint_add(BigInt *dst, BigInt *a, BigInt *b);
void naive_add(BigInt *dest, uint32_t operand);
void naive_mult(BigInt *dest, uint32_t multiplier);
void naive_divide(BigInt *dividend, uint32_t divisor, BigInt *quo, uint32_t *rem);
void bigint_mem_dump(BigInt bigint);
void bigint_left_shift(BigInt *bigint, uint32_t shift_by);
void bigint_right_shift(BigInt *bigint, uint32_t shift_by);
void bigint_increment_size(BigInt *bigint);
void bigint_to_dec_str(BigInt bigint, char *str_buf, size_t str_buf_size);
bool bigint_isequal_uint32(BigInt a, uint32_t b);
void bigint_shallow_copy(BigInt *dst, BigInt *src);
void bigint_deep_copy(BigInt *dst, BigInt *src);
#endif

#ifdef BIG_INT_IMPLEMENTATION
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BASE 32
#define INIT_SIZE 16

BigInt bigint_alloc() {
    BigInt new_int;
    new_int.buf = (uint32_t *)calloc(1, INIT_SIZE * sizeof(uint32_t));
    new_int.size = 1;
    new_int.capacity = INIT_SIZE;
    return new_int;
}

void bigint_clear(BigInt *bigint) {
    free(bigint->buf);
    bigint->buf = (uint32_t *)calloc(1, INIT_SIZE * sizeof(uint32_t));
    assert(bigint->buf != NULL && "memory allocation failed");
    bigint->size = 1;
    bigint->capacity = INIT_SIZE;
}

void bigint_free(BigInt *bigint) {
    free(bigint->buf);
    bigint->size = 0;
    bigint->capacity = 0;
}

void bigint_set(BigInt *num, char *arr) {
    num->size = 1;
    bigint_increment_size(num);
    for (int i = 0; i < strlen(arr); i++) {
        // convert char to digit
        uint32_t digit = arr[i] - '0';

        // mult BigInt by 10
        naive_mult(num, 10);

        // add digit to BigInt
        naive_add(num, digit);
    }
}

// this function should not be used outside and is private to the library
void bigint_increment_size(BigInt *bigint) {
    bigint->size++;
    if (bigint->size >= bigint->capacity) {
        bigint_expand(bigint);
    }
}

void bigint_expand(BigInt *num) {
    num->capacity *= 2;
    num->buf = (uint32_t *)realloc(num->buf, num->capacity * sizeof(uint32_t));
    assert(num->buf != NULL && "buy more ram bro\n");
}

void bigint_add(BigInt *dst, BigInt *a, BigInt *b) {
    bigint_clear(dst);

    uint32_t carry = 0, a_curr = 0, b_curr = 0;
    size_t n = (a->size > b->size ? a->size : b->size);
    for (int i = 0; i < n; i++) {
        if (i < a->size)
            a_curr = a->buf[i];
        else
            a_curr = 0;

        if (i < b->size)
            b_curr = b->buf[i];
        else
            b_curr = 0;

        // store sum in 64 bit variable
        uint64_t sum = (uint64_t)a_curr + b_curr + carry;
        // store lower 32 bits at i th place
        dst->buf[i] = (uint32_t)(sum & 0xFFFFFFFF);
        // store higher 32 bits in carry
        carry = (uint32_t)(sum >> 32);

        if (BIGINT_GUARD(dst) != 0) {
            bigint_increment_size(dst);
        }
    }
}

void naive_add(BigInt *dest, uint32_t operand) {
    // store addition of least significant digit and operand in 64 bit variable
    uint64_t sum = (uint64_t)dest->buf[0] + operand;
    // store lower 32 bits at 0 th place
    dest->buf[0] = (uint32_t)(sum & 0xFFFFFFFF);
    // store higher 32 bits in carry for next iteration
    uint32_t carry = (uint32_t)(sum >> 32);

    // propogate carry
    size_t i = 0;
    while (carry != 0) {
        i++;
        // add carry to next digit
        uint64_t temp = dest->buf[i] + carry;
        // store lower 32 bits at i th place
        dest->buf[i] = (uint32_t)(temp & 0xFFFFFFFF);
        // update carry
        carry = (uint32_t)(temp >> 32);

        if (BIGINT_GUARD(dest) != 0) {
            bigint_increment_size(dest);
        }
    }
}

void naive_mult(BigInt *dest, uint32_t multiplier) {

    // store product of least significant digit and multiplier in 64 bit variable
    uint32_t carry = 0;
    int i;
    for (i = 0; i < dest->size; i++) {
        uint64_t product = (uint64_t)dest->buf[i] * multiplier + carry;
        // store lower 32 bits at 0 th place
        dest->buf[i] = (uint32_t)(product & 0xFFFFFFFF);
        // store higher 32 bits in carry for next iteration
        carry = (uint32_t)(product >> 32);
        if (BIGINT_GUARD(dest) != 0) {
            bigint_increment_size(dest);
        }
    }

    // If there is still a carry, add new digit
    dest->buf[i] = carry;
    if (BIGINT_GUARD(dest) != 0) {
        bigint_increment_size(dest);
    }
}

void naive_divide(BigInt *dividend, uint32_t divisor, BigInt *quo, uint32_t *rem) {
    *rem = 0;
    bigint_set(quo, "0");

    // iterate over all the bits on BigInt from MSB to LSB
    for (int i = dividend->size - 2; i >= 0; i--) {
        for (int j = 0; j < BASE; j++) {
            // extract a single bit
            uint32_t curr_bit = dividend->buf[i] & (1 << (BASE - j - 1));
            curr_bit = (curr_bit > 0 ? 1 : 0);

            // perform division
            *rem = *rem << 1;
            *rem += curr_bit;
            bigint_left_shift(quo, 1);
            if (*rem >= divisor) {
                *rem -= divisor;
                naive_add(quo, 1);
            }
        }

        if (BIGINT_GUARD(quo) != 0) {
            bigint_increment_size(quo);
        }
    }
}

void bigint_left_shift(BigInt *bigint, uint32_t shift_by) {
    assert(shift_by < 32 && "Cannot shift more than 31 bits at a time");
    // this function iterates from MSB to LSB with a 32 bit window
    // each iteration current window is shifted by `shift_by`
    // and the bits that will get discarded in the next iteration are stored in discarded
    // then the discarded bits from next window is added to current window

    if(shift_by == 0) {
        return;
    }
    
    for (int i = bigint->size - 1; i > 0; i--) {
        bigint->buf[i] <<= shift_by;
        uint32_t discarded = (bigint->buf[i - 1] >> (BASE - shift_by)) & ((1U << shift_by) - 1);
        bigint->buf[i] |= discarded;
    }
    
    // handle overflow that may happend after left shift 
    if(BIGINT_GUARD(bigint) != 0){
        bigint_increment_size(bigint);
    }
    bigint->buf[0] <<= shift_by;
}

void bigint_right_shift(BigInt *bigint, uint32_t shift_by) {
    assert(shift_by < 32 && "Cannot shift more than 31 bits at a time");
    // this function iterates from LSB to MSB with a 32 bit window
    // each iteration current window is right shifted by `shift_by`
    // and the bits that will get discarded in the next iteration are stored in discarded
    // then the discarded bits from next window is added to current window

    if (shift_by == 0) {
        return;
    }

    for (int i = 0; i < bigint->size - 1; i++) {
        bigint->buf[i] >>= shift_by;
        uint32_t discarded = bigint->buf[i + 1] & ((1 << shift_by) - 1U);
        bigint->buf[i] |= discarded << (BASE - shift_by);
    }

    // handle size reduction
    if (bigint->size > 2 && bigint->buf[bigint->size - 2] == 0) {
        bigint->size--;
    }
}
void bigint_to_dec_str(BigInt bigint, char *str_buf, size_t str_buf_size) {
    // divide the bigint successivly by 10 and push the remainder to a string buffer
    uint32_t rem = 0;
    size_t i = 0;
    BigInt quo;
    do {
        assert(i < str_buf_size && "buffer overflow");
        quo = bigint_alloc();
        naive_divide(&bigint, 10, &quo, &rem);
        str_buf[i++] = rem + '0';
        bigint_free(&bigint);
        bigint = quo;
    } while (!bigint_isequal_uint32(bigint, 0));

    // reverse string buffer
    size_t left = 0, right = --i;
    while (left < right) {
        char temp = str_buf[left];
        str_buf[left] = str_buf[right];
        str_buf[right] = temp;
        left++;
        right--;
    }
}

bool bigint_isequal_uint32(BigInt a, uint32_t b) {
    if (a.buf[0] != b) {
        return false;
    }
    for (int i = 1; i < a.size; i++) {
        if (a.buf[i] != 0) {
            return false;
        }
    }
    return true;
}

void bigint_shallow_copy(BigInt *dst, BigInt *src) {
    dst->size = src->size;
    dst->capacity = src->capacity;
    dst->buf = src->buf;
}
void bigint_deep_copy(BigInt *dst, BigInt *src) {
    dst->size = src->size;
    dst->capacity = src->capacity;
    memcpy(dst->buf, src->buf, sizeof(src->buf[0]) * src->size);
}

// writes content of buff from most significant to least to stdout
void bigint_mem_dump(BigInt bigint) {
    for (int i = bigint.size - 1; i >= 0; i--) {
        // printf("0x%x ", bigint.buf[i]);
        printf("%010u ", bigint.buf[i]);
    }
    printf("\n");
}

#endif
