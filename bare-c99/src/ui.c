#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "pt.h"
#include "pt-delay.h"
#include "ticker.h"
#include "lcd.h"
#include "charmaps.h"
#include "display_items.h"


#define REFRESH_TIME 10  //times the ticker freq (10ms)

//Blinking frequency
#define BLINK_FREQ 5                //The number of display refresh loops it takes to blink
#define UNBLINK_FREQ BLINK_FREQ*2   //Same loops to unblink

//UI STATUS
#define STARTING 0
#define MAIN_SCREEN 1
#define MENU 2

uint8_t ui_status = STARTING;
uint8_t last_ui_status = 99;

display_item starting_items[4];
display_item main_items[6];
display_item menu_items[3];

uint8_t blink_count = 0;
uint8_t selected_param = 99;

//TEMP
uint16_t volume_param = 300;

lcd_t lcd;


/*******************
 * Public methods
 *******************/

/*
 * Sets the selected screen status to be displayed asynchronously
 */
void set_UI_status(uint8_t status_id) {
  ui_status = status_id;
}

/*
 * Gets the current screen status
 */
uint8_t get_UI_status(void){
  return ui_status;
}

/*
 * Selects the given parameter and starts blinking it asynchronously and
 * disables any other blinking parameter to make sure that just one is selected.
 * @param param_id The selected parameter id. Use the defined macro from display_items.h
 */
void select_param(uint8_t param_id) {
  switch (ui_status) {
    case STARTING:
      for (size_t i = 0; starting_items[i].label[0] != SENTINEL[0]; i++) {
        starting_items[i].is_blinking = (i == param_id);
      }
      break;
    case MAIN_SCREEN:
      for (size_t i = 0; main_items[i].label[0] != SENTINEL[0]; i++) {
        main_items[i].is_blinking = (i == param_id);
      }
      break;
    case MENU:
      for (size_t i = 0; menu_items[i].label[0] != SENTINEL[0]; i++) {
        menu_items[i].is_blinking = (i == param_id);
      }
      break;
  }
  selected_param = param_id;
}


void set_value_selected_param(char value_str[3]) {
  switch (ui_status) {
    case STARTING:
        starting_items[selected_param].value = value_str;
        starting_items[selected_param].is_changed = true;
      break;
    case MAIN_SCREEN:
        main_items[selected_param].value = value_str;
        main_items[selected_param].is_changed = true;
      break;
    case MENU:
        menu_items[selected_param].value = value_str;
        menu_items[selected_param].is_changed = true;
      break;
  }
}

void print_item(display_item item){
  lcd_move_cursor(lcd,item.label_col,item.label_row);
  lcd_printf(lcd, item.label);
  lcd_move_cursor(lcd,item.value_col,item.value_row);
  lcd_print(lcd, item.value);
}

void print_item_value(display_item item){
  lcd_move_cursor(lcd,item.value_col,item.value_row);
  lcd_print(lcd, item.value);
}

//Prints the value characters to the blinking state (full-squared character)
void blink_item_value(display_item item){
  lcd_move_cursor(lcd,item.value_col,item.value_row);
  for (uint8_t i = 0; i < strlen(item.value); i++) {
    lcd_send(lcd,LCD_DATA,BLINKING_CHAR);
  }
}

void create_custom_chars(void) {
  lcd_create_char(lcd,U,U_char);
  lcd_create_char(lcd,P,P_char);
  lcd_create_char(lcd,C,C_char);
}

void charge_main_items(void) {
  main_items[VOLUME] = volume;
  main_items[RATIO] = ratio;
  main_items[FREQ] = freq;
  main_items[PEAK] = peak;
  main_items[PLATEAU] = plateau;
  main_items[PLATEAU + 1] = sentinel;
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
  lcd = lcd_bind(&PORTD,4,5);
  lcd_on(lcd);
  create_custom_chars();
  lcd_disable_autoscroll(lcd);
  lcd_disable_cursor(lcd);
  lcd_clear(lcd);
  charge_main_items();
  charge_starting_items();
  charge_menu_items();
}


bool UI_status_changed(void){
  return ui_status != last_ui_status;
}



void UI_print_screen(display_item screen_items[]){
  for (uint8_t i = 0; true; i++) {
    if (screen_items[i].label[0] == SENTINEL[0]) {
      break;
    }
    print_item(screen_items[i]);
  }
}

void UI_print_changed_params(display_item screen_items[]){
  for (uint8_t i = 0; true; i++) {
    if (screen_items[i].label[0] == SENTINEL[0]) {
        break;
    } else if (screen_items[i].is_changed == true) {
        print_item_value(screen_items[i]);
        screen_items[i].is_changed = false;
        blink_count = 0; //If user changes the value, the blink starts from begining to have better user feedback
    } else if(screen_items[i].is_blinking){
        if (blink_count == BLINK_FREQ) {
            blink_item_value(screen_items[i]);
        } else if (blink_count == UNBLINK_FREQ) {
            print_item_value(screen_items[i]);
        }
    }
  }
}

void update_blink_counter(void) {
  if (blink_count < UNBLINK_FREQ) {
    blink_count++;
  } else {
    blink_count = 0;
  }
}

void reset_blink_counter(void){
  blink_count = 0;
}

/*****************************************************************
 * Refresh display thread
 *****************************************************************/
PT_THREAD(ui_thread(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    if (UI_status_changed()) {
      lcd_clear_without_delay(lcd);
      reset_blink_counter();
      select_param(99);

      chronos = ticker_get();
      PT_WAIT_WHILE(pt, ticker_get() - chronos <= 1); //Waits 1 tick (10ms to make sure the LCD is cleared)

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


//Thread temporal per canviar la pantalla cada cert temps
PT_THREAD(blink_thread(struct pt *pt))
{
  static uint16_t chronos2;
  PT_BEGIN(pt);

  for(;;) {
    chronos2 = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos2 <= 400);
    switch (ui_status) {
      case STARTING:
        set_UI_status(MAIN_SCREEN);
        break;
      case MAIN_SCREEN:
        set_UI_status(MENU);
        break;
      case MENU:
        set_UI_status(STARTING);
        break;
    }
  }
  PT_END(pt);
}

//Thread per a canviar parametres en certs moments
PT_THREAD(params_thread(struct pt *pt))
{
  static uint16_t chronos3;
  static char value_st[3];

  PT_BEGIN(pt);

  for(;;) {
    chronos3 = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos3 <= 50);
    if (ui_status == MAIN_SCREEN) {
      select_param(VOLUME);
      if (volume_param < 400) {
        volume_param += 10;
      } else {
        volume_param = 300;
      }
      sprintf(value_st,"%i",volume_param);
      // value_st[0]='3';
      // value_st[1]='3';
      // value_st[2]='3';
      set_value_selected_param(value_st);
    }
    //sprintf(main_items[VOLUME].value,"%i",volume_param);
  }
  PT_END(pt);
}

int main(void) {
  /* context dels threads */
  struct pt ui_ctx, blink_ctx, params_ctx;

  /* init modules */
  ticker_setup();
  ticker_start();
  UI_init();
  sei();

  /* init contexts */
  PT_INIT(&ui_ctx);
  PT_INIT(&blink_ctx);
  PT_INIT(&params_ctx);

  /* do schedule of threads */
  for(;;) {
    (void)PT_SCHEDULE(ui_thread(&ui_ctx));
    (void)PT_SCHEDULE(blink_thread(&blink_ctx));
    (void)PT_SCHEDULE(params_thread(&params_ctx));
  }
  return 0;
}
