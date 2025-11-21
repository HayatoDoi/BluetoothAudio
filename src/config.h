#pragma once

/* I2S出力関連 */
#define BCK_PIN     14
#define LRCK_PIN    27
#define DIN_PIN     26

/* Bluetoothの設定 */
#define BLUETOOTH_DEFAULT_VOLUME    64 /* 初回起動時の音量 */
#define BLUETOOTH_PREF_NAMESPACE    "bt-audio"
#define BLUETOOTH_PREF_KEY_VOL      "volume"
#define BLUETOOTH_VOL_UP_SIZE       10

/* ステアリングスイッチ関連 */
#define SW_PIN      34
#define V_IN        3.3 /* 入力電圧 */
#define R_REF       10000.0 /* 基準抵抗 */
