#ifndef _INTERFACECMD_H_
#define _INTERFACECMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#define CMD_MAX_LIST 30   //TG 8/1/22 increased from 20
#define CMD_MAX_CHAR 100

typedef struct
{
  char gcode[CMD_MAX_CHAR];
  uint8_t src;   // 0: TouchScreen Cmd, 1: Serial Port 2 rx Cmd, 2: Serial Port 3 rx Cmd
} GCODE;

typedef struct
{
  GCODE   queue[CMD_MAX_LIST];
  uint8_t index_r; // Ring buffer read position
  uint8_t index_w; // Ring buffer write position
  uint8_t count;   // Count of commands in the queue
} GCODE_QUEUE;

extern GCODE_QUEUE infoCmd;
extern GCODE_QUEUE infoCacheCmd;

bool storeCmd(const char * format,...);
void mustStoreCmd(const char * format,...);
void mustStoreScript(const char * format,...);
bool storeCmdFromUART(uint8_t port, const char * gcode);
void mustStoreCacheCmd(const char * format,...);
bool moveCacheToCmd(void);
void clearCmdQueue(void);
void parseQueueCmd(void);
void sendQueueCmd(void);

#ifdef __cplusplus
}
#endif

#endif
