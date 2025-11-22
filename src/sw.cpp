/*******************************************************
 *
 * sw.cpp
 *
 * ステアリングスイッチの値を読み込むプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#include "config.hpp"
#include "sw.hpp"

#include <Arduino.h>
#include <stdint.h>

/* 抵抗値の許容差（単位：オーム）*/
#define ALLOWABLE 200

/* ステアリングスイッチの抵抗値テーブル */
static const struct
{
    enum STEERING_SW id;
    char name[80];
    int32_t value; /* 抵抗値（単位：オーム）*/
} STEERING_SW_TABLE[] = {
    {NONE, "NONE", 217586},      // 何も押されていない
    {VOL_UP, "VOL_UP", 133},     // ボリュームアップ
    {VOL_DOWN, "VOL_DOWN", 243}, // ボリュームダウン
    {NEXT, "NEXT", 424},         // 曲送り
    {PREV, "PREV", 1572},        // 曲戻り
    {TEST1, "TEST1", 10000},     // for debug.
    {TEST2, "TEST2", 5000}       // for debug.
};
static const int32_t STEERING_SW_TABLE_LEN =
    sizeof(STEERING_SW_TABLE) / sizeof(STEERING_SW_TABLE[0]);

/* スイッチ機能のセットアップ */
void sw_setup()
{
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
}

/* ステアリングスイッチの値を読み込む*/
enum STEERING_SW sw_read()
{
    enum STEERING_SW _id;

    /* ピンに出力される電圧の読み込み */
    float v_out = analogReadMilliVolts(SW_PIN) / 1000.0;
    if (v_out > 3.1)
    {
        /* 出力電圧が3.1Vを超えたら未接続とみなす */
        return NONE;
    }

    /* 抵抗値の計算とスイッチの特定 */
    int32_t rx = R_REF * (v_out / (V_IN - v_out));
    for (int32_t i = 0; i < STEERING_SW_TABLE_LEN; i++)
    {
        if (rx > STEERING_SW_TABLE[i].value - ALLOWABLE &&
            rx < STEERING_SW_TABLE[i].value + ALLOWABLE)
        {
            enum STEERING_SW id = STEERING_SW_TABLE[i].id;
            if (id == _id)
            {
                return NONE; /* 値に変化のないときは未接続とみなす */
            }
            return id;
        }
    }

    return NONE; /* 不明な値の場合は未接続とみなす */
}
