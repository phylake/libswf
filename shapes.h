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
#include "gradients.h"

namespace swf {
    //-----------------------------------------
    //               ShapeBase
    //-----------------------------------------
    class ShapeBase : public AbstractVData {
    protected:
        short shape_context_;
        
    public:
        ShapeBase(Version & version, short shape_context);
        
        virtual int set_shape_context(short value);
    };
    
    //-----------------------------------------
    //                  LineStyle
    //-----------------------------------------
    class LineStyle : public ShapeBase {
    protected:
        RGB color3_;//Shape3
        RGB color12_;//Shape1 and Shape2
        
    public:
        LineStyle(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //                  FillStyle
    //-----------------------------------------
    class FillStyle : public LineStyle {
        U8 fill_style_type_;
        MATRIX gradient_matrix_;
        Gradient gradient_;
        FocalGradient gradient_focal_;
        U16 bitmap_id_;
        MATRIX bitmap_matrix_;
        
    public:
        unsigned short static const SOLID                       = 0x00;
        unsigned short static const LINEAR_GRADIENT             = 0x10;
        unsigned short static const RADIAL_GRADIENT             = 0x12;
        unsigned short static const FOCAL_RADIAL_GRADIENT       = 0x13;
        unsigned short static const REPEATING_BITMAP            = 0x40;
        unsigned short static const CLIPPED_BITMAP              = 0x41;
        unsigned short static const NON_SMOOTH_REPEATING_BITMAP = 0x42;
        unsigned short static const NON_SMOOTH_CLIPPED_BITMAP   = 0x43;
        
        FillStyle(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
        virtual int set_shape_context(short value);
    };
    
    //-----------------------------------------
    //                  LineStyle2
    //-----------------------------------------
    class LineStyle2 : public LineStyle {
    protected:
        U16 width_;
        
        /*
         * UB[2] StartCapStyle
         * UB[2] JoinStyle
         * UB[1] HasFillFlag
         * UB[1] NoHScaleFlag
         * UB[1] NoVScaleFlag
         * UB[1] PixelHintingFlag
         */
        char b0;
        
        /*
         * UB[5] Reserved
         * UB[1] NoClose
         * UB[2] EndCapStyle
         */
        char b1;
        
        U16 miter_limit_factor_;
        FillStyle fill_type_;
    
    public:
        static const short kJoinStyleRound = 0;
        static const short kJoinStyleBevel = 1;
        static const short kJoinStyleMiter = 2;
        
        static const short kEndcapStyleRound  = 0;
        static const short kEndcapStyleNone   = 1;
        static const short kEndcapStyleSquare = 2;
        
        LineStyle2(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
        
        short start_cap_style();
        short join_style();
        short endcap_style();
        bool has_fill();
        bool no_h_scale();
        bool no_v_scale();
        bool pixel_hinting();
        bool no_close();
    };
    
    //-----------------------------------------
    //              FillStyleArray
    //-----------------------------------------
    class FillStyleArray : public ShapeBase {
        U8 count_;
        U16 count_ext_;
        std::vector<FillStyle *> styles_;
    
    public:
        FillStyleArray(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              LineStyleArray
    //-----------------------------------------
    class LineStyleArray : public ShapeBase {
        U8 count_;
        U16 count_ext_;
        std::vector<LineStyle *> styles_;
    
    public:
        LineStyleArray(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //                 Shape
    //-----------------------------------------
    class Shape : public ShapeBase {
        U8 count_;
        U16 count_ext_;
        std::vector<LineStyle *> styles_;
    
    public:
        Shape(Version & version, short shape_context);
        
        virtual void fromSWF( buf_type *& buf );
    };

}

#endif //libswf_shapes_h
