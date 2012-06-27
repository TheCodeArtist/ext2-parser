// Copyright 2009 Steve Gribble (gribble [at] cs.washington.edu)
// May 22nd, 2009
//
// This file defines some types used in other C files and headers,
// in particular defining integer types with explicit byte lengths.

#ifndef EXT2READER_INC_TYPES_H
#define EXT2READER_INC_TYPES_H

// define NULL if not already defined by compiler
#ifndef NULL
#define NULL ((void*) 0)
#endif

// true or false values
typedef int os_bool_t;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// explicitly-sized versions of integer types
typedef __signed char os_int8_t;
typedef unsigned char os_uint8_t;
typedef short os_int16_t;
typedef unsigned short os_uint16_t;
typedef int os_int32_t;
typedef unsigned int os_uint32_t;
typedef long long os_int64_t;
typedef unsigned long long os_uint64_t;

// return the offset of 'member' relative to the beginning of a struct type
#define offsetof(type, member) ((os_int32_t) (&((type*)0)->member))

#endif  // EXT2READER_INC_TYPES_H
