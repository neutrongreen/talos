#include "auton_select.h"
int auton_id = 0;
//side 0 is left 1 is right
bool auton_side = false;
//insperied by
//https://github.com/kunwarsahni01/Vex-Autonomous-Selector/blob/master/selection.cpp
//create defualt tabe

int auton_count = 4;
int side_count = 2;
const char *btnmMap[] = {"No Auton","1 Point","2 Point","3 Point"};
const char *sideBtnmMap[] = {"Left", "Right"};

lv_res_t autonBtnmAction(lv_obj_t *btnm, const char *txt){
	//printf("red button: %s released\n", txt);

	for(int i = 0; i < auton_count; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			auton_id = i;
		}
	}

	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_res_t sideBtnmAction(lv_obj_t *btnm, const char *txt){
	//printf("red button: %s released\n", txt);

	for(int i = 0; i < side_count; i++){
		if(strcmp(txt, sideBtnmMap[i]) == 0){
			auton_side = i;
		}
	}

	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_obj_t *screen;
lv_obj_t *autonBtnm;
lv_obj_t *sideBtnm;

void gui_init(int default_auton){
  int i = 0;
  auton_id = default_auton;

  lv_theme_t *th = lv_theme_alien_init(0, NULL);
  lv_theme_set_current(th);
  screen = lv_scr_act();


  //create red btnm
  autonBtnm = lv_btnm_create(screen, NULL);
  lv_btnm_set_map(autonBtnm, btnmMap);
  lv_btnm_set_action(autonBtnm, autonBtnmAction);
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
}
