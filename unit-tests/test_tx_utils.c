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
#include <string.h>

#include <cmocka.h>

#include "transaction/utils.h"
#include "transaction/types.h"

static void test_tx_utils(void **state) {
    (void) state;

    const uint8_t good_ascii[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21};  // Hello!
    const uint8_t bad_ascii[] = {0x32, 0xc3, 0x97, 0x32, 0x3d, 0x34};   // 2×2=4

    assert_true(transaction_utils_check_encoding(good_ascii, sizeof(good_ascii)));
    assert_false(transaction_utils_check_encoding(bad_ascii, sizeof(bad_ascii)));
}

static void test_script_public_key_to_address(void **state) {
    (void) state;

    // uint8_t schnorr_spk[34] = {
    //     0x20,
    //     0xFA, 0x2B, 0x85, 0x72, 0xB6, 0x18, 0x36, 0x2A,
    //     0x26, 0x12, 0x8D, 0xB3, 0x88, 0xF0, 0x4E, 0xD1,
    //     0xA9, 0x5C, 0xCC, 0xD8, 0xE1, 0x89, 0xF9, 0xC1,
    //     0xBD, 0x6C, 0x57, 0x66, 0x8B, 0x11, 0xB2, 0xD7,
    //     0xac
    // };
    uint8_t schnorr_spk[34] = {
        0x20, 
        0xef, 0x96, 0xc7, 0xd6, 0x46, 0x75, 0x51, 0x6b, 0xa8,
        0xb0, 0x0e, 0xaf, 0xf8, 0xd8, 0xcd, 0x55, 0x1e, 0x21, 
        0xa9, 0x8b, 0xcf, 0x9b, 0xae, 0x17, 0xf2, 0x88, 0xa9, 
        0xe7, 0x3e, 0x32, 0xac, 0xc1, 
        0xac
    };

    uint8_t schnorr_address[68] = {0};

    script_public_key_to_address(schnorr_address, sizeof(schnorr_address), schnorr_spk, sizeof(schnorr_spk));

    // assert_string_equal((char *) schnorr_address, "kaspa:qrazhptjkcvrv23xz2xm8z8sfmg6jhxvmrscn7wph4k9we5tzxedwfxf0v6f8");
    assert_string_equal((char *) schnorr_address, "hoosat:qrhed37kge64z6agkq82l7xce423ugdf308ehtsh72y2nee7x2kvzxv6xqwd4");

    uint8_t p2sh_spk[35] = {
        OP_BLAKE2B, 0x20,
        0x2e, 0x15, 0xf8, 0xf9, 0x30, 0xd9, 0x8a, 0x12, 0x08, 0xde, 0xa3, 0xc7, 0x0c, 0x47, 0x85, 0x26,
        0x9b, 0xbf, 0x9f, 0x8d, 0x50, 0x66, 0x28, 0x24, 0x63, 0x12, 0xcd, 0x38, 0x77, 0x13, 0x66, 0x2a,
        OP_EQUAL
    };

    uint8_t p2sh_address[68] = {0};

    script_public_key_to_address(p2sh_address, sizeof(p2sh_address), p2sh_spk, sizeof(p2sh_spk));

    // assert_string_equal((char *) p2sh_address, "kaspa:precqv0krj3r6uyyfa36ga7s0u9jct0v4wg8ctsfde2gkrsgwgw8jgxfzfc98");
    //assert_string_equal((char *) schnorr_address, "hoosat:qrhed37kge64z6agkq82l7xce423ugdf308ehtsh72y2nee7x2kvzxv6xqwd4");
    assert_string_equal((char *) p2sh_address, "hoosat:precqv0krj3r6uyyfa36ga7s0u9jct0v4wg8ctsfde2gkrsgwgw8jgxfzfc98");

    uint8_t ecdsa_even_spk[35] = {
        0x21, 0x02,
        0xd5, 0xfd, 0xc7, 0xad, 0x11, 0xa6, 0x5d, 0x0b,
        0xbe, 0x78, 0x82, 0xfc, 0x3d, 0xbc, 0x91, 0xb5,
        0x86, 0x1d, 0x18, 0x2d, 0xcc, 0xe7, 0x9f, 0x7c,
        0x1b, 0xe5, 0xbf, 0xd3, 0x0a, 0x67, 0x7c, 0xd6,
        0xad
    };

    uint8_t ecdsa_even_address[70] = {0};

    script_public_key_to_address(ecdsa_even_address, sizeof(ecdsa_even_address), ecdsa_even_spk, sizeof(ecdsa_even_spk));

    assert_string_equal((char *) ecdsa_even_address, "kaspa:qypdtlw845g6vhgtheug9lpahjgmtpsarqkueeul0sd7t07npfnhe4s7fd82n0v");

    uint8_t ecdsa_odd_spk[35] = {
        0x21, 0x03,
        0xe3, 0x10, 0x0d, 0x85, 0xef, 0xae, 0x93, 0xe0,
        0xc2, 0xfc, 0x65, 0x4b, 0x2f, 0x0c, 0x33, 0x58,
        0x4f, 0x21, 0x3a, 0x3f, 0xdf, 0xfd, 0x02, 0x3c,
        0x82, 0x12, 0x77, 0xb2, 0x17, 0x89, 0xe0, 0x64,
        0xad
    };

    uint8_t ecdsa_odd_address[70] = {0};

    script_public_key_to_address(ecdsa_odd_address, sizeof(ecdsa_odd_address), ecdsa_odd_spk, sizeof(ecdsa_odd_spk));

    assert_string_equal((char *) ecdsa_odd_address, "kaspa:qyp7xyqdshh6aylqct7x2je0pse4snep8glallgz8jppyaajz7y7qeq4x79fq4z");
}

static void test_calc_fees(void **state) {
    (void) state;

    // clang-format off
    transaction_output_t txout[1] = {0};
    transaction_input_t txin[2] = {0};

    txin[0].value = (uint64_t)  0xFFFFFFFFFFFFFF00;
    txin[1].value = (uint64_t)  0x00000000000000FF;
    // Total:                   0xFFFFFFFFFFFFFFFF
    txout[0].value = (uint64_t) 0xFFFFFFFFFFFFF00F;
    // Difference:              0x0000000000000FF0

    uint64_t fees = calc_fees(txin, 2, txout, 1);
    uint64_t expected_fee = (uint64_t) 0x00000000000FF0;

    assert_true(fees == expected_fee);
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_tx_utils),
                                       cmocka_unit_test(test_script_public_key_to_address),
                                       cmocka_unit_test(test_calc_fees)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
