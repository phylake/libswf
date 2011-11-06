//
//  display_list.h
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_display_list_h
#define libswf_display_list_h

#include "actions.h"

namespace swf {
    
    //-----------------------------------------
    //              ClipEventFlags
    //-----------------------------------------
    class ClipEventFlags : public AbstractVData {
    public:
        bool keyUp;
        bool keyDown;
        bool mouseUp;
        bool mouseDown;
        bool mouseMove;
        bool unload;
        bool enterFrame;
        bool load;
        bool dragOver;
        bool rollOut;
        bool rollOver;
        bool releaseOutside;
        bool release;
        bool press;
        bool initialize;
        bool data;
        
        //below this line: swf >= 6
        
        //reserved UB[5]
        bool construct;
        bool keyPress;
        bool dragOut;
        //reserved UB[8]
        
        ClipEventFlags(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ClipActionRecord
    //-----------------------------------------
    class ClipActionRecord : public AbstractVData {
    public:
        ClipEventFlags eventFlags;
        U32 size;
        U8 keyCode;
        std::vector<ActionRecord *> actions;
        
        ClipActionRecord(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ClipActions
    //-----------------------------------------
    class ClipActions : public AbstractVData {
    protected:
        bool isEnd( buf_type * buf );
    public:
        U16 reserved;
        ClipEventFlags allEventFlags;
        std::vector<ClipActionRecord *> records;
        U16 endFlag5;
        U32 endFlag6;
        
        ClipActions(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ColorMatrixFilter
    //-----------------------------------------
    class ColorMatrixFilter : public AbstractData {
    public:
        std::vector<U32 *> matrix;
        
        ColorMatrixFilter();
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ConvolutionFilter
    //-----------------------------------------
    class ConvolutionFilter : public AbstractData {
        U8 matrixX;
        U8 matrixY;
        U32 divisor;
        U32 bias;
        std::vector<U32 *> matrix;
        RGB color;
        //reserved UB[6]
        bool clamp;
        bool preserveAlpha;
    public:
        ConvolutionFilter();
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              BlurFilter
    //-----------------------------------------
    class BlurFilter : public AbstractData {
    protected:
        U32 blurX;
        U32 blurY;
        unsigned short passes;//UB[5]
        //reserved UB[3]
    public:
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type getBlurX();
        fixed_type getBlurY();
        unsigned short getPasses();
    };
    
    //-----------------------------------------
    //              GlowFilter
    //-----------------------------------------
    class GlowFilter : public AbstractData {
    protected:
        RGB color;
        U32 blurX;
        U32 blurY;
        U16 strength;
        bool innerShadow;
        bool knockout;
        bool compositeSource;
        unsigned short passes;//UB[5]
    public:
        GlowFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type getBlurX();
        fixed_type getBlurY();
        fixed8_type getStrength();
        bool getInnerShadow();
        bool getKnockout();
        bool getCompositeSource();
        unsigned short getPasses();
    };
    
    //-----------------------------------------
    //              DropshadowFilter
    //-----------------------------------------
    class DropshadowFilter : public AbstractData {
    protected:
        RGB color;
        U32 blurX;
        U32 blurY;
        U32 angle;
        U32 distance;
        U16 strength;
        bool innerShadow;
        bool knockout;
        bool compositeSource;
        unsigned short passes;//UB[5]
    public:
        DropshadowFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type getBlurX();
        fixed_type getBlurY();
        fixed_type getAngle();
        fixed_type getDistance();
        fixed8_type getStrength();
        bool getInnerShadow();
        bool getKnockout();
        bool getCompositeSource();
        unsigned short getPasses();
    };
    
    //-----------------------------------------
    //              BevelFilter
    //-----------------------------------------
    class BevelFilter : public AbstractData {
    protected:
        RGB shadowColor;
        RGB highlightColor;
        U32 blurX;
        U32 blurY;
        U32 angle;
        U32 distance;
        U16 strength;
        bool innerShadow;
        bool knockout;
        bool compositeSource;
        bool onTop;
        unsigned short passes;//UB[4]
    public:
        BevelFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type getBlurX();
        fixed_type getBlurY();
        fixed_type getAngle();
        fixed_type getDistance();
        fixed8_type getStrength();
        bool getInnerShadow();
        bool getKnockout();
        bool getCompositeSource();
        bool getOnTop();
        unsigned short getPasses();
    };
    
    //-----------------------------------------
    //              GradientGlowFilter
    //-----------------------------------------
    class GradientGlowFilter : public AbstractData {
    protected:
        U8 numColors;
        std::vector<RGB *> colors;
        std::vector<U8 *> ratios;
        U32 blurX;
        U32 blurY;
        U32 angle;
        U32 distance;
        U16 strength;
        bool innerShadow;
        bool knockout;
        bool compositeSource;
        bool onTop;
        unsigned short passes;//UB[4]
    public:
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              GradientBevelFilter
    //-----------------------------------------
    class GradientBevelFilter : public GradientGlowFilter {
    public:
        /* identical to GradientGlowFilter. no override */
        //virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              Filter
    //-----------------------------------------
    class Filter : public AbstractData {
    public:
        unsigned short static const FILTER_DROPSHADOW    = 0;
        unsigned short static const FILTER_BLUR          = 1;
        unsigned short static const FILTER_GLOW          = 2;
        unsigned short static const FILTER_BEVEL         = 3;
        unsigned short static const FILTER_GRADIENTGLOW  = 4;
        unsigned short static const FILTER_CONVOLUTION   = 5;
        unsigned short static const FILTER_COLORMATRIX   = 6;
        unsigned short static const FILTER_GRADIENTBEVEL = 7;
        
        U8 filterId;
        AbstractData * concreteFilter;
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              FilterList
    //-----------------------------------------
    class FilterList : public AbstractData {
    public:
        U8 numberOfFilters;
        std::vector<Filter *> filters;
        
        virtual void fromSWF( buf_type *& buf );
    };

}

#endif //libswf_display_list_h
