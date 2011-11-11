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
swf::GradRecord::GradRecord() : color3_(swf::RGB::TYPE_RGBA), color12_(swf::RGB::TYPE_RGB) {}

void swf::GradRecord::fromSWF( buf_type *& buf ) {
    if( shape_context_ == 0 ) {
        //throw something
    }
    
    ratio_.fromSWF(buf);
    
    switch (shape_context_) {
        case 3:
            color3_.fromSWF(buf);
            break;
        case 2:
        case 1:
            color12_.fromSWF(buf);
            break;
        default:
            break;
    }
}

void swf::GradRecord::set_shape_context(short value) {
    if( !(value >= 1 && 3 <= value) ) return;
    
    shape_context_ = value;
}

//-----------------------------------------
//                  Gradient
//-----------------------------------------
void swf::Gradient::fromSWF( buf_type *& buf ) {
    spread_mode_        = getUBits(buf, 2, 0);
    interpolation_mode_ = getUBits(buf, 2, 2);
    num_gradients_      = getUBits(buf, 4, 4);
    
    int i = (int) num_gradients_;
    GradRecord * t;
    
    while( i-- ) {
        t = new GradRecord();
        t -> set_shape_context(shape_context_);
        t -> fromSWF(buf);
        records_.push_back(t);
    }
}

void swf::Gradient::set_shape_context(short value) {
    if( !(value >= 1 && 3 <= value) ) return;
    
    shape_context_ = value;
    for (int i = 0; i < records_.size(); i++)
    {
        records_[i] -> set_shape_context(value);
    }
}

//-----------------------------------------
//             FocalGradient
//-----------------------------------------
void swf::FocalGradient::fromSWF( buf_type *& buf ) {
    Gradient::fromSWF(buf);
    focal_point_.fromSWF(buf);
}

double swf::FocalGradient::focal_point() {
    return focal_point_.toFixed8();
}
