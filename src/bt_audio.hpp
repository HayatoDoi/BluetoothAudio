/*******************************************************
 *
 * bt_audio.hpp
 *
 * Bluetoothのオーディオ部分を制御するプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#pragma once

/* Bluetooth機能のセットアップ */
void bt_setup();

/* 音量操作 */
void bt_volume_up();
void bt_volume_down();

/* 再生制御 */
void bt_next();
void bt_previous();
void bt_play();
void bt_stop();

/* Bluetoothが接続状態か否か */
bool bt_is_connected();
