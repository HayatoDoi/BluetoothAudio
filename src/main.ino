// #include <Arduino.h>

#include "config.h"
#include "sw.h"

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include <Preferences.h>

I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);
Preferences preferences;
int32_t now_volume = 64;

void volume_changed(int volume) {
    Serial.printf("Volume changed to: %d\n", volume);
    now_volume = volume;
    int current_saved = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, -1);
    if (current_saved != now_volume) {
        preferences.putInt(BLUETOOTH_PREF_KEY_VOL, volume);
        Serial.println("Volume saved to NVS");
    }
}

void setup() {
    // NVS Setup
    preferences.begin(BLUETOOTH_PREF_NAMESPACE, false);
    now_volume = preferences.getInt(BLUETOOTH_PREF_KEY_VOL, BLUETOOTH_DEFAULT_VOLUME);

    // I2S Setup
    auto cfg = i2s.defaultConfig();
    cfg.pin_bck = BCK_PIN;
    cfg.pin_ws = LRCK_PIN;
    cfg.pin_data = DIN_PIN;
    i2s.begin(cfg);

    // アナログ読み取りのセットアップ
    analogReadResolution(12); // 12bit (0-4095)
    analogSetAttenuation(ADC_11db);

    // Audio Setup
    a2dp_sink.set_on_volumechange(volume_changed);
    a2dp_sink.set_volume(now_volume);
    a2dp_sink.set_auto_reconnect(true, 1000);
    a2dp_sink.start("D-CAR-AUDIO");
    while(!a2dp_sink.is_connected()) { 
        delay(1000);
    }
    a2dp_sink.play();

    // Serial Setup
    Serial.begin(115200);

    Serial.println("System initialized.");
}

void loop() {
    delay(10);
    static enum STEERING_SW _sw = NONE; /* ひとつ前のループで入手したスイッチID */
    enum STEERING_SW sw = read_sw();
    if (sw == _sw) {
        return;
    }
    Serial.print("sw = ");
    Serial.println(sw);
    switch (sw) {
        case VOL_UP:
            now_volume += BLUETOOTH_VOL_UP_SIZE;
            a2dp_sink.set_volume(now_volume);
            break;
        case VOL_DOWN:
            now_volume -= BLUETOOTH_VOL_UP_SIZE;
            a2dp_sink.set_volume(now_volume);
            break;
        case NEXT:
            a2dp_sink.next();
            break;
        case PREV:
            a2dp_sink.previous();
            break;
        case TEST1:
            a2dp_sink.next();
            break;
        case TEST2:
            a2dp_sink.previous();
            break;
        default:
            break;
    }
    _sw = sw;
}
