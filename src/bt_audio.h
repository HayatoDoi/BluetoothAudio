#pragma once

// Bluetoothオーディオの初期化
void bt_setup();

// 音量操作
void bt_volume_up();
void bt_volume_down();

// 再生制御
void bt_next();
void bt_previous();
void bt_play();
void bt_stop();

// 接続状態確認（必要であれば）
bool bt_is_connected();
