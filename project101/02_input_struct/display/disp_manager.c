#include <disp_manager.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

/*manage low-level LCD and WEB*/
static PDispOpr g_DispDevs = NULL;
static PDispOpr g_DispDefault = NULL;
static DispBuff g_tDispBuff;
static int line_width;
static int pixel_width;

int PutPixel(int x, int y, unsigned int dwColor)
{
	unsigned char *pen_8 = (unsigned char *)(g_tDispBuff.buff+y*line_width+x*pixel_width);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_tDispBuff.iBpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_tDispBuff.iBpp);
			return -1;
			break;
		}
	}
	return 0;
}

void RegisterDisplay(PDispOpr ptDispOpr)
{
    ptDispOpr->ptNext = g_DispDevs;
    g_DispDevs = ptDispOpr;
}

int SelectDefaultDisplay(char *name)
{
    PDispOpr pTmp = g_DispDevs;
    while (pTmp)
    {
        if (strcmp(name, pTmp->name) == 0)
        {
            g_DispDefault = pTmp;
            return 0;
        }

        pTmp = pTmp->ptNext;
    }

    return -1;
}

int InitDefaultDisplay(void)
{
    int ret;

    ret = g_DispDefault->DeviceInit();
    if(ret)
    {
        printf("DeviceInit err\n");
        return -1;
    }


    g_DispDefault->GetBuffer(&g_tDispBuff);
    if(ret)
    {
        printf("GetBuffer err\n");
        return -1;
    }

    line_width = g_tDispBuff.iXres * g_tDispBuff.iBpp/8;
    pixel_width = g_tDispBuff.iBpp/8;
    return 0;
}

int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptPDispBuff)
{
    return g_DispDefault->FlushRegion(ptRegion, ptPDispBuff);
    
}
PDispBuff GetDispalyBuffer(void)
{
	return &g_tDispBuff;
}

void DisplayInit(void)
{
	extern void FramebufferInit(void);
    FramebufferInit();
    //WebInit();
}

