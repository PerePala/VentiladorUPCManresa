#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include "pin.h"

//Button status
#define RELEASED    0
#define CLICKED     1   //When a rising edge is detected
#define PRESSED     2

/**
 * @brief Type to define a button
 */
typedef struct {
  pin_t pin;
  uint8_t status;
} button_t;

button_t btn_create(volatile uint8_t *port, uint8_t pin);

void btn_update_status(button_t *btn);

#endif