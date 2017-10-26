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

int cycle_mode;
int cycle_speed;
int red_duty;
int green_duty;
int blue_duty;

//GLOBALS
int g_sec01;
int g_sec10;
int g_min01;
int g_min10;

//FUNCTIONS
void    init(void);
void    init_delay(void);
void    init_ivl2(void);
void    init_clock();
void    micro_delay(int delay);
void    led_set(void);
void    ivl2_set_digit(int index, int val);



#endif	/* CHASY_H */

