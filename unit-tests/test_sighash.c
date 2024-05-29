/*****************************************************************************
 * MIT License
 *
 * Copyright (c) 2024 Hoosat Oy
 * Copyright (c) 2023 coderofstuff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <cmocka.h>

// #include "./import/blake2-impl.h"
// #include "./import/blake2b.h"
#include "./import/blake3_impl.h"
#include "./import/blake3.h"
#include "sighash.h"
#include "./transaction/types.h"
#include "types.h"

/* Start hacks */
// void os_longjmp(unsigned int exception) {}

// struct cx_xblake_s {
//     cx_blake2b_t blake2b;
//     uint64_t     m[16];
//     uint64_t     v[16];
//     uint8_t      buffer[BLAKE2B_OUTBYTES];
//     uint8_t      block1[BLAKE2B_BLOCKBYTES];
// } ;
// typedef struct cx_xblake_s cx_xblake_t;

// union cx_u {
//   cx_xblake_t blake;
// };
// union cx_u G_cx;

// global_ctx_t G_context;

// uint8_t* input_public_key;

// /* End hacks */

static void test_sighash(void **state) {
    uint8_t input_public_key_data[32] = {
        0xe9, 0xed, 0xf6, 0x7a, 0x32, 0x58, 0x68, 0xec,
        0xc7, 0xcd, 0x85, 0x19, 0xe6, 0xca, 0x52, 0x65,
        0xe6, 0x5b, 0x7d, 0x10, 0xf5, 0x60, 0x66, 0x46,
        0x1c, 0xea, 0xbf, 0x0c, 0x2b, 0xc1, 0xc5, 0xad
    };
    uint8_t input_prev_tx_id[32] = {
        0x38, 0x5a, 0xd6, 0xf2, 0xca, 0x04, 0xbd, 0x6e, 
        0x40, 0xf7, 0x21, 0x9b, 0xe4, 0x91, 0x96, 0xb0, 
        0x15, 0x22, 0x3b, 0xbb, 0x0b, 0x8d, 0xcb, 0x4f, 
        0xc9, 0xc7, 0x7d, 0x22, 0x4b, 0xda, 0x07, 0x69
    };

    uint8_t output_script_public_key[34] = {
        0x20, 0x81, 0xf7, 0x12, 0x96, 0xff, 0x0c, 0x20, 0x4a, 0xc3, 0xa6, 
        0x38, 0x12, 0xd4, 0x33, 0xa5, 0x8c, 0xa2, 0xbd, 0x61, 0x5d, 0x01, 
        0x43, 0xbc, 0xc2, 0x1c, 0x99, 0x17, 0x7e, 0x19, 0x72, 0x71, 0xdb, 
        0xac
    };
                                

    transaction_input_t txin;
    transaction_output_t txout;
    transaction_t tx;

    memset(&txin, 0, sizeof(txin));
    memset(&txout, 0, sizeof(txout));
    memset(&tx, 0, sizeof(tx));

    memcpy(txin.tx_id, input_prev_tx_id, sizeof(input_prev_tx_id));
    txin.index = 1;
    txin.value = 2;

    memcpy(txout.script_public_key, output_script_public_key, sizeof(output_script_public_key));
    txout.value = txin.value; // Assume no fee

    tx.version = 1;
    tx.tx_inputs[0] = txin;
    tx.tx_input_len = 1;
    tx.tx_outputs[0] = txout;
    tx.tx_output_len = 1;

    uint8_t out_hash[32] = {0};
    bool success = calc_sighash(&tx, &txin, input_public_key_data, out_hash, sizeof(out_hash));

    uint8_t res[32] = {
        0x2b, 0xc4, 0xfe, 0x5b, 0xb7, 0xb9, 0x79, 0x18, 0x22, 0xd1, 0x80, 0xc3, 0x3e, 0x32, 0x6e, 0xc7, 
        0x1f, 0xff, 0x64, 0x50, 0x11, 0xb7, 0x6c, 0x53, 0xbc, 0x08, 0xdc, 0x92, 0xa5, 0x85, 0xd2, 0x6d
    };

    printf("out_hash: ");
    for (int i = 0; i < sizeof(out_hash); i++) {
        printf("%02x", out_hash[i]);
    }
    printf("\n");

    assert_memory_equal(out_hash, res, 32);
    assert_true(success);
}

static void test_sighash_zeros(void **state) {
    uint8_t input_public_key_data[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t input_prev_tx_id[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint8_t output_script_public_key[34] = {
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xac
    };

    transaction_input_t txin;
    transaction_output_t txout;
    transaction_t tx;

    memset(&txin, 0, sizeof(txin));
    memset(&txout, 0, sizeof(txout));
    memset(&tx, 0, sizeof(tx));

    memcpy(txin.tx_id, input_prev_tx_id, sizeof(input_prev_tx_id));
    txin.index = 0;
    txin.value = 0;

    memcpy(txout.script_public_key, output_script_public_key, sizeof(output_script_public_key));
    txout.value = txin.value; // Assume no fee

    tx.version = 0;
    tx.tx_inputs[0] = txin;
    tx.tx_input_len = 1;
    tx.tx_outputs[0] = txout;
    tx.tx_output_len = 1;

    uint8_t out_hash[32] = {0};
    bool success = calc_sighash(&tx, &txin, input_public_key_data, out_hash, sizeof(out_hash));
    
    uint8_t res[32] = {
        0xb7, 0x98, 0x47, 0xbe, 0xb0, 0x93, 0xe2, 0x54, 0xb2, 0xf7, 0x27, 0xd3, 0xee, 0x68, 0x9c, 0x06, 
        0x7f, 0x78, 0x11, 0x0a, 0xc6, 0x8f, 0x75, 0xce, 0xc4, 0xcf, 0x80, 0x33, 0x44, 0x8a, 0xa8, 0x60
    };

    printf("out_hash: ");
    for (int i = 0; i < sizeof(out_hash); i++) {
        printf("%02x", out_hash[i]);
    }
    printf("\n");

    assert_memory_equal(out_hash, res, 32);
    assert_true(success);
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_sighash),
                                       cmocka_unit_test(test_sighash_zeros)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
