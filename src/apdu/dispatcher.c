#include <stdint.h>
#include <stdbool.h>

#include "dispatcher.h"
#include "../constants.h"
#include "../globals.h"
#include "../types.h"
#include "../io.h"
#include "../sw.h"
#include "../common/buffer.h"
#include "../handler/get_version.h"
#include "../handler/get_app_name.h"
#include "../handler/get_public_key.h"
#include "../handler/sign_tx.h"

#ifdef HAVE_DEBUG_APDU
#include "../handler/debug.h"
#endif

int apdu_dispatcher(const command_t *cmd) {
    if (cmd->cla != CLA) {
        return io_send_sw(SW_CLA_NOT_SUPPORTED);
    }

    buffer_t buf = {0};

    switch (cmd->ins) {
        case GET_VERSION:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_version();
        case GET_APP_NAME:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_app_name();
        case GET_PUBLIC_KEY:
            if (cmd->p1 > 1 || cmd->p2 > 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_get_public_key(&buf, (bool) cmd->p1);
        case SIGN_TX:
            if ((cmd->p1 == P1_START && cmd->p2 != P2_MORE) ||    //
                (cmd->p1 == P1_OUTPUTS && cmd->p2 != P2_MORE) ||  //
                (
                    cmd->p1 != P1_START &&
                    cmd->p1 != P1_OUTPUTS &&
                    cmd->p1 != P1_INPUTS &&
                    cmd->p1 != P1_NEXT_SIGNATURE
                ) ||
                (cmd->p2 != P2_LAST && cmd->p2 != P2_MORE)) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data && cmd->p1 != P1_NEXT_SIGNATURE) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_sign_tx(&buf, cmd->p1, (bool) (cmd->p2 & P2_MORE));
#ifdef HAVE_DEBUG_APDU
        case DEBUG_APDU:
            return handler_debug(cmd->p1);
#endif
        default:
            return io_send_sw(SW_INS_NOT_SUPPORTED);
    }
}
