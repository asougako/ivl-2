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

//FUNCTIONS
void    init(void);
void    ivl2_print(int digit);
void    ivl2_set_digit(int digit, int val);
void    init_delay(void);
void    init_ivl2(void);

#endif	/* CHASY_H */

