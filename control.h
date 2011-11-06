//
//  control.h
//  libswf
//
//  Created by Brandon Cook on 11/5/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_control_h
#define libswf_control_h

#include "data.h"

namespace swf {
    //-----------------------------------------
    //             TagNameBase
    //-----------------------------------------
    class TagNameBase : public AbstractTag {
        U16 numSymbols;
        std::vector<U16 *> tags;
        std::vector<String *> names;
    public:
        virtual void tagsFromSWF(buf_type *& buf);
    };
}

#endif //libswf_control_h
