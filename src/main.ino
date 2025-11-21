// #include <Arduino.h>

#include "config.h"
#include "sw.h"

enum STEERING_SW read_sw();
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

void setup() {
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
            a2dp_sink.volume_up();
            break;
        case VOL_DOWN:
            a2dp_sink.volume_down();
            break;
        case NEXT:
            a2dp_sink.next(); // OK
            break;
        case PREV:
            a2dp_sink.previous(); // OK
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
