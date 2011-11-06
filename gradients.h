//
//  gradients.h
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_gradients_h
#define libswf_gradients_h

#include "data.h"

namespace swf {
    //-----------------------------------------
    //                  GradRecord
    //-----------------------------------------
    class GradRecord : public AbstractData {
        short int shape_;
        
        U8 ratio_;
        RGB color3_;//Shape3
        RGB color12_;//Shape1 and Shape2
    public:
        GradRecord(int shape);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //                  Gradient
    //-----------------------------------------
    class Gradient : public AbstractVData {
        char spread_mode_;
    public:
        Gradient(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
}

#endif //libswf_gradients_h
