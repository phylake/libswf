#ifndef SWF_H
#define SWF_H

#include <stdio.h>
#include <vector.h>
#include "swfamf.h"
#include "math.h"

#define buf_type unsigned char

namespace swf
{
    static unsigned getUBits(buf_type * ptr, unsigned int n, unsigned int startAt);
    static unsigned getUBits(    char * ptr, unsigned int n, unsigned int startAt);
    static   signed getSBits(buf_type * ptr, unsigned int n, unsigned int startAt);
    static   signed getSBits(    char * ptr, unsigned int n, unsigned int startAt);
    
	//--------------------------------------------------------------
	//
	//			               ABSTRACT BASE
	//
	//--------------------------------------------------------------
	
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
    
	//--------------------------------------------------------------
	//
	//			               DATA TYPES
	//
	//--------------------------------------------------------------
	
    //----------------------------------------------------
	//                       General
	//----------------------------------------------------
    
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
	
	//-----------------------------------------
	//                   U8
	//-----------------------------------------
	class U8 : public AbstractData {
		unsigned char value;
	public:
		virtual void fromSWF( buf_type *& buf );
		unsigned char getValue();
	};
	
	//-----------------------------------------
	//                   U16
	//-----------------------------------------	
	class U16 : public AbstractData {
		unsigned short int value;
	public:
		virtual void fromSWF( buf_type *& buf );
		double toFixed8();
		float toFloat();
		unsigned short int getValue();
	};
	
	//-----------------------------------------
	//                   U32
	//-----------------------------------------	
	class U32 : public AbstractData {
		unsigned int value;
	public:
        virtual void fromSWF( buf_type *& buf );
		
        double toFixed16();//TODO double?
		float toFloat();
        
		unsigned int getValue();
        void setValue(unsigned int value);
	};
    
    //-----------------------------------------
    //                 String
    //-----------------------------------------
	class String : public AbstractData {
        std::string value;
    public:
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
        unsigned char nTranlateBits;
        U32 translateX;
        U32 translateY;
	public:
		virtual void fromSWF( buf_type *& buf );
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
    
    //----------------------------------------------------
	//                       Actions
	//----------------------------------------------------
    
    //-----------------------------------------
	//              ActionRecord
	//-----------------------------------------
    class ActionRecord : public AbstractData {
    public:
        U8 actionCode;
        U16 length;
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //----------------------------------------------------
	//                       DisplayList
	//----------------------------------------------------
    
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
    class ClipActionRecord : public AbstractData {
    public:
        ClipEventFlags eventFlags;
        U32 size;
        U8 keyCode;
        std::vector<ActionRecord> actions;
        
        virtual void fromSWF( buf_type *& buf );
    };
    
    //-----------------------------------------
	//              ClipActions
	//-----------------------------------------
    class ClipActions : public AbstractVData {
    public:
        U16 reserved;
        ClipEventFlags allEventFlags;
        std::vector<ClipActionRecord> records;
        U16 endFlag5;
        U32 endFlag6;
        
        ClipActions(Version & version);
        virtual void fromSWF( buf_type *& buf );
    };
    
    //--------------------------------------------------------------
	//
	//			                   TAGS
	//
	//--------------------------------------------------------------
	
	//-----------------------------------------
	//               RecordHeader
	//-----------------------------------------
	class RecordHeader {
		U16 tagCodeAndLength;
		U32 tagLength;
		short int tag;
		bool isShort;
	public:
		virtual void fromSWF( buf_type *& buf );
		short int type();
		unsigned int length();
	};
	
	//-----------------------------------------
	//                AbstractTag
	//-----------------------------------------
	class AbstractTag : public AbstractBase {
	protected:
        //DISALLOW_COPY_AND_ASSIGN(AbstractTag)
		buf_type * _buffer;
	public:
		RecordHeader * recordHeader;
		virtual ~AbstractTag() = 0;
	};
    
    //-----------------------------------------
	//               AbstractVTag
	//-----------------------------------------
	class AbstractVTag : public AbstractTag, protected VersionRequirement {
	public:
        AbstractVTag(Version & version);
	};
	
	//-----------------------------------------
	//                 SWFHeader
	//-----------------------------------------
	class SWFHeader : public AbstractTag {
		bool _compressed;
		U8 __version;
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
        unsigned char * versionPtr();
		U32	fileLength();
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
	class PlaceObject2 : public AbstractTag {
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