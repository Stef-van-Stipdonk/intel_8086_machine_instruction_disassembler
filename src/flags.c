#include "flags.h"

// Each bit corrisponds to a specific flag being set.
// The ones currently used and implemented are:
// ZF 7th bit
// SF 8th bit
u16 flags = 0;

static void set_zf_flag_state(bool state) { flags = flags | (1 << 6); }

static const bool get_zf_flag_state() { return flags | (1 << 6); };

static void set_sf_flag_state(bool state) { flags = flags | (1 << 7); };
static const bool get_sf_flag_state() { return flags | (1 << 7); };
