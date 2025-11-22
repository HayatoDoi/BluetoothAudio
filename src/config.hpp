/*******************************************************
 *
 * config.hpp
 *
 * 各プログラムの設定値を定義したヘッダファイル。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#pragma once

/* I2S出力関連 */
#define I2S0_BCK_PIN 14
#define I2S0_LRCK_PIN 26
#define I2S0_DIN_PIN 27
#define I2S1_BCK_PIN 16
#define I2S1_LRCK_PIN 15
#define I2S1_DIN_PIN 4

/* Bluetoothの設定 */
#define BLUETOOTH_DEVICE_NAME "D-CAR-AUDIO" /* Bluetoothデバイス名 */
#define BLUETOOTH_DEFAULT_VOLUME 64         /* 初回起動時の音量 */
#define BLUETOOTH_PREF_NAMESPACE "bt-audio"
#define BLUETOOTH_PREF_KEY_VOL "volume"
#define BLUETOOTH_VOL_UP_SIZE 10

#define SAMPLE_RATE 44100

/* ステアリングスイッチ関連 */
#define SW_PIN 34
#define V_IN 3.3      /* 入力電圧 */
#define R_REF 10000.0 /* 基準抵抗 */
