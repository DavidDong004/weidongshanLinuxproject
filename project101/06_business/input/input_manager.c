#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>


#include <input_manager.h>
static PInputDevice g_InputDevs = NULL;

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;


/* start of circle buffer */
#define BUFFER_LEN 20
static int g_iRead  = 0;
static int g_iWrite = 0;
static InputEvent g_atInputEvents[BUFFER_LEN];

static int isInputBufferFull(void)
{
    return (g_iRead == ((g_iWrite + 1) % BUFFER_LEN));
}

static int isInputBufferEmpty(void)
{
    return (g_iRead == g_iWrite);
}

static int PutInputEventToBuffer(PInputEvent ptInputEvent) 
{
    if (!isInputBufferFull())
    {
        g_atInputEvents[g_iWrite] = *ptInputEvent;
        g_iWrite = (g_iWrite + 1) % BUFFER_LEN;
        return 0;
    }
    else
    {
        return -1;
    }
}

static int GetInputEventFromBuffer(PInputEvent ptInputEvent) 
{
    if (!isInputBufferEmpty())
    {
        *ptInputEvent = g_atInputEvents[g_iRead];
        g_iRead = (g_iRead + 1) % BUFFER_LEN;
        return 0;
    }
    else
    {
        return -1;
    }
}


/* end of circle buffer*/

void RegisterInputDevice(PInputDevice ptInputDev)
{
    ptInputDev->ptNext = g_InputDevs;
    g_InputDevs = ptInputDev;
}



void InputSystemInit(void) 
{
    /* regiseter touchscreen */
    extern void TouchscreenRegister(void); 
    TouchscreenRegister();

    /* regiseter netinput */
    extern void NetInputRegister(void); 
    NetInputRegister();
}

static void *input_recv_thread_func (void *data)
{
    PInputDevice ptInputDev = (PInputDevice)data;
    InputEvent tEvent;
    int ret;
    int ret_buffer;
	while (1)
	{   
        /* read data */
        ret = ptInputDev->GetInputEvent(&tEvent);
        
        if (!ret)
        {
            /* save data */
            pthread_mutex_lock(&g_tMutex);
            ret_buffer = PutInputEventToBuffer(&tEvent);

            /* wake up thread which is waiting for data */
            pthread_cond_signal(&g_tConVar);
            pthread_mutex_unlock(&g_tMutex);	

        }
	}

	return NULL;
}

void InputDeviceInit(void)
{
    int ret;
    pthread_t tid;

    /* for each input device, init, pthread_create*/
    PInputDevice ptTmp = g_InputDevs;
    while (ptTmp)
    {
        /* init device */
        ret = ptTmp->DeviceInit();
        /* pthread create */
        if (!ret) 
        {
            ret = pthread_create(&tid, NULL, input_recv_thread_func, ptTmp);
        }

        ptTmp = ptTmp->ptNext;
    }
    return;
}

int GetInputEvent(PInputEvent ptInputEvent)
{
    InputEvent tEvent;
    int ret;
    /* no data the sleep */
    pthread_mutex_lock(&g_tMutex);
    if(!GetInputEventFromBuffer(&tEvent))
    {
        *ptInputEvent = tEvent;
        pthread_mutex_unlock(&g_tMutex);
        return 0;
    }
    else
    {
        /* sleep to wait */
        pthread_cond_wait(&g_tConVar, &g_tMutex);
        if(!GetInputEventFromBuffer(&tEvent))
        {
            *ptInputEvent = tEvent;
            ret = 0;
        }
        else
        {
            ret = -1;
        }	
        pthread_mutex_unlock(&g_tMutex);
    }
    return ret;
    /* return data */
}