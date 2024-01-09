#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <page_manager.h>
#include <stdlib.h>

#define FONTDATAMAX 4096

int main(int argc, char **argv)
{
    PagesRegister();
	Page("main")->Run(NULL);
	return 0;	
}

