#ifndef FLAGS_H
#define FLAGS_H

#include "define.h"
#include <malloc.h>
#include <stdbool.h>

extern u16 flags;

void flags_set_zf_flag();
void flags_unset_zf_flag();
const bool flags_get_zf_flag();

void flags_set_sf_flag();
void flags_unset_sf_flag();
const bool flags_get_sf_flag();
#endif // FLAGS_H
