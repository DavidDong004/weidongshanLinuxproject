#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include <sys/time.h>
#include <common.h>

typedef struct PageAction{
    char *name;
    void (*Run)(void *pParams);
    struct PageAction *ptNext;
}PageAction, *PPageAction;


void PageRegister(PPageAction ptPageAction);
void PagesRegister(void);
PPageAction Page(char *name);

#endif





