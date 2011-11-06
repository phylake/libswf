//
//  data.cpp
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "data.h"

//----------------------------------------------------
//                       Versioning
//----------------------------------------------------
unsigned char swf::Version::version() {
    return value;
}

unsigned short swf::Version::versionNum() {
    return (unsigned short)value;
}

void swf::MutableVersion::setVersion(unsigned char version) {
    value = version;
}

void swf::MutableVersion::setVersion(buf_type *& buf) {
    setVersion(*buf);
    buf++;
}

swf::VersionRequirement::VersionRequirement(Version & version) : _version(version) {}

//-----------------------------------------
//                 AbstractBase
//-----------------------------------------
swf::AbstractBase::~AbstractBase() {}

//-----------------------------------------
//                 AbstractData
//-----------------------------------------
swf::AbstractData::~AbstractData() {}

//-----------------------------------------
//         AbstractVData
//-----------------------------------------
swf::AbstractVData::AbstractVData(Version & version) : VersionRequirement(version) {}

swf::AbstractVData::~AbstractVData() {}

//-----------------------------------------
//                   U8
//-----------------------------------------
void swf::U8::fromSWF( buf_type *& buf ) {
    value = buf[0] & 0x000000ff;
    buf++;
}

unsigned char swf::U8::readAhead( buf_type * buf ) {
    return buf[0] & 0x000000ff;
}

unsigned char swf::U8::getValue() {
    return value;
}

//-----------------------------------------
//                   U16
//-----------------------------------------
void swf::U16::fromSWF( buf_type *& buf ) {
    value =
    buf[0] << 0 & 0x000000ff |
    buf[1] << 8 & 0x0000ff00
    ;
    buf += 2;
}

unsigned short swf::U16::readAhead( buf_type * buf ) {
    return
    buf[0] << 0 & 0x000000ff |
    buf[1] << 8 & 0x0000ff00
    ;
}

unsigned short int swf::U16::getValue() {
    return value;
}

double swf::U16::toFixed8() {
    double decimal = 0;
    
    if (value & 0x00ff)
    {
        /* divide by the maximum decimal precision that can be held in 8 bits */
        decimal = (double)(value & 0x00ff) / pow(10, ceil(log10(pow(2, 8))));
    }
    
    return ((value & 0xff00) >> 8) + decimal;
}

float swf::U16::toFloat() {
    unsigned ret = 0;
    //int s = (value << 16) & 0x80000000;
    //int e = (value >> 10) & 0x1f;
    //int m = value & 0x3ff;
    
    
    
    return ret;
}

//-----------------------------------------
//                   U32
//-----------------------------------------
void swf::U32::fromSWF( buf_type *& buf ) {
    value =
    buf[0] <<  0 & 0x000000ff |
    buf[1] <<  8 & 0x0000ff00 |
    buf[2] << 16 & 0x00ff0000 |
    buf[3] << 24 & 0xff000000
    ;
    buf += 4;
}

unsigned int swf::U32::readAhead( buf_type * buf ) {
    return
    buf[0] <<  0 & 0x000000ff |
    buf[1] <<  8 & 0x0000ff00 |
    buf[2] << 16 & 0x00ff0000 |
    buf[3] << 24 & 0xff000000
    ;
}

unsigned int swf::U32::getValue() {
    return value;
}

void swf::U32::setValue(unsigned int value) {
    (*this).value = value;
}

fixed16_type swf::U32::toFixed16() {
    fixed16_type decimal = 0;
    
    if (value & 0x0000ffff)
    {
        /* divide by the maximum decimal precision that can be held in 16 bits */
        decimal = (fixed16_type)(value & 0x0000ffff) / pow(10, ceil(log10(pow(2, 16))));
    }
    
    return ((value & 0xffff0000) >> 16) + decimal;
}

/*
 The spec's FIXED
 */
fixed_type swf::U32::toFixed() {
    return toFixed16();
}

float swf::U32::toFloat() {
    return *(float *)&value;
}

//-----------------------------------------
//                   EU32
//-----------------------------------------
void swf::EU32::fromSWF( buf_type *& buf ) {    
    value = buf[0];
    
    if (!(value & 0x00000080))
    {
        buf++;
        return;
    }
    
    value = (value & 0x0000007f) | buf[1]<<7;
    if (!(value & 0x00004000))
    {
        buf += 2;
        return;
    }
    
    value = (value & 0x00003fff) | buf[2]<<14;
    if (!(value & 0x00200000))
    {
        buf += 3;
        return;
    }
    
    value = (value & 0x001fffff) | buf[3]<<21;
    if (!(value & 0x10000000))
    {
        buf += 4;
        return;
    }
    
    value = (value & 0x0fffffff) | buf[4]<<28;
    buf += 5;
}

unsigned int swf::EU32::readAhead( buf_type * buf ) {
    return
    buf[0] <<  0 & 0x000000ff |
    buf[1] <<  8 & 0x0000ff00 |
    buf[2] << 16 & 0x00ff0000 |
    buf[3] << 24 & 0xff000000
    ;
}

unsigned int swf::EU32::getValue() {
    return value;
}

void swf::EU32::setValue(unsigned int value) {
    (*this).value = value;
}

fixed16_type swf::EU32::toFixed16() {
    fixed16_type decimal = 0;
    
    if (value & 0x0000ffff)
    {
        /* divide by the maximum decimal precision that can be held in 16 bits */
        decimal = (fixed16_type)(value & 0x0000ffff) / pow(10, ceil(log10(pow(2, 16))));
    }
    
    return ((value & 0xffff0000) >> 16) + decimal;
}

/*
 The spec's FIXED
 */
fixed_type swf::EU32::toFixed() {
    return toFixed16();
}

float swf::EU32::toFloat() {
    return *(float *)&value;
}

//-----------------------------------------
//                 String
//-----------------------------------------
void swf::String::fromSWF( buf_type *& buf ) {
    value.assign((char *)buf);
    buf += value.length() + 1;
}

//-----------------------------------------
//                   Twip
//-----------------------------------------
signed int swf::Twip::toPX() {
    return value/20;
}

//-----------------------------------------
//                   RGB
//-----------------------------------------
swf::RGB::RGB( int type ) {
    (*this).type = type;
}

void swf::RGB::fromSWF( buf_type *& buf ) {
    value.fromSWF(buf);
    
    if( type == 0 ) buf--;//only 3 bytes were relevant
}

//-----------------------------------------
//                   RECT
//-----------------------------------------
void swf::RECT::fromSWF( buf_type *& buf ) {
    int i = 0;
    
    nBits      = getUBits(buf, 5);
    xMin.value = getSBits(buf, nBits, 5 + nBits * i++);
    xMax.value = getSBits(buf, nBits, 5 + nBits * i++);
    yMin.value = getSBits(buf, nBits, 5 + nBits * i++);
    yMax.value = getSBits(buf, nBits, 5 + nBits * i++);
    
    buf += (unsigned) ceil( (5 + (nBits * i)) / (sizeof(*buf) * 8.0f) );
    
#ifdef DEBUG
    printf("i: %i\n", i);
    printf("nBits: %i\n", nBits);
    printf("xMin: %i\n", xMin.toPX());
    printf("xMax: %i\n", xMax.toPX());
    printf("yMin: %i\n", yMin.toPX());
    printf("yMax: %i\n", yMax.toPX());
#endif
}

unsigned int swf::RECT::toBE() {
    return 0;
}

//-----------------------------------------
//                   MATRIX
//-----------------------------------------
void swf::MATRIX::fromSWF(buf_type *& buf) {
    int i;
    unsigned offset = 0;
    signed sTemp;
    
    hasScale = getUBits(buf, 1, offset++);/* HasScale UB[1] */
    if ( hasScale )
    {
        i = 0;
        
        /* NScaleBits UB[5] */
        nScaleBits = getUBits(buf, 5, offset);
        offset += 5;
        
        sTemp = getUBits(buf, nScaleBits, offset + nScaleBits * i++);
        scaleX.setValue( sTemp );
        
        sTemp = getUBits(buf, nScaleBits, offset + nScaleBits * i++);
        scaleY.setValue( sTemp );
        
        offset += nScaleBits * i;
    }
    
    hasRotate = getUBits(buf, 1, offset++);/* HasRotate UB[1] */
    if ( hasRotate )
    {
        i = 0;
        
        /* NRotateBits UB[5] */
        nRotateBits = getUBits(buf, 5, offset);
        offset += 5;
        
        sTemp = getSBits(buf, nRotateBits, offset + nRotateBits * i++);
        rotateSkew0.setValue( sTemp );
        
        sTemp = getSBits(buf, nRotateBits, offset + nRotateBits * i++);
        rotateSkew1.setValue( sTemp );
        
        offset += nRotateBits * i;
    }
    
    /* always has translation */
    i = 0;
    
    /* NTranslateBits UB[5] */
    nTranslateBits = getUBits(buf, 5, offset);
    offset += 5;
    
    if( nTranslateBits > 0 )
    {
        sTemp = getSBits(buf, nTranslateBits, offset + nTranslateBits * i++);
        translateX.value = sTemp;
        
        sTemp = getSBits(buf, nTranslateBits, offset + nTranslateBits * i++);
        translateY.value = sTemp;
        
        offset += nTranslateBits * i;
    }
    
    buf += (unsigned) ceil(offset / (sizeof(*buf) * 8.0f));
    
#ifdef DEBUG
    /*
     printf("\t\tMATRIX\n");
     printf("\t\t\t%i hasScale\n", hasScale);
     printf("\t\t\t%i nScaleBits\n", nScaleBits);
     if( hasScale )
     {
     printf("\t\t\t%f scaleX\n", scaleX.toFixed16());
     printf("\t\t\t%f scaleY\n", scaleY.toFixed16());
     }
     printf("\t\t\t%i hasRotate\n", hasRotate);
     printf("\t\t\t%i nRotateBits\n", nRotateBits);
     if( hasRotate )
     {
     printf("\t\t\t%f rotateSkew0\n", rotateSkew0.toFixed16());
     printf("\t\t\t%f rotateSkew1\n", rotateSkew1.toFixed16());
     }
     printf("\t\t\t%i nTranslateBits\n", nTranslateBits);
     printf("\t\t\t%i translateX\n", translateX.toPX());
     printf("\t\t\t%i translateY\n", translateY.toPX());
     */
#endif
}

double swf::MATRIX::xPrime(double x, double y) {
    return x * scaleX.toFixed16() + y * rotateSkew1.toFixed16() + translateX.toPX();
}

double swf::MATRIX::yPrime(double x, double y) {
    return x * rotateSkew0.toFixed16() + y * rotateSkew0.toFixed16() + translateY.toPX();
}

//-----------------------------------------
//                   CXFORM
//-----------------------------------------
void swf::CXFORM::fromSWF(buf_type *& buf) {
    int i = 0;
    hasAddTerms  = getUBits(buf, 1, i++);
    hasMultTerms = getUBits(buf, 1, i++);
    nBits        = getUBits(buf, 4, i++);
    
    i = 6;
    if( hasMultTerms ) {
        rm = getUBits(buf, 4, i);
        i += nBits;
        gm = getUBits(buf, 4, i);
        i += nBits;
        bm = getUBits(buf, 4, i);
        i += nBits;
    }
    
    if( hasAddTerms ) {
        ra = getUBits(buf, 4, i);
        i += nBits;
        ga = getUBits(buf, 4, i);
        i += nBits;
        ba = getUBits(buf, 4, i);
        i += nBits;
    }
    
    buf += (unsigned) ceil(i / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//              CXFORMWITHALPHA
//-----------------------------------------
void swf::CXFORMWITHALPHA::fromSWF(buf_type *& buf) {
    int i = 0;
    hasAddTerms  = getUBits(buf, 1, i++);
    hasMultTerms = getUBits(buf, 1, i++);
    nBits        = getUBits(buf, 4, i++);
    
    i = 6;
    if( hasMultTerms ) {
        rm = getUBits(buf, nBits, i);
        i += nBits;
        gm = getUBits(buf, nBits, i);
        i += nBits;
        bm = getUBits(buf, nBits, i);
        i += nBits;
        am = getUBits(buf, nBits, i);
        i += nBits;
    }
    
    if( hasAddTerms ) {
        ra = getUBits(buf, nBits, i);
        i += nBits;
        ga = getUBits(buf, nBits, i);
        i += nBits;
        ba = getUBits(buf, nBits, i);
        i += nBits;
        aa = getUBits(buf, nBits, i);
        i += nBits;
    }
    
    buf += (unsigned) ceil(i / (sizeof(*buf) * 8.0f));
}

//-----------------------------------------
//                RecordHeader
//-----------------------------------------
void swf::RecordHeader::fromSWF( buf_type *& buf ) {
    tagCodeAndLength.fromSWF(buf);
    
    tag = (tagCodeAndLength.getValue() >> 6) & 0x3ff;
    
    /*
     check the last 6 bits of U16
     All 1's means this is a long header
     */
    
    unsigned int shortLen = tagCodeAndLength.getValue() & 0x3f;
    if ( shortLen == 0x3f ) {
        isShort = false;
        tagLength.fromSWF( buf );
    } else {
        isShort = true;
        tagLength.setValue( shortLen );
    }
}

short int swf::RecordHeader::type() {
    return tag;
}

unsigned swf::RecordHeader::length() {
    return tagLength.getValue();
}


//-----------------------------------------
//                AbstractTag
//-----------------------------------------
swf::AbstractTag::~AbstractTag() {}

//-----------------------------------------
//                AbstractVTag
//-----------------------------------------
swf::AbstractVTag::AbstractVTag(Version & version) : VersionRequirement(version) {}

swf::AbstractVTag::~AbstractVTag() {}
