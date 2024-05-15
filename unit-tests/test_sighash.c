/*****************************************************************************
 * MIT License
 *
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
    uint8_t input_public_key_data[32] = {0xe9, 0xed, 0xf6, 0x7a, 0x32, 0x58, 0x68, 0xec,
                                    0xc7, 0xcd, 0x85, 0x19, 0xe6, 0xca, 0x52, 0x65,
                                    0xe6, 0x5b, 0x7d, 0x10, 0xf5, 0x60, 0x66, 0x46,
                                    0x1c, 0xea, 0xbf, 0x0c, 0x2b, 0xc1, 0xc5, 0xad};

    // uint8_t input_prev_tx_id[32] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    //                                 0x99, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    //                                 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    //                                 0x99, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    uint8_t input_prev_tx_id[32] = {0x38, 0x5a, 0xd6, 0xf2, 0xca, 0x04, 0xbd, 0x6e, 
                                    0x40, 0xf7, 0x21, 0x9b, 0xe4, 0x91, 0x96, 0xb0, 
                                    0x15, 0x22, 0x3b, 0xbb, 0x0b, 0x8d, 0xcb, 0x4f, 
                                    0xc9, 0xc7, 0x7d, 0x22, 0x4b, 0xda, 0x07, 0x69};

    // uint8_t output_script_public_key[34] = {0x20,
    //                                         0xc6, 0x2c, 0xf3, 0x0e, 0x4e, 0x57, 0xc5, 0x92,
    //                                         0x20, 0x86, 0x46, 0x02, 0x35, 0xd5, 0x15, 0x7a,
    //                                         0x62, 0xf2, 0x66, 0x6a, 0xa2, 0x27, 0x07, 0xcb,
    //                                         0x9b, 0x58, 0x8f, 0x62, 0x11, 0xfc, 0x0e, 0xd6,
    //                                         0xac};
    uint8_t output_script_public_key[34] = {0x20, 
                                            0x81, 0xf7, 0x12, 0x96, 0xff, 0x0c, 0x20, 0x4a, 
                                            0xc3, 0xa6, 0x38, 0x12, 0xd4, 0x33, 0xa5, 0x8c, 
                                            0xa2, 0xbd, 0x61, 0x5d, 0x01, 0x43, 0xbc, 0xc2, 
                                            0x1c, 0x99, 0x17, 0x7e, 0x19, 0x72, 0x71, 0xdb, 
                                            0xac};
                                

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
        // 0x7c, 0xcd, 0xa6, 0xc6, 0x4a, 0x18, 0x1e, 0x62,
        //                0x63, 0xf0, 0xee, 0xe2, 0xed, 0xc8, 0x59, 0xdb,
        //                0xcd, 0x9d, 0xe7, 0x17, 0xc0, 0x65, 0xea, 0x8e,
        //                0x7d, 0xce, 0x10, 0x81, 0xbe, 0xc5, 0xba, 0xa5
        0x21, 0xb1, 0x69, 0xcd, 0x90, 0xa9, 0x02, 0x8c, 0xbe, 0x06, 0x9c, 0xf0, 0x94, 0xa9, 0x17, 0xd3,
        0xe4, 0xf7, 0x1e, 0x9e, 0x4c, 0x36, 0x81, 0x38, 0xcc, 0x14, 0x37, 0x58, 0x2d, 0x92, 0xf3, 0xdf
        };

    printf("out_hash: ");
    for (int i = 0; i < sizeof(out_hash); i++) {
        printf("%02x", out_hash[i]);
    }
    printf("\n");

    assert_memory_equal(out_hash, res, 32);
    assert_true(success);
}

//612d56e633ee5da1caa4563c6ace0c98d3549ad4e3d2b1f1ea6810e6c34047bd
//f7d2c11a79a042512d510b923dbea1b586d0f1d36806a522466a89e5787be8b7
static void test_sighash_zeros(void **state) {
    uint8_t input_public_key_data[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t input_prev_tx_id[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t output_script_public_key[34] = {0x20,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0xac};

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

    // uint8_t res[32] = {0x61, 0x2d, 0x56, 0xe6, 0x33, 0xee, 0x5d, 0xa1,
    //                    0xca, 0xa4, 0x56, 0x3c, 0x6a, 0xce, 0x0c, 0x98,
    //                    0xd3, 0x54, 0x9a, 0xd4, 0xe3, 0xd2, 0xb1, 0xf1,
    //                    0xea, 0x68, 0x10, 0xe6, 0xc3, 0x40, 0x47, 0xbd};
    uint8_t res[32] = {0xf7, 0xd2, 0xc1, 0x1a, 0x79, 0xa0, 0x42, 0x51, 0x2d, 0x51, 0x0b, 0x92, 0x3d, 0xbe, 0xa1, 0xb5,
    0x86, 0xd0, 0xf1, 0xd3, 0x68, 0x06, 0xa5, 0x22, 0x46, 0x6a, 0x89, 0xe5, 0x78, 0x7b, 0xe8, 0xb7};

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
