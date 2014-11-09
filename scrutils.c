
#include "shorttyp.h"
#include "gsscr.h"
#include "keyb.h"
#include "scrutils.h"


tFBorder FBorder;

//=======================================================================
void Scr_ShowHint(uchar *Hint);
void Scr_InitDownMenu(char S[]);
void Scr_CenterText(uchar Y, uchar *T);
void Scr_Frame(char x0,char y0,char x1,char y1,char Attr,char Shadow);
void Scr_DrawFrame();
void Scr_DestructFrame();
void Scr_PutChar(char X,char Y,char Atr,char Ch);
void Scr_RolWindowUP(uchar _X0,uchar _Y0,uchar _X1,uchar _Y1);
uchar Scr_Confirm(uchar Type, uchar Width, uchar Height, uchar *Title, uchar *Text, uchar Attr, uchar Shadow );
uchar Scr_Message(uchar *Text);
void Scr_InitScreen();
//=======================================================================

//------------------------------------------------------------------------
//  Vykresleni dolniho menu (klavesove zkratky)
//------------------------------------------------------------------------
void Scr_InitDownMenu(char S[])
{ uchar C,I;

  textattr(16+32+64);
  window(1,25,80,25); clrscr();
  I=0;C=0;
  gotoxy(2,1);
  while(S[I]!=0)
  { if(S[I]=='~') { C=!C; I++; continue; }
    if(C) textcolor(4);  else textcolor(0);
    putch(S[I]);
    I++;
  }
  window(1,1,80,25);
}

//------------------------------------------------------------------------
//  Zobrazi text T uprostred radku Y
//------------------------------------------------------------------------
void Scr_CenterText(uchar Y, uchar *T)
{ uchar X;

  X=80/2 - Str_StrSize(T)/2;

  gotoxy(X,Y);
  printf(T);
}


//-----------------------------------------------------------------------
// Vykresli na obrazovku ramecek s rozmery x0, y0, x1, y1
// s attributy Attr
// Pokud je Shadow = True je vykreslovan stin ramecku
//------------------------------------------------------------------------
void Scr_Frame(char x0,char y0,char x1,char y1,char Attr,char Shadow)
{ uchar x,y;

  textattr(Attr);

  for(x=x0;x<x1;x++)
  for(y=y0;y<(y1-1);y++)
  { gotoxy(x+1,y+1);putch(' ');
  }

  gotoxy(x0,y0);putch('Ú');
  for(x=0;x<(x1-x0);x++) putch('Ä');
  putch('¿');
  for(y=0;y<(y1-y0);y++)
  { textattr(Attr);
    gotoxy(x0,y0+y+1);putch('³');
    gotoxy(x1+1,y0+y+1);putch('³');
  }

  textattr(Attr);
  gotoxy(x0,y1);putch('À');
  for(x=0;x<(x1-x0);x++) putch('Ä');
  putch('Ù');

  if(Shadow)
  { for(y=0;y<(y1-y0+1);y++)
    { textcolor(7);textbackground(0);
      gotoxy(x1+2,y0+y+1);cprintf("°°");
    }
    textcolor(7);textbackground(0);
    gotoxy(x0+2,y1+1);
    for(x=0;x<(x1-x0);x++) putch('°');
  }
}


void Scr_DrawFrame()
{
  uchar center;

  if(FBorder.NO_Frames < ScrBuffers)
  {
    if(FBorder.Type)
        GetScr(FBorder.PosX-2,FBorder.PosY-1,FBorder.PosX+FBorder.Width+4,FBorder.PosY+FBorder.Height+2);
    else
        GetScr(FBorder.PosX-1,FBorder.PosY-1,FBorder.PosX+FBorder.Width+4,FBorder.PosY+FBorder.Height+2);

    if(FBorder.ShowHint)
    {
        Scr_ShowHint("\0x0");
    }

    window(1,1,80,25);
    Scr_Frame(FBorder.PosX,FBorder.PosY,FBorder.PosX+FBorder.Width+1,FBorder.PosY+1+FBorder.Height,FBorder.Color,FBorder.Shadow);
    if(FBorder.Type) { gotoxy(FBorder.PosX-1,FBorder.PosY+1); cprintf("Ã´"); }

    if(FBorder.Title[0])
    {
      center=0; while(FBorder.Title[center]) { if(center<80) center++; else break; }
      gotoxy(FBorder.PosX+(FBorder.Width/2)-(center/2)+1,FBorder.PosY); printf(" %s ",FBorder.Title);
    }

    if(FBorder.DownMenu)
    {
      Scr_InitDownMenu(FBorder.DownMenu);
    }

    textattr(FBorder.Color);
    if(FBorder.Window)
    {
      window(FBorder.PosX+1,FBorder.PosY+1,FBorder.PosX+FBorder.Width+1,FBorder.PosY+FBorder.Height);
    }

    FBorder.NO_Frames++;
  }
  else
  {
    cprintf("\r\nVytvoren maximalni pocet frames !\r\n");
  }
}


void Scr_RefreshFrame()
{
  //Scr_Frame(FBorder.PosX,FBorder.PosY,FBorder.PosX+FBorder.Width+1,FBorder.PosY+1+FBorder.Height,FBorder.Color,FBorder.Shadow);
  if(FBorder.Window)
  {
    window(FBorder.PosX+1,FBorder.PosY+1,FBorder.PosX+FBorder.Width+1,FBorder.PosY+FBorder.Height);
  }

  if(FBorder.DownMenu)
  {
    Scr_InitDownMenu(FBorder.DownMenu);
  }
}


void Scr_DestructFrame()
{
  if(FBorder.NO_Frames)
  {
    if(FBorder.Type)
      SetScr(FBorder.PosX-2,FBorder.PosY-1,FBorder.PosX+FBorder.Width+4,FBorder.PosY+FBorder.Height+2);
    else
      SetScr(FBorder.PosX-1,FBorder.PosY-1,FBorder.PosX+FBorder.Width+4,FBorder.PosY+FBorder.Height+2);

    if(FBorder.Window)
    {
      window(1,1,80,25);
    }

    FBorder.NO_Frames--;
  }
  else
  {
    cprintf("\r\nNeni vytvoren zadny frame !\r\n");
  }
}


//------------------------------------------------------------------------
// Zobrazi znak Ch na pozici X,Y s barvou Atr
//------------------------------------------------------------------------
void Scr_PutChar(char X,char Y,char Atr,char Ch)
{ gotoxy(X+1,Y+1);
  textattr(Atr);
  putch(Ch);

  //pokeb(0xB800,Y*80*2+X*2,Ch);
  //pokeb(0xB800,Y*80*2+X*2+1,Atr);
  // Je to to same, rychlejsi, ale nefunguje vzdy spolehlive
}


//------------------------------------------------------------------------
// Roluje okno _X0, _Y0, _X1, _Y1 o jeden radek nahoru
// pradne misto ma atribut dany BH
//------------------------------------------------------------------------
void Scr_RolWindowUP(uchar _X0,uchar _Y0,uchar _X1,uchar _Y1)
{  asm{ MOV  AH,6
        MOV  CH,_Y0
        MOV  CL,_X0
        MOV  DH,_Y1
        MOV  DL,_X1
        MOV  AL,1
        MOV  BH,15
        INT  0x10
      }
}


//------------------------------------------------------------------------
// Zobrazi hlasku
//------------------------------------------------------------------------
uchar Scr_Confirm(uchar Type, uchar Width, uchar Height, uchar *Title, uchar *Text, uchar Attr, uchar Shadow )
{ uchar x,y,out=0;

  x = 80/2 - Width/2;
  y = 25/2 - Height/2;

  switch(Type)
  { case 0: cprintf("%s ... ",Text);
            while(1)
            { GetKey();
              if(kbALT_X || kbESC || kbN) { out=0; break; }
              if(kbENTER || kbSPACE || kbA) { out=1; break; }
            }
            break;

    case 1: HideTextCursor();
            SaveCursorPosAuto();
            GetScr(x-2,y-1,x+Width+3,y+Height+1);
            Scr_Frame(x,y,x+Width,y+Height,Attr,Shadow);
            gotoxy(x+Width/2-Str_StrSize(Title)/2 - 1,y);
            textattr(Attr);
            cprintf(" %s ",Title);
            window(x+3,y+1,x+Width-3,y+Height-1);
            cprintf(Text);
            while(1)
            { GetKey();
              if(kbALT_X || kbESC || kbN) { out=0; break; }
              if(kbENTER || kbSPACE || kbA) { out=1; break; }
            }
            window(1,1,80,25);
            SetScr(x-2,y-1,x+Width+3,y+Height+1);
            LoadCursorPosAuto();
            break;
  }
  return(out);
}

//------------------------------------------------------------------------
// Zobrazi hlasku
//------------------------------------------------------------------------
uchar Scr_Message(uchar *Text)
{ cprintf("%s ... ",Text);
  while(1)
  { GetKey();
    if(kbALT_X || kbESC || kbN) return(0);
    if(kbENTER || kbSPACE || kbA) return(1);
  }
}


//------------------------------------------------------------------------
// Zobrazuje malou napovedu ( na radku 24 )
//------------------------------------------------------------------------
void Scr_ShowHint(uchar *Hint)
{  textattr(10+16);
   window(1,24,80,24); clrscr(); window(1,1,80,25);
   gotoxy(1,24);
   if(Hint[0]!=0) cprintf(" %s",Hint);
}

//-----------------------------------------------------------------------
//  Vykresleni zakladni obrazovky
//------------------------------------------------------------------------
void Scr_InitScreen(uchar *Text)
{
  uint I;

  textmode(3);
  textcolor(7);textbackground(0);
  window(1,1,80,25); clrscr();
  textcolor(1);textbackground(7);
  gotoxy(1,2);
  for(I=0;I<1840;I++) putch('°');
  textbackground(7);
  window(1,1,80,1); clrscr();
  window(1,1,80,25);
  textcolor(0);
  Scr_CenterText(1,Text);
  gotoxy(1,2);
}


void Scr_Init()
{
  FBorder.NO_Frames = 0;
  FBorder.ShowHint = 0;
}
