#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include <common.h>
#include <font_manager.h>

typedef struct DispBuff {
    int iXres;
    int iYres;
    int iBpp;
    char *buff;
}DispBuff,*PDispBuff;


typedef struct DispOpr {
    char *name;
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    int (*GetBuffer)(PDispBuff ptDispBuff);
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptPDispBuff);
    struct DispOpr *ptNext;
}DispOpr,*PDispOpr;

void RegisterDisplay(PDispOpr ptDispOpr);

void DisplaySystemRegister(void);
int SelectDefaultDisplay(char *name);
int InitDefaultDisplay(void);

int PutPixel(int x, int y, unsigned int dwColor);
int DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor);
int DrawRegion(PRegion ptRegion, unsigned int dwColor);
int DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor);

int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptPDispBuff);

PDispBuff GetDispalyBuffer(void);

#endif 