#include "twi.h"
#include <avr/io.h>
#include <stddef.h>
#include <stdbool.h>

int main(){
    DDRC &= ~(1<<DDC4);//set 4 and 5 as outputs
    DDRC &= ~(1<<DDC5);
    PORTC |= (1<<PC4);//enable pulldown resistors
    PORTC |= (1<<PC5);
    sei();

    twi_status_t status; 
    status = twi_init(100000);
    while (status!=TWI_SUCCESS);

    twi_message_t messages[2] = {
        {
            .address = (0x68<<1)|WRITE,
            .buffer = (uint8_t[1]){0x75},
            .size = 1,
        },
        {
            .address = (0x68<<1)|READ,
            .buffer = (uint8_t[1]){},
            .size = 1,
        },
    };
    twi_enque(messages,2);
    while(twi_status!=TWI_SUCCESS);
    
    //messages[1].buffer[0] = 0x68

    while(1){
        loop();
    }
}

void loop(){

}