//
//  shapes.cpp
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "shapes.h"

swf::FillStyle::FillStyle(Version & version) :
    AbstractVData(version),
    color3_(swf::RGB::TYPE_RGBA),
    color12_(swf::RGB::TYPE_RGB) {}

void swf::FillStyle::fromSWF( buf_type *& buf ) {}