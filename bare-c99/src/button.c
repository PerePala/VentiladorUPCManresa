#include "button.h"
#include "pin.h"


button_t btn_create(volatile uint8_t *port, uint8_t pin){
    button_t btn;
    btn.pin = pin_bind(port, pin, InputPullup);
    btn.status = RELEASED;
    return btn;
}

void btn_update_status(button_t *btn){
    if (!pin_r(btn->pin)){   //Button pressed
      if (btn->status != PRESSED){
        btn->status++;
      }
    } else {                //Button released
      btn->status = RELEASED;
    }
}

