#include <Arduino.h>

#include "midi.h"
#include "sensors.h"
#include "state.h"
#include "util.h"
#include "commands.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    sensor_manager.add_sensor(sensor_t(2, 3, 200));
    midi_mixer.set_range(0, 10, 200);
    sensor_manager.sensors[0].print();
    STATE = RUNNING;
}

void loop() {
    cmd_read();
    if (STATE == RUNNING) {
        sensor_manager.read();
        if (millis() >= midi_mixer.timer) {
            midi_mixer.timer = millis() + MIDI_INTERVAL_MS;
            sensor_values_t* values = sensor_manager.update_all_values();
            // values->print();
            midi_mixer.send_values_simple(*values, 0);
            midi_mixer.flush();
        }
    }
}