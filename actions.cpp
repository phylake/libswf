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
    actionCode.fromSWF(buf);
    if ( actionCode.getValue() >= 0x80 ) {
        length.fromSWF(buf);
        buf += length.getValue();
    }
}

unsigned swf::ActionRecord::getTotalLength() {
    return 1 + 2 + length.getValue();
}
