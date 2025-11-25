#ifndef COMM_SHM_H
#define COMM_SHM_H

typedef struct {
} comm_shm_cfg_t;

typedef struct {
        bool comm_ok;
} comm_shm_lo_t;

typedef struct {
        comm_shm_lo_t lo;
} comm_shm_t;

#endif // !COMM_SHM_H
