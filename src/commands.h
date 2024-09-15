#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>

#include "midi.h"
#include "state.h"
#include "sensors.h"

void cmd_state() { Serial.println(state_string(STATE)); }
void cmd_start() {
    STATE = RUNNING;
    Serial.println(state_string(STATE));
}
void cmd_stop() {
    STATE = STOPPED;
    Serial.println(state_string(STATE));
}
void cmd_dump() {
    sensors.print();
}

struct Command {
    void (*cmd)();
    const char* name;
};

const Command commands[]{
    {cmd_state, "state"},
    {cmd_start, "start"},
    {cmd_stop, "stop"},
    {cmd_dump, "i"},
};

void read_commands() {
    static const byte n_commands = sizeof(commands) / sizeof(Command);
    if (Serial.available() > 0) {
        static const size_t buffer_l = 16;
        char buffer[buffer_l];
        size_t l = Serial.readBytes(buffer, buffer_l-1);
        buffer[l] = '\0';
        for (int i = 0; i < n_commands; i++) {
            if(strcmp(buffer, commands[i].name) == 0) {
                commands[i].cmd();
                return;
            }
        }
    }
}

#endif