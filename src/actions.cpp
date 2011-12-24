//
//  actions.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "actions.h"

//-----------------------------------------
//              ActionRecord
//-----------------------------------------
void swf::ActionRecord::fromSWF( buf_type *& buf ) {
    action_code_.fromSWF(buf);
    if ( action_code_.value() >= 0x80 ) {
        length_.fromSWF(buf);
        buf += length_.value();
    }
}

unsigned swf::ActionRecord::getTotalLength() {
    return 1 + 2 + length_.value();
}
