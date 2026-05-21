#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			2		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
	uint8_t Data[32];						//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
extern const uint8_t book[];
extern const uint8_t chess[];
extern const uint8_t tool[];
extern const uint8_t ear[];
extern const uint8_t badminton[];
extern const uint8_t brush[];
extern const uint8_t caravan[];
extern const uint8_t fish[];
extern const uint8_t game[];
extern const uint8_t music[];
extern const uint8_t shopping[];
extern const uint8_t telephone[];
extern const uint8_t telescope[];
extern const uint8_t left[];
extern const uint8_t right[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
