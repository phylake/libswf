//
//  control.cpp
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "control.h"

//-----------------------------------------
//             TagNameBase
//-----------------------------------------
void swf::TagNameBase::tagsFromSWF(buf_type *& buf) {
    numSymbols.fromSWF(buf);
    int i;
    
    i = (int)numSymbols.value();
    U16 * tag;
    String * name;
    
    while( i-- ) {
        tag  = new U16;
        name = new String;
        
        tag  -> fromSWF(buf);
        name -> fromSWF(buf);
        
        tags.push_back(tag);
        names.push_back(name);
    }
    
#ifdef DEBUG
    for (i = 0; i < tags.size(); i++) {
        tag = tags[i];
        name = names[i];
        
        printf("\t%i ", (int)tag->value());
        printf(name->value_.data());
        printf("\n");
    }
#endif
}
