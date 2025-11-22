/*******************************************************
 *
 * dsp.cpp
 *
 * 信号処理ためのプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#pragma once
#include <stdint.h>

/* DSP機能のセットアップ */
void dsp_setup();

/* 信号処理を実行する */
void dsp_process(int16_t *front_buff, int16_t *rear_buff, int32_t count);
