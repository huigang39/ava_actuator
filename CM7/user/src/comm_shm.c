#include "stm32h7xx_hal.h"

#include "comm_shm.h"
#include "user_ver.h"

static void comm_shm_hsem_init(void);
static void comm_shm_uid_init(comm_shm_t *comm_shm);
static void comm_shm_ver_init(comm_shm_t *comm_shm);

void
comm_shm_init(comm_shm_t *comm_shm)
{
        memset((void *)COMM_SHM_ADDR_BASE, 0, COMM_SHM_SIZE_BASE);

        comm_shm_hsem_init();
        comm_shm_uid_init(comm_shm);
        comm_shm_ver_init(comm_shm);
}

void
comm_shm_sync_rt(comm_shm_t *comm_shm, foc_t *foc)
{
        DECL_PTRS(foc, lo);

        lo->vel_pid.cfg.kp = comm_shm->rt.pid_param.cur_vel_kp;
        lo->vel_pid.cfg.ki = comm_shm->rt.pid_param.cur_vel_ki;
        lo->pos_pid.cfg.kp = comm_shm->rt.pid_param.cur_pos_kp;
        lo->pd_pid.cfg.kp  = comm_shm->rt.pid_param.pd_kp;
        lo->pd_pid.cfg.kd  = comm_shm->rt.pid_param.pd_kd;
        lo->ref_pvct       = comm_shm->rt.ref_pvct;

        comm_shm->rt.fdb_pvct = lo->fdb_pvct;
}

// TODO: 写入失败检测, 放入check.c中实现
void
comm_shm_write_file(u32 file_addr, void *src, u32 size)
{
        memcpy((void *)file_addr, src, size);
        *(comm_shm_op_e *)&file_addr = COMM_SHM_OP_WRITE;
}

void
comm_shm_read_file(u32 file_addr, void *dst, u32 size)
{
        *(comm_shm_op_e *)&file_addr = COMM_SHM_OP_READ;
        memcpy(dst, (void *)file_addr, size);
}

static void
comm_shm_hsem_init(void)
{
        __HAL_RCC_HSEM_CLK_ENABLE();
        HAL_HSEM_FastTake(0);
}

static void
comm_shm_uid_init(comm_shm_t *comm_shm)
{
        u32 *uid = comm_shm->uid;
        uid[0]   = HAL_GetUIDw0();
        uid[1]   = HAL_GetUIDw1();
        uid[2]   = HAL_GetUIDw2();
        uid[3]   = uid[0] + uid[1] + uid[2];
}

static void
comm_shm_ver_init(comm_shm_t *comm_shm)
{
        comm_shm_ver_t *m7_ver = &comm_shm->m7_ver;
        m7_ver->num            = USER_VER_NUM;
        memcpy(m7_ver->str, USER_VER_STR, strlen(USER_VER_STR));
}
