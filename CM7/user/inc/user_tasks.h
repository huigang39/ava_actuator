#ifndef USER_TASKS_H
#define USER_TASKS_H

typedef enum {
        CTL_WORD_NULL,
        CTL_WORD_CALI,
        CTL_WORD_DISABLE,
        CTL_WORD_ENABLE,
} ctl_word_e;

typedef enum {
        CTL_MODE_NULL,
        CTL_MODE_CUR,
        CTL_MODE_VEL,
        CTL_MODE_POS,
        CTL_MODE_ASC,
} ctl_mode_e;

typedef struct {
        ctl_word_e ctl_word;
        ctl_mode_e ctl_mode;
} user_t;

extern user_t user;

void set_ctl_word(user_t *user, foc_t *foc);
void set_ctl_mode(user_t *user, foc_t *foc);

void user_init(void);
void user_loop_task(void *arg);

#endif // !USER_TASKS_H
