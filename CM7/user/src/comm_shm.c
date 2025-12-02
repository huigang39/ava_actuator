#include "stm32h7xx_hal.h"

#include "comm_shm.h"
#include "user_ver.h"

static void comm_shm_init_hsem(void);
static void comm_shm_init_uid(comm_shm_t *comm_shm);
static void comm_shm_init_ver(comm_shm_t *comm_shm);

static void comm_shm_init_pid_param(comm_shm_t *comm_shm, foc_t *foc);
static void comm_shm_sync_pid_param(comm_shm_t *comm_shm, foc_t *foc);

void
comm_shm_init(comm_shm_t *comm_shm)
{
        memset((void *)COMM_SHM_ADDR_BASE, 0, COMM_SHM_SIZE_BASE);

        comm_shm_init_hsem();
        comm_shm_init_uid(comm_shm);
        comm_shm_init_ver(comm_shm);
}

// TODO: 写入失败检测, 放入check.c中实现
void
comm_shm_store(void *dst, void *src, usz size)
{
        u32 *op = (u32 *)dst;
        memcpy(op + 1, src, size);
        *op = COMM_SHM_OP_WRITE;
}

void
comm_shm_load(void *dst, void *src, usz size)
{
        u32 *op = (u32 *)src;
        *op     = COMM_SHM_OP_READ;

        do {
                memcpy(dst, op + 1, size);
        } while (*op != COMM_SHM_OP_IDLE || *op == COMM_SHM_OP_ERROR);
}

static void
comm_shm_init_hsem(void)
{
        __HAL_RCC_HSEM_CLK_ENABLE();
        HAL_HSEM_FastTake(0);
}

void
comm_shm_sync_rt(comm_shm_t *comm_shm, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        if (!lo->fault.PARAM_SYNC) {
                comm_shm_init_pid_param(comm_shm, foc);
                lo->fault.PARAM_SYNC = true;
        }

        comm_shm_sync_pid_param(comm_shm, foc);
        foc_set_ref(foc, comm_shm->rt.ref_pvct);

        comm_shm->rt.fdb_pvct = lo->fdb_pvct;
}

static void
comm_shm_init_uid(comm_shm_t *comm_shm)
{
        u32 *uid = comm_shm->uid;
        uid[0]   = HAL_GetUIDw0();
        uid[1]   = HAL_GetUIDw1();
        uid[2]   = HAL_GetUIDw2();
        uid[3]   = uid[0] + uid[1] + uid[2];
}

static void
comm_shm_init_ver(comm_shm_t *comm_shm)
{
        comm_shm_ver_t *m7_ver = &comm_shm->m7_ver;
        m7_ver->num            = USER_VER_NUM;
        memcpy(m7_ver->str, USER_VER_STR, strlen(USER_VER_STR));
}

static void
comm_shm_init_pid_param(comm_shm_t *comm_shm, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        comm_shm->rt.pid_param.cur_vel_kp = lo->vel_pid.cfg.kp;
        comm_shm->rt.pid_param.cur_vel_ki = lo->vel_pid.cfg.ki;
        comm_shm->rt.pid_param.cur_pos_kp = lo->pos_pid.cfg.kp;
        comm_shm->rt.pid_param.pd_kp      = lo->pd_pid.cfg.kp;
        comm_shm->rt.pid_param.pd_kd      = lo->pd_pid.cfg.kd;
}

static void
comm_shm_sync_pid_param(comm_shm_t *comm_shm, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        lo->vel_pid.cfg.kp = comm_shm->rt.pid_param.cur_vel_kp;
        lo->vel_pid.cfg.ki = comm_shm->rt.pid_param.cur_vel_ki;
        lo->pos_pid.cfg.kp = comm_shm->rt.pid_param.cur_pos_kp;
        lo->pd_pid.cfg.kp  = comm_shm->rt.pid_param.pd_kp;
        lo->pd_pid.cfg.kd  = comm_shm->rt.pid_param.pd_kd;
}
