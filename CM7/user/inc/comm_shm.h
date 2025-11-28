#ifndef COMM_SHM_H
#define COMM_SHM_H

#include "module.h"

typedef enum {
        COMM_SHM_ADDR_BASE = 0x38000000,
        COMM_SHM_ADDR_UID  = 0x38004C00,
        COMM_SHM_ADDR_CFG  = 0x3800E000,
        COMM_SHM_ADDR_RT   = 0x3800F000,
} comm_shm_addr_e;

typedef enum {
        COMM_SHM_SIZE_BASE = 0x20000,
        COMM_SHM_SIZE_UID  = 0x00010,
        COMM_SHM_SIZE_CFG  = 0x01000,
        COMM_SHM_SIZE_RT   = 0x00400,
} comm_shm_size_e;

typedef enum {
        COMM_SHM_OP_NONE  = 0x00000000,
        COMM_SHM_OP_IDLE  = 0x11111111,
        COMM_SHM_OP_READ  = 0x22222222,
        COMM_SHM_OP_WRITE = 0x33333333,
        COMM_SHM_OP_ERROR = 0x44444444,
} comm_shm_op_e;

typedef struct {
        u32            uid[4];
        foc_ref_pvct_t ref_pvct;
        foc_fdb_pvct_t fdb_pvct;
} comm_shm_t;

void comm_shm_init(comm_shm_t *comm_shm);

#endif // !COMM_SHM_H
