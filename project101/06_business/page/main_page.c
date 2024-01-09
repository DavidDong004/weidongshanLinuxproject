#include <common.h>
#include <asm-generic/errno.h>
#include <stdio.h>
#include <math.h>

#include <string.h>
#include <ui.h>
#include <page_manager.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <config.h>

#define X_GAP 5
#define Y_GAP 5
#define TouchscreenSensitivity 15
 
static Button g_tButtons[ITEMCFG_MAX_NUM];
static int g_tButtonCnt;

static int MainPageOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
    int error;
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;
    char name[100];
    char status[100];
    char *strButton;
    strButton = ptButton->name;

    /* 1. For Touchscreen event */
    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        /* 1.1 Determine if it can be clicked */
        if (GetItemCfgByName(ptButton->name)->bCanBeTouched == 0)
        {
            return -1;
        }
        /* 1.2 Change color */
        if (ptInputEvent->iPressure >= TouchscreenSensitivity)
        {
            printf("ptInputEvent->iPressure is %d\n", ptInputEvent->iPressure);
            ptButton->status = !ptButton->status;
        }
        if (ptButton->status)
        {
            dwColor = BUTTON_PRESSED_COLOR;
        }
    }
    /* 2. For Net event */
    else if(ptInputEvent->iType == INPUT_TYPE_NET)
    {
        /* 2.1 Change color based on the string the Net passed in: wifi ok, wifi err, burn 70% */
        sscanf(ptInputEvent->str, "%s %s", name, status);
        if(strcmp(status, "ok") == 0)
        {
            dwColor = BUTTON_PRESSED_COLOR;
        }
        else if (strcmp(status, "err") == 0)
        {
            dwColor = BUTTON_DEFAULT_COLOR;
        }
        else if (status[0] >= '0' && status[0] <= '9')
        {
            dwColor = BUTTON_PERCENT_COLOR;
            strButton = status;
        }
        else
        {
            return -1;
        }


    }
    else
    {
        return -1;
    }

    /* Draw the base color */
    error = DrawRegion(&(ptButton->tRegion), dwColor);
    
    /*Write and center the text*/
    error = DrawTextInRegionCentral(strButton, &(ptButton->tRegion), BUTTON_TEXT_COLOR);

    /* flush to lcd/web */
    error = FlushDisplayRegion(&(ptButton->tRegion), ptDispBuff);

    return 0;
}

static int GenerateButtons()
{
    int width, height;
    int n_per_line;
    int row, rows;
    int col;
    int n;
    PDispBuff pDispBuff;
    int xres, yres;
    int start_x, start_y;
    int pre_start_x, pre_start_y;
    PButton pButton;
    int i = 0;

    /* Calculate the single button's width/height */
    g_tButtonCnt = n = GetItemCfgCount();

    pDispBuff = GetDispalyBuffer();
    xres      = pDispBuff->iXres;
    yres      = pDispBuff->iYres;

    width      = sqrt(1.0/0.618 * xres * yres / n);
    n_per_line = xres / width + 1;
    width      = xres / n_per_line;
    height     = 0.618 * width;

    /* Centering: calculating the Region of each button */
    start_x = (xres - width * n_per_line) / 2;
    rows    = n / n_per_line; //int may cause problem
    if (rows * n_per_line < n) 
    {
       rows++; 
    }
    start_y = (yres - rows * height)/2;

    /* Calculate the Region of each button */
    for (row = 0; (row < rows) && (i < n); row++)
    {
        pre_start_y = start_y + row * height;
        pre_start_x = start_x - width;
        for (col = 0; (col < n_per_line) && (i < n); col++)
        {

            pButton = &g_tButtons[i];
            pButton->tRegion.iLeftUpX = pre_start_x + width;
            pButton->tRegion.iLeftUpY = pre_start_y;
            pButton->tRegion.iWidth   = width - X_GAP;
            pButton->tRegion.iHeigh   = height - Y_GAP;
            pre_start_x = pButton->tRegion.iLeftUpX;

            /* InitButton */
            InitButton(pButton, GetItemCfgByIndex(i)->name, NULL, NULL, MainPageOnPressed); 
            i++;

        }
    }

    /* OnDraw */
    for (i = 0; i < n; i++)
    {
        g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);
    }
    return 1;
}

static int isTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
    if (iX < ptRegion->iLeftUpX || iX >= (ptRegion->iLeftUpX + ptRegion->iWidth))
    {
        return 0;
    }
    if (iY < ptRegion->iLeftUpY || iY >= (ptRegion->iLeftUpY + ptRegion->iHeigh))
    {
        return 0;
    }
    return 1;

}

static PButton GetButtonByName(char *name)
{
    int i;
    for (i = 0; i< g_tButtonCnt; i++)
    {
        if (strcmp(name, g_tButtons[i].name) == 0)
        {
            return &g_tButtons[i];
        }
    }

    return NULL;

}

static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
    int i;
    char name[100];

    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        for (i = 0; i< g_tButtonCnt; i++)
        {
            if (isTouchPointInRegion(ptInputEvent->iX, ptInputEvent->iY, &(g_tButtons[i].tRegion)))
            {
                return &g_tButtons[i];
            }
        }
    }
    else if(ptInputEvent->iType == INPUT_TYPE_NET)
    {
        sscanf(ptInputEvent->str, "%s", name);
        return GetButtonByName(name);

    }
    else
    {
        return NULL;
    }
    return NULL;
}

static void MainPageRun(void *pParams)
{
    // printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    int error;
    InputEvent tInputEvent;
    PButton ptButton;
    PDispBuff ptDispBuff = GetDispalyBuffer();

    /* Read config file */
    error = ParseConfigFile();
    if (error)
    {
        return;
    }

    /* Generate buttons and interfaces based on config file */
    GenerateButtons();

    while(1)
    {
        /* read input event*/
        error =  GetInputEvent(&tInputEvent);
        if (error)
        {
            printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        }

        /* Find the button base on the input event */
        ptButton = GetButtonByInputEvent(&tInputEvent);
        if (!ptButton)
        {
            printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
            continue;
        }
        ptButton->OnPressed(ptButton, ptDispBuff, &tInputEvent);


        /* Call the button's Onpressed function */
    }
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run  = MainPageRun,
};

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}