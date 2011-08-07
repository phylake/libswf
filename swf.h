#ifndef SWF_H
#define SWF_H

#include <stdio.h>
#include "types.h"

namespace swf {
	
class TagBase {
protected:
	TagBase *next;
	TagBase *prev;
	char * _buffer;
public:

};

class SWFHeader : public TagBase {
	bool _compressed;
	U8 _version;
	U32 _fileLength;
	RECT _frameSize;
	U16 _frameRate;
	U16 _frameCount;
public:
	void fromSWF(char * stream);
	void continueWith(char *& stream);
	
	//
	//accessors
	//
	bool compressed();
	U32	fileLength();
	RECT frameSize();
};

class SWF {
	char ** _buffer;
public:
	SWFHeader header;
	
	void fromSWF(char *& stream);
	void continueWith(char *& stream);
	
	void toSWF();
	
	//
	//accessors
	//
};

}
#endif