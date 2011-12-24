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
    namespace gradients {
        enum kGradientSpreadMode
        {
            PAD,
            REFLECT,
            REPEAT/*,
            RESERVED*/
        };
        
        enum kGradientInterpolationMode
        {
            NORMAL,
            LINEAR/*,
            RESERVED,
            RESERVED*/
        };
    }
    
    //-----------------------------------------
    //                GradRecord
    //-----------------------------------------
    class GradRecord : public AbstractData {
        short shape_context_;
        
        U8 ratio_;
        RGB color3_;//Shape3
        RGB color12_;//Shape1 and Shape2
    public:
        GradRecord();
        virtual void fromSWF( buf_type *& buf );
        
        void set_shape_context(short value);
    };
    
    //-----------------------------------------
    //                Gradient
    //-----------------------------------------
    class Gradient : public AbstractData {
    protected:
        short shape_context_;
        
        char spread_mode_;
        char interpolation_mode_;
        char num_gradients_;
        std::vector<GradRecord *> records_;
        
    public:
        virtual void fromSWF( buf_type *& buf );
        
        void set_shape_context(short value);
    };
    
    //-----------------------------------------
    //             FocalGradient
    //-----------------------------------------
    class FocalGradient : public Gradient {
        U16 focal_point_;
    public:
        virtual void fromSWF( buf_type *& buf );
        
        double focal_point();
    };
}

#endif //libswf_gradients_h
