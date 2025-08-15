#ifndef TWI_H
#define TWI_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define WRITE 0
#define READ 1

typedef enum {
    TWI_SUCCESS,
    TWI_FAILURE,
    TWI_NORMAL,
} twi_status_t;

typedef struct {
    uint8_t address;
    uint8_t *buffer;
    uint8_t size;
} twi_message_t;


static int calc_bitrate(const uint8_t scl_frequency);
twi_status_t twi_init(const uint8_t scl_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);

#endif