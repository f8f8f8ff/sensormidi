#include "midi.h"

midi_mixer_t midi_mixer;

void ramp_ease_t::set(int v) { set_point = v * MIDI_MULTIPLIER; }

int ramp_ease_t::get() {
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

void ramp_ease_t::print_rate() {
    Serial.print("up ");
    Serial.print(rate_up);
    Serial.print(" down ");
    Serial.print(rate_down);
    Serial.print("\n");
}

void ramp_ease_t::print() {
    Serial.print(F("setpt "));
    Serial.print(set_point);
    Serial.print(F(" value "));
    Serial.print(val);
    Serial.print(" ");
    print_rate();
}

void midi_mixer_t::set_range(uint8_t n, unsigned int min, unsigned int max) {
    if (n > SENSORS_MAX) return;
    sensor_min[n] = min;
    sensor_max[n] = max;
}

void midi_mixer_t::print_range(uint8_t n) {
    if (n > SENSORS_MAX) return;
    Serial.print(F("range "));
    Serial.print(n);
    Serial.print(": ");
    Serial.print(sensor_min[n]);
    Serial.print(" ");
    Serial.print(sensor_max[n]);
    Serial.print("\n");
}

void midi_mixer_t::print_ease(uint8_t n) {
    if (n > SENSORS_MAX) return;
    Serial.print(F("ease "));
    Serial.print(n);
    ramps[n].print();
}

void midi_mixer_t::set_ease_rate(uint8_t n, byte rate_up, byte rate_down) {
    if (n > SENSORS_MAX) return;
    ramps[n].rate_up = rate_up;
    ramps[n].rate_down = rate_down;
}

void midi_mixer_t::send_values_simple(sensor_values_t& values, byte channel) {
    for (int i = 0; i < values.len; i++) {
        ramps[i].set(
            map_sensor_midi(values.s[i], sensor_min[i], sensor_max[i]));
        send_value(channel, i, ramps[i].get());
    }
}

void midi_mixer_t::send_value(byte channel, byte cc, byte value) {
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

void midi_mixer_t::flush() { MidiUSB.flush(); }

int midi_mixer_t::map_sensor_midi(int v, unsigned int min, unsigned int max) {
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

void midi_mixer_t::touch_cc(byte cc) {
    Serial.print(F("touched cc "));
    Serial.println(cc);
    if (cc < 0 || cc > 127) {
        return;
    }
    send_value(0, cc, 0);
    flush();
}

void midi_mixer_t::print() {
    Serial.println(F("midi_mixer:"));
    for (int i = 0; i < SENSORS_MAX; i++) {
        print_range(i);
        print_ease(i);
    }
}

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