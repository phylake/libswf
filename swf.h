#ifndef SWF_H
#define SWF_H

#include <stdio.h>
#include <vector.h>
#include "swfamf.h"
#include "math.h"

namespace swf
{
	
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
		virtual void fromSWF( char *& ptr) = 0;
		virtual ~AbstractBase() = 0;
	};
	
	//--------------------------------------------------------------
	//
	//			               DATA TYPES
	//
	//--------------------------------------------------------------
	
	//-----------------------------------------
	//                AbstractData
	//-----------------------------------------
	class AbstractData : public AbstractBase {
    protected:
        bool valueSet;
	public:
		static unsigned int getUBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
		static   signed int getSBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
		
		virtual ~AbstractData() = 0;
	};
	
	//-----------------------------------------
	//                   U8
	//-----------------------------------------
	class U8 : public AbstractData {
		unsigned char value;
	public:
		virtual void fromSWF( char *& ptr );
		unsigned char toBE();
	};
	
	//-----------------------------------------
	//                   U16
	//-----------------------------------------	
	class U16 : public AbstractData {
		unsigned short int value;
	public:
		virtual void fromSWF( char *& ptr );
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
        virtual void fromSWF( char *& ptr );
		
        double toFixed16();
		float toFloat();
        
		unsigned int getValue();
        void setValue(unsigned int value);
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
	//                   RECT
	//-----------------------------------------
	class RECT : public AbstractData {
	public:
		unsigned char nBits;
		Twip xMin;
		Twip xMax;
		Twip yMin;
		Twip yMax;
		virtual void fromSWF( char *& ptr );
		unsigned int toBE();
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
		virtual void fromSWF( char *& stream );
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
		virtual void fromSWF(char *& stream);
		void continueWith(char *& stream);
		virtual ~SWFHeader();
		
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
	public:
		virtual void fromSWF(char *& stream);
		virtual ~ShowFrame();
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
		virtual void fromSWF(char *& stream);
		virtual ~FileAttributes();
	};
	
	//-----------------------------------------
	//                    SWF
	//-----------------------------------------
	class SWF {
		char ** _buffer;
	public:
		SWFHeader header;
		
		virtual void fromSWF(char *& stream);
		void continueWith(char *& stream);
		
		void toSWF();
		
		//
		//accessors
		//
	};

}
#endif