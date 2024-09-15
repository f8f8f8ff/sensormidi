#include "commands.h"

const cmd_t commands[]{
    {(void*)cmd_state, cmd_arg_t::NOARG, "state"},
    {(void*)cmd_start, cmd_arg_t::NOARG, "start"},
    {(void*)cmd_stop, cmd_arg_t::NOARG, "stop"},
    {(void*)cmd_dump, cmd_arg_t::NOARG, "dump"},
    {(void*)cmd_send_cc, cmd_arg_t::INTARG, "send"},
    {(void*)cmd_range_get, cmd_arg_t::INTARG, "r?"},
    {(void*)cmd_range_set, cmd_arg_t::INTARG_3, "r"},
    {(void*)cmd_ease_get, cmd_arg_t::INTARG, "e?"},
    {(void*)cmd_ease_set, cmd_arg_t::INTARG_3, "e"},
};

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
    Serial.print(F("MIDI_INTERVAL "));
    Serial.print(MIDI_INTERVAL_MS);
    Serial.print(F("SENSORS_MAX "));
    Serial.print(SENSORS_MAX);
    Serial.print(F("PING_INTERVAL_MS "));
    Serial.print(PING_INTERVAL_MS);
    sensor_manager.print();
    midi_mixer.print();
}
void cmd_send_cc(int cc) {
    midi_mixer.touch_cc(cc);
}
void cmd_range_set(int sensor, int min, int max) {
    midi_mixer.set_range(sensor, min, max);
    midi_mixer.print_range(sensor);
}
void cmd_range_get(int sensor) {
    midi_mixer.print_range(sensor);
}
void cmd_ease_set(int sensor, int rate_up, int rate_down) {
    midi_mixer.set_ease_rate(sensor, rate_up, rate_down);
    midi_mixer.print_ease(sensor);
}
void cmd_ease_get(int sensor) {
    midi_mixer.print_ease(sensor);
}

void cmd_read() {
    static const byte n_commands = sizeof(commands) / sizeof(cmd_t);
    if (Serial.available() > 0) {
        static const size_t buffer_l = 16;
        static char buffer[buffer_l];

        size_t l = Serial.readBytes(buffer, buffer_l-1);
        buffer[l] = '\0';
        char b_tmp[l];
        strcpy(b_tmp, buffer);
        char* cmd_tok = strtok(b_tmp, " ");

        for (int i = 0; i < n_commands; i++) {
            if(strcmp(cmd_tok, commands[i].name) == 0) {
                _cmd_run(commands[i], buffer, l);
                return;
            }
        }
        Serial.print(F("not understood: "));
        Serial.print(buffer);
        Serial.print(F("\ncmds:\n"));
        for (int i = 0; i < n_commands; i++) {
            Serial.print(commands[i].name);
            Serial.print(" ");
        }
        Serial.print("\n");
    }
}

int _cmd_arg_int_read(int *args, int n_args, char* s) {
    char* token;
    token = strtok(s, " ");
    if (token == NULL) return 0;
    token = strtok(NULL, " ");
    int i = 0;
    while (token != NULL && i < 3) {
        args[i] = atoi(token);
        i += 1;
        token = strtok(NULL, " ");
    }
    return i;
}

void _cmd_run(cmd_t c, char* input, size_t input_l) {
    switch (c.type) {
        case cmd_arg_t::NOARG:
            ((cmd_fn_noarg)c.cmd)();
            break;
        case cmd_arg_t::INTARG:
            int arg;
            if (_cmd_arg_int_read(&arg, 1, input) != 1) break;
            ((cmd_fn_intarg)c.cmd)(arg);
            break;
        case cmd_arg_t::INTARG_3:
            int args[3];
            int l = _cmd_arg_int_read(args, 3, input);
            if (l != 3) break;
            ((cmd_fn_intarg_3)c.cmd)(args[0], args[1], args[2]);
            break;
    }
}

