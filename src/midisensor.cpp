#include <Arduino.h>

#include "midi.h"
#include "sensors.h"
#include "state.h"
#include "util.h"
#include "commands.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    // set up sensors
    // sensor 1
    sensor_manager.add_sensor(sensor_t(2, 3, 200));
    midi_mixer.set_range(0, 10, 200);
    midi_mixer.set_ease_rate(0, 32, 8);
    // sensor 2
    sensor_manager.add_sensor(sensor_t(4, 5, 200));
    midi_mixer.set_range(1, 10, 200);
    midi_mixer.set_ease_rate(1, 32, 8);
    // sensor 3
    sensor_manager.add_sensor(sensor_t(6, 7, 200));
    midi_mixer.set_range(2, 10, 200);
    midi_mixer.set_ease_rate(2, 32, 8);
    // sensor 4
    sensor_manager.add_sensor(sensor_t(8, 9, 200));
    midi_mixer.set_range(3, 10, 200);
    midi_mixer.set_ease_rate(3, 32, 8);
    // sensor 5
    sensor_manager.add_sensor(sensor_t(10, 16, 200));
    midi_mixer.set_range(4, 10, 200);
    midi_mixer.set_ease_rate(4, 32, 8);

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