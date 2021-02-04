#include "auton_select.h"
int auton_id = 0;
//side 0 is left 1 is right
int auton_side = 0;
//insperied by
//https://github.com/kunwarsahni01/Vex-Autonomous-Selector/blob/master/selection.cpp
//create defualt tabe
const int autonSize = 4;
const int autonIdSize = 2;
static const char *btnmMapItems[] = {"No Auton","1 Point","2 Point いち", "2 Point に", "3 Point", ""};
static const char *sideBtnmMapItems[] = {"Left", "Right", ""};
lv_obj_t *screen = lv_obj_create(NULL, NULL);;
lv_obj_t *autonBtnm;
lv_obj_t *sideBtnm;


lv_res_t btnmAction(lv_obj_t *btnm, const char *txt)
{
	for(int i = 0; i < autonSize; i++){
		if(strcmp(btnmMapItems[i], txt) == 0){
			auton_id = i;
		}
	}
	return LV_RES_OK;
}

lv_res_t sideBtnmAction(lv_obj_t *btnm, const char *txt)
{
	for(int i = 0; i < autonIdSize; i++){
		if(strcmp(sideBtnmMapItems[i], txt) == 0){
			auton_side = i;
		}
	}
	return LV_RES_OK;
}

void gui_init(int default_auton){
	static const char *btnmMap[] = {"No Auton","1 Point","2 Point いち", "2 Point に", "3 Point", ""};
	static const char *sideBtnmMap[] = {"Left", "Right", ""};

  int i = 0;
  auton_id = default_auton;

  lv_theme_t *th = lv_theme_alien_init(0, NULL);
  lv_theme_set_current(th);


  //create red btnm
  autonBtnm = lv_btnm_create(screen, NULL);
  lv_btnm_set_map(autonBtnm, btnmMap);
	lv_btnm_set_action(autonBtnm, btnmAction);
  lv_btnm_set_toggle(autonBtnm, true, abs(auton_id));//3
  lv_obj_set_size(autonBtnm, 450, 50);
  lv_obj_set_pos(autonBtnm, 0, 100);
  lv_obj_align(autonBtnm, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  sideBtnm = lv_btnm_create(screen, NULL);
  lv_btnm_set_map(sideBtnm, sideBtnmMap);
	lv_btnm_set_action(sideBtnm, sideBtnmAction);
  lv_btnm_set_toggle(sideBtnm, true, abs(auton_id));//3
  lv_obj_set_size(sideBtnm, 450, 50);
  lv_obj_set_pos(sideBtnm, 0, 100);
  lv_obj_align(sideBtnm, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

	lv_scr_load(screen);
}
