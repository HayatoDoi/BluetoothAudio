#include "bt_audio.h"
#include "config.h"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <Preferences.h>

// 内部変数・オブジェクト
static I2SStream i2s;
static BluetoothA2DPSink a2dp_sink(i2s);
static Preferences preferences;
static int32_t now_volume = BLUETOOTH_DEFAULT_VOLUME;

// 音量変更時のコールバック
static void volume_changed(int volume) {
    Serial.printf("Volume changed to: %d\n", volume);
    now_volume = volume;
    int current_saved = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, -1);
    if (current_saved != now_volume) {
        preferences.putInt(BLUETOOTH_PREF_KEY_VOL, volume);
        Serial.println("Volume saved to NVS");
    }
}

void bt_setup() {
    // NVS Setup
    preferences.begin(BLUETOOTH_PREF_NAMESPACE, false);
    now_volume = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, BLUETOOTH_DEFAULT_VOLUME);

    // I2S Setup
    auto cfg = i2s.defaultConfig();
    cfg.pin_bck = BCK_PIN;
    cfg.pin_ws = LRCK_PIN;
    cfg.pin_data = DIN_PIN;
    i2s.begin(cfg);

    // Audio Setup
    a2dp_sink.set_on_volumechange(volume_changed);
    a2dp_sink.set_volume(now_volume);
    a2dp_sink.set_auto_reconnect(true, 1000);
    a2dp_sink.start("D-CAR-AUDIO");
    
    // 接続待機（必要に応じて非同期化も検討してください）
    Serial.println("Waiting for connection...");
    while(!a2dp_sink.is_connected()) { 
        delay(1000);
    }
    // a2dp_sink.play(); // start()で自動再生される場合が多いですが、明示が必要なら記述
}

void bt_volume_up() {
    now_volume += BLUETOOTH_VOL_UP_SIZE;
    if (now_volume > 127) now_volume = 127; // 最大値制限
    a2dp_sink.set_volume(now_volume);
}

void bt_volume_down() {
    now_volume -= BLUETOOTH_VOL_UP_SIZE;
    if (now_volume < 0) now_volume = 0; // 最小値制限
    a2dp_sink.set_volume(now_volume);
}

void bt_next() {
    a2dp_sink.next();
}

void bt_previous() {
    a2dp_sink.previous();
}

void bt_play() {
    a2dp_sink.play();
}

void bt_stop() {
    a2dp_sink.stop();
}

bool bt_is_connected() {
    return a2dp_sink.is_connected();
}
