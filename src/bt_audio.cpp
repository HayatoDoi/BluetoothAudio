/*******************************************************
 *
 * bt_audio.cpp
 *
 * Bluetoothのオーディオ部分を制御するプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#include "config.hpp"
#include "bt_audio.hpp"
#include "dual_i2s.hpp"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <stdint.h>
#include <Preferences.h>

static I2SStream i2s;
static BluetoothA2DPSink a2dp_sink(i2s);
static Preferences preferences;
static int32_t now_volume = BLUETOOTH_DEFAULT_VOLUME;

/* 音量変更時のコールバック */
static void volume_changed(int32_t volume)
{
    Serial.printf("Volume changed to: %d\n", volume);
    now_volume = volume;

    /* NVSにボリュームの値を保存。再接続時に値を引きつぐため。 */
    int32_t current_saved = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, -1);
    if (current_saved != now_volume)
    {
        preferences.putInt(BLUETOOTH_PREF_KEY_VOL, volume);
        Serial.println("Volume saved to NVS");
    }
}

/* Bluetooth機能のセットアップ */
void bt_setup()
{
    /* NVSのセットアップ */
    preferences.begin(BLUETOOTH_PREF_NAMESPACE, false);
    now_volume = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, BLUETOOTH_DEFAULT_VOLUME);

    /* I2S機能のセットアップ */
    dual_i2s_setup();

    /* A2DPのセットアップ */
    a2dp_sink.set_stream_reader(dual_i2s_put_buf, false);
    a2dp_sink.set_on_volumechange(volume_changed);
    a2dp_sink.set_volume(now_volume);
    a2dp_sink.set_auto_reconnect(true, 1000);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);

    /* 接続待機 */
    Serial.println("Waiting for connection...");
    while (!a2dp_sink.is_connected())
    {
        delay(1000);
    }
    a2dp_sink.play();
}

/* ボリュームアップ */
void bt_volume_up()
{
    now_volume += BLUETOOTH_VOL_UP_SIZE;
    if (now_volume > 127)
        now_volume = 127; // 最大値制限
    a2dp_sink.set_volume(now_volume);
}

/* ボリュームダウン */
void bt_volume_down()
{
    now_volume -= BLUETOOTH_VOL_UP_SIZE;
    if (now_volume < 0)
        now_volume = 0; // 最小値制限
    a2dp_sink.set_volume(now_volume);
}

/* 曲送り */
void bt_next()
{
    a2dp_sink.next();
}

/* 曲戻し */
void bt_previous()
{
    a2dp_sink.previous();
}

/* 曲を再生 */
void bt_play()
{
    a2dp_sink.play();
}

/* 曲を停止 */
void bt_stop()
{
    a2dp_sink.stop();
}

/* Bluetoothが接続状態か否か */
bool bt_is_connected()
{
    return a2dp_sink.is_connected();
}
