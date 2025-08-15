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

ISR(TWI_vect){
    switch(TW_STATUS){
        case TW_START:
        case TW_REP_START:
            twi_vars.state = TWI_NORMAL;
            current_msg = twi_vars.msg++;
            --twi_vars.msg_count;
            TWDR = current_msg->address;//tx address
            TWCR &= ~(1<<TWSTA);
            return;

        case TW_MT_SLA_ACK://SLA+W, ACK
            TWDR = *(current_msg->buffer);//send data
            current_msg.size--;
            return;
        case TW_MT_DATA_ACK://DATA, ACK
        case TW_MR_SLA_ACK://sla+r 
        case TW_MR_DATA_ACK://data rx

        case TW_MT_ARB_LOST:
        case TW_MR_SLA_NACK:

        case TW_MT_DATA_NACK:
 	    case TW_MR_DATA_NACK:

 	    case TW_NO_INFO:
 	    case TW_BUS_ERROR:
    }
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

twi_status_t twi_enqueue(twi_message_t *messages, size_t count){
    if(twi_vars.busy){
        return TWI_FAILURE;
    }if(count<1){
        return TWI_FAILURE;
    }
    twi_vars.busy = 1;
    twi_vars.msg = messages;
    twi_vars.msg_count = count;

    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
    return TWI_SUCCESS;
}