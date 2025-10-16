#include "periph_cfg.h"
#include "startup.h"

#include "other_tasks.h"
#include "param_cfg.h"

#include "user_tasks.h"

user_t user;

void set_ctl_word(user_t *user, foc_t *foc) {
        DECL_PTRS(foc, lo);

        switch (user->ctl_word) {
        case CTL_WORD_CALI: {
                foc->lo.e_state = FOC_STATE_CALI;
                break;
        }
        case CTL_WORD_DISABLE: {
                foc->lo.e_state = FOC_STATE_DISABLE;
                break;
        }
        case CTL_WORD_ENABLE: {
                foc->lo.e_state = FOC_STATE_ENABLE;
                break;
        }
        default:
                break;
        }
}

void set_ctl_mode(user_t *user, foc_t *foc) {
        DECL_PTRS(foc, lo);

        switch (user->ctl_mode) {
        case CTL_MODE_VEL: {
                lo->e_mode = FOC_MODE_VEL;
                break;
        }
        case CTL_MODE_POS: {
                lo->e_mode = FOC_MODE_POS;
                break;
        }
        case CTL_MODE_CUR: {
                lo->e_mode = FOC_MODE_CUR;
                break;
        }
        case CTL_MODE_ASC: {
                lo->e_mode = FOC_MODE_ASC;
                break;
        }
        default:
                break;
        }
}

void user_init(void) {
}

void user_loop_task(void *arg) {
        ARG_UNUSED(arg);

        log_info(&g_log, 1, "user loop\n");
        set_ctl_word(&user, &foc);
        set_ctl_mode(&user, &foc);
}
