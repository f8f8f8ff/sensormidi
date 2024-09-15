#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>

#include "midi.h"
#include "state.h"
#include "sensors.h"


#endif

typedef void(*cmd_fn_noarg)(void);
typedef void(*cmd_fn_intarg)(int);
typedef void(*cmd_fn_intarg_3)(int, int, int);

enum class cmd_arg_t {
    NOARG,
    INTARG,
    INTARG_3,
};

struct cmd_t {
    void* cmd;
    cmd_arg_t type;
    const char* name;
};

void cmd_read();
void _cmd_run(cmd_t c, char* input, size_t input_l);
int _cmd_arg_int_read(int* args, int n_args, char* s);

extern const cmd_t commands[];

void cmd_state();
void cmd_start();
void cmd_stop();
void cmd_dump();

void cmd_send_cc(int cc);

void cmd_range_set(int sensor, int min, int max);
void cmd_range_get(int sensor);

void cmd_ease_set(int sensor, int rate_up, int rate_down);
void cmd_ease_get(int sensor);
