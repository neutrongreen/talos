#include "auton_select.h"
int auton_id = 0;
//insperied by
//https://github.com/kunwarsahni01/Vex-Autonomous-Selector/blob/master/selection.cpp
//create defualt tabe

lv_obj_t *screen;

void gui_init(int default_auton){
  int i = 0;
  auton_id = default_auton;

  lv_theme_t *th = lv_theme_alien_init(0, NULL);
  lv_theme_set_current(th);
  screen = lv_scr_act();



}
