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
        bool key_up_;
        bool key_down_;
        bool mouse_up_;
        bool mouse_down_;
        bool mouse_move_;
        bool unload_;
        bool enter_frame_;
        bool load_;
        bool drag_over_;
        bool roll_out_;
        bool roll_over_;
        bool release_outside_;
        bool release_;
        bool press_;
        bool initialize_;
        bool data_;
        
        //below this line: swf >= 6
        
        //reserved UB[5]
        bool construct_;
        bool key_press_;
        bool drag_out_;
        //reserved UB[8]
        
    public:
        ClipEventFlags(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ClipActionRecord
    //-----------------------------------------
    class ClipActionRecord : public AbstractVData {
        ClipEventFlags event_flags_;
        U32 size_;
        U8 key_code_;
        std::vector<ActionRecord *> actions_;
    public:
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
        U16 reserved_;
        ClipEventFlags all_event_flags_;
        std::vector<ClipActionRecord *> records_;
        U16 end_flag5_;
        U32 end_flag6_;
        
        ClipActions(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ColorMatrixFilter
    //-----------------------------------------
    class ColorMatrixFilter : public AbstractData {
    public:
        std::vector<U32 *> matrix_;
        
        ColorMatrixFilter();
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              ConvolutionFilter
    //-----------------------------------------
    class ConvolutionFilter : public AbstractData {
        U8 matrix_x_;
        U8 matrix_y_;
        U32 divisor_;
        U32 bias_;
        std::vector<U32 *> matrix_;
        RGB color_;
        //reserved UB[6]
        bool clamp_;
        bool preserve_alpha_;
    public:
        ConvolutionFilter();
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              BlurFilter
    //-----------------------------------------
    class BlurFilter : public AbstractData {
    protected:
        U32 blur_x_;
        U32 blur_y_;
        unsigned short passes_;//UB[5]
        //reserved UB[3]
    public:
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type blur_x() const;
        fixed_type blur_y() const;
        unsigned short passes() const;
    };
    
    //-----------------------------------------
    //              GlowFilter
    //-----------------------------------------
    class GlowFilter : public AbstractData {
    protected:
        RGB color;
        U32 blur_x_;
        U32 blur_y_;
        U16 strength_;
        bool inner_shadow_;
        bool knockout_;
        bool composite_source_;
        unsigned short passes_;//UB[5]
    public:
        GlowFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type blur_x();
        fixed_type blur_y();
        fixed8_type strength();
        bool inner_shadow();
        bool knockout();
        bool composite_source();
        unsigned short passes();
    };
    
    //-----------------------------------------
    //              DropshadowFilter
    //-----------------------------------------
    class DropshadowFilter : public AbstractData {
    protected:
        RGB color;
        U32 blur_x_;
        U32 blur_y_;
        U32 angle_;
        U32 distance_;
        U16 strength_;
        bool inner_shadow_;
        bool knockout_;
        bool composite_source_;
        unsigned short passes_;//UB[5]
    public:
        DropshadowFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type blur_x() const;
        fixed_type blur_y() const;
        fixed_type angle() const;
        fixed_type distance() const;
        fixed8_type strength() const;
        bool inner_shadow() const;
        bool knockout() const;
        bool composite_source() const;
        unsigned short passes() const;
    };
    
    //-----------------------------------------
    //              BevelFilter
    //-----------------------------------------
    class BevelFilter : public AbstractData {
    protected:
        RGB shadowColor;
        RGB highlightColor;
        U32 blur_x_;
        U32 blur_y_;
        U32 angle_;
        U32 distance_;
        U16 strength_;
        bool inner_shadow_;
        bool knockout_;
        bool composite_source_;
        bool on_top_;
        unsigned short passes_;//UB[4]
    public:
        BevelFilter();
        virtual void fromSWF( buf_type *& buf );
        
        fixed_type getblur_x_() const;
        fixed_type blur_y() const;
        fixed_type angle() const;
        fixed_type distance() const;
        fixed8_type strength() const;
        bool inner_shadow() const;
        bool knockout() const;
        bool composite_source() const;
        bool on_top() const;
        unsigned short passes() const;
    };
    
    //-----------------------------------------
    //              GradientGlowFilter
    //-----------------------------------------
    class GradientGlowFilter : public AbstractData {
    protected:
        U8 num_colors_;
        std::vector<RGB *> colors_;
        std::vector<U8 *> ratios;
        U32 blur_x_;
        U32 blur_y_;
        U32 angle;
        U32 distance;
        U16 strength_;
        bool inner_shadow_;
        bool knockout_;
        bool composite_source_;
        bool on_top_;
        unsigned short passes_;//UB[4]
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
        
        U8 filter_id_;
        AbstractData * concrete_filter_;
        
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
