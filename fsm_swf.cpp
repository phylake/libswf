
#line 1 "fsm_swf.rl"
#include <stdio.h>
#include "fsm_swf.h"


#line 17 "fsm_swf.rl"



#line 12 "fsm_swf.cpp"
static const char _swf_actions[] = {
	0, 1, 0
};

static const char _swf_key_offsets[] = {
	0, 0, 2, 3, 4
};

static const unsigned char _swf_trans_keys[] = {
	67u, 70u, 87u, 70u, 0
};

static const char _swf_single_lengths[] = {
	0, 2, 1, 1, 0
};

static const char _swf_range_lengths[] = {
	0, 0, 0, 0, 0
};

static const char _swf_index_offsets[] = {
	0, 0, 3, 5, 7
};

static const char _swf_trans_targs[] = {
	2, 2, 0, 3, 0, 4, 0, 0, 
	0
};

static const char _swf_trans_actions[] = {
	0, 0, 0, 0, 0, 1, 0, 0, 
	0
};

static const int swf_start = 1;
static const int swf_first_final = 4;
static const int swf_error = 0;

static const int swf_en_main = 1;


#line 20 "fsm_swf.rl"

void Parser::fsmParseSWF(const char *p, unsigned int len) {
	int cs;
	const char *pe = p + len;
	
	
#line 61 "fsm_swf.cpp"
	{
	cs = swf_start;
	}

#line 26 "fsm_swf.rl"
	
#line 68 "fsm_swf.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _swf_trans_keys + _swf_key_offsets[cs];
	_trans = _swf_index_offsets[cs];

	_klen = _swf_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _swf_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _swf_trans_targs[_trans];

	if ( _swf_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _swf_actions + _swf_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 8 "fsm_swf.rl"
	{
		printf("action A\n");
	}
	break;
#line 147 "fsm_swf.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 27 "fsm_swf.rl"
}