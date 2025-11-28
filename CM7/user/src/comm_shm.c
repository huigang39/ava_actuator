#include "comm_shm.h"

#include "stm32h7xx_hal.h"

static void comm_shm_hsem_init(void);
static void comm_shm_uid_init(comm_shm_t *comm_shm);

void
comm_shm_init(comm_shm_t *comm_shm)
{
        memset((void *)COMM_SHM_ADDR_BASE, 0, COMM_SHM_SIZE_BASE);

        comm_shm_hsem_init();
        comm_shm_uid_init(comm_shm);
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

        uid[0] = HAL_GetUIDw0();
        uid[1] = HAL_GetUIDw1();
        uid[2] = HAL_GetUIDw2();
        uid[3] = uid[0] + uid[1] + uid[2];
}
