#ifndef MIDI_H
#define MIDI_H
#include <MIDIUSB.h>

#include "sensors.h"
#include "util.h"

void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}

#define MIDI_INTERVAL 50
#define MIDI_MULTIPLIER 8
unsigned long timer_midi = MIDI_INTERVAL;

class ramp {
   public:
    unsigned int set_point = 0;
    unsigned int val = 0;
    byte rate_up;
    byte rate_down;

    void set(int v);
    int get();
    ramp(byte rate_up = MIDI_MULTIPLIER, byte rate_down = MIDI_MULTIPLIER)
        : rate_up(rate_up), rate_down(rate_down) {}
};

void ramp::set(int v) { set_point = v * MIDI_MULTIPLIER; }
int ramp::get() {
    int diff = set_point - val;
    if (diff == 0) return val / MIDI_MULTIPLIER;
    if (diff > 0) {
        if (diff < rate_up) {
            val = set_point;
        } else {
            val += rate_up;
        }
    } else {
        if (-diff < rate_down) {
            val = set_point;
        } else {
            val -= rate_down;
        }
    }
    // DebugPrintln(set_point);
    // DebugPrintln(val / MIDI_MULTIPLIER);
    return val / MIDI_MULTIPLIER;
}

class midi_mixer {
   private:
    int map_sensor_midi(int v, unsigned int min, unsigned int max);

   public:
    ramp ramps[SENSORS_MAX];
    unsigned int sensor_min[SENSORS_MAX];
    unsigned int sensor_max[SENSORS_MAX];
    void set_range(uint8_t n, unsigned int min, unsigned int max);
    void send_values_simple(sensor_values& values, byte channel);
    void send_value(byte channel, byte cc, byte value);
    void flush();
    midi_mixer() {
        for (int i = 0; i < SENSORS_MAX; i++) {
            set_range(i, 0, 300);
        }
    }
};
midi_mixer midi;

void midi_mixer::set_range(uint8_t n, unsigned int min, unsigned int max) {
    if (n > SENSORS_MAX) return;
    sensor_min[n] = min;
    sensor_max[n] = max;
}

void midi_mixer::send_values_simple(sensor_values& values, byte channel) {
    for (int i = 0; i < values.len; i++) {
        ramps[i].set(
            map_sensor_midi(values.s[i], sensor_min[i], sensor_max[i]));
        send_value(channel, i, ramps[i].get());
    }
}

void midi_mixer::send_value(byte channel, byte cc, byte value) {
    /*
    Serial.print("send_value ");
    Serial.print(channel);
    Serial.print(" ");
    Serial.print(cc);
    Serial.print(" ");
    Serial.print(value);
    Serial.print("\n");
    */
    controlChange(channel, cc, value);
}

void midi_mixer::flush() { MidiUSB.flush(); }

int midi_mixer::map_sensor_midi(int v, unsigned int min, unsigned int max) {
    int result;
    if (v <= min) {
        result = 127;
    } else if (v >= max) {
        result = 0;
    } else {
        result = map(v, max, min, 0, 127);
    }
    // DebugPrintln(v);
    // DebugPrintln(result);
    return result;
}


#endif