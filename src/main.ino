#include "config.h"
#include "sw.h"
#include "bt_audio.h"

#include <Arduino.h>

// 関数ポインタの型定義（引数なし、戻り値なしの関数）
typedef void (*ActionFunc)();

// キーと実行関数のペアを定義する構造体
struct KeyAction
{
    STEERING_SW key;
    ActionFunc action;
};

// アクションマップの定義
// ここに追記するだけで機能割り当てが可能になります
const KeyAction ACTION_MAP[] = {
    {VOL_UP, bt_volume_up},
    {VOL_DOWN, bt_volume_down},
    {NEXT, bt_next},
    {PREV, bt_previous},
    {TEST1, bt_next},    // TEST1もNEXTと同じ動き
    {TEST2, bt_previous} // TEST2もPREVと同じ動き
};

void setup()
{
    Serial.begin(115200);

    // アナログ読み取りのセットアップ
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // Bluetooth機能のセットアップ
    bt_setup();

    Serial.println("System initialized.");
}

void loop()
{
    delay(10);

    static enum STEERING_SW _sw = NONE;
    enum STEERING_SW sw = read_sw();

    // 状態が変わっていない、またはNONEの場合は何もしない
    if (sw == _sw || sw == NONE || sw == UNKNOWN)
    {
        _sw = sw; // NONEの場合も状態更新は必要（離したことを検知するため）
        return;
    }

    Serial.print("sw = ");
    Serial.println(sw);

    // アクションマップを検索して対応する関数を実行
    size_t map_size = sizeof(ACTION_MAP) / sizeof(KeyAction);
    for (size_t i = 0; i < map_size; i++)
    {
        if (ACTION_MAP[i].key == sw)
        {
            // 関数が登録されていれば実行
            if (ACTION_MAP[i].action != nullptr)
            {
                ACTION_MAP[i].action();
            }
            break; // 見つかったらループを抜ける
        }
    }

    _sw = sw;
}
