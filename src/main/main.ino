// #include <Arduino.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

void setup() {
    // I2S Setup
    auto cfg = i2s.defaultConfig();
    cfg.pin_bck = 14;
    cfg.pin_ws = 27;
    cfg.pin_data = 26;
    i2s.begin(cfg);

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
    Serial.println("looping...");
    delay(1000);
}
