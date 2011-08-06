#include "swf.h"

//-----------------------------------------
//                SWFHeader
//-----------------------------------------
void swf::SWFHeader::fromSWF( char * stream) {
	_buffer = stream;
	_compressed = stream[0] == 'C';
	
	stream += 4;
	_fileLength.fromSWF(stream);
	printf("fileLength %i\n", _fileLength.toBE());
}

void swf::SWFHeader::continueWith(char * stream) {
	if( stream != 0 ) {
		_buffer	= stream;
	}
	
	_frameSize.fromSWF(_buffer);
}

swf::U32 swf::SWFHeader::fileLength() {
	return _fileLength;
}

bool swf::SWFHeader::compressed() {
	return _compressed;
}

//-----------------------------------------
//                  SWF
//-----------------------------------------
void swf::SWF::fromSWF(char *& stream) {
	*_buffer = stream;
	header.fromSWF(stream);
}

void swf::SWF::continueWith(char *& stream) {
	*_buffer = stream;
	header.continueWith(stream);
}

void swf::SWF::toSWF() {}
