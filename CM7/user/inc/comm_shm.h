#ifndef COMM_SHM_H
#define COMM_SHM_H

#include "module.h"

typedef struct {
} comm_shm_cfg_t;

typedef struct {
        bool comm_ok;
} comm_shm_lo_t;

typedef struct {
        comm_shm_lo_t lo;
} comm_shm_t;

void comm_shm_init(comm_shm_t *comm_shm, const comm_shm_cfg_t comm_shm_cfg);
void comm_shm_exec(comm_shm_t *comm_shm);

#endif // !COMM_SHM_H
