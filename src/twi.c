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
    return ((F_CPU/scl_frequency)-16)/2; //assume TWPS or prescaler = 1
}
