#include "swf.h"

//-----------------------------------------
//                SWFHeader
//-----------------------------------------
void swf::SWFHeader::fromSWF( char * stream) {
	_compressed = stream[0] == 'C';
	
	stream += 3;//skip 'W' and 'F'
	_version.fromSWF(stream);
	_fileLength.fromSWF(stream);
}

void swf::SWFHeader::continueWith(char *& stream) {
	_frameSize.fromSWF(stream);
	_frameRate.fromSWF(stream);
	_frameCount.fromSWF(stream);
	
	#ifdef DEBUG
	printf("fileLength %i\n", _fileLength.getValue());
	printf("frameRate %f\n", _frameRate.toFixed8());
	printf("frameCount %i\n", _frameCount.getValue());
	#endif
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
	_buffer = &stream;
	header.fromSWF(stream);
}

void swf::SWF::continueWith(char *& stream) {
	_buffer = &stream;
	//(*_buffer) += 4;//want to do this because outBuf is cropped at the moment
	header.continueWith(stream);
}

void swf::SWF::toSWF() {}
