#ifndef STATE_H
#define STATE_H

enum State {
    RUNNING,
    STOPPED,
} STATE;

char* state_string(State s) {
    static char* state_string;
    switch (s) {
        case RUNNING:
            state_string = "RUNNING";
            break;
        case STOPPED:
            state_string = "STOPPED";
            break;
    }
    return state_string;
}

#endif