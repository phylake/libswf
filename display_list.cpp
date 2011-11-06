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
    keyUp          = getUBits(buf, 1, i++);
    keyDown        = getUBits(buf, 1, i++);
    mouseUp        = getUBits(buf, 1, i++);
    mouseDown      = getUBits(buf, 1, i++);
    mouseMove      = getUBits(buf, 1, i++);
    unload         = getUBits(buf, 1, i++);
    enterFrame     = getUBits(buf, 1, i++);
    load           = getUBits(buf, 1, i++);
    dragOver       = getUBits(buf, 1, i++);
    rollOut        = getUBits(buf, 1, i++);
    rollOver       = getUBits(buf, 1, i++);
    releaseOutside = getUBits(buf, 1, i++);
    release        = getUBits(buf, 1, i++);
    press          = getUBits(buf, 1, i++);
    initialize     = getUBits(buf, 1, i++);
    data           = getUBits(buf, 1, i++);
}

//-----------------------------------------
//              ClipActionRecord
//-----------------------------------------
swf::ClipActionRecord::ClipActionRecord(Version & version) : AbstractVData(version), eventFlags(version) {}

void swf::ClipActionRecord::fromSWF( buf_type *& buf ) {
    eventFlags.fromSWF(buf);
    size.fromSWF(buf);
    
    buf += size.getValue();
    //keyCode.fromSWF(buf);
    //TODO actions vector
}

//-----------------------------------------
//              ClipActions
//-----------------------------------------
swf::ClipActions::ClipActions(Version & version) : AbstractVData(version), allEventFlags(version) {}

bool swf::ClipActions::isEnd( buf_type * buf ) {
    if( _version.versionNum() == 5 )
    {
        return endFlag5.readAhead(buf) == 0;
    }
    else if( _version.versionNum() == 6 )
    {
        return endFlag6.readAhead(buf) == 0;
    }
    
    return true;
}

void swf::ClipActions::fromSWF( buf_type *& buf ) {
    reserved.fromSWF(buf);
    allEventFlags.fromSWF(buf);
    
    
    ClipActionRecord * car;
    do {
        car = new ClipActionRecord(_version);
        car -> fromSWF(buf);
        records.push_back(car);
    } while ( !isEnd(buf) );
    
    
    if( _version.versionNum() == 5 )
    {
        endFlag5.fromSWF(buf);
    }
    else if( _version.versionNum() == 6 )
    {
        endFlag6.fromSWF(buf);
    }
    else
    {
        //throw an error
    }
}

//-----------------------------------------
//              ColorMatrixFilter
//-----------------------------------------
swf::ColorMatrixFilter::ColorMatrixFilter() : matrix(20) {
}

void swf::ColorMatrixFilter::fromSWF( buf_type *& buf ) {
    U32 * t;
    int i = (int)matrix.size();
    while( i-- ) {
        t = new U32;
        t -> fromSWF(buf);
        matrix.push_back(t);
    }
}

//-----------------------------------------
//              ConvolutionFilter
//-----------------------------------------
swf::ConvolutionFilter::ConvolutionFilter() : color(RGB::TYPE_RGBA) {}

void swf::ConvolutionFilter::fromSWF( buf_type *& buf ) {
    matrixX.fromSWF(buf);
    matrixY.fromSWF(buf);
    divisor.fromSWF(buf);
    bias.fromSWF(buf);
    
    U32 * t;
    int i = matrixX.getValue() * matrixY.getValue();
    while( i-- ) {
        t = new U32;
        t -> fromSWF(buf);
        matrix.push_back(t);
    }
    
    color.fromSWF(buf);
    
    clamp         = getUBits(buf, 1, 6);
    preserveAlpha = getUBits(buf, 1, 7);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              BlurFilter
//-----------------------------------------
void swf::BlurFilter::fromSWF( buf_type *& buf ) {
    blurX.fromSWF(buf);
    blurY.fromSWF(buf);
    
    passes = getUBits(buf, 5);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              GlowFilter
//-----------------------------------------
swf::GlowFilter::GlowFilter() : color(RGB::TYPE_RGBA) {}

void swf::GlowFilter::fromSWF( buf_type *& buf ) {
    color.fromSWF(buf);
    blurX.fromSWF(buf);
    blurY.fromSWF(buf);
    strength.fromSWF(buf);
    
    int i = 0;
    innerShadow     = getUBits(buf, 1, i++);
    knockout        = getUBits(buf, 1, i++);
    compositeSource = getUBits(buf, 1, i++);
    passes          = getUBits(buf, 5, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              DropshadowFilter
//-----------------------------------------
swf::DropshadowFilter::DropshadowFilter() : color(RGB::TYPE_RGBA) {}

void swf::DropshadowFilter::fromSWF( buf_type *& buf ) {
    color.fromSWF(buf);
    blurX.fromSWF(buf);
    blurY.fromSWF(buf);
    angle.fromSWF(buf);
    distance.fromSWF(buf);
    strength.fromSWF(buf);
    
    int i = 0;
    innerShadow     = getUBits(buf, 1, i++);
    knockout        = getUBits(buf, 1, i++);
    compositeSource = getUBits(buf, 1, i++);
    passes          = getUBits(buf, 5, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              BevelFilter
//-----------------------------------------
swf::BevelFilter::BevelFilter() : shadowColor(RGB::TYPE_RGBA), highlightColor(RGB::TYPE_RGBA) {}

void swf::BevelFilter::fromSWF( buf_type *& buf ) {
    shadowColor.fromSWF(buf);
    highlightColor.fromSWF(buf);
    blurX.fromSWF(buf);
    blurY.fromSWF(buf);
    angle.fromSWF(buf);
    distance.fromSWF(buf);
    strength.fromSWF(buf);
    
    int i = 0;
    innerShadow     = getUBits(buf, 1, i++);
    knockout        = getUBits(buf, 1, i++);
    compositeSource = getUBits(buf, 1, i++);
    onTop           = getUBits(buf, 1, i++);
    passes          = getUBits(buf, 4, i);
    
    buf += (unsigned) (8 / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              GradientGlowFilter
//-----------------------------------------
void swf::GradientGlowFilter::fromSWF( buf_type *& buf ) {
    numColors.fromSWF(buf);
    
    int i;
    RGB * tRGB;
    U8 * tU8;
    
    i = numColors.getValue();
    while( i-- ) {
        tRGB = new RGB(RGB::TYPE_RGBA);
        tRGB -> fromSWF(buf);
        colors.push_back(tRGB);
    }
    
    i = numColors.getValue();
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
    filterId.fromSWF(buf);
    
    switch ((unsigned short)filterId.getValue()) {
        case FILTER_DROPSHADOW:
            concreteFilter = new DropshadowFilter;
            break;
        case FILTER_BLUR:
            concreteFilter = new BlurFilter;
            break;
        case FILTER_GLOW:
            concreteFilter = new GlowFilter;
            break;
        case FILTER_BEVEL:
            concreteFilter = new BevelFilter;
            break;
        case FILTER_GRADIENTGLOW:
            concreteFilter = new GradientGlowFilter;
            break;
        case FILTER_CONVOLUTION:
            concreteFilter = new ConvolutionFilter;
            break;
        case FILTER_COLORMATRIX:
            concreteFilter = new ColorMatrixFilter;
            break;
        case FILTER_GRADIENTBEVEL:
            concreteFilter = new GradientBevelFilter;
            break;
    }
}

//-----------------------------------------
//              FilterList
//-----------------------------------------
void swf::FilterList::fromSWF( buf_type *& buf ) {
    numberOfFilters.fromSWF(buf);
    unsigned short i = (unsigned short)numberOfFilters.getValue();
    
    Filter * ptr;
    while ( i-- ) {
        ptr = new Filter;
        ptr -> fromSWF(buf);
        filters.push_back(ptr);
    }
}
