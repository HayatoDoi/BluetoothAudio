/*******************************************************
 *
 * dual_i2s.cpp
 *
 * 音声信号をI2Sに出力するためのプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#include "config.hpp"
#include "dsp.hpp"

#include "driver/i2s.h"
#include "freertos/ringbuf.h"
#include <Arduino.h>

#define RINGBUF_SIZE (16 * 1024)

static RingbufHandle_t buf_handle;
static void i2s_output_task(void *param);

/* I2S機能のセットアップ */
void dual_i2s_setup()
{
    /* 共通設定 */
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true};

    /* ピン設定 (I2S_NUM_0) */
    i2s_pin_config_t pin_config0 = {
        .bck_io_num = I2S0_BCK_PIN,
        .ws_io_num = I2S0_LRCK_PIN,
        .data_out_num = I2S0_DIN_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE};

    /* ピン設定 (I2S_NUM_1) */
    i2s_pin_config_t pin_config1 = {
        .bck_io_num = I2S1_BCK_PIN,
        .ws_io_num = I2S1_LRCK_PIN,
        .data_out_num = I2S1_DIN_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE};

    /* ドライバインストール */
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config0);

    i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &pin_config1);

    /* リングバッファの作成 (No-Splitモード: データの断片化を防ぐ) */
    buf_handle = xRingbufferCreate(RINGBUF_SIZE, RINGBUF_TYPE_NOSPLIT);
    if (buf_handle == NULL)
    {
        Serial.println("Failed to create ring buffer");
        return;
    }

    /* 書き込みには時間がかかるためCore1のタスクで実行する */
    xTaskCreatePinnedToCore(
        i2s_output_task, // 関数名
        "I2S_Writer",    // タスク名
        4096,            // スタックサイズ
        NULL,            // 引数
        5,               // 優先度 (標準より高め)
        NULL,            // ハンドル
        1                // Core 1
    );

    /* DSP機能のセットアップ */
    dsp_setup();
}

/* データ書き込みタスク */
static void i2s_output_task(void *param)
{
    uint32_t item_size;

    while (true)
    {
        uint8_t *data = NULL;
        uint8_t *front_buff = NULL;
        uint8_t *rear_buff = NULL;
        size_t front_written = 0;
        size_t rear_written = 0;

        /* リングバッファからデータを取り出す*/
        data = (uint8_t *)xRingbufferReceive(buf_handle, &item_size, portMAX_DELAY);
        if (data == NULL)
        {
            goto END;
        }

        /* 編集可能な領域に値をコピー */
        front_buff = (uint8_t *)malloc(sizeof(uint8_t) * item_size);
        if (front_buff == NULL)
        {
            Serial.println("Failed to allocate memory for buffers");
            goto END;
        }
        memcpy(front_buff, data, item_size);
        rear_buff = (uint8_t *)malloc(sizeof(uint8_t) * item_size);
        if (rear_buff == NULL)
        {
            Serial.println("Failed to allocate memory for buffers");
            goto END;
        }
        memcpy(rear_buff, data, item_size);

        /* 信号処理 (DSP: Digital Signal Processing)を実行 */
        dsp_process((int16_t *)front_buff, (int16_t *)rear_buff, item_size / 2); // 16bitステレオなので2で割る

        /* ノンブロッキングで2つのI2Sにデータを書き込み */
        while (front_written < item_size || rear_written < item_size)
        {
            size_t written_chunk = 0;
            if (front_written < item_size)
            {
                i2s_write(I2S_NUM_0,
                          front_buff + front_written,
                          item_size - front_written,
                          &written_chunk,
                          0); /* 書き込めるものだけ書き込む */
                if (written_chunk > 0)
                {
                    front_written += written_chunk;
                }
            }
            if (rear_written < item_size)
            {
                i2s_write(I2S_NUM_1,
                          rear_buff + rear_written,
                          item_size - rear_written,
                          &written_chunk,
                          0); /* 書き込めるものだけ書き込む */
                if (written_chunk > 0)
                {
                    rear_written += written_chunk;
                }
            }
        }

    END:
        vRingbufferReturnItem(buf_handle, (void *)data);
        if (front_buff)
        {
            free(front_buff);
        }
        if (rear_buff)
        {
            free(rear_buff);
        }
    }
}

/* バッファーに音声データを追加 */
void dual_i2s_put_buf(const uint8_t *data, uint32_t len)
{
    xRingbufferSend(buf_handle, data, len, pdMS_TO_TICKS(10));
}
