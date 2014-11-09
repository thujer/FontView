
#include "shorttyp.h"


typedef struct
{
  uchar PosX;
  uchar PosY;
  uchar Width;
  uchar Height;
  uchar Color;
  uchar Shadow;
  uchar Type;
  uchar Window;
  uchar ShowHint;
  uchar NO_Frames;
  uchar *DownMenu;
  uchar *Title;

} tFBorder;

extern tFBorder FBorder;


void  Scr_InitDownMenu(char S[]);
void  Scr_CenterText(uchar Y, uchar *T);
void  Scr_Frame(char x0,char y0,char x1,char y1,char Attr,char Shadow);
void  Scr_DrawFrame();
void  Scr_RefreshFrame();
void  Scr_DestructFrame();
void  Scr_PutChar(char X,char Y,char Atr,char Ch);
void  Scr_RolWindowUP(uchar _X0,uchar _Y0,uchar _X1,uchar _Y1);
uchar Scr_Confirm(uchar Type, uchar Width, uchar Height, uchar *Title, uchar *Text, uchar Attr, uchar Shadow );
uchar Scr_Message(uchar *Text);
void  Scr_ShowHint(uchar *Hint);
void  Scr_InitScreen(uchar *Text);
void  Scr_Init();
