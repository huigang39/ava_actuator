#ifndef STARTUP_H
#define STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module.h"

extern foc_t   foc;
extern scher_t scher;
extern fft_t   fft;

void init(void);
void foc_loop(void);
void scher_loop(void);

#ifdef __cplusplus
}
#endif

#endif //! STARTUP_H
