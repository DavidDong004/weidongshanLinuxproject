#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <ui.h>
#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    int error;

    if (argc != 2)
	{
		printf("Usage: %s <font_file>\n", argv[0]);
		return -1;
	}

    /* Init Disp System */
    DisplayInit();

    SelectDefaultDisplay("fb");

    InitDefaultDisplay();

    /* Init Input System */
    InputInit();

	InputDeviceInit();

    /* Init Font System*/
	FontsRegister();

    error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

    /* Init Page System*/
    PagesRegister();


    /* Run the main interface of the business system*/
	Page("main")->Run(NULL);
	
	return 0;	
}

