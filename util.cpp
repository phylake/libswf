//
//  util.cpp
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "util.h"

unsigned int swf::getUBits(buf_type * buf, unsigned int n, unsigned int startAt) {
    unsigned int ret = 0;
    unsigned int i = 0;
    unsigned int pos;
    unsigned int t;
    unsigned int scale;
    
    //pointer adjustment
    buf += (unsigned int) floor(startAt / 8);
    
    //scaling
    startAt = startAt % 8;
    n = n % 32;
    
    do {
        scale = (i + startAt) % 8;
        
        /* shift sizeof(char) * 8 - 1 scaled by i plus a starting position */
        pos = 8 - 1 - scale;
        if( i > 0 && scale == 0 )
        {
            buf++;
        }
        
        t = *buf >> pos;
        
#ifdef LIBSWF_DEBUG
        //printf("%i", t & 1);
#endif
        
        ret <<= 1;
        ret |= t & 1;
    } while (++i < n);
    
#ifdef LIBSWF_DEBUG
    //printf("\n");
#endif
    
    return ret;
}

unsigned int swf::getUBits(char * buf, unsigned int n, unsigned int startAt) {
    return getUBits((buf_type *)buf, n, startAt);
}

signed int swf::getSBits(buf_type * buf, unsigned int n, unsigned int startAt) {
    unsigned int ret = getUBits(buf, n, startAt);
    
    if ( ret & (1<<(n-1)) )
    {//msb is 1
        ret |= ((~0) << n);
    }
    
    return ret;
}

signed int swf::getSBits(char * buf, unsigned int n, unsigned int startAt) {
    return getSBits((buf_type *)buf, n, startAt);
}
