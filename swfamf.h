#ifndef SWF_AMF_H
#define SWF_AMF_H

//turn printf()s on
#define DEBUG

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&);                 \
void operator=(const TypeName&);

#endif