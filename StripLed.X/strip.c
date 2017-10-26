
#include "chasy.h"

void    init_strip(void)
{
    /*  GPIO*/
    TRISBbits.TRISB6 = 0;   //Output
    LATBbits.LATB6 = 0;     //Low
//    ODCBbits.ODCB6 = 1;     //ODC
}

#define led_nb 8
#define color_depth 24

int32_t    led_index;
int32_t    bit_index = 0;

e_led     strip_color[led_nb];

void    set_led(uint32_t index, uint32_t red, uint32_t green, uint32_t blue)
{
    strip_color[index].g = green;
    strip_color[index].b = blue;
    strip_color[index].r = red;
}

void    set_strip(void)
{
    uint32_t    val = 0;
    
    asm volatile("di");
    for(led_index = 0; led_index < led_nb; led_index++)
    {
        val = (strip_color[led_index].r << 16) + (strip_color[led_index].g << 8) + strip_color[led_index].b;
        for (bit_index = color_depth - 1; bit_index >= 0; bit_index--)
        {
            if (((val >> bit_index) & 1u) == 0)
            {
                LATBSET = 0x40;     //as fast as possible
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                LATBCLR = 0x40;
            }
            else    //Pulse > 350ns
            {
                LATBSET = 0x40;
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                asm volatile("NOP");
                LATBCLR = 0x40;
            }
        }
    }
    asm volatile("ei");
}
