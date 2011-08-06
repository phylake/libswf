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
	//011110000000000000000101010111110000000000000000	
	
	unsigned int OUT_SIZE = swf.header.fileLength().toBE();
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
		
		swf.continueWith((char *)outBuf);
	} else {
		swf.continueWith();
	}
		
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

unsigned int getN32Bits(char * ptr, unsigned int n, unsigned int startAt = 0) {
	unsigned int ret = 0;
	unsigned int i = 0;
	unsigned int pos;
	unsigned int t;
	unsigned int scale;
	
	//pointer adjustment
	ptr += (unsigned int) floor(startAt / 8);
	
	//scaling
	startAt = startAt % 8;
	n = n % 32;
	
	do {
		scale = (i + startAt) % 8;
		
		/* shift sizeof(char) * 8 - 1 scaled by i plus a starting position */
		pos = 8 - 1 - scale;
		if( i > 0 && scale == 0 )
		{
			ptr++;
		}
		
		t = *ptr >> pos;
		
		t &= 1;
		printf("%i", t);
		
		ret <<= 1;
		ret |= t & 1;
	} while (++i < n);
	
	return ret;
}

void test() {
	char ar[9];
	ar[0] = 0x78;
	ar[1] = 0x00;
	ar[2] = 0x05;
	ar[3] = 0x5f;
	ar[4] = 0x00;
	ar[5] = 0x00;
	ar[6] = 0x0f;
	ar[7] = 0xa0;
	ar[8] = 0x00;
	
	
	unsigned int ret;
	ret = getN32Bits(&ar[0], 5, 0);printf("\n");
	ret = getN32Bits(&ar[0], 15, 5);printf("\n");
	ret = getN32Bits(&ar[0], 15, 20);printf("\n");
	ret = getN32Bits(&ar[0], 15, 35);printf("\n");
	ret = getN32Bits(&ar[0], 15, 50);printf("\n");
}

int main (int argc, char * const argv[]) {
	char * buf;
	unsigned int * bufLen;
	
	startWithFile(argv[0], buf, bufLen);
	Parser::fsmParseSWF((char *)buf, *bufLen);
	
	return 0;
}