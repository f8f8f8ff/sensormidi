#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <NewPing.h>

#include "medianfilter.h"

#define SENSORS_MAX 5
#define PING_INTERVAL_MS 30

class sensor_t {
   public:
    unsigned int reading = 0;
    unsigned int current_reading = 0;
    bool changed = true;
    median_filter_t filter;
    unsigned long timer = 0;
    NewPing ultrasonic;
    unsigned int max_dist_cm;
    bool initialized;
    uint8_t pin_trig;
    uint8_t pin_echo;

    void print();
    void max_dist_set(unsigned int _max_dist_cm);
    sensor_t(uint8_t pin_trig = 0, uint8_t pin_echo = 0,
             unsigned int max_dist_cm = 300)
        : ultrasonic(pin_trig, pin_echo, max_dist_cm),
          max_dist_cm(max_dist_cm),
          pin_trig(pin_trig),
          pin_echo(pin_echo) {
        if (pin_trig == 0 && pin_echo == 0) {
            initialized = false;
        } else {
            initialized = true;
        }
    }
};

struct sensor_values_t {
    unsigned int s[SENSORS_MAX];
    uint8_t len;
    void print();
};

class sensor_manager_t {
   public:
    uint8_t current_sensor = 0;
    uint8_t n_sensors = 0;
    sensor_t sensors[SENSORS_MAX];
    sensor_values_t values;

    void add_sensor(sensor_t s);
    void read();
    int value(int n);
    sensor_values_t* update_all_values();
    void echo_check();
    void print();
    sensor_manager_t() {};
};

extern sensor_manager_t sensor_manager;

#endif