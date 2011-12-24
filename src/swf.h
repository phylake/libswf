//
//  swf.h
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_swf_h
#define libswf_swf_h

#include <iostream>
#include <vector>
#include <math.h>

#include "libswf.h"
#include "util.h"
#include "data.h"
#include "display_list.h"
#include "control.h"
#include "actions.h"
#include "shapes.h"
#include "gradients.h"

namespace swf {
    
    //-----------------------------------------
    //                 SWFHeader
    //-----------------------------------------
    class SWFHeader : public AbstractTag {
        bool _compressed;
        U8 version__;
        MutableVersion * version_;
        U32 _fileLength;
        RECT _frameSize;
        U16 _frameRate;
        U16 _frameCount;
    
    public:
        virtual void fromSWF(buf_type *& buf);
        void continueWith(buf_type *& buf);

        //
        //accessors
        //
        bool compressed();
        U8 version();
        MutableVersion * versionPtr();
        U32 fileLength();
        RECT frameSize();
        U16 frameRate();
        U16 frameCount();
    };

    //-----------------------------------------
    //             1 ShowFrame
    //-----------------------------------------
    class ShowFrame : public AbstractTag {
    public:/* nothing here. only contains a record header */
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //             4 PlaceObject
    //-----------------------------------------
    class PlaceObject : public AbstractTag {
        U16 characterId;
        U16 depth;
        MATRIX matrix;
        //CXFORM colorTransform;
    public:
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //             9 SetBackgroundColor
    //-----------------------------------------
    class SetBackgroundColor : public AbstractTag {
    public:
        RGB color;
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //             26 PlaceObject2
    //-----------------------------------------
    /*
     version: >= SWF 3
     */
    class PlaceObject2 : public AbstractVTag {
    protected:
        bool hasClipActions;//>= SWF 5
        bool hasClipDepth;
        bool hasName;
        bool hasRatio;
        bool hasColorTransform;
        bool hasMatrix;
        bool hasCharacter;
        bool moves;

        U16 depth;
        U16 characterId;
        MATRIX matrix;
        CXFORMWITHALPHA colorTransform;
        U16 ratio;
        String name;
        U16 clipDepth;
        ClipActions clipActions;
    public:
        PlaceObject2(Version & version);
        virtual void fromSWF(buf_type *& buf);
    };
    
    //-----------------------------------------
    //             56 ExportAssets
    //-----------------------------------------
    class ExportAssets : public TagNameBase {
    public:
        virtual void fromSWF(buf_type *& buf);
    };
    
    //-----------------------------------------
    //             57 ImportAssets
    //-----------------------------------------
    class ImportAssets : public TagNameBase {
    public:
        String url;
        
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //             69 FileAttributes
    //-----------------------------------------
    class FileAttributes : public AbstractTag {
        bool _useDirectBlit;
        bool _useGPU;
        bool _hasMetadata;
        bool _isAS3;
        bool _useNetwork;
    public:
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //             70 PlaceObject3
    //-----------------------------------------
    /*
     version: >= SWF 8
     */
    class PlaceObject3 : public PlaceObject2 {
    protected:
        /* //< inherited */

        //< bool hasClipActions;
        //< bool hasClipDepth;
        //< bool hasName;
        //< bool hasRatio;
        //< bool hasColorTransform;
        //< bool hasMatrix;
        //< bool hasCharacter;
        //< bool moves;
        //reserved UB[3]
        bool hasImage;
        bool hasClassName;
        bool hasCacheAsBitmap;
        bool hasBlendMode;
        bool hasFilterList;

        //< U16 depth;
        String className;
        //< U16 characterId;
        //< MATRIX matrix;
        //< CXFORMWITHALPHA colorTransform;
        //< U16 ratio;
        //< String name;
        //< U16 clipDepth;
        FilterList surfaceFilterList;
        U8 blendMode;
        U8 bitmapCache;
        //< ClipActions clipActions;

    public:
        PlaceObject3(Version & version);
        virtual void fromSWF(buf_type *& buf);
    };
    
    //-----------------------------------------
    //             71 ImportAssets2
    //-----------------------------------------
    class ImportAssets2 : public ImportAssets {
    public:
        //reserved U8
        //reserved U8
        
        virtual void fromSWF(buf_type *& buf);
    };
    
    //-----------------------------------------
    //             76 SymbolClass
    //-----------------------------------------
    class SymbolClass : public TagNameBase {
    public:
        virtual void fromSWF(buf_type *& buf);
    };
    
    //-----------------------------------------
    //           78 DefineScalingGrid
    //-----------------------------------------
    class DefineScalingGrid : public AbstractTag {
    public:
        U16 characterId;
        RECT splitter;
        
        virtual void fromSWF(buf_type *& buf);
    };

    //-----------------------------------------
    //                    SWF
    //-----------------------------------------
    class SWF {
        buf_type ** _buffer;
    public:
        SWFHeader header;
        
        virtual void fromSWF(buf_type *& buf);
        void continueWith(buf_type *& buf);

        void toSWF();

        //
        //accessors
        //
    };

}
#endif
