#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <disp_manager.h>
#include <font_manager.h>
#include <ui.h>
#include <stdlib.h>

#define FONTDATAMAX 4096

int main(int argc, char **argv)
{
    int error;
    PDispBuff ptBuffer;

	FontBitMap tFontBitMap;
	Region tRegion;
	Button tButton; 
	

	if (argc != 4)
	{
		printf("Usage: %s <font_file> <lcd_x> <lcd_y>\n", argv[0]);
		return -1;
	}

	tRegion.iLeftUpX = strtol(argv[2], NULL, 0);
	tRegion.iLeftUpY = strtol(argv[3], NULL, 0);
	tRegion.iWidth = 300;
	tRegion.iHeigh = 100;


    DisplayInit();

    SelectDefaultDisplay("fb");

    InitDefaultDisplay();

	ptBuffer = GetDispalyBuffer();

	FontsRegister();

	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}
	
	InitButton(&tButton, "test", &tRegion, NULL, NULL);
	tButton.OnDraw(&tButton, ptBuffer);
	while(1)
	{
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);
	}
	
	
	return 0;	
}

