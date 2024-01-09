

#include "font_manager.h"
#include <disp_manager.h>
#include <ui.h>
#include <stdio.h>


static int DefaultOnDraw(struct Button *ptButton, PDispBuff ptDispBuff)
{
    int error;
    
    /* Draw the base color */
    error = DrawRegion(&(ptButton->tRegion), BUTTON_DEFAULT_COLOR);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    /*Write and center the text*/
    SetFontSize(ptButton->iFontSize);
    error = DrawTextInRegionCentral(ptButton->name, &(ptButton->tRegion), BUTTON_TEXT_COLOR);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    /* flush to lcd/web */
    error = FlushDisplayRegion(&(ptButton->tRegion), ptDispBuff);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    return 0;

}

static int DefaultOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;
    int error;

    ptButton->status = !ptButton->status;
    if (ptButton->status)
    {
        dwColor = BUTTON_PRESSED_COLOR;
    }
    /* Draw the base color */
    error = DrawRegion(&(ptButton->tRegion), dwColor);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    /*Write and center the text*/
    error = DrawTextInRegionCentral(ptButton->name, &(ptButton->tRegion), BUTTON_TEXT_COLOR);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    /* flush to lcd/web */
    error = FlushDisplayRegion(&(ptButton->tRegion), ptDispBuff);
    if (error == -1)
    {
        printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    
    return 0;
}



void InitButton(PButton ptButton, char *name, PRegion ptRegion,ONDRAW_FUNC OnDraw,ONPRESSED_FUNC OnPressed)
{
    ptButton->status    = 0;
    ptButton->name      = name;
    if (ptRegion)
    {
        ptButton->tRegion   = *ptRegion;
    }
    ptButton->OnDraw    = OnDraw ? OnDraw : DefaultOnDraw;
    ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}