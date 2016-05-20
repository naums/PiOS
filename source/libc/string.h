/**
 * \file interface to string.s
 * \author Stefan Naumann
 * \date 30.01.2016
 **/

#ifndef BASE_C_STRING_H
#define BASE_C_STRING_H

// todo: move into types.h ?
typedef unsigned int size_t;

void * memcpy  ( void* target,     void* source,     size_t size );
void * memset  ( void* target,     int value,        size_t size );
int    memcmp  ( void* target,     void* source,     size_t size );
char * strcpy  ( char* ptr1,       const char* ptr2 );
char * strncpy ( char* ptr1,       const char* ptr2, size_t size );
size_t strlen  ( char* ptr );
char * strcat  ( char* ptr1,       const char* ptr2 );
int    strcmp  ( const char* ptr1, const char* ptr2 );

#endif

