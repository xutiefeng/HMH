#ifndef FUNCTAB_H
#define FUNCTAB_H

#include "globe.h"



#define LONG_TO_BIN(n)                       \
                     (                       \
                       ((n >> 21) & 0x80) |  \
                       ((n >> 18) & 0x40) |  \
                       ((n >> 15) & 0x20) |  \
                       ((n >> 12) & 0x10) |  \
                       ((n >> 9)  & 0x08) |  \
                       ((n >> 6)  & 0x04) |  \
                       ((n >> 3)  & 0x02) |  \
                       ((n )      & 0x01)    \
                     )

#define BIN(n) LONG_TO_BIN(0x##n##l)
#define B(n)   BIN(n)
#define b(n)   BIN(n)

#define  TAB_ISUM(x)              sizeof(x)//(sizeof(x)/sizeof(x[0]))

#endif


