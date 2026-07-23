// Q. Store a function pointer inside a struct (e.g. struct Device { void (*init)(void); void (*read)(void); };),
// populate two different "devices," and call their methods through the struct.

#include <stdio.h>

typedef struct {
    void(*init)(void);
    void(*read)(void);
} Device;

void uart_init(void){
    printf("UART Device:\n");
    printf("UART initialized.\n");
}

void uart_read(void){
    printf("UART reading data.\n\n");
}

void sensor_init(void){
    printf("Sensor Device:\n");
    printf("Sensor initialized.\n");
}

void sensor_read(void){
    printf("Sensor reading data.\n");
}

int main(){
    Device UART;
    UART.init=uart_init;
    UART.read=uart_read;

    Device Sensor;
    Sensor.init=sensor_init;
    Sensor.read=sensor_read;

    (UART.init)();
    (UART.read)();

    (Sensor.init)();
    (Sensor.read)();

    return 0;
}