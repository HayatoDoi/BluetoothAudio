/*******************************************************
 *
 * main.ino
 *
 * メインプログラム。各機能を結合し、動作させる。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#include "config.hpp"
#include "sw.hpp"
#include "bt_audio.hpp"

#include <Arduino.h>

/* スイッチが押されたときの動作 */
static const struct
{
    STEERING_SW key;
    void (*action)();
} ACTION_TABLE[] = {
    {VOL_UP, bt_volume_up},
    {VOL_DOWN, bt_volume_down},
    {NEXT, bt_next},
    {PREV, bt_previous},
    {TEST1, bt_next},    // for debug.
    {TEST2, bt_previous} // for debug.
};
static const int32_t ACTION_TABLE_LEN =
    sizeof(ACTION_TABLE) / sizeof(ACTION_TABLE[0]);

/* 各機能のセットアップ */
void setup()
{
    /* シリアルポートの初期化 */
    Serial.begin(115200);

    /* スイッチ機能のセットアップ */
    sw_setup();

    /* Bluetooth機能のセットアップ */
    bt_setup();

    Serial.println("System initialized.");
}

/* メインループ */
void loop()
{
    delay(10);

    /* 押されたスイッチに合わせて処理を実行する */
    enum STEERING_SW sw = sw_read();
    Serial.print("sw = ");
    Serial.println(sw);
    for (size_t i = 0; i < ACTION_TABLE_LEN; i++)
    {
        if (ACTION_TABLE[i].key == sw)
        {
            ACTION_TABLE[i].action();
            break;
        }
    }
}
