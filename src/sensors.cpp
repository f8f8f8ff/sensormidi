#include "sensors.h"

sensor_manager_t sensor_manager;

void sensor_t::max_dist_set(unsigned int _max_dist_cm) {
    max_dist_cm = _max_dist_cm;
    ultrasonic = NewPing(pin_trig, pin_echo, max_dist_cm);
}

void sensor_t::print() {
    Serial.print(F("trig "));
    Serial.print(pin_trig);
    Serial.print(F(" echo "));
    Serial.print(pin_echo);
    Serial.print(F(" current_reading "));
    Serial.print(current_reading);
    Serial.print(F(" reading "));
    Serial.print(reading);
    Serial.print(F(" timer "));
    Serial.print(timer);
    Serial.print(F(" max_dist "));
    Serial.print(max_dist_cm);
    Serial.print(F(" init "));
    Serial.print(initialized);
    Serial.print("\n");
}

void sensor_values_t::print() {
        Serial.print("sensor_values:");
        for (int i = 0; i < len; i++) {
            Serial.print(s[i]);
            Serial.print(", ");
        }
        Serial.print("\n");
}

void sensor_manager_t::add_sensor(sensor_t s) {
    if (n_sensors >= SENSORS_MAX) {
        return;
    }
    sensors[n_sensors] = s;
    n_sensors += 1;
    values.len = n_sensors;

    sensors[n_sensors - 1].timer = PING_INTERVAL_MS * n_sensors;
}

void _echo_check() { sensor_manager.echo_check(); }

void sensor_manager_t::read() {
    bool DEBUG = false;
    // check if a sensor is ready to be read again
    // Serial.println("reading");
    for (int i = 0; i < n_sensors; i++) {
        sensor_t& s = sensors[i];
        if (millis() >= s.timer) {
            if (DEBUG) {
                Serial.print("sensor ");
                Serial.println(i);
                s.print();
            }
            // reset timer
            s.timer += PING_INTERVAL_MS * n_sensors;
            s.ultrasonic.timer_stop();
            // add last reading
            s.reading = s.current_reading;
            s.filter.add(s.reading);
            // set next reading
            current_sensor = i;
            s.current_reading = s.max_dist_cm + 1;
            s.ultrasonic.ping_timer(_echo_check);
        }
    }
}

void sensor_manager_t::echo_check() {
    sensor_t& s = sensors[current_sensor];
    if (s.ultrasonic.check_timer()) {
        s.current_reading = s.ultrasonic.ping_result / US_ROUNDTRIP_CM;
    }
}

int sensor_manager_t::value(int n) { return sensors[n].filter.read(); }

sensor_values_t* sensor_manager_t::update_all_values() {
    values.len = n_sensors;
    for (int i = 0; i < n_sensors; i++) {
        values.s[i] = value(i);
    }
    return &values;
}

void sensor_manager_t::print() {
    Serial.print(F("n_sensors "));
    Serial.print(n_sensors);
    Serial.print("\n");
    values.print();
    for (int i = 0; i < n_sensors; i++) {
        Serial.print(i);
        Serial.print(": ");
        sensors[i].print();
    }
}


