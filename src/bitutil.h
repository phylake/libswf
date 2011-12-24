//
//  bitutil.h
//  libswf
//
//  Created by Brandon Cook on 11/19/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_bitutil_h
#define libswf_bitutil_h

namespace swf {

    template <typename T>
    class BitUtil {
        T * beg_;
        T * end_;
        T * cur_;
        
        int state_;
        
    public:
        static const int kStateLastPointer = 0x1;
        static const int kMaxRead          = 0x2;
        
        BitUtil() {
            
        };
        
        void set_beg( T * value ) {
            beg_ = value;
        }
        
        void set_end( T * value ) {
            end_ = value;
        }
        
        T * cur() {
            return cur_;
        }
        
        const unsigned& operator[](unsigned const value) {
            return 0;
        }
    };
    
    
    
    /*
     * n the number of bits to read
     * startWith the bit number (1-based) to read first
     */
    template<typename T>
    T getUBits(T * buf, unsigned n, unsigned startWith) {
        T ret = 0;
        unsigned i = 0;
        unsigned pos;
        unsigned t;
        unsigned scale;
        const short bits = sizeof(*buf) * 8;
        
        //pointer adjustment
        buf += (unsigned int) startWith / bits;
        
        //scaling
        startWith = startWith % bits;
        n = n % (sizeof(n) * 8);
        
        do {
            scale = (i + startWith) % bits;
            
            /* shift sizeof(T) * 8 - 1 scaled by i plus a starting position */
            pos = bits - 1 - scale;
            if( i > 0 && scale == 0 )
            {
                buf++;
            }
            
            t = *buf >> pos;
            
#ifdef LIBSWF_DEBUG
            //printf("%i", t & 1);
#endif
            
            ret <<= 1;
            ret |= t & 1;
        } while (++i < n);
        
#ifdef LIBSWF_DEBUG
        //printf("\n");
#endif
        
        return ret;
    };

}

#endif //libswf_bitutil_h
