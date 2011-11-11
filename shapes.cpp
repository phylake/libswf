//
//  shapes.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "shapes.h"

//-----------------------------------------
//                  FillStyle
//-----------------------------------------
swf::FillStyle::FillStyle(Version & version, short shape_context) :
    AbstractVData(version),
    shape_context_(shape_context),
    color3_(swf::RGB::TYPE_RGBA),
    color12_(swf::RGB::TYPE_RGB) {}

void swf::FillStyle::fromSWF( buf_type *& buf ) {
    fill_style_type_.fromSWF(buf);
    
    switch (fill_style_type_.value()) {
        case SOLID:
            switch (shape_context_) {
                case 3:
                    color3_.fromSWF(buf);
                    break;
                case 2:
                case 1:
                    color12_.fromSWF(buf);
                default:
                    break;
            }
            break;
        case LINEAR_GRADIENT:
        case RADIAL_GRADIENT:
            gradient_matrix_.fromSWF(buf);
            gradient_.fromSWF(buf);
            break;
        case FOCAL_RADIAL_GRADIENT:
            gradient_matrix_.fromSWF(buf);
            gradient_focal_.fromSWF(buf);
            break;
        case REPEATING_BITMAP:
        case CLIPPED_BITMAP:
        case NON_SMOOTH_REPEATING_BITMAP:
        case NON_SMOOTH_CLIPPED_BITMAP:
            bitmap_id_.fromSWF(buf);
            bitmap_matrix_.fromSWF(buf);
        default:
            break;
    }
}

void swf::FillStyle::set_shape_context(short value) {
    if( !(value >= 1 && 3 <= value) ) return;
    
    shape_context_ = value;
    gradient_.set_shape_context(value);
    gradient_focal_.set_shape_context(value);
}

//-----------------------------------------
//              FillStyleArray
//-----------------------------------------
void swf::FillStyleArray::fromSWF( buf_type *& buf ) {
    count_.fromSWF(buf);
    
    if( count_.value() == 0xff )
    {
        count_ext_.fromSWF(buf);
    }
    
    //TODO styles_
}

