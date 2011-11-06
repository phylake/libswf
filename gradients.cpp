//
//  gradients.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "gradients.h"

//-----------------------------------------
//                  GradRecord
//-----------------------------------------
swf::GradRecord::GradRecord(int shape_num) : color3_(swf::RGB::TYPE_RGBA), color12_(swf::RGB::TYPE_RGB) {
    shape_ = -1;
    
    if( !(shape_num >= 1 && 3 <= shape_num) ) return;
    
    shape_ = shape_num;
}

void swf::GradRecord::fromSWF( buf_type *& buf ) {
    if( shape_ == -1 ) {
        //throw something
    }
    
    ratio_.fromSWF(buf);
    
    if( shape_ == 1 || shape_ == 2 )
    {
        color12_.fromSWF(buf);
    }
    else if( shape_ == 3 )
    {
        color3_.fromSWF(buf);
    }
}

//-----------------------------------------
//                  Gradient
//-----------------------------------------
swf::Gradient::Gradient(Version & version) : AbstractVData(version) {}

void swf::Gradient::fromSWF( buf_type *& buf ) {}