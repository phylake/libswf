//
//  swf.cpp
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#include "swf.h"

//-----------------------------------------
//              ActionRecord
//-----------------------------------------
void swf::ActionRecord::fromSWF( buf_type *& buf ) {
    actionCode.fromSWF(buf);
    if ( actionCode.getValue() >= 0x80 ) {
        length.fromSWF(buf);
        buf += length.getValue();
    }
}

unsigned swf::ActionRecord::getTotalLength() {
    return 1 + 2 + length.getValue();
}

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

//-----------------------------------------
//             TagNameBase
//-----------------------------------------
void swf::TagNameBase::tagsFromSWF(buf_type *& buf) {
    numSymbols.fromSWF(buf);
    int i;
    
    i = (int)numSymbols.getValue();
    U16 * tag;
    String * name;
    
    while( i-- ) {
        tag  = new U16;
        name = new String;
        
        tag  -> fromSWF(buf);
        name -> fromSWF(buf);
        
        tags.push_back(tag);
        names.push_back(name);
    }
    
#ifdef DEBUG
    for (i = 0; i < tags.size(); i++) {
        tag = tags[i];
        name = names[i];
        
        printf("\t%i ", (int)tag->getValue());
        printf(name->value.data());
        printf("\n");
    }
#endif
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


//-----------------------------------------
//                SWFHeader
//-----------------------------------------
void swf::SWFHeader::fromSWF(buf_type *& buf) {
    _compressed = buf[0] == 'C';

    buf += 3;//skip 'W' and 'F'
    __version.fromSWF(buf);
    _version = new MutableVersion;
    _version -> setVersion(__version.getValue());
    _fileLength.fromSWF(buf);
}

void swf::SWFHeader::continueWith(buf_type *& buf) {
    _frameSize.fromSWF(buf);
    _frameRate.fromSWF(buf);
    _frameCount.fromSWF(buf);

#ifdef DEBUG
    printf("SWFHeader\n");
    printf("\t%i compressed\n", _compressed);
    printf("\t%i version\n", __version.getValue());
    printf("\t%i fileLength\n", _fileLength.getValue());
    //printf("\t%i frameSize\n", _frameSize.getValue());
    printf("\t%f frameRate\n", _frameRate.toFixed8());
    printf("\t%i frameCount\n", _frameCount.getValue());
#endif
}

bool swf::SWFHeader::compressed() {
    return _compressed;
}

swf::U8 swf::SWFHeader::version() {
    return __version;
}

swf::MutableVersion * swf::SWFHeader::versionPtr() {
    return _version;
}

swf::U32 swf::SWFHeader::fileLength() {
    return _fileLength;
}

swf::RECT swf::SWFHeader::frameSize() {
    return _frameSize;
}

swf::U16 swf::SWFHeader::frameRate() {
    return _frameRate;
}

swf::U16 swf::SWFHeader::frameCount() {
    return _frameCount;
}

//-----------------------------------------
//             1 ShowFrame
//-----------------------------------------
void swf::ShowFrame::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf(" 1 ShowFrame\n");
#endif
}

//-----------------------------------------
//             9 SetBackgroundColor
//-----------------------------------------
void swf::SetBackgroundColor::fromSWF(buf_type *& buf) {
    color.fromSWF(buf);

#ifdef DEBUG
    printf(" 9 SetBackgroundColor\n");
#endif

}

//-----------------------------------------
//             26 PlaceObject2
//-----------------------------------------
swf::PlaceObject2::PlaceObject2(Version & version) : AbstractVTag(version), clipActions(version) {}

//swf::PlaceObject2::~PlaceObject2() {}

void swf::PlaceObject2::fromSWF(buf_type *& buf) {
    int i = 0;
    hasClipActions    = getUBits(buf, 1, i++);
    hasClipDepth      = getUBits(buf, 1, i++);
    hasName           = getUBits(buf, 1, i++);
    hasRatio          = getUBits(buf, 1, i++);
    hasColorTransform = getUBits(buf, 1, i++);
    hasMatrix         = getUBits(buf, 1, i++);
    hasCharacter      = getUBits(buf, 1, i++);
    moves             = getUBits(buf, 1, i++);

    buf++;

    depth.fromSWF(buf);
    if( hasCharacter )      characterId.fromSWF(buf);
    if( hasMatrix )         matrix.fromSWF(buf);
    if( hasColorTransform ) colorTransform.fromSWF(buf);
    if( hasRatio )          ratio.fromSWF(buf);
    if( hasName )           name.fromSWF(buf);
    if( hasClipDepth )      clipDepth.fromSWF(buf);
    if( hasClipActions )    clipActions.fromSWF(buf);

#ifdef DEBUG
    printf("26 PlaceObject2\n");
    printf("\t%i hasClipActions\n",    hasClipActions);
    printf("\t%i hasClipDepth\n",      hasClipDepth);
    printf("\t%i hasName\n",           hasName);
    printf("\t%i hasRatio\n",          hasRatio);
    printf("\t%i hasColorTransform\n", hasColorTransform);
    printf("\t%i hasMatrix\n",         hasMatrix);
    printf("\t%i hasCharacter\n",      hasCharacter);
    printf("\t%i moves\n",             moves);
#endif
}

//-----------------------------------------
//             56 ExportAssets
//-----------------------------------------
void swf::ExportAssets::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf("56 ExportAssets\n");
#endif
    tagsFromSWF(buf);
}

//-----------------------------------------
//             57 ImportAssets
//-----------------------------------------
void swf::ImportAssets::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf("57 ImportAssets\n");
#endif
    url.fromSWF(buf);
    tagsFromSWF(buf);
}

//-----------------------------------------
//             69 FileAttributes
//-----------------------------------------
void swf::FileAttributes::fromSWF(buf_type *& buf) {
    _useDirectBlit = *buf & 0x40;
    _useGPU        = *buf & 0x20;
    _hasMetadata   = *buf & 0x10;
    _isAS3         = *buf & 0x08;
    _useNetwork    = *buf & 0x01;

    buf += 4;

#ifdef DEBUG
    printf("69 FileAttributes\n");
    printf("\t%i useDirectBlit\n", _useDirectBlit);
    printf("\t%i useGPU\n",        _useGPU);
    printf("\t%i hasMetadata\n",   _hasMetadata);
    printf("\t%i isAS3\n",         _isAS3);
    printf("\t%i useNetwork\n",    _useNetwork);
#endif
}

//-----------------------------------------
//             70 PlaceObject3
//-----------------------------------------
swf::PlaceObject3::PlaceObject3(Version & version) : PlaceObject2(version) {}

void swf::PlaceObject3::fromSWF(buf_type *& buf) {
    int i = 0;
    hasClipActions    = getUBits(buf, 1, i++);
    hasClipDepth      = getUBits(buf, 1, i++);
    hasName           = getUBits(buf, 1, i++);
    hasRatio          = getUBits(buf, 1, i++);
    hasColorTransform = getUBits(buf, 1, i++);
    hasMatrix         = getUBits(buf, 1, i++);
    hasCharacter      = getUBits(buf, 1, i++);
    moves             = getUBits(buf, 1, i++);

    i += 3;//reserved UB[3]

    hasImage          = getUBits(buf, 1, i++);
    hasClassName      = getUBits(buf, 1, i++);
    hasCacheAsBitmap  = getUBits(buf, 1, i++);
    hasBlendMode      = getUBits(buf, 1, i++);
    hasFilterList     = getUBits(buf, 1, i++);

    buf += (unsigned) (i / (sizeof(*buf) * 8.0f));

    depth.fromSWF(buf);
    if( hasClassName || (hasImage && hasCharacter) ) className.fromSWF(buf);
    if( hasCharacter ) characterId.fromSWF(buf);
    if( hasMatrix ) matrix.fromSWF(buf);
    if( hasColorTransform ) colorTransform.fromSWF(buf);
    if( hasRatio ) ratio.fromSWF(buf);
    if( hasName ) name.fromSWF(buf);
    if( hasClipDepth ) clipDepth.fromSWF(buf);
    if( hasFilterList ) surfaceFilterList.fromSWF(buf);
    if( hasBlendMode ) blendMode.fromSWF(buf);
    if( hasCacheAsBitmap ) bitmapCache.fromSWF(buf);
    if( hasClipActions ) clipActions.fromSWF(buf);

#ifdef DEBUG
    printf("70 PlaceObject3\n");
    printf("\t%i hasClipActions\n",    hasClipActions);
    printf("\t%i hasClipDepth\n",      hasClipDepth);
    printf("\t%i hasName\n",           hasName);
    printf("\t%i hasRatio\n",          hasRatio);
    printf("\t%i hasColorTransform\n", hasColorTransform);
    printf("\t%i hasMatrix\n",         hasMatrix);
    printf("\t%i hasCharacter\n",      hasCharacter);
    printf("\t%i moves\n",             moves);
    printf("\t%i hasImage\n",          hasImage);
    printf("\t%i hasClassName\n",      hasClassName);
    printf("\t%i hasCacheAsBitmap\n",  hasCacheAsBitmap);
    printf("\t%i hasBlendMode\n",      hasBlendMode);
    printf("\t%i hasFilterList\n",     hasFilterList);
#endif
}

//-----------------------------------------
//             71 ImportAssets2
//-----------------------------------------
void swf::ImportAssets2::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf("71 ImportAssets2\n");
#endif
    
    url.fromSWF(buf);
    buf += 2;//reserved U8s
    tagsFromSWF(buf);
}

//-----------------------------------------
//             76 SymbolClass
//-----------------------------------------
void swf::SymbolClass::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf("76 SymbolClass\n");
#endif
    
    tagsFromSWF(buf);
}

//-----------------------------------------
//           78 DefineScalingGrid
//-----------------------------------------
void swf::DefineScalingGrid::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf("78 DefineScalingGrid\n");
#endif

    characterId.fromSWF(buf);
    splitter.fromSWF(buf);
}


//-----------------------------------------
//                  SWF
//-----------------------------------------
void swf::SWF::fromSWF(buf_type *& buf) {
    _buffer = &buf;
    header.fromSWF(buf);
}

void swf::SWF::continueWith(buf_type *& buf) {
    _buffer = &buf;
    //(*_buffer) += 4;//want to do this but outBuf is cropped at the moment
    header.continueWith(buf);

    std::vector<RecordHeader> vrh;
    RecordHeader * rh;

    std::vector<AbstractTag> vt;
    AbstractTag * t;
    short tv;
    do {
        rh = new RecordHeader();
        rh->fromSWF(buf);

        tv = rh->type();
        switch (tv) {
            case 0:
                printf("%2i End\n", tv);
                buf += rh->length();
                break;
            case 1:
                //printf("%2i ShowFrame\n", tv);
                //buf += rh->length();

                t = new ShowFrame;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 2:
                printf("%2i DefineShape\n", tv);
                buf += rh->length();
                break;
            case 4:
                printf("%2i PlaceObject\n", tv);
                buf += rh->length();
                break;
            case 5:
                printf("%2i RemoveObject\n", tv);
                buf += rh->length();
                break;
            case 6:
                printf("%2i DefineBits\n", tv);
                buf += rh->length();
                break;
            case 7:
                printf("%2i DefineButton\n", tv);
                buf += rh->length();
                break;
            case 8:
                printf("%2i JPEGTables\n", tv);
                buf += rh->length();
                break;
            case 9:
                //printf("%2i SetBackgroundColor\n", tv);
                //buf += rh->length();

                t = new SetBackgroundColor;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 10:
                printf("%i DefineFont\n", tv);
                buf += rh->length();
                break;
            case 11:
                printf("%i DefineText\n", tv);
                buf += rh->length();
                break;
            case 12:
                printf("%i DoAction\n", tv);
                buf += rh->length();
                break;
            case 13:
                printf("%i DefineFontInfo\n", tv);
                buf += rh->length();
                break;
            case 14:
                printf("%i DefineSound\n", tv);
                buf += rh->length();
                break;
            case 15:
                printf("%i StartSound\n", tv);
                buf += rh->length();
                break;
            case 17:
                printf("%i DefineButtonSound\n", tv);
                buf += rh->length();
                break;
            case 18:
                printf("%i SoundbufHead\n", tv);
                buf += rh->length();
                break;
            case 19:
                printf("%i SoundbufBlock\n", tv);
                buf += rh->length();
                break;
            case 20:
                printf("%i DefineBitsLossless\n", tv);
                buf += rh->length();
                break;
            case 21:
                printf("%i DefineBitsJPEG2\n", tv);
                buf += rh->length();
                break;
            case 22:
                printf("%i DefineShape2\n", tv);
                buf += rh->length();
                break;
            case 23:
                printf("%i DefineButtonCxform\n", tv);
                buf += rh->length();
                break;
            case 24:
                printf("%i Protect\n", tv);
                buf += rh->length();
                break;
            case 26:
                //printf("%i PlaceObject2\n", tv);
                //buf += rh->length();

                t = new PlaceObject2(*header.versionPtr());
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 28:
                printf("%i RemoveObject2\n", tv);
                buf += rh->length();
                break;
            case 32:
                printf("%i DefineShape3\n", tv);
                buf += rh->length();
                break;
            case 33:
                printf("%i DefineText2\n", tv);
                buf += rh->length();
                break;
            case 34:
                printf("%i DefineButton2\n", tv);
                buf += rh->length();
                break;
            case 35:
                printf("%i DefineBitsJPEG3\n", tv);
                buf += rh->length();
                break;
            case 36:
                printf("%i DefineBitsLossless2\n", tv);
                buf += rh->length();
                break;
            case 37:
                printf("%i DefineEditText\n", tv);
                buf += rh->length();
                break;
            case 39:
                printf("%i DefineSprite\n", tv);
                buf += rh->length();
                break;
            case 43:
                printf("%i FrameLabel\n", tv);
                buf += rh->length();
                break;
            case 45:
                printf("%i SoundbufHead2\n", tv);
                buf += rh->length();
                break;
            case 46:
                printf("%i DefineMorphShape\n", tv);
                buf += rh->length();
                break;
            case 48:
                printf("%i DefineFont2\n", tv);
                buf += rh->length();
                break;
            case 56:
                //printf("%i ExportAssets\n", tv);
                //buf += rh->length();
                
                t = new ExportAssets;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 57:
                //printf("%i ImportAssets\n", tv);
                //buf += rh->length();
                
                t = new ImportAssets;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 58:
                printf("%i EnableDebugger\n", tv);
                buf += rh->length();
                break;
            case 59:
                printf("%i DoInitAction\n", tv);
                buf += rh->length();
                break;
            case 60:
                printf("%i DefineVideobuf\n", tv);
                buf += rh->length();
                break;
            case 61:
                printf("%i VideoFrame\n", tv);
                buf += rh->length();
                break;
            case 62:
                printf("%i DefineFontInfo2\n", tv);
                buf += rh->length();
                break;
            case 64:
                printf("%i EnableDebugger2\n", tv);
                buf += rh->length();
                break;
            case 65:
                printf("%i ScriptLimits\n", tv);
                buf += rh->length();
                break;
            case 66:
                printf("%i SetTabIndex\n", tv);
                buf += rh->length();
                break;
            case 69:
                //printf("%i FileAttributes\n", tv);
                //buf += rh->length();

                t = new FileAttributes;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 70:
                //printf("%i PlaceObject3\n", tv);
                //buf += rh->length();

                t = new PlaceObject3(*header.versionPtr());
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 71:
                //printf("%i ImportAssets2\n", tv);
                //buf += rh->length();
                
                t = new ImportAssets2;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 73:
                printf("%i DefineFontAlignZones\n", tv);
                buf += rh->length();
                break;
            case 74:
                printf("%i CSMTextSettings\n", tv);
                buf += rh->length();
                break;
            case 75:
                printf("%i DefineFont3\n", tv);
                buf += rh->length();
                break;
            case 76:
                //printf("%i SymbolClass\n", tv);
                //buf += rh->length();
                
                t = new SymbolClass;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 77:
                printf("%i Metadata\n", tv);
                buf += rh->length();
                break;
            case 78:
                //printf("%i DefineScalingGrid\n", tv);
                //buf += rh->length();
                
                t = new DefineScalingGrid;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 82:
                printf("%i DoABC\n", tv);
                buf += rh->length();
                break;
            case 83:
                printf("%i DefineShape4\n", tv);
                buf += rh->length();
                break;
            case 84:
                printf("%i DefineMorphShape2\n", tv);
                buf += rh->length();
                break;
            case 86:
                printf("%i DefineSceneAndFrameLabelData\n", tv);
                buf += rh->length();
                break;
            case 87:
                printf("%i DefineBinaryData\n", tv);
                buf += rh->length();
                break;
            case 88:
                printf("%i DefineFontName\n", tv);
                buf += rh->length();
                break;
            case 89:
                printf("%i StartSound2\n", tv);
                buf += rh->length();
                break;
            case 90:
                printf("%i DefineBitsJPEG4\n", tv);
                buf += rh->length();
                break;
            case 91:
                printf("%i DefineFont4\n", tv);
                buf += rh->length();
                break;

            default:
                printf("\nEncountered unknown tag\n");
                buf += rh->length();
                break;
        }

        //t -> recordHeader = rh;
        //t -> fromSWF(buf);
        //t -> setVersion(<#unsigned char *version#>)

        vrh.push_back( *rh );
        //vt.push_back( *t );
    } while ( rh->type() != 0 );
}

void swf::SWF::toSWF() {}
