//
//  shapes.h
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_shapes_h
#define libswf_shapes_h

#include "data.h"

namespace swf {
    //-----------------------------------------
    //                  FillStyle
    //-----------------------------------------
    class FillStyle : public AbstractVData {
        U8 fill_style_type_;
        RGB color3_;//Shape3
        RGB color12_;//Shape1 and Shape2
    public:
        unsigned short static const SOLID                       = 0x00;
        unsigned short static const LINEAR_GRADIENT             = 0x10;
        unsigned short static const RADIAL_GRADIENT             = 0x12;
        unsigned short static const FOCAL_RADIAL_GRADIENT       = 0x13;
        unsigned short static const REPEATING_BITMAP            = 0x40;
        unsigned short static const CLIPPED_BITMAP              = 0x41;
        unsigned short static const NON_SMOOTH_REPEATING_BITMAP = 0x42;
        unsigned short static const NON_SMOOTH_CLIPPED_BITMAP   = 0x43;
        
        FillStyle(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
}

#endif //libswf_shapes_h
