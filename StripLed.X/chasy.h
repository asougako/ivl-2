/* 
 * File:   chasy.h
 * Author: bocal
 *
 * Created on September 21, 2017, 11:42 AM
 */

#ifndef CHASY_H
#define	CHASY_H

//INCLUDES
#include <xc.h>

//DEFINES
#define int8_t      char
#define int16_t     short
#define int32_t     int
#define int64_t     long

#define uint8_t     unsigned char
#define uint16_t    unsigned short
#define uint32_t    unsigned int
#define uint64_t    unsigned long

#define main_loop while(1)

//TYPES
typedef struct  s_led
{
    int b;
    int r;
    int g;
}               e_led;

//GLOBALS
e_led     strip_color[8];

//FUNCTIONS




#endif	/* CHASY_H */

