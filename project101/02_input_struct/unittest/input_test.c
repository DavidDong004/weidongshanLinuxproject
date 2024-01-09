#include <asm-generic/errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <input_manager.h>

int main(int argc, char **argv)
{
	int ret;
	InputEvent tEvent;

	InputInit();
	InputDeviceInit();

	while(1)
	{	
		printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		ret = GetInputEvent(&tEvent);
        if (ret != 0)
        {
			printf("%s %s %d, ret = %d\n", __FILE__, __FUNCTION__, __LINE__, ret);
            printf("GetInputEvent err\n");
            return -1;
        }
        else
        {	
			printf("%s %s %d, event.iType = %d\n", __FILE__, __FUNCTION__, __LINE__, tEvent.iType );
			if(tEvent.iType == INPUT_TYPE_TOUCH)
			{
				printf("Type      : %d\n", tEvent.iType);
				printf("iX        : %d\n", tEvent.iX);
				printf("iY        : %d\n", tEvent.iY);
				printf("iPressure : %d\n", tEvent.iPressure);
			}
			else if(tEvent.iType == INPUT_TYPE_NET)
			{
				printf("Type ; %d\n", tEvent.iType);
            	printf("str ; %s\n", tEvent.str);

			}
        }
	}
	
	return 0;	
}

