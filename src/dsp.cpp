/*******************************************************
 *
 * dsp.cpp
 *
 * 信号処理ためのプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#include "config.hpp"

#include <Arduino.h>
#include <stdint.h>

enum CHANNEL_MAP
{
    FRONT_LEFT = 0,
    FRONT_RIGHT = 1,
    REAR_LEFT = 2,
    REAR_RIGHT = 3,
    CHANNEL_MAX = 4,
};
static const int32_t VOLUME_MAP[CHANNEL_MAX] = {
    [FRONT_LEFT] = 100,  /* % */
    [FRONT_RIGHT] = 100, /* % */
    [REAR_LEFT] = 100,   /* % */
    [REAR_RIGHT] = 100,  /* % */
};
static const float DELAY_MAP[CHANNEL_MAX] = {
    [FRONT_LEFT] = 0,  /* msec */
    [FRONT_RIGHT] = 0, /* msec */
    [REAR_LEFT] = 0,   /* msec */
    [REAR_RIGHT] = 0,  /* msec */
};

/* 車両にあわせるとこんな感じか？ */
// static const int32_t VOLUME_MAP[CHANNEL_MAX] = {
//     [FRONT_LEFT] = 105,  /* % */
//     [FRONT_RIGHT] = 100, /* % */
//     [REAR_LEFT] = 110,   /* % */
//     [REAR_RIGHT] = 110,  /* % */
// };
// static const float DELAY_MAP[CHANNEL_MAX] = {
//     [FRONT_LEFT] = 2.94, /* msec */
//     [FRONT_RIGHT] = 0,   /* msec */
//     [REAR_LEFT] = 6.6,   /* msec */
//     [REAR_RIGHT] = 5.88, /* msec */
// };

#define DELAY_MAX_MS 101
static int16_t *delay_buffer = NULL;          // 過去の音を溜めるバッファ
static uint32_t delay_buffer_len = 0;         // バッファの長さ（要素数）
static uint32_t delay_pos[CHANNEL_MAX] = {0}; // 現在の読み書き位置

static void dsp_vlume_control(int16_t *channel[CHANNEL_MAX]);
static void dsp_delay_control(int16_t *channel[CHANNEL_MAX]);

/* DSP機能のセットアップ */
void dsp_setup()
{
    /* 遅延用のバッファーを確保 */
    delay_buffer_len = (uint32_t)(SAMPLE_RATE * (DELAY_MAX_MS / 1000.0) * CHANNEL_MAX);
    delay_buffer = (int16_t *)malloc(delay_buffer_len * sizeof(int16_t));
    if (delay_buffer == NULL)
    {
        Serial.println("Failed to allocate delay memory!");
    }
    memset(delay_buffer, 0, delay_buffer_len * sizeof(int16_t));
}

/* 信号処理を実行する */
void dsp_process(int16_t *front_buff, int16_t *rear_buff, int32_t count)
{
    for (int32_t i = 0; i < count; i += 2)
    {
        int16_t *channel[CHANNEL_MAX];
        channel[FRONT_LEFT] = &front_buff[i];
        channel[FRONT_RIGHT] = &front_buff[i + 1];
        channel[REAR_LEFT] = &rear_buff[i];
        channel[REAR_RIGHT] = &rear_buff[i + 1];

        /* 音量調整 */
        dsp_vlume_control(channel);

        /* 遅延処理 */
        dsp_delay_control(channel);
    }
}

/* 音量調整 */
static void dsp_vlume_control(int16_t *channel[CHANNEL_MAX])
{
    for (int32_t ch = 0; ch < CHANNEL_MAX; ch++)
    {
        *channel[ch] = (int16_t)((*channel[ch]) * VOLUME_MAP[ch] / 100);
    }
}

/* 遅延処理 */
static void dsp_delay_control(int16_t *channel[CHANNEL_MAX])
{
    for (int32_t ch = 0; ch < CHANNEL_MAX; ch++)
    {
        /* 遅延時間が0の場合はスキップ */
        if (DELAY_MAP[ch] == 0)
        {
            continue;
        }

        /* 遅延バッファーの読み書き位置を計算 */
        uint32_t delay_samples = (uint32_t)(SAMPLE_RATE * (DELAY_MAP[ch] / 1000.0));
        uint32_t write_pos = delay_pos[ch];
        uint32_t read_pos = (write_pos + delay_buffer_len - delay_samples * CHANNEL_MAX) % delay_buffer_len;

        /* 遅延バッファーに現在のサンプルを書き込み */
        delay_buffer[write_pos] = *channel[ch];

        /* 遅延バッファーから遅延後のサンプルを読み込み */
        *channel[ch] = delay_buffer[read_pos];

        /* 読み書き位置を更新 */
        delay_pos[ch] = (write_pos + CHANNEL_MAX) % delay_buffer_len;
    }
}
