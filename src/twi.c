#include "twi.h"
#include <avr/io.h>
#include <util/twi.h>


typedef struct{
    volatile uint8_t busy;
    volatile twi_message_t *msg;
    volatile size_t msg_count;
    volatile twi_status_t state;
}twi_variables_t;

static volatile twi_variables_t twi_vars;
static volatile twi_message_t current_msg;

static int calc_bitrate(const uint8_t scl_frequency){
    return ((F_CPU/scl_frequency)-16)/2; // assume TWPS or prescaler = 1
}

twi_status_t twi_init(const uint8_t scl_frequency){
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Ensure prescaler is set to 1

    TWCR |= (1<<TWEN); // enable twi
    TWCR |= (1<<TWIE); // enable interrupt


    TWBR = calc_bitrate(scl_frequency);

    // set default variable values
    twi_vars.busy = 0;
    twi_vars.state = TWI_NORMAL;

    return TWI_NORMAL;
}