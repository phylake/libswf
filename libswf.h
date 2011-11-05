#ifndef libswf_libswf_h
#define libswf_libswf_h

#define buf_type unsigned char

#define fixed8_type float
#define fixed16_type double
#define fixed_type fixed16_type

//turn printf()s on
#define DEBUG

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&);                 \
void operator=(const TypeName&);

#endif //libswf_libswf_h