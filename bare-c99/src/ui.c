#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "pt.h"
#include "pt-delay.h"
#include "ticker.h"
#include "lcdpcf8574.h"
#include "charmaps.h"
#include "display_items.h"
#include "pin.h"
#include "button.h"
#include "encoder.h"
#include "blink.h"

#define REFRESH_TIME 10  //times the ticker freq (10ms)


//MACHINE STATUS
#define STOP 0
#define WORKING 1

uint8_t machine_status = STOP;      /*!< The current machine's state */


//FORM IDs
#define STARTING 0
#define MAIN_SCREEN 1
#define MENU 2

uint8_t form_id = 99;               /*!< The loaded form id */
bool is_form_changed = true;        /*!< True when a new form has changed and needs to be refreshed */
field_t *form = NULL;               /*!< A pointer to the current loaded form */
uint8_t num_of_fields = 0;          /*!< Contains the number of fields contained on the loaded form */

field_t *selected_field = NULL;     /*!< A pointer to the selected field */
uint16_t new_selected_value = 0;

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
 * 
 * @param     new_form_id The new form's identifier
 */
void set_form(uint8_t new_form_id) {
  form_id = new_form_id;
  is_form_changed = true;
  switch (new_form_id) {
    case STARTING:
      form = starting_form;
      num_of_fields = STARTING_FORM_NUM;
      break;
    case MAIN_SCREEN:
      form = main_form;
      num_of_fields = MAIN_FORM_NUM;
      break;
    case MENU:
      form = menu_form;
      num_of_fields = MENU_FORM_NUM;
      break;
  }
}


/**
 * @brief   Prints the field's value (not the label)
 * @param   field The field to print its value
 */
void print_field_value(field_t *field){
  lcd_gotoxy(field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, field->value);
  lcd_puts(value_ch);
}


/**
 * @brief   Prints the field's new selected value (not the label)
 * @param   field The field to print its value
 */
void print_field_new_value(field_t *field){
  lcd_gotoxy(field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, new_selected_value);
  lcd_puts(value_ch);
}


/**
 * @brief     Selects the given parameter and starts blinking it asynchronously and disables any other blinking parameter to make sure that just one is selected.
 * @param     field_id The selected parameter id. Use the defined macro from display_items.h
 */
void select_field(uint8_t field_id) {
  reset_blink_counter();

  //Previous selected field
  if (selected_field != NULL){
    print_field_value(selected_field);
    selected_field->is_selected = false;
  }

  //Actual selected field
  if (field_id != 255){
    selected_field = &form[field_id];
    selected_field->is_selected = true;
    new_selected_value = selected_field->value;
  } else {
    selected_field = NULL;
    new_selected_value = 0;
  }
}


/**
 * @brief Unselects the current selected field. Internally selects the last value (255)
 */
void unselect_field(){
  select_field(255);
}


/**
 * @brief Changes the new selected value applying the increment param
 * 
 * @param increment The value to increment (positive or negative)
 */
void change_value_selected_field(int8_t increment) {
  if (selected_field != NULL){
    //Calculate 
    new_selected_value += (increment * selected_field->step_val);
    
    //Check limits (min & max)
    if (new_selected_value > selected_field->max_val){
      new_selected_value = selected_field->max_val;
    } else if (new_selected_value < selected_field->min_val){
      new_selected_value = selected_field->min_val;
    }

    selected_field->is_changed = true;
  }
}


/**
 * @brief Save the modified value to the corresponding field's struc
 */
void save_selected_value(){
  selected_field->value = new_selected_value;
}


/**
 * @brief Print the corresponding field to the lcd
 * 
 * @param field Pointer to the field 
 */
void print_field(field_t *field){
  lcd_gotoxy(field->label_col,field->label_row);
  lcd_puts(field->label);
  if (field->value != 255){
    print_field_value(field);
  }
}



/**
 * @brief Print the blinking full-squared characters to the corresponding field
 * 
 * @param field Pointer to the field
 */
void blink_item_value(field_t *field){
  lcd_gotoxy(field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, new_selected_value);
  for (uint8_t i = 0; value_ch[i] != '\0'; i++) {
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


/**
 * @brief Initializes the display
 */
void display_init(void) {
  lcd_init(LCD_DISP_ON);
  lcd_led(0); //To activate LCD backlight

  create_custom_chars();
  lcd_clrscr();
  set_form(STARTING);
}


/**
 * @brief Initializes the buttons and the encoder
 */
void buttons_init(void){
  buttons[0] = btn_create(&PORTB, 1);
  buttons[1] = btn_create(&PORTB, 0);
  buttons[2] = btn_create(&PORTD, 7);
  buttons[3] = btn_create(&PORTD, 6);
  buttons[4] = btn_create(&PORTC, 3);
  buttons[5] = btn_create(&PORTD, 4);
  encoder = enc_create(&PORTD, 2, &PORTD, 3);
}


/**
 * @brief Update all the button status to check if the user has pressed them
 */
void update_buttons_status(){
  for(uint8_t i=0; i<NUM_OF_BUTTONS; i++){
    btn_update_status(&buttons[i]);      
  }
}


/**
 * @brief   Returns true if the button number has been clicked (rising edge detected)
 * 
 * @param   btn_id The button identifier
 * @return  A boolean (true if rising edge detected)
 */
bool is_clicked(uint8_t btn_id){
  return buttons[btn_id].status == CLICKED;
}


/**
 * @brief Prints the whole current form to the lcd 
 */
void print_form(void){
  for (uint8_t i = 0; i < num_of_fields; i++) {
    print_field(&form[i]);
  }
  is_form_changed = false;
}


/**
 * @brief Searches for the changed fields inside the current form and prints its values
 */
void print_changed_fields(void){
  for (uint8_t i = 0; i < num_of_fields; i++) {
    if(form[i].is_selected){
      if (is_blink_time()) {
        blink_item_value(&form[i]);
      } else if (is_unblink_time() | form[i].is_changed) {
        print_field_new_value(&form[i]);
        form[i].is_changed = false;    
        reset_unblink_counter();
      }
    } else if (form[i].is_changed) {
      print_field_value(&form[i]);
      form[i].is_changed = false;
      reset_unblink_counter();
    }
  }
}



/*****************************************************************
 * @brief   Protothread to refresh the display if needed
 *****************************************************************/
PT_THREAD(display_thread(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    if (is_form_changed) {
      lcd_clrscr();
      reset_blink_counter();
      unselect_field();
      print_form();
    } else {
      print_changed_fields();
    }

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

    switch (form_id) {
      case STARTING:
        _delay_ms(1000);            //To be changed. Simulates the homing sequence.
        set_form(MAIN_SCREEN);
        break;
      case MAIN_SCREEN:
        if (is_clicked(BTN_1)){
          select_field(VOLUME);
        } else if (is_clicked(BTN_2)){
          select_field(RATIO);
        } else if (is_clicked(BTN_3)){
          select_field(FREQ);
        } else if (is_clicked(BTN_MENU)){
          set_form(MENU);
        } else if (is_clicked(BTN_ENC) & (selected_field != NULL) ){
          save_selected_value();
          unselect_field();
        } else if (is_clicked(BTN_START)){  
          if (machine_status == STOP){                  //*** Start cycle ***
            main_form[START_STOP].val_format = STOP_ST;
            main_form[PLATEAU].val_format = UINT_FORMAT;
            main_form[PEAK].val_format = UINT_FORMAT;
            machine_status = WORKING;
          } else if (machine_status == WORKING){        //*** Stop cycle ***
            main_form[START_STOP].val_format = START_ST;
            main_form[PLATEAU].val_format = OFF_ST;
            main_form[PEAK].val_format = OFF_ST;
            machine_status = STOP;
          }         
          main_form[START_STOP].is_changed = true;
          main_form[PLATEAU].is_changed = true;
          main_form[PEAK].is_changed = true;
        } else if (get_position(&encoder) != 0){
          change_value_selected_field(get_position(&encoder));
          reset_position(&encoder);
        }
        break;
      case MENU:
        if (is_clicked(BTN_MENU)){
          set_form(MAIN_SCREEN);
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
  struct pt display_ctx, buttons_ctx, encoder_ctx;

  /* init modules */
  ticker_setup();
  ticker_start();
  display_init();
  buttons_init();
  sei();

  /* init contexts */
  PT_INIT(&display_ctx);
  PT_INIT(&buttons_ctx);
  PT_INIT(&encoder_ctx);

  /* do schedule of threads */
  for(;;) {
    (void)PT_SCHEDULE(buttons_thread(&buttons_ctx));
    (void)PT_SCHEDULE(encoder_thread(&encoder_ctx));
    (void)PT_SCHEDULE(display_thread(&display_ctx));
  }
  return 0;
}
