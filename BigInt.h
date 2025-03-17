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
#include <stdint.h>
#include <stdlib.h>

typedef struct binint {
    uint32_t *buf;   // array to store numbers with base 2^32
    size_t size;     // used memeory
    size_t capacity; // total allcated memory
} BigInt;

BigInt bigint_alloc();
void bigint_set(BigInt *num, char *arr);
void bigint_expand(BigInt *num);
void naive_add(BigInt *dest, uint32_t operand);
void naive_mult(BigInt *dest, uint32_t multiplier);
void bigint_mem_dump(BigInt bigint);

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
    new_int.buf = (uint32_t *)malloc(INIT_SIZE * sizeof(uint32_t));
    new_int.size = 1;
    new_int.capacity = INIT_SIZE;
    return new_int;
}

void bigint_set(BigInt *num, char *arr) {
    for (int i = 0; i < strlen(arr); i++) {
        // convert char to digit
        uint32_t digit = arr[i] - '0';

        // mult BigInt by 10
        naive_mult(num, 10);

        // add digit to BigInt
        naive_add(num, digit);
    }
}

void bigint_expand(BigInt *num) {
    num->capacity *= 2;
    num->buf = (uint32_t *)realloc(num->buf, num->capacity * sizeof(uint32_t));
    assert(num->buf != NULL && "buy more ram bro\n");
}

void bigint_add(BigInt *dst, BigInt *src) {
    uint32_t carry = 0;
    for (int i = 0; i < dst->size; i++) {
        // store sum in 64 bit variable
        uint64_t sum = (uint64_t)dst->buf[i] + src->buf[i] + carry;
        // store lower 32 bits at i th place
        dst->buf[i] = (uint32_t)(sum & 0xFFFFFFFF);
        // store higher 32 bits in carry
        carry = (uint32_t)(sum >> 32);
    }
}

void naive_add(BigInt *dest, uint32_t operand) {
    // store addition of least significant digit and operand in 64 bit variable
    uint64_t product = (uint64_t)dest->buf[0] + operand;
    // store lower 32 bits at 0 th place
    dest->buf[0] = (uint32_t)(product & 0xFFFFFFFF);
    // store higher 32 bits in carry for next iteration
    uint32_t carry = (uint32_t)(product >> 32);

    // propogate carry
    size_t i = 0;
    while (carry != 0) {
        i++;
        if (i == dest->size) {
            bigint_expand(dest);
        }
        // add carry to next digit
        uint64_t temp = dest->buf[i] + carry;
        // store lower 32 bits at i th place
        dest->buf[i] = (uint32_t)(temp & 0xFFFFFFFF);
        // update carry
        carry = (uint32_t)(temp >> 32);
        dest->size++;
    }
}

void naive_mult(BigInt *dest, uint32_t multiplier) {
    // store product of least significant digit and multiplier in 64 bit variable
    uint64_t product = (uint64_t)dest->buf[0] * multiplier;
    // store lower 32 bits at 0 th place
    dest->buf[0] = (uint32_t)(product & 0xFFFFFFFF);
    // store higher 32 bits in carry for next iteration
    uint32_t carry = (uint32_t)(product >> 32);

    // propogate carry
    size_t i = 0;
    while (carry != 0) {
        i++;
        if (i == dest->size) {
            bigint_expand(dest);
        }
        // add carry to next digit
        uint64_t temp = dest->buf[i] + carry;
        // store lower 32 bits at i th place
        dest->buf[i] = (uint32_t)(temp & 0xFFFFFFFF);
        // update carry
        carry = (uint32_t)(temp >> 32);
        dest->size++;
    }
}

void bigint_print(BigInt *bigint) {}

// writes content of buff from most significant to least to stdout
void bigint_mem_dump(BigInt bigint) {
    for (int i = bigint.size - 1; i >= 0; i--) {
        // printf("0x%x ", bigint.buf[i]);
        printf("%u ", bigint.buf[i]);
    }
    printf("\n");
}

#endif
