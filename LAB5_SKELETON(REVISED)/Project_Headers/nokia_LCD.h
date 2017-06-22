#ifndef NOKIA_LCD_H
#define NOKIA_LCD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//********************************************************************
//
//            General Function Definitions
//
//********************************************************************
typedef enum {FontSmall, FontMedium, FontLarge} FontSize; 

// Only on Elecfreaks model
void lcd_pwmOn(void);
void lcd_pwmOff(void);

void lcd_initialise(void);
void lcd_setContrast(uint8_t setting);
void lcd_clear(int color);
void LCDPrintLogo(void);

void lcd_drawPixel(int x, int y, int color);
void lcd_drawLine(int x0, int y0, int x1, int y1, int color);
void lcd_drawRect(int x0, int y0, int x1, int y1, int fill, int color);
void lcd_drawCircle(int x, int y, int radius, int color, int sectors);
void lcd_write130x130bmp(uint8_t bmp[131*131]);
void lcd_putChar(char c, int x, int y, FontSize fontSize, int fColor, int bColor);
void lcd_putStr(const char *pString, int x, int y, FontSize fontSize, int fColor, int bColor);

//********************************************************************
//
//               LCD Dimension Definitions
//
//********************************************************************
#define ROW_LENGTH   132
#define COL_HEIGHT   132
#define ENDPAGE      132
#define ENDCOL       130

// Usable area
#define LCD_X_MIN         (1)
#define LCD_X_MAX         (130)
#define LCD_Y_MIN         (1)
#define LCD_Y_MAX         (130)

//*******************************************************
//            12-Bit Color Definitions
//*******************************************************
#define BLACK     0x000
#define NAVY      0x008
#define BLUE      0x00F
#define TEAL      0x088
#define EMERALD   0x0C5
#define GREEN     0x0F0
#define CYAN      0x0FF
#define SLATE     0x244
#define INDIGO    0x408
#define TURQUOISE 0x4ED
#define OLIVE     0x682
#define MAROON    0x800
#define PURPLE    0x808
#define GRAY      0x888
#define SKYBLUE   0x8CE
#define BROWN     0xB22
#define CRIMSON   0xD13
#define ORCHID    0xD7D
#define RED       0xF00
#define MAGENTA   0xF0F
//#define ORANGE    0xF40
#define PINK      0xF6A
#define CORAL     0xF75
#define SALMON    0xF87
#define ORANGE    0xFA0
#define GOLD      0xFD0
#define YELLOW    0xFF0
#define WHITE     0xFFF

//*******************************************************
//       Circle Definitions
//*******************************************************
#define SECTOR_0_45          0x01
#define SECTOR_45_90         0x02
#define SECTOR_90_135        0x04
#define SECTOR_135_180       0x08
#define SECTOR_180_225       0x10
#define SECTOR_225_270       0x20
#define SECTOR_270_315       0x40
#define SECTOR_315_360       0x80

#define QUAD_NORTH_EAST   (SECTOR_0_45|SECTOR_45_90)
#define QUAD_NORTH_WEST   (SECTOR_90_135|SECTOR_135_180)
#define QUAD_SOUTH_WEST   (SECTOR_180_225|SECTOR_225_270)  
#define QUAD_SOUTH_EAST   (SECTOR_270_315|SECTOR_315_360)

#define FULLCIRCLE    (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST)
#define OPENSOUTH     (QUAD_NORTH_EAST|QUAD_NORTH_WEST)
#define OPENNORTH     (QUAD_SOUTH_EAST|QUAD_SOUTH_WEST)
#define OPENEAST      (QUAD_NORTH_WEST|QUAD_SOUTH_WEST)
#define OPENWEST      (QUAD_NORTH_EAST|QUAD_SOUTH_EAST)
#define OPENNORTHEAST (QUAD_NORTH_WEST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST)
#define OPENNORTHWEST (QUAD_NORTH_EAST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST)
#define OPENSOUTHEAST (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_WEST)
#define OPENSOUTHWEST (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_EAST)

#ifdef __cplusplus
}
#endif

#endif /* NOKIA_LCD_H */
