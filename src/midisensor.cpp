#include <Arduino.h>

#include "midi.h"
#include "sensors.h"
#include "state.h"
#include "util.h"
#include "commands.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    sensors.add_sensor(sensor(2, 3, 200));
    midi.set_range(0, 10, 200);
    sensors.sensors[0].print();
    STATE = RUNNING;
}

void loop() {
    read_commands();
    if (STATE == RUNNING) {
        sensors.read();
        if (millis() >= timer_midi) {
            timer_midi = millis() + MIDI_INTERVAL;
            sensor_values* values = sensors.update_all_values();
            // values->print();
            midi.send_values_simple(*values, 0);
            midi.flush();
        }
    }
}