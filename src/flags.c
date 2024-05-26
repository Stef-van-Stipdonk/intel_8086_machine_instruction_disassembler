#include "flags.h"

// Each bit corrisponds to a specific flag being set.
// The ones currently used and implemented are:
// ZF 7th bit
// SF 8th bit
u16 flags = 0;

void flags_set_zf_flag() { flags |= (1 << 6); }
void flags_unset_zf_flag() { flags &= ~(1 << 6); }

const bool flags_get_zf_flag() { return (flags & (1 << 6)) != 0; };

void flags_set_sf_flag() { flags |= (1 << 7); };
void flags_unset_sf_flag() { flags &= ~(1 << 7); }
const bool flags_get_sf_flag() { return (flags & (1 << 7)) != 0; };
