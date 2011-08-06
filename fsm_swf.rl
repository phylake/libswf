#include <stdio.h>
#include "fsm_swf.h"

%%{
	machine swf;
	alphtype unsigned char;
	
	action A{
		printf("action A\n");
	}
	action B{
		printf("action B\n");
	}
	swfheader = ('F' | 'C') 'W' 'F';
	main := swfheader @A;
	
}%%

%% write data;

void Parser::fsmParseSWF(const char *p, unsigned int len) {
	int cs;
	const char *pe = p + len;
	
	%% write init;
	%% write exec;
}