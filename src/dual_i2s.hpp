#pragma once

#include <stdint.h>

void dual_i2s_setup();
void dual_i2s_put_buf(const uint8_t *data, uint32_t len);
