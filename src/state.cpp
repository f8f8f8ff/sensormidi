#include "state.h"

state_t STATE = RUNNING;

char* state_string(state_t s) {
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