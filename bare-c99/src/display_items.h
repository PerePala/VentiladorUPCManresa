#ifndef DISPLAY_ITEMS_H_
#define DISPLAY_ITEMS_H_

#include "charmaps.h"

#define SENTINEL "0"

/*
 * Screen items IDs
 */

//Main screen items IDs
#define MAIN_SC_ITEMS_NUM 7   //Including sentinel
#define VOLUME 0
#define RATIO 1
#define FREQ 2
#define PEAK 3
#define PLATEAU 4
#define START_STOP 5

//Starting screen items IDs
#define START_SC_ITEMS_NUM 4  //Including sentinel
#define UPC 0
#define MANRESA 1
#define RESPIRADOR 2

//Menu screen items IDs
#define MENU_SC_ITEMS_NUM 2   //Including sentinel
#define MENU_ID 0

//Char array where the converted values will be stored
char value_ch[20]; 

//Start & stop strings
#define START_ST "START->"
#define STOP_ST " STOP->"

#define OFF_ST "--"

/*
 * Type to define a display item formed by a label and its value
 */
typedef struct display_items {
  char *label;
  uint8_t label_col;
  uint8_t label_row;
  uint16_t value;
  char *val_format;
  uint8_t value_col;
  uint8_t value_row;
  bool is_blinking;
  bool is_changed;
  uint8_t step_val;
} display_item;


/*************************************
 * Main screen items
 *************************************/
display_item volume = {
  "VOLUME",
  0, //Label Column
  2,  //Label Row
  400, //Value
  "%u",   //Value's format
  1, //Value Column
  3,  //Value Row
  false, //Blinking
  false, //Is changed
  2       //Step value
};

display_item ratio = {
  "I:R",
  9, //Label Column
  2,  //Label Row
  1, //Value     **************Canviar******************
  "1:%u",   //Value's format
  9, //Value Column
  3,  //Value Row
  false, //Blinking
  false, //Is changed
  1       //Step value
};

display_item freq = {
  "FREQ",
  16, //Label Column
  2,  //Label Row
  22, //Value
  "%u",   //Value's format
  17, //Value Column
  3,  //Value Row
  false, //Blinking
  false, //Is changed
  1       //Step value
};

display_item peak = {
  "PEAK:",
  0, //Label Column
  0,  //Label Row
  36, //Value
  OFF_ST,   //Value's format
  6, //Value Column
  0,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item plateau = {
  "PLATEAU:",
  0, //Label Column
  1,  //Label Row
  25, //Value
  OFF_ST,   //Value's format
  9, //Value Column
  1,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item start_stop = {
  " ",
  12, //Label Column
  0, //Label Row
  0,    //Value: Not used
  START_ST,   //Value's format
  13,
  0
};


/*************************
 * Starting screen items
 *************************/

 display_item upc = {
   UPC_STR,
   8, //Label Column
   0  //Label Row
 };

 display_item manresa = {
   "Manresa",
   6, //Label Column
   1  //Label Row
 };

 display_item respirador = {
   "VENTILATOR",
   5, //Label Column
   2  //Label Row
 };





 /*************************
  * Menu screen items
  *************************/

  display_item menu = {
    "MENU",
    8, //Label Column
    0  //Label Row
  };





const display_item sentinel = {
  SENTINEL
};



#endif
