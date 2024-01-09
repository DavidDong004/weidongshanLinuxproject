
#include <config.h>
#include <stdio.h>
#include <string.h>

static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM]; //Subsequent needs to be replaced with a chained table

static int g_tItemCfgCount = 0;

int ParseConfigFile(void)
{
    FILE *fp;
    char buf[100];
    char *p;

    /* 1.open config file */
    fp = fopen(CFG_FILE, "r");
    if(!fp)
    {
        printf("can not open cfg file %s\n", CFG_FILE);
        return -1;
    }

    while(fgets(buf,100,fp))
    {
        /* 2.1 read each line */
        buf[99] = '\0';

        /* 2.2  eliminate spaces and TAB in the front*/
        p = buf;
        while (*p == ' ' || *p == '\t')
        {
            p++;            
        }

        /* 2.3 ignore the notes */
        if (*p == '#')
        {
            continue;
        }

        /* 2.4 deal the line */
        g_tItemCfgs[g_tItemCfgCount].command[0] = '\0';
        g_tItemCfgs[g_tItemCfgCount].index = g_tItemCfgCount;
        sscanf(p, "%s %d %s", g_tItemCfgs[g_tItemCfgCount].name, &g_tItemCfgs[g_tItemCfgCount].bCanBeTouched, \
                              g_tItemCfgs[g_tItemCfgCount].command);
        g_tItemCfgCount++;
    }
    return 0;
}
int GetItemCfgCount(void)
{
    return g_tItemCfgCount;
}
PItemCfg GetItemCfgByIndex(int index)
{
    if (index < g_tItemCfgCount)
    {
        return &g_tItemCfgs[index];
    }
    else
    {
        return NULL;
    }
}
PItemCfg GetItemCfgByName(char *name)
{
    int i;
    for (i = 0; i < g_tItemCfgCount; i++)
    {
        if(strcmp(name, g_tItemCfgs[i].name) == 0)
        {
            return &g_tItemCfgs[i];
        }
    }
    return NULL;
}

