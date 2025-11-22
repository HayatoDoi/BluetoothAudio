/*******************************************************
 *
 * dual_i2s.cpp
 *
 * 音声信号をI2Sに出力するためのプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#pragma once

#include <stdint.h>

/* I2S機能のセットアップ */
void dual_i2s_setup();

/* バッファーに音声データを追加 */
void dual_i2s_put_buf(const uint8_t *data, uint32_t len);
