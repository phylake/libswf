#ifndef SWF_H
#define SWF_H

#include <stdio.h>
#include <vector.h>
#include "swfamf.h"
#include "math.h"

namespace swf
{
	static unsigned int getUBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
    static unsigned int getUBits(         char * ptr, unsigned int n, unsigned int startAt);
    static   signed int getSBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
    static   signed int getSBits(         char * ptr, unsigned int n, unsigned int startAt);
    
	//--------------------------------------------------------------
	//
	//			               ABSTRACT BASE
	//
	//--------------------------------------------------------------
	
	//-----------------------------------------
	//                AbstractBase
	//-----------------------------------------
	class AbstractBase {
	public:
		virtual void fromSWF( char *& buf ) = 0;
        //virtual void toSWF( char *& buf ) = 0;
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
	//                   U8
	//-----------------------------------------
	class U8 : public AbstractData {
		unsigned char value;
	public:
		virtual void fromSWF( char *& buf );
		unsigned char toBE();
	};
	
	//-----------------------------------------
	//                   U16
	//-----------------------------------------	
	class U16 : public AbstractData {
		unsigned short int value;
	public:
		virtual void fromSWF( char *& buf );
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
        virtual void fromSWF( char *& buf );
		
        double toFixed16();//TODO double?
		float toFloat();
        
		unsigned int getValue();
        void setValue(unsigned int value);
	};
    
    //-----------------------------------------
    //                 String
    //-----------------------------------------
	class String : AbstractData {
        std::string value;
    public:
        void virtual fromSWF( char *& buf );
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
        virtual void fromSWF( char *& buf );
        
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
		virtual void fromSWF( char *& buf );
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
		virtual void fromSWF( char *& buf );
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
		virtual void fromSWF( char *& buf );
	};
    
    //-----------------------------------------
	//              CXFORMWITHALPHA
	//-----------------------------------------
	class CXFORMWITHALPHA : public CXFORM {
        unsigned char am;
        unsigned char aa;
	public:
		virtual void fromSWF( char *& buf );
	};
    
    //----------------------------------------------------
	//                       Actions
	//----------------------------------------------------
    
    //-----------------------------------------
	//              ActionRecord
	//-----------------------------------------
    class ActionRecord : AbstractData {
    public:
        U8 actionCode;
        U16 length;
        
        virtual void fromSWF( char *& buf );
    };
    
    //----------------------------------------------------
	//                       DisplayList
	//----------------------------------------------------
    
    //-----------------------------------------
	//              ClipEventFlags
	//-----------------------------------------
    class ClipEventFlags : AbstractData {
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
        
        virtual void fromSWF( char *& buf );
    };
    
    //-----------------------------------------
	//              ClipActionRecord
	//-----------------------------------------
    class ClipActionRecord : AbstractData {
    public:
        ClipEventFlags eventFlags;
        U32 size;
        U8 keyCode;
        std::vector<ActionRecord> actions;
        
        virtual void fromSWF( char *& buf );
    };
    
    //-----------------------------------------
	//              ClipActions
	//-----------------------------------------
    class ClipActions : AbstractData {
    public:
        U16 reserved;
        ClipEventFlags allEventFlags;
        std::vector<ClipActionRecord> records;
        U16 endFlag5;
        U32 endFlag6;
        
        virtual void fromSWF( char *& buf );
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
		virtual void fromSWF( char *& buf );
		short int type();
		unsigned int length();
	};
	
	//-----------------------------------------
	//                AbstractTag
	//-----------------------------------------
	class AbstractTag : public AbstractBase {
    private:
        //AbstractTag(const AbstractTag&);
        AbstractTag& operator=(const AbstractTag&);
	protected:
		//AbstractTag *next;
		//AbstractTag *prev;
		char * _buffer;
	public:
		RecordHeader * recordHeader;
		virtual ~AbstractTag() = 0;
	};
	
	//-----------------------------------------
	//                 SWFHeader
	//-----------------------------------------
	class SWFHeader : public AbstractTag {
		bool _compressed;
		U8 _version;
		U32 _fileLength;
		RECT _frameSize;
		U16 _frameRate;
		U16 _frameCount;
	public:
		virtual void fromSWF(char *& buf);
		void continueWith(char *& buf);
		
		//
		//accessors
		//
		bool compressed();
		U32	fileLength();
		RECT frameSize();
	};
    
    //-----------------------------------------
	//             1 ShowFrame
	//-----------------------------------------
	class ShowFrame : public AbstractTag {
	public:/* nothing here. only contains a record header */
		virtual void fromSWF(char *& buf);
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
		virtual void fromSWF(char *& buf);
	};
    
    //-----------------------------------------
	//             9 SetBackgroundColor
	//-----------------------------------------
	class SetBackgroundColor : public AbstractTag {
	public:
        RGB color;
		virtual void fromSWF(char *& buf);
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
		virtual void fromSWF(char *& buf);
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
		virtual void fromSWF(char *& buf);
	};
	
	//-----------------------------------------
	//                    SWF
	//-----------------------------------------
	class SWF {
		char ** _buffer;
	public:
		SWFHeader header;
		
		virtual void fromSWF(char *& buf);
		void continueWith(char *& buf);
		
		void toSWF();
		
		//
		//accessors
		//
	};

}
#endif