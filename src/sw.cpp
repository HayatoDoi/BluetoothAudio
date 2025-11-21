#include "config.h"
#include "sw.h"

#include <Arduino.h>
#include <stdint.h>

#define ALLOWABLE 200
typedef struct
{
    enum STEERING_SW id;
    char name[80];
    int32_t value; // テスターで測定した値
} STEERING_SW_TABLE_TYPE;
const STEERING_SW_TABLE_TYPE STEERING_SW_TABLE[] = {
    {NONE, "NONE", 217586},      // 何も押されていない
    {VOL_UP, "VOL_UP", 133},     // ボリュームアップ
    {VOL_DOWN, "VOL_DOWN", 243}, // ボリュームダウン
    {NEXT, "NEXT", 424},         // 曲送り
    {PREV, "PREV", 1572},        // 曲戻り
    {TEST1, "TEST1", 10000},     // for debug.
    {TEST2, "TEST2", 5000}       // for debug.
};

enum STEERING_SW read_sw()
{
    float v_out = analogReadMilliVolts(SW_PIN) / 1000.0;
    /* 出力電圧が3.1Vを超えたら未接続とみなす */
    if (v_out > 3.1)
    {
        return NONE;
    }
    int32_t rx = R_REF * (v_out / (V_IN - v_out));
    int32_t loop_end = sizeof(STEERING_SW_TABLE) / sizeof(STEERING_SW_TABLE_TYPE);
    for (int32_t i = 0; i < loop_end; i++)
    {
        if (rx > STEERING_SW_TABLE[i].value - ALLOWABLE &&
            rx < STEERING_SW_TABLE[i].value + ALLOWABLE)
        {
            return STEERING_SW_TABLE[i].id;
        }
    }
    return UNKNOWN;
}
