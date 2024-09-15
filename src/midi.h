#ifndef MIDI_H
#define MIDI_H
#include <MIDIUSB.h>

#include "sensors.h"
#include "util.h"

#define MIDI_INTERVAL_MS 50
#define MIDI_MULTIPLIER 8

void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);

class ramp_ease_t {
   public:
    unsigned int set_point = 0;
    unsigned int val = 0;
    byte rate_up;
    byte rate_down;

    void set(int v);
    int get();
    void print_rate();
    void print();
    ramp_ease_t(byte rate_up = MIDI_MULTIPLIER,
                byte rate_down = MIDI_MULTIPLIER)
        : rate_up(rate_up), rate_down(rate_down) {}
};

class midi_mixer_t {
   private:
    int map_sensor_midi(int v, int min, int max);

   public:
    unsigned long timer = MIDI_INTERVAL_MS;
    ramp_ease_t ramps[SENSORS_MAX];
    int sensor_min[SENSORS_MAX];
    int sensor_max[SENSORS_MAX];

    void send_values_simple(sensor_values_t& values, byte channel);
    void send_value(byte channel, byte cc, byte value);
    void touch_cc(byte cc);
    void flush();

    void set_range(uint8_t n, int min, int max);
    void print_range(uint8_t n);
    void set_ease_rate(uint8_t n, byte rate_up, byte rate_down);
    void print_ease(uint8_t n);
    void print();

    midi_mixer_t() {
        for (int i = 0; i < SENSORS_MAX; i++) {
            set_range(i, 0, 300);
        }
    }
};

extern midi_mixer_t midi_mixer;

#endif

