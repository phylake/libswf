#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <zlib.h>

#include "swf.h"
#include "fsm_swf.h"
#include "swfamf.h"

//static const unsigned int zlibBufferSize = 256000;//256K
static const unsigned short int uncompressedBytes = 8;

static void dumpBuf(unsigned char * const buf, unsigned int size) {
	for (int i = 0; i < size; i++)
	{
		if (i > 0 && i % 16 == 0) {
			printf("\n");
		}
		
		//printf("%02x ", *(buf + i) & 0xFF);
		//printf("%02x ", buf[i] & 0xFF);
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

static int startWithFile(const char * file, char *& finalBuffer, unsigned int *& finalBufferLength) {
	//
	//open file
	//
	int fd = open("the.swf", O_RDONLY);
	switch (fd) {
		case EACCES:
			printf("EACCES %i\n", fd);
			return 0;
			break;
		case -1:
			printf("err open\n");
			return 0;
			break;
			//default: break;
	}
	printf("fd: %i\n", fd);
	
	//
	//fstat
	//
	struct stat statbuf;
	fstat(fd, &statbuf);
	
	const off_t IN_SIZE = statbuf.st_size;
	unsigned char inBuf[IN_SIZE];
	
	//
	//read file
	//
	int rd = read(fd, inBuf, IN_SIZE);
	close(fd);
	
	if( rd != IN_SIZE ) {
		printf("read bytes != file stat st_size\n");
		return 0;
	}
	
	if( rd == -1 ) {
		printf("no bytes read");
		return 0;
	}
	printf("IN_SIZE %lli\n", IN_SIZE);
	
	printf("\n\ninBuf\n");
	dumpBuf(inBuf, IN_SIZE);
	
	char * temp;
	temp = (char *)&inBuf[0];
	
	swf::SWF swf;
	swf.fromSWF(temp);
	
	//
	//zlib
	//
	//swf::SWFHeader sh;
	//sh.fromSWF((char *)inBuf);
	//78 00 05 5f 00 00 0f a0 00 00 0c 01 00 44 11 08
	//01111
	//000000000000000
	//010101011111000
	//000000000000000
	//001111101000000
	
	unsigned int OUT_SIZE = swf.header.fileLength().getValue();
	unsigned char outBuf[OUT_SIZE - uncompressedBytes];//known uncompressed size - already uncompressed bytes
	
	if ( swf.header.compressed() ) {
		printf("OUT_SIZE %i\n", OUT_SIZE);
		int zRet;
		
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		
		/* begin with compressed bytes (8 bytes in) */
		strm.avail_in = IN_SIZE - uncompressedBytes;
		strm.next_in = &inBuf[uncompressedBytes];
		
		zRet = inflateInit(&strm);
		printf("inflateInit %i\n", zRet);
		if (zRet != Z_OK) {
			printf("zRet != Z_OK\n");
			return zRet;
		}
		
		strm.avail_out = OUT_SIZE;// - uncompressedBytes;
		strm.next_out = outBuf;//&outBuf[ uncompressedBytes ];
		
		zRet = inflate(&strm, Z_FINISH);
		printf("inflate %i\n", zRet);
		(void)inflateEnd(&strm);
		
		printf("\n\noutBuf\n");
		dumpBuf(outBuf, OUT_SIZE);
		
		temp = temp = (char *)&outBuf[0];
		//swf.continueWith((char *)outBuf);
	} else {
		//swf.continueWith();
	}
	swf.continueWith(temp);
	
	finalBuffer = (char *)outBuf;
	finalBufferLength = &OUT_SIZE;
	return 0;
}

static unsigned int rotateBits(unsigned int value, int n) {
	unsigned int result, bits;
	
	if( n > 0 )
		n = n % 32;
	else
		n = -(-n % 32);
	
	if( n == 0 ) return value;
	
	else if( n > 0 )
	{
		bits = value >> (32 - n);
		result = value << n | bits;
	}
	else
	{
		n = -n;
		bits = value << (32 - n);
		result = value >> n | bits;
	}
	return result;
}

void test() {
	/*
	 1-5-10
	 1-8-23
	 */
	
	//1: 0x3c00
	//-2: 0xc000
	
	unsigned ss = sizeof(unsigned short) * 8;
	unsigned sf = sizeof(float) * 8;
	
	//unsigned short value = 0x3555;
	unsigned short value = 0x3c00;
	
	if ( !(value & 0x3ff) ) return;
	
	int s = (value << 16) & 0x80000000;
	int e = (value >> 10) & 0x1f;
	int m = value & 0x3ff;
	
	if( 0 ) {
		int h = 0;
		while( !(m & 0x400) ) {
			m <<= 1;
			h++;
		}
		m &= 0x3ff;
		e -= h;
		e += 0x6f;
	} else {
		e += 112;
	}

	
	unsigned ret = 0;
	//ret |= s;
	ret = (value & 0x8000 ) << 16;
	ret |= e << 16;
	ret |= m << 13;
	
	float f = *(float*)&ret;
	if( f ) {
	}
}

int main (int argc, char * const argv[]) {
	//test(); return 0;
	
    char * buf;
	unsigned int * bufLen;
	
	startWithFile(argv[0], buf, bufLen);
	//Parser::fsmParseSWF((char *)buf, *bufLen);
	
	return 0;
}