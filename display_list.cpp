//
//  display_list.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "display_list.h"

//-----------------------------------------
//              ClipEventFlags
//-----------------------------------------
swf::ClipEventFlags::ClipEventFlags(Version & version) : AbstractVData(version) {}

void swf::ClipEventFlags::fromSWF( buf_type *& buf ) {
    int i = 0;
    key_up_          = getUBits(buf, 1, i++);
    key_down_        = getUBits(buf, 1, i++);
    mouse_up_        = getUBits(buf, 1, i++);
    mouse_down_      = getUBits(buf, 1, i++);
    mouse_move_      = getUBits(buf, 1, i++);
    unload_          = getUBits(buf, 1, i++);
    enter_frame_     = getUBits(buf, 1, i++);
    load_            = getUBits(buf, 1, i++);
    drag_over_       = getUBits(buf, 1, i++);
    roll_out_        = getUBits(buf, 1, i++);
    roll_over_       = getUBits(buf, 1, i++);
    release_outside_ = getUBits(buf, 1, i++);
    release_         = getUBits(buf, 1, i++);
    press_           = getUBits(buf, 1, i++);
    initialize_      = getUBits(buf, 1, i++);
    data_            = getUBits(buf, 1, i++);
    
    if( version_.version_num() >= 6 )
    {
        i += 5;//reserved UB[5]
        construct_   = getUBits(buf, 1, i++);
        key_press_   = getUBits(buf, 1, i++);
        drag_out_    = getUBits(buf, 1, i++);
        i += 8;//reserved UB[8]
    }
    
    buf += (unsigned) ceil(i / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              ClipActionRecord
//-----------------------------------------
swf::ClipActionRecord::ClipActionRecord(Version & version) : AbstractVData(version), event_flags_(version) {}

void swf::ClipActionRecord::fromSWF( buf_type *& buf ) {
    event_flags_.fromSWF(buf);
    size_.fromSWF(buf);
    
    buf += size_.value();
    //key_code_.fromSWF(buf);
    //TODO actions vector
}

//-----------------------------------------
//              ClipActions
//-----------------------------------------
swf::ClipActions::ClipActions(Version & version) : AbstractVData(version), all_event_flags_(version) {}

bool swf::ClipActions::isEnd( buf_type * buf ) {
    if( version_.version_num() == 5 )
    {
        return end_flag5_.readAhead(buf) == 0;
    }
    else if( version_.version_num() == 6 )
    {
        return end_flag6_.readAhead(buf) == 0;
    }
    
    return true;
}

void swf::ClipActions::fromSWF( buf_type *& buf ) {
    reserved_.fromSWF(buf);
    all_event_flags_.fromSWF(buf);
    
    
    ClipActionRecord * car;
    do {
        car = new ClipActionRecord(version_);
        car -> fromSWF(buf);
        records_.push_back(car);
    } while ( !isEnd(buf) );
    
    
    if( version_.version_num() == 5 )
    {
        end_flag5_.fromSWF(buf);
    }
    else if( version_.version_num() == 6 )
    {
        end_flag6_.fromSWF(buf);
    }
    else
    {
        //throw an error
    }
}

//-----------------------------------------
//              ColorMatrixFilter
//-----------------------------------------
swf::ColorMatrixFilter::ColorMatrixFilter() : matrix_(20) {
}

void swf::ColorMatrixFilter::fromSWF( buf_type *& buf ) {
    U32 * t;
    int i = (int)matrix_.size();
    while( i-- ) {
        t = new U32;
        t -> fromSWF(buf);
        matrix_.push_back(t);
    }
}

//-----------------------------------------
//              ConvolutionFilter
//-----------------------------------------
swf::ConvolutionFilter::ConvolutionFilter() : color_(RGB::TYPE_RGBA) {}

void swf::ConvolutionFilter::fromSWF( buf_type *& buf ) {
    matrix_x_.fromSWF(buf);
    matrix_y_.fromSWF(buf);
    divisor_.fromSWF(buf);
    bias_.fromSWF(buf);
    
    U32 * t;
    int i = matrix_x_.value() * matrix_y_.value();
    while( i-- ) {
        t = new U32;
        t -> fromSWF(buf);
        matrix_.push_back(t);
    }
    
    color_.fromSWF(buf);
    
    clamp_          = getUBits(buf, 1, 6);
    preserve_alpha_ = getUBits(buf, 1, 7);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              BlurFilter
//-----------------------------------------
void swf::BlurFilter::fromSWF( buf_type *& buf ) {
    blur_x_.fromSWF(buf);
    blur_y_.fromSWF(buf);
    
    passes_ = getUBits(buf, 5);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              GlowFilter
//-----------------------------------------
swf::GlowFilter::GlowFilter() : color(RGB::TYPE_RGBA) {}

void swf::GlowFilter::fromSWF( buf_type *& buf ) {
    color.fromSWF(buf);
    blur_x_.fromSWF(buf);
    blur_y_.fromSWF(buf);
    strength_.fromSWF(buf);
    
    int i = 0;
    inner_shadow_     = getUBits(buf, 1, i++);
    knockout_         = getUBits(buf, 1, i++);
    composite_source_ = getUBits(buf, 1, i++);
    passes_           = getUBits(buf, 5, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              DropshadowFilter
//-----------------------------------------
swf::DropshadowFilter::DropshadowFilter() : color(RGB::TYPE_RGBA) {}

void swf::DropshadowFilter::fromSWF( buf_type *& buf ) {
    color.fromSWF(buf);
    blur_x_.fromSWF(buf);
    blur_y_.fromSWF(buf);
    angle_.fromSWF(buf);
    distance_.fromSWF(buf);
    strength_.fromSWF(buf);
    
    int i = 0;
    inner_shadow_     = getUBits(buf, 1, i++);
    knockout_         = getUBits(buf, 1, i++);
    composite_source_ = getUBits(buf, 1, i++);
    passes_           = getUBits(buf, 5, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              BevelFilter
//-----------------------------------------
swf::BevelFilter::BevelFilter() : shadowColor(RGB::TYPE_RGBA), highlightColor(RGB::TYPE_RGBA) {}

void swf::BevelFilter::fromSWF( buf_type *& buf ) {
    shadowColor.fromSWF(buf);
    highlightColor.fromSWF(buf);
    blur_x_.fromSWF(buf);
    blur_y_.fromSWF(buf);
    angle_.fromSWF(buf);
    distance_.fromSWF(buf);
    strength_.fromSWF(buf);
    
    int i = 0;
    inner_shadow_     = getUBits(buf, 1, i++);
    knockout_         = getUBits(buf, 1, i++);
    composite_source_ = getUBits(buf, 1, i++);
    on_top_           = getUBits(buf, 1, i++);
    passes_           = getUBits(buf, 4, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              GradientGlowFilter
//-----------------------------------------
void swf::GradientGlowFilter::fromSWF( buf_type *& buf ) {
    num_colors_.fromSWF(buf);
    
    int i;
    RGB * tRGB;
    U8 * tU8;
    
    i = num_colors_.value();
    while( i-- ) {
        tRGB = new RGB(RGB::TYPE_RGBA);
        tRGB -> fromSWF(buf);
        colors_.push_back(tRGB);
    }
    
    i = num_colors_.value();
    while( i-- ) {
        tU8 = new U8;
        tU8 -> fromSWF(buf);
        ratios.push_back(tU8);
    }
}

//-----------------------------------------
//              GradientBevelFilter
//-----------------------------------------
/* identical to GradientGlowFilter. no override */
//void swf::GradientBevelFilter::fromSWF( buf_type *& buf ) {}

//-----------------------------------------
//              Filter
//-----------------------------------------
void swf::Filter::fromSWF( buf_type *& buf ) {
    filter_id_.fromSWF(buf);
    
    switch ((unsigned short)filter_id_.value()) {
        case FILTER_DROPSHADOW:
            concrete_filter_ = new DropshadowFilter;
            break;
        case FILTER_BLUR:
            concrete_filter_ = new BlurFilter;
            break;
        case FILTER_GLOW:
            concrete_filter_ = new GlowFilter;
            break;
        case FILTER_BEVEL:
            concrete_filter_ = new BevelFilter;
            break;
        case FILTER_GRADIENTGLOW:
            concrete_filter_ = new GradientGlowFilter;
            break;
        case FILTER_CONVOLUTION:
            concrete_filter_ = new ConvolutionFilter;
            break;
        case FILTER_COLORMATRIX:
            concrete_filter_ = new ColorMatrixFilter;
            break;
        case FILTER_GRADIENTBEVEL:
            concrete_filter_ = new GradientBevelFilter;
            break;
    }
}

//-----------------------------------------
//              FilterList
//-----------------------------------------
void swf::FilterList::fromSWF( buf_type *& buf ) {
    numberOfFilters.fromSWF(buf);
    unsigned short i = (unsigned short)numberOfFilters.value();
    
    Filter * ptr;
    while ( i-- ) {
        ptr = new Filter;
        ptr -> fromSWF(buf);
        filters.push_back(ptr);
    }
}
