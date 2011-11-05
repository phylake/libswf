//
//  util.h
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_util_h
#define libswf_util_h

#include "libswf.h"
#include "math.h"
#include <stdio.h>

namespace swf
{
    unsigned getUBits(buf_type * ptr, unsigned int n, unsigned int startAt = 0);
    unsigned getUBits(    char * ptr, unsigned int n, unsigned int startAt = 0);
      signed getSBits(buf_type * ptr, unsigned int n, unsigned int startAt = 0);
      signed getSBits(    char * ptr, unsigned int n, unsigned int startAt = 0);
}

#endif
