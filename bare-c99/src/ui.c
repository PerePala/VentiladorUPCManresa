#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "pt.h"
#include "pt-delay.h"
#include "ticker.h"
// #include "lcd.h"
#include "lcdpcf8574.h"

#include "charmaps.h"
#include "display_items.h"

#include "pin.h"
#include "button.h"
#include "encoder.h"
#include "blink.h"

#define REFRESH_TIME 10  //times the ticker freq (10ms)


//UI STATUS
#define STARTING 0
#define MAIN_SCREEN 1
#define MENU 2

//MACHINE STATUS
#define STOP 0
#define WORKING 1

uint8_t machine_status = STOP;

//Defines the first screen to be load
uint8_t ui_status = STARTING;

uint8_t last_ui_status = 99;

display_item starting_items[START_SC_ITEMS_NUM];
display_item main_items[MAIN_SC_ITEMS_NUM];
display_item menu_items[MENU_SC_ITEMS_NUM];


int8_t selected_param = 99;

/**
 * Buttons 
 */
#define NUM_OF_BUTTONS 6
#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_MENU 3
#define BTN_START 4
#define BTN_ENC 5
button_t buttons[NUM_OF_BUTTONS];

//Encoder
encoder_t encoder;
#define ENCODER_REFRESH_TIME 1


/**
 * @brief     Sets the selected screen status to be displayed asynchronously
 */
void set_UI_status(uint8_t status_id) {
  ui_status = status_id;
}

/**
 * @brief      Gets the current screen status
 */
uint8_t get_UI_status(void){
  return ui_status;
}

/**
 * @brief   Prints to the screen the item value (not the label)
 * @param   item The item to print its value
 */
void print_item_value(display_item *item){
  lcd_gotoxy(item->value_col, item->value_row);
  sprintf(value_ch, item->val_format, item->value);
  lcd_puts(value_ch);
}


/**
 @brief     Selects the given parameter and starts blinking it asynchronously and disables any other blinking parameter to make sure that just one is selected.
 @param     param_id The selected parameter id. Use the defined macro from display_items.h
 */
void select_param(int8_t param_id) {
  reset_blink_counter();
  switch (ui_status) {
    case STARTING:
      for (size_t i = 0; i<START_SC_ITEMS_NUM; i++) {
        if (starting_items[i].is_blinking){     //To avoid leaving the last selected param with the blinking char
          print_item_value(&starting_items[i]);
        }
        starting_items[i].is_blinking = (i == param_id);
      }
      break;
    case MAIN_SCREEN:
      for (size_t i = 0; i<MAIN_SC_ITEMS_NUM; i++) {
        if (main_items[i].is_blinking){     //To avoid leaving the last selected param with the blinking char
          print_item_value(&main_items[i]);
        }
        main_items[i].is_blinking = (i == param_id);
      }
      break;
    case MENU:
      for (size_t i = 0; i<MENU_SC_ITEMS_NUM; i++) {
        if (menu_items[i].is_blinking){     //To avoid leaving the last selected param with the blinking char
          print_item_value(&menu_items[i]);
        }
        menu_items[i].is_blinking = (i == param_id);
      }
      break;
  }
  selected_param = param_id;
}


void set_value_selected_param(int8_t difference) {
  display_item *item;
  switch (ui_status) {
    case STARTING:
        item = &starting_items[selected_param];
      break;
    case MAIN_SCREEN:
        item = &main_items[selected_param];
      break;
    case MENU:
        item = &menu_items[selected_param];
      break;
    default:
      return;
  }
  //Calculate 
  item->value += (difference * item->step_val);
  
  //Check limits (min & max)
  if (item->value > item->max_val){
    item->value = item->max_val;
  } else if (item->value < item->min_val){
    item->value = item->min_val;
  }

  item->is_changed = true;

}


void print_item(display_item *item){
  lcd_gotoxy(item->label_col,item->label_row);
  lcd_puts(item->label);
  if (item->value != 255){
    print_item_value(item);
  }
}


//Prints the value characters to the blinking state (full-squared character)
void blink_item_value(display_item *item){
  lcd_gotoxy(item->value_col, item->value_row);
  sprintf(value_ch, item->val_format, item->value);
  for (uint8_t i = 0; value_ch[i] != '\0'; i++) {   //************CANVIAR EL 3************
    lcd_data(BLINKING_CHAR);
  }
}



/**
 * @brief Creates and send the custom charmaps to the LCD
 */
void create_custom_chars(void) {
  lcd_create_custom_char(U,U_char);
  lcd_create_custom_char(P,P_char);
  lcd_create_custom_char(C,C_char);
}

void charge_main_items(void) {
  main_items[VOLUME] = volume;
  main_items[RATIO] = ratio;
  main_items[FREQ] = freq;
  main_items[PEAK] = peak;
  main_items[PLATEAU] = plateau;
  main_items[START_STOP] = start_stop;
  main_items[START_STOP + 1] = sentinel;
}

void charge_starting_items(void) {
  starting_items[UPC] = upc;
  starting_items[MANRESA] = manresa;
  starting_items[RESPIRADOR] = respirador;
  starting_items[RESPIRADOR+1] = sentinel;
}

void charge_menu_items(void) {
  menu_items[MENU_ID] = menu;
  menu_items[MENU_ID+1] = sentinel;
}

void UI_init(void) {
  lcd_init(LCD_DISP_ON);
  lcd_led(0); //To activate LCD backlight

  create_custom_chars();
  lcd_clrscr();
  
  charge_main_items();
  charge_starting_items();
  charge_menu_items();
}

void buttons_init(void){
  buttons[0] = btn_create(&PORTB, 1);
  buttons[1] = btn_create(&PORTB, 0);
  buttons[2] = btn_create(&PORTD, 7);
  buttons[3] = btn_create(&PORTD, 6);
  buttons[4] = btn_create(&PORTC, 3);
  buttons[5] = btn_create(&PORTD, 4);
  encoder = enc_create(&PORTD, 2, &PORTD, 3);
}



void update_buttons_status(){
  for(uint8_t i=0; i<NUM_OF_BUTTONS; i++){
    btn_update_status(&buttons[i]);      
  }
}


/**
@brief    Returns true if the button number has been clicked (rising edge detected)
@param    btn_id  The button identifier
@return   A boolean (true if rising edge detected)
*/
bool is_clicked(uint8_t btn_id){
  return buttons[btn_id].status == CLICKED;
}


bool UI_status_changed(void){
  return ui_status != last_ui_status;
}



void UI_print_screen(display_item screen_items[]){
  for (uint8_t i = 0; true; i++) {
    if (screen_items[i].label[0] == SENTINEL[0]) {
      break;
    }
    print_item(&screen_items[i]);
  }
}

void UI_print_changed_params(display_item screen_items[]){
  for (uint8_t i = 0; true; i++) {
    if (screen_items[i].label[0] == SENTINEL[0]) {
        break;
    } else if (screen_items[i].is_changed == true) {
        print_item_value(&screen_items[i]);
        screen_items[i].is_changed = false;
        reset_unblink_counter();
    } else if(screen_items[i].is_blinking){
        if (is_blink_time()) {
            blink_item_value(&screen_items[i]);
        } else if (is_unblink_time()) {
            print_item_value(&screen_items[i]);
        }
    }
  }
}




/*****************************************************************
 * @brief   Protothread to refresh the display if needed
 *****************************************************************/
PT_THREAD(ui_thread(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    if (UI_status_changed()) {
      lcd_clrscr();
      reset_blink_counter();
      select_param(99);

      switch (ui_status) {
        case STARTING:
            UI_print_screen(starting_items);
          break;
        case MAIN_SCREEN:
            UI_print_screen(main_items);
          break;
        case MENU:
            UI_print_screen(menu_items);
          break;
      }
    } else {
      switch (ui_status) {
        case MAIN_SCREEN:
            UI_print_changed_params(main_items);
          break;
        case MENU:
            UI_print_changed_params(menu_items);
          break;
      }
    }

    last_ui_status = ui_status;
    update_blink_counter();

    chronos = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos <= REFRESH_TIME);
  }
  PT_END(pt);
}




/****************************************************************************************
 * @brief   Protothread to read the buttons status and make changes to the display if needed
 ****************************************************************************************/
PT_THREAD(buttons_thread(struct pt *pt))
{
  static uint16_t chronos2;
  PT_BEGIN(pt);

  for(;;) {
    chronos2 = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos2 <= 10);

    update_buttons_status();

    switch (ui_status) {
      case STARTING:
        _delay_ms(1000);            //To be changed. Simulates the homing sequence.
        set_UI_status(MAIN_SCREEN);
        break;
      case MAIN_SCREEN:
        if (is_clicked(BTN_1)){
          select_param(VOLUME);
        } else if (is_clicked(BTN_2)){
          select_param(RATIO);
        } else if (is_clicked(BTN_3)){
          select_param(FREQ);
        } else if (is_clicked(BTN_MENU)){
          set_UI_status(MENU);
        } else if (is_clicked(BTN_ENC) & (selected_param != 99) ){
          select_param(99);
        } else if (is_clicked(BTN_START)){  
          if (machine_status == STOP){                  //*** Start cycle ***
            main_items[START_STOP].val_format = STOP_ST;
            main_items[PLATEAU].val_format = "%u";
            main_items[PEAK].val_format = "%u";
            machine_status = WORKING;
          } else if (machine_status == WORKING){        //*** Stop cycle ***
            main_items[START_STOP].val_format = START_ST;
            main_items[PLATEAU].val_format = OFF_ST;
            main_items[PEAK].val_format = OFF_ST;
            machine_status = STOP;
          }         
          main_items[START_STOP].is_changed = true;
          main_items[PLATEAU].is_changed = true;
          main_items[PEAK].is_changed = true;
        } else if (get_position(&encoder) != 0){
          set_value_selected_param(get_position(&encoder));
          reset_position(&encoder);
        }
        break;
      case MENU:
        if (is_clicked(BTN_MENU)){
          set_UI_status(MAIN_SCREEN);
        }
        break;
    }
  }
  PT_END(pt);
}




/**********************************************************************************
 * @brief   Protothread to read the encoder's pulses and update its position
 **********************************************************************************/
PT_THREAD(encoder_thread(struct pt *pt))
{
  static uint16_t chronos3;
  PT_BEGIN(pt);

  for(;;) {
    chronos3 = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos3 <= ENCODER_REFRESH_TIME);
    enc_update_position(&encoder);
  }
  PT_END(pt);
}



int main(void) {
  /* context dels threads */
  struct pt ui_ctx, buttons_ctx, encoder_ctx;

  /* init modules */
  ticker_setup();
  ticker_start();
  UI_init();
  buttons_init();
  sei();

  /* init contexts */
  PT_INIT(&ui_ctx);
  PT_INIT(&buttons_ctx);
  PT_INIT(&encoder_ctx);

  /* do schedule of threads */
  for(;;) {
    (void)PT_SCHEDULE(ui_thread(&ui_ctx));
    (void)PT_SCHEDULE(buttons_thread(&buttons_ctx));
    (void)PT_SCHEDULE(encoder_thread(&encoder_ctx));
  }
  return 0;
}
