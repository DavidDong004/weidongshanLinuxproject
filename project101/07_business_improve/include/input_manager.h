#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>
#include <common.h>

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2


typedef struct InputEvent {
	struct timeval	tTime;
    int iType;
    int iX;
    int iY;
    int iPressure;
    char str[1024];
}InputEvent, *PInputEvent;

typedef struct InputDevice {
    char *name;
    int (*GetInputEvent)(PInputEvent ptInputEvent);
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    struct InputDevice *ptNext;
}InputDevice, *PInputDevice;

void RegisterInputDevice(PInputDevice ptInputDev);
void InputSystemRegister(void);
void InputDeviceInit(void);
int GetInputEvent(PInputEvent ptInputEvent);
#endif 

