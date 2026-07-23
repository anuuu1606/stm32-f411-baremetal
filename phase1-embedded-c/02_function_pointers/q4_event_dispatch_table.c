// Q. Simulate a tiny "event dispatch table": an enum of event types (EVENT_BUTTON, EVENT_TIMER, EVENT_UART),
// an array of function pointers indexed by the enum, each printing a different message.
// Dispatch based on a variable holding the event type.

#include <stdio.h>

enum EVENT_TYPE {EVENT_BUTTON, EVENT_TIMER, EVENT_UART};

void handle_button(void) {
    printf("Button has been pressed.\n");
}

void handle_timer(void) {
    printf("Timer has fired.\n");
}

void handle_uart(void) {
    printf("UART data received.\n");
}

void (*event_table[3])(void) = {handle_button, handle_timer, handle_uart};

void run_event(enum EVENT_TYPE event) {
    event_table[event]();
}

int main() {
    enum EVENT_TYPE current_event = EVENT_TIMER;

    run_event(current_event);   // uses the variable 
    run_event(EVENT_BUTTON);    // can also pass an enum value directly 

    return 0;
}