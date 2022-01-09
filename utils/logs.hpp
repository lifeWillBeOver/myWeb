#pragma once

#include <stdio.h>


static FILE *stream = NULL;


#ifdef __DEBUG__
    stream = stdout;
#else
    stream = fopen("./log/server.log", "a+");
#endif
