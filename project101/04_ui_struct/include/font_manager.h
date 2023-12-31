#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include <sys/time.h>
#include <common.h>

#ifndef NULL
#define NULL (void *)0
#endif 

typedef struct FontBitMap {
    Region tRegion;
    int iCurOriginX;
    int iCurOriginY;
    int iNextOriginX;
    int iNextOriginY;
	unsigned char *pucBuffer;
}FontBitMap, *PFontBitMap;

typedef struct FontOpr {
    char *name;
	int (*FontInit)(char *aFineName);
    int (*SetFontSize)(int iFontSize);
    int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);
    struct FontOpr *ptNext;
}FontOpr, *PFontOpr;

void RegisterFont(PFontOpr ptFontOpr);

void FontsRegister(void);
int SelectAndInitFont(char *aFontOprName, char *aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);

#endif 

