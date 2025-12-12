#include "stm32h7xx_hal.h"

#include "comm_shm.h"
#include "user_ver.h"

AT("comm_shm") comm_shm_map_t g_comm_shm_map;

static void comm_shm_init_hsem(void);
static void comm_shm_init_uid(comm_shm_t *map);
static void comm_shm_init_ver(comm_shm_t *map);

static void comm_shm_init_pid_param(comm_shm_t *comm_shm);
static void comm_shm_sync_pid_param(comm_shm_t *comm_shm);

static void comm_shm_sync_errcode(comm_shm_t *comm_shm);

void
comm_shm_init(comm_shm_t *comm_shm, const comm_shm_cfg_t comm_shm_cfg)
{
        CFG_INIT(comm_shm, comm_shm_cfg);
        DECL(comm_shm, cfg);
        RENAME(cfg->map, map);
        RENAME(&cfg->check->lo.err, err);

        memset((void *)map, 0, sizeof(*map));

        comm_shm_init_hsem();
        comm_shm_init_uid(comm_shm);
        comm_shm_init_ver(comm_shm);

        // 获取参数文件
        map->param_file.op = COMM_SHM_OP_READ;

        err->bit.PARAM_SYNC = true;
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
comm_shm_sync_rt(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);
        RENAME(cfg->foc, foc);
        RENAME(&cfg->map->rt, rt);
        RENAME(&cfg->check->lo.err, err);

        if (err->bit.PARAM_SYNC) {
                comm_shm_init_pid_param(comm_shm);
                err->bit.PARAM_SYNC = false;
        }

        comm_shm_sync_pid_param(comm_shm);
        foc_set_ref(foc, rt->ref_pvct);

        rt->fdb_pvct                 = foc_get_fdb(foc);
        rt->inv_status.inverter_temp = foc->in.temp.inverter;
        rt->inv_status.coil_temp0    = foc->in.temp.coil[0];
        rt->inv_status.coil_temp1    = foc->in.temp.coil[1];
        rt->inv_status.v_bus         = foc->in.v_bus;
        comm_shm_sync_errcode(comm_shm);
}

static void
comm_shm_init_uid(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);

        u32 *uid = cfg->map->uid;
        uid[0]   = HAL_GetUIDw0();
        uid[1]   = HAL_GetUIDw1();
        uid[2]   = HAL_GetUIDw2();
        uid[3]   = uid[0] + uid[1] + uid[2];
}

static void
comm_shm_init_ver(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);

        comm_shm_ver_t *m7_ver = &cfg->map->m7_ver;
        m7_ver->num            = USER_VER_NUM;
        memcpy(m7_ver->str, USER_VER_STR, strlen(USER_VER_STR));
}

static void
comm_shm_init_pid_param(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);
        RENAME(cfg->foc, foc);

        cfg->map->rt.pid_param.cur_vel_kp = foc->lo.vel_pid.cfg.kp;
        cfg->map->rt.pid_param.cur_vel_ki = foc->lo.vel_pid.cfg.ki;
        cfg->map->rt.pid_param.cur_pos_kp = foc->lo.pos_pid.cfg.kp;
        cfg->map->rt.pid_param.pd_kp      = foc->lo.pd_pid.cfg.kp;
        cfg->map->rt.pid_param.pd_kd      = foc->lo.pd_pid.cfg.kd;
}

static void
comm_shm_sync_pid_param(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);
        RENAME(cfg->foc, foc);
        RENAME(&cfg->map->rt, rt);

        foc->lo.vel_pid.cfg.kp = rt->pid_param.cur_vel_kp;
        foc->lo.vel_pid.cfg.ki = rt->pid_param.cur_vel_ki;
        foc->lo.pos_pid.cfg.kp = rt->pid_param.cur_pos_kp;
        foc->lo.pd_pid.cfg.kp  = rt->pid_param.pd_kp;
        foc->lo.pd_pid.cfg.kd  = rt->pid_param.pd_kd;
}

static void
comm_shm_sync_errcode(comm_shm_t *comm_shm)
{
        DECL(comm_shm, cfg);
        RENAME(&cfg->map->rt, rt);
        RENAME(&cfg->check->lo.err, err);

        rt->errcode.ext1.bit.under_vbus         = err->bit.UNDER_VBUS;
        rt->errcode.ext1.bit.over_vbus          = err->bit.OVER_VBUS;
        rt->errcode.ext1.bit.over_cur           = err->bit.OVER_CUR;
        rt->errcode.ext1.bit.coil_over_temp     = err->bit.COIL_OVER_TEMP;
        rt->errcode.ext1.bit.inverter_over_temp = err->bit.INVERTER_OVER_TEMP;
}
