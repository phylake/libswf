//
//  shapes.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "shapes.h"

//-----------------------------------------
//               ShapeBase
//-----------------------------------------
swf::ShapeBase::ShapeBase(Version & version, short shape_context) : AbstractVData(version) {}

int swf::ShapeBase::set_shape_context(short value) {
    if( !(1 >= value && value <= 3) ) return -1;
    
    shape_context_ = value;
    return 0;
}

//-----------------------------------------
//                  LineStyle
//-----------------------------------------
swf::LineStyle::LineStyle(Version & version, short shape_context) :
    ShapeBase(version, shape_context),
    color3_(swf::RGB::TYPE_RGBA),
    color12_(swf::RGB::TYPE_RGB) {}

void swf::LineStyle::fromSWF( buf_type *& buf ) {
    
}

//-----------------------------------------
//                  FillStyle
//-----------------------------------------
swf::FillStyle::FillStyle(Version & version, short shape_context) : LineStyle(version, shape_context) {}

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

int swf::FillStyle::set_shape_context(short value) {
    int ret = ShapeBase::set_shape_context(value);
    if( ret != 0 )
    {
        return ret;
    }
    
    shape_context_ = value;
    gradient_.set_shape_context(value);
    gradient_focal_.set_shape_context(value);
    
    return ret;
}

//-----------------------------------------
//                  LineStyle2
//-----------------------------------------
swf::LineStyle2::LineStyle2(Version & version, short shape_context) :
    LineStyle(version, shape_context),
    fill_type_(version, shape_context) {}

void swf::LineStyle2::fromSWF( buf_type *& buf ) {
    width_.fromSWF(buf);
    b0 = *buf;
    b1 = *(buf + 1);
    
    buf += 2;
    
    if( join_style() == kJoinStyleMiter )
    {
        miter_limit_factor_.fromSWF(buf);
    }
    
    if ( has_fill() )
    {
        fill_type_.fromSWF(buf);
    }
    else
    {
        color3_.fromSWF(buf);
    }
}

short swf::LineStyle2::start_cap_style() {
    return (b0 >> 6) & 0x3;
}

short swf::LineStyle2::join_style() {
    return (b0 >> 4) & 0x3;
}

short swf::LineStyle2::endcap_style() {
    return b1 & 0x3;
}

bool swf::LineStyle2::has_fill() {
    return b0 & 0x8;
}

bool swf::LineStyle2::no_h_scale() {
    return b0 & 0x4;
}

bool swf::LineStyle2::no_v_scale() {
    return b0 & 0x2;
}

bool swf::LineStyle2::pixel_hinting() {
    return b0 & 0x1;
}

bool swf::LineStyle2::no_close() {
    return b1 & 0x4;
}

//-----------------------------------------
//              FillStyleArray
//-----------------------------------------
swf::FillStyleArray::FillStyleArray(Version & version, short shape_context) : ShapeBase(version, shape_context) {}

void swf::FillStyleArray::fromSWF( buf_type *& buf ) {
    count_.fromSWF(buf);
    
    short actual_count = (short) count_.value();
    if( count_.value() == 0xff )
    {
        count_ext_.fromSWF(buf);
        actual_count = (short) count_ext_.value();
    }
    
    FillStyle * fs;
    while( actual_count-- )
    {
        fs = new FillStyle(version_, shape_context_);
        fs -> fromSWF(buf);
        styles_.push_back(fs);
    }
}

//-----------------------------------------
//              LineStyleArray
//-----------------------------------------
swf::LineStyleArray::LineStyleArray(Version & version, short shape_context) : ShapeBase(version, shape_context) {}

void swf::LineStyleArray::fromSWF( buf_type *& buf ) {
    count_.fromSWF(buf);
    
    short actual_count = (short) count_.value();
    if( count_.value() == 0xff )
    {
        count_ext_.fromSWF(buf);
        actual_count = (short) count_ext_.value();
    }
    
    LineStyle * ls;
    while( actual_count-- )
    {
        switch (shape_context_) {
            case 4:
                ls = new LineStyle2(version_, shape_context_);
                break;
            case 3:
            case 2:
            case 1:
                ls = new LineStyle(version_, shape_context_);
            default:
                break;
        }
        ls -> fromSWF(buf);
        styles_.push_back(ls);
    }
}

//-----------------------------------------
//                 Shape
//-----------------------------------------
swf::Shape::Shape(Version & version, short shape_context) : ShapeBase(version, shape_context) {}

void swf::Shape::fromSWF( buf_type *& buf ) {
    
}




