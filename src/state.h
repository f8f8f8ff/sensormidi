#ifndef STATE_H
#define STATE_H

enum state_t {
    RUNNING,
    STOPPED,
};
extern state_t STATE;

char* state_string(state_t s);

#endif
