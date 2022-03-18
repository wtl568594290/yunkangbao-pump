#include	"delay.h"

/**
 * @brief ÑÓÊ±º¯Êý
 * 
 * @param ms 1-255
 */
void  Delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = FOSC / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void Delay3000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 36;
	j = 150;
	k = 212;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
