#ifndef DISPLAY_ITEMS_H_
#define DISPLAY_ITEMS_H_

#include "charmaps.h"

#define SENTINEL "0"

/*
 * Screen items IDs
 */

//Main screen items IDs
#define VOLUME 0
#define RATIO 1
#define FREQ 2
#define PEAK 3
#define PLATEAU 4

//Starting screen items IDs
#define UPC 0
#define MANRESA 1
#define RESPIRADOR 2

//Menu screen items IDs
#define MENU_ID 0


/*
 * Type to define a display item formed by a label and its value
 */
typedef struct display_items {
  char *label;
  uint8_t label_col;
  uint8_t label_row;
  char *value;
  uint8_t value_col;
  uint8_t value_row;
  bool is_blinking;
  bool is_changed;
} display_item;


/*************************************
 * Main screen items
 *************************************/
display_item volume = {
  "VOLUME",
  0, //Label Column
  2,  //Label Row
  "400", //Value
  1, //Value Column
  3,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item ratio = {
  "I:R",
  9, //Label Column
  2,  //Label Row
  "1:2", //Value
  9, //Value Column
  3,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item freq = {
  "FREQ",
  16, //Label Column
  2,  //Label Row
  "22", //Value
  17, //Value Column
  3,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item peak = {
  "PEAK:",
  0, //Label Column
  0,  //Label Row
  "36", //Value
  6, //Value Column
  0,  //Value Row
  false, //Blinking
  false //Is changed
};

display_item plateau = {
  "PLATEAU:",
  0, //Label Column
  1,  //Label Row
  "25", //Value
  9, //Value Column
  1,  //Value Row
  false, //Blinking
  false //Is changed
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
