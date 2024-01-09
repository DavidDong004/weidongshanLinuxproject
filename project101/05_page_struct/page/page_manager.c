
#include <page_manager.h>
#include <common.h>
#include <string.h>


static PPageAction g_ptPages = NULL;

void PageRegister(PPageAction ptPageAction)
{
    ptPageAction->ptNext = g_ptPages;
    g_ptPages = ptPageAction;
}

PPageAction Page(char *name)
{
    PPageAction ptTemp = g_ptPages;

    while(ptTemp)
    {
        if(strcmp(name, ptTemp->name) == 0)
        {
            return ptTemp;
        }
        ptTemp = ptTemp->ptNext;
    }

    return NULL;
}

void PagesRegister(void)
{
    extern void MainPageRegister(void);
    MainPageRegister();
}