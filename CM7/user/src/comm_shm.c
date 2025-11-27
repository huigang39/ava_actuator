#include "comm_shm.h"

void
comm_shm_init(comm_shm_t *comm_shm, const comm_shm_cfg_t comm_shm_cfg)
{
        CFG_INIT(comm_shm, comm_shm_cfg);
}

void
comm_shm_exec(comm_shm_t *comm_shm)
{
        DECL_PTRS(comm_shm, lo);
}
