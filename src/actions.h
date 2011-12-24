//
//  actions.h
//  libswf
//
//  Created by Brandon Cook on 11/6/11.
//  Copyright 2011 Brandon Cook. All rights reserved.
//

#ifndef libswf_actions_h
#define libswf_actions_h

#include "data.h"

namespace swf {
    //-----------------------------------------
    //              ActionRecord
    //-----------------------------------------
    class ActionRecord : public AbstractData {
    public:
        U8 action_code_;
        U16 length_;
        
        virtual void fromSWF( buf_type *& buf );
        unsigned getTotalLength();
    };
}

#endif //libswf_actions_h
