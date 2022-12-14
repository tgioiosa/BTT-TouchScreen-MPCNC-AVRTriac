//TG MODIFIED*****
#include "UnifiedHeat.h"
#include "includes.h"

const MENUITEMS UnifiedHeatItems = {
  // title
  LABEL_UNIFIEDHEAT,
  // icon                          label
  {
    {ICON_REMOVED,                 LABEL_REMOVED},      //TG 7/17/22 was HEAT-PREHEAT    
    {ICON_REMOVED,                 LABEL_REMOVED},      //TG 7/17/22 was HEAT-HEAT
    {ICON_FAN,                     LABEL_FAN},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_REMOVED,                 LABEL_REMOVED},       //TG 2/18/21 was COOLDOWN
    {ICON_BACKGROUND,              LABEL_BACKGROUND},
    {ICON_BACK,                    LABEL_BACK},
  }
};

void menuUnifiedHeat(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&UnifiedHeatItems);

  while (infoMenu.menu[infoMenu.cur] == menuUnifiedHeat)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:
        //infoMenu.menu[++infoMenu.cur] = menuPreheat;  //TG 7/17/22 removed
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuSpindle;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.cur] = menuFan;
        break;

      case KEY_ICON_5:
        //heatCoolDown();                               //TG 7/17/22 removed
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default:
        break;
    }

    loopProcess();
  }
}
