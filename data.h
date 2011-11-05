//
//  data.h
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_data_h
#define libswf_data_h

#include <stdio.h>
#include <vector.h>
#include "libswf.h"
#include "math.h"
#include "util.h"

namespace swf {
    //----------------------------------------------------
    //                       Versioning
    //----------------------------------------------------
    
    class Version {
    protected:
        unsigned char value;
    public:
        unsigned char version();
        unsigned short versionNum();
    };
    
    class MutableVersion : public Version {
    public:
        void setVersion(unsigned char value);
        void setVersion(buf_type *& value);
    };
    
    class VersionRequirement {
    protected:
        Version & _version;
    public:
        VersionRequirement(Version & version);
    };
    
    //-----------------------------------------
    //                AbstractBase
    //-----------------------------------------
    class AbstractBase {
    public:
        virtual void fromSWF( buf_type *& buf ) = 0;
        //virtual void toSWF( buf_type *& buf ) = 0;
        virtual ~AbstractBase() = 0;
    };
    
    //-----------------------------------------
    //                AbstractData
    //-----------------------------------------
    class AbstractData : public AbstractBase {
    public:
        virtual ~AbstractData() = 0;
    };
    
    //-----------------------------------------
    //         AbstractVData
    //-----------------------------------------
    class AbstractVData : public AbstractBase, protected VersionRequirement {
    public:
        AbstractVData(Version & version);
        virtual ~AbstractVData() = 0;
    };
    
    //----------------------------------------------------
    //                       General
    //----------------------------------------------------
    
    //-----------------------------------------
    //                   U8
    //-----------------------------------------
    class U8 : public AbstractData {
        unsigned char value;
    public:
        virtual void fromSWF( buf_type *& buf );
        unsigned char readAhead( buf_type * buf );
        unsigned char getValue();
    };
    
    //-----------------------------------------
    //                   U16
    //-----------------------------------------
    class U16 : public AbstractData {
        unsigned short int value;
    public:
        virtual void fromSWF( buf_type *& buf );
        unsigned short readAhead( buf_type * buf );
        unsigned short int getValue();
        double toFixed8();
        float toFloat();
    };
    
    //-----------------------------------------
    //                   U32
    //-----------------------------------------
    class U32 : public AbstractData {
        unsigned int value;
    public:
        virtual void fromSWF( buf_type *& buf );
        unsigned int readAhead( buf_type * buf );
        
        fixed16_type toFixed16();
        fixed_type toFixed();
        float toFloat();
        
        unsigned int getValue();
        void setValue(unsigned int value);
    };
    
    //-----------------------------------------
    //                   EU32
    //-----------------------------------------
    class EU32 : public AbstractData {
        unsigned int value;
    public:
        virtual void fromSWF( buf_type *& buf );
        unsigned int readAhead( buf_type * buf );
        
        fixed16_type toFixed16();
        fixed_type toFixed();
        float toFloat();
        
        unsigned int getValue();
        void setValue(unsigned int value);
    };
    
    //-----------------------------------------
    //                 String
    //-----------------------------------------
    class String : public AbstractData {
    public:
        std::string value;
        void virtual fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //                   Twip
    //-----------------------------------------
    class Twip {
        
    public:
        signed int value;
        signed int toPX();
    };
    
    //-----------------------------------------
    //                   RGB
    //-----------------------------------------
    class RGB : public AbstractData {
        U32 value;
        int type;
    public:
        short static const TYPE_ARGB  = -1;
        short static const  TYPE_RGB  =  0;
        short static const  TYPE_RGBA =  1;
        /*
         -1 ARGB
         0  RGB
         1  RGBA
         */
        RGB( int type = 0 );
        virtual void fromSWF( buf_type *& buf );
        
        /*unsigned char r();
         unsigned char g();
         unsigned char b();
         unsigned char a();*/
    };
    
    //-----------------------------------------
    //                   RECT
    //-----------------------------------------
    class RECT : public AbstractData {
    public:
        unsigned char nBits;
        Twip xMin;
        Twip xMax;
        Twip yMin;
        Twip yMax;
        virtual void fromSWF( buf_type *& buf );
        unsigned int toBE();
    };
    
    //-----------------------------------------
    //                   MATRIX
    //-----------------------------------------
    class MATRIX : public AbstractData {
        //
        //scale
        //
        bool hasScale;
        unsigned char nScaleBits;
        U32 scaleX;
        U32 scaleY;
        
        //
        //rotation
        //
        bool hasRotate;
        unsigned char nRotateBits;
        U32 rotateSkew0;
        U32 rotateSkew1;
        
        //
        //translation
        //
        unsigned char nTranslateBits;
        Twip translateX;
        Twip translateY;
    public:
        virtual void fromSWF( buf_type *& buf );
        
        inline double xPrime(double x, double y);
        inline double yPrime(double x, double y);
    };
    
    //-----------------------------------------
    //                   CXFORM
    //-----------------------------------------
    class CXFORM : public AbstractData {
    protected:
        bool hasAddTerms;
        bool hasMultTerms;
        unsigned short nBits;
        
        unsigned char rm;
        unsigned char gm;
        unsigned char bm;
        unsigned char ra;
        unsigned char ga;
        unsigned char ba;
    public:
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
    //              CXFORMWITHALPHA
    //-----------------------------------------
    class CXFORMWITHALPHA : public CXFORM {
        unsigned char am;
        unsigned char aa;
    public:
        virtual void fromSWF( buf_type *& buf );
    };

}

#endif
