#include "MachineSettings.h"

//load values on page change and reload
void loaditemsCustomGcode(LISTITEMS * customItems, CUSTOM_GCODES * customcodes, uint8_t gc_cur_page)
{
  uint8_t gc_page_count = (customcodes->count + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE;

  for (uint32_t i = 0; i < LISTITEM_PER_PAGE; i++)
  {
    uint32_t item_index = gc_cur_page * LISTITEM_PER_PAGE + i;
    if (item_index < customcodes->count)
    {
      customItems->items[i].icon = ICONCHAR_CODE;
      customItems->items[i].titlelabel.index = LABEL_DYNAMIC;
      customItems->items[i].titlelabel.address = customcodes->name[item_index];
    }
    else
    {
      customItems->items[i].icon = ICONCHAR_BACKGROUND;
      customItems->items[i].titlelabel.index = LABEL_BACKGROUND;
    }
  }
  // set page up down button according to page count and current page
  if (customcodes->count <= LISTITEM_PER_PAGE)
  {
    customItems->items[5].icon = ICONCHAR_BACKGROUND;
    customItems->items[6].icon = ICONCHAR_BACKGROUND;
  }
  else
  {
    if (gc_cur_page == 0)
    {
      customItems->items[5].icon = ICONCHAR_BACKGROUND;
      customItems->items[6].icon = ICONCHAR_PAGEDOWN;
    }
    else if (gc_cur_page == (gc_page_count - 1))
    {
      customItems->items[5].icon = ICONCHAR_PAGEUP;
      customItems->items[6].icon = ICONCHAR_BACKGROUND;
    }
    else
    {
      customItems->items[5].icon = ICONCHAR_PAGEUP;
      customItems->items[6].icon = ICONCHAR_PAGEDOWN;
    }
  }
}

void menuCustom(void)
{
  LISTITEMS customItems = {
    // title
    LABEL_CUSTOM,
    // icon                 ItemType    Item Title        item value text(only for custom value)
    {
      {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_BACKGROUND, LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_PAGEUP,     LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_PAGEDOWN,   LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
      {ICONCHAR_BACK,       LIST_LABEL, LABEL_BACKGROUND, LABEL_BACKGROUND},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;
  CUSTOM_GCODES customcodes;

  //load custom codes
  W25Qxx_ReadBuffer((u8 *)&customcodes, CUSTOM_GCODE_ADDR, sizeof(CUSTOM_GCODES));

  uint8_t gc_page_count = (customcodes.count + LISTITEM_PER_PAGE - 1) / LISTITEM_PER_PAGE;
  uint8_t gc_cur_page = 0;

  loaditemsCustomGcode(&customItems, &customcodes, gc_cur_page);
  menuDrawListPage(&customItems);

  while (infoMenu.menu[infoMenu.cur] == menuCustom)
  {
    key_num = menuKeyGetValue();

    if (key_num < LISTITEM_PER_PAGE)
    {
      uint32_t item_index = gc_cur_page * LISTITEM_PER_PAGE + key_num;
      if (item_index < customcodes.count)
        mustStoreScript(customcodes.gcode[item_index]);
    }

    switch (key_num)
    {
      case KEY_ICON_5:
        if (gc_page_count > 1)
        {
          if (gc_cur_page > 0)
          {
            gc_cur_page--;
            loaditemsCustomGcode(&customItems, &customcodes, gc_cur_page);
            menuRefreshListPage();
          }
        }
        break;

      case KEY_ICON_6:
        if (gc_page_count > 1)
        {
          if (gc_cur_page < gc_page_count - 1)
          {
            gc_cur_page++;
            loaditemsCustomGcode(&customItems, &customcodes, gc_cur_page);
            menuRefreshListPage();
          }
        }
        break;

      case KEY_ICON_7:
        gc_cur_page = 0;
        infoMenu.cur--;
        break;

      default:
        break;
    }

    loopProcess();
  }
}

void menuRGBSettings(void)
{
  MENUITEMS RGBItems = {
    // title
    LABEL_RGB_SETTINGS,
    // icon                          label
    {
      {ICON_RGB_RED,                 LABEL_RED},
      {ICON_RGB_GREEN,               LABEL_GREEN},
      {ICON_RGB_BLUE,                LABEL_BLUE},
      {ICON_RGB_WHITE,               LABEL_WHITE},
      {ICON_RGB_OFF,                 LABEL_OFF},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&RGBItems);

  while (infoMenu.menu[infoMenu.cur] == menuRGBSettings)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:  //Red
        storeCmd("M150 R255 U0 B0 P255\n");
        break;

      case KEY_ICON_1:  //Green
        storeCmd("M150 R0 U255 B0 P255\n");
        break;

      case KEY_ICON_2:  //Blue
        storeCmd("M150 R0 U0 B255 P255\n");
        break;

      case KEY_ICON_3:  //White
        storeCmd("M150 R255 U255 B255 P255\n");
        break;

      case KEY_ICON_4:  //Turn Off
        storeCmd("M150 R0 U0 B0 P0\n");
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

#ifdef QUICK_EEPROM_BUTTON

void menuEepromSettings(void)
{
  MENUITEMS eepromSettingsItems = {
    // title
    LABEL_EEPROM_SETTINGS,
    // icon                          label
    {
      {ICON_EEPROM_SAVE,             LABEL_SAVE},
      {ICON_EEPROM_RESTORE,          LABEL_RESTORE},
      {ICON_EEPROM_RESET,            LABEL_RESET},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&eepromSettingsItems);

  while (infoMenu.menu[infoMenu.cur] == menuEepromSettings)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:
        // save to EEPROM
        if (infoMachineSettings.EEPROM == 1)
        {
          setDialogText(eepromSettingsItems.title.index, LABEL_EEPROM_SAVE_INFO, LABEL_CONFIRM, LABEL_CANCEL);
          showDialog(DIALOG_TYPE_QUESTION, saveEepromSettings, NULL, NULL);
        }
        break;

      case KEY_ICON_1:
        // restore from EEPROM
        if (infoMachineSettings.EEPROM == 1)
        {
          setDialogText(eepromSettingsItems.title.index, LABEL_EEPROM_RESTORE_INFO, LABEL_CONFIRM, LABEL_CANCEL);
          showDialog(DIALOG_TYPE_QUESTION, restoreEepromSettings, NULL, NULL);
        }
        break;

      case KEY_ICON_2:
        // reset EEPROM
        if (infoMachineSettings.EEPROM == 1)
        {
          setDialogText(eepromSettingsItems.title.index, LABEL_EEPROM_RESET_INFO, LABEL_CONFIRM, LABEL_CANCEL);
          showDialog(DIALOG_TYPE_QUESTION, resetEepromSettings, NULL, NULL);
        }
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

#endif  // QUICK_EEPROM_BUTTON

void menuMachineSettings(void)
{
  // 1 title, ITEM_PER_PAGE items (icon + label)
  MENUITEMS machineSettingsItems = {
    // title
    LABEL_MACHINE_SETTINGS,
    // icon                          label
    {
      {ICON_PARAMETER,               LABEL_PARAMETER_SETTING},
      {ICON_GCODE,                   LABEL_TERMINAL},
      {ICON_CUSTOM,                  LABEL_CUSTOM},
      {ICON_RGB_SETTINGS,            LABEL_RGB_SETTINGS},
      {ICON_TUNING,                  LABEL_TUNING},
      #ifdef QUICK_EEPROM_BUTTON
        {ICON_EEPROM_SAVE,             LABEL_EEPROM_SETTINGS},
      #else
        {ICON_BACKGROUND,              LABEL_BACKGROUND},
      #endif
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;
  //const ITEM itemCaseLight = {ICON_CASE_LIGHT, LABEL_CASE_LIGHT};   //TG 7/22/22 removed

  //if (infoMachineSettings.caseLightsBrightness == ENABLED)          //TG 7/22/22 removed
  //  machineSettingsItems.items[KEY_ICON_6] = itemCaseLight;         //TG 7/22/22 removed

  menuDrawPage(&machineSettingsItems);

  while (infoMenu.menu[infoMenu.cur] == menuMachineSettings)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.cur] = menuParameterSettings;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuTerminal;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.cur] = menuCustom;
        break;

      case KEY_ICON_3:
        infoMenu.menu[++infoMenu.cur] = menuRGBSettings;
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuTuning;
        break;

      #ifdef QUICK_EEPROM_BUTTON
        case KEY_ICON_5:
          infoMenu.menu[++infoMenu.cur] = menuEepromSettings;
          break;
      #endif

      case KEY_ICON_6:
        //if (infoMachineSettings.caseLightsBrightness == ENABLED)    //TG 7/22/22 removed
        //  infoMenu.menu[++infoMenu.cur] = menuCaseLight;            //TG 7/22/22 removed
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
