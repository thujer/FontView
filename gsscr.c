#define uchar unsigned char

#ifdef CompileObj
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#endif

#include "shorttyp.h"
#include "gsscr.h"

uchar far *ScrBuf[ScrBuffers];  // Obrazovkove buffery
uchar ScrBuf_ix=0;              // Index pro obrazove buffery


//-----------------------------------------------------------------------
// Vyhradi pamet pro pouzivani funkci GetScr, SetScr, GetScreen, SetScreen
// Pokud neni pozadovana pamet v dispozici, vyhlasi chybu
//------------------------------------------------------------------------
void Scr_InitGSVideo()
{
	char B;

  ScrBuf_ix=0;

  for(B=0; B<ScrBuffers; B++)
    ScrBuf[B]=0;
}



//-----------------------------------------------------------------------
// Uvolni pamet alokovanou procedurou InitGSVideo()
//------------------------------------------------------------------------
void DoneGSVideo()
{
	char B;

  ScrBuf_ix=0;

  for(B=0; B<ScrBuffers; B++)
  {
  	if(ScrBuf[B]!=0)
        farfree(ScrBuf[B]);

    ScrBuf[B]=0;
  }
}


//-----------------------------------------------------------------------
// Ulozi do bufferu Buf vyrez obrazovky dany rozmery x0, y0, x1, y1
//------------------------------------------------------------------------
void GetScr(char x0,char y0,char x1,char y1)
{
	char x,y;
  int bufsize;

  bufsize=(x1-x0+1)*2 + (y1-y0+1)*(x1-x0+1)*2;  // Vypocet velikosti vyrezu

  //gotoxy(2,1);
  //printf("x0:%3i",x0);
  //printf("x1:%3i",x1);
  //printf("y0:%3i",y0);
  //printf("y1:%3i",y1);
  //printf("Buf:%3i",bufsize);


  ScrBuf[ScrBuf_ix]=0;
  ScrBuf[ScrBuf_ix]=(char far *) farmalloc(bufsize);
  if(ScrBuf[ScrBuf_ix]==0) printf("\r\nERROR-GetScr: Nelze alokovat pamet pro ulozeni obrazu. Index: %2d",ScrBuf_ix);
  else
  { for (y=y0; y<y1; y++)
    { for (x=x0; x<x1; x++)
      { ScrBuf[ScrBuf_ix][(x-x0)*2 + (y-y0)*(x1-x0)*2]=peekb(0xB800,x*2 + y*80*2);
        ScrBuf[ScrBuf_ix][(x-x0)*2 + (y-y0)*(x1-x0)*2 + 1]=peekb(0xB800,x*2 + y*80*2 + 1);
      }
    }
    if(ScrBuf_ix < ScrBuffers) ScrBuf_ix++;
    else cprintf("\r\nPrekrocen maximalni index pro ulozeni vyrezu !");
  }
}


//-----------------------------------------------------------------------
// Obnovi z bufferu Buf vyrez obrazovky dany rozmery x0, y0, x1, y1
//------------------------------------------------------------------------
void SetScr(char x0,char y0,char x1,char y1)
{
	uchar x,y;

  if(ScrBuf_ix == 0) printf("\r\nERROR-SetScr: Neni ulozen zadny vyrez, nelze obnovit obraz !");
  else
  { if(ScrBuf_ix > ScrBuffers) cprintf("\r\nERROR-SetScr: Prekrocen maximalni index pro obnoveni vyrezu !");
    else
    { ScrBuf_ix--;
      if(ScrBuf[ScrBuf_ix]==0) printf("\r\nERROR-SetScr: Nebyla alokovana pamet obraz nebyl ulozen! Index: %2d",ScrBuf_ix);
      else
      { for (y=y0; y<y1; y++)
        { for (x=x0; x<x1; x++)
          { pokeb(0xB800,x*2+y*80*2,    ScrBuf[ScrBuf_ix][(x-x0)*2 + (y-y0)*(x1-x0)*2    ]);
            pokeb(0xB800,x*2+y*80*2 + 1,ScrBuf[ScrBuf_ix][(x-x0)*2 + (y-y0)*(x1-x0)*2 + 1]);
          }
        }
        farfree(ScrBuf[ScrBuf_ix]);  // Uvolneni pouzite pameti
        ScrBuf[ScrBuf_ix]=0;      // Nulovani ukazatele
      }
    }
  }

  //{ if(ScrBuf_ix) ScrBuf_ix--;
  //
  //  for (y=y0;y<=y1;y++)
  //  for (x=x0;x<=x1;x++)
  //   { pokeb(0xB800,x*2+y*80*2,ScrBuf[ScrBuf_ix][x*2 + y*80*2]);
  //     pokeb(0xB800,x*2+y*80*2 + 1,ScrBuf[ScrBuf_ix][x*2 + y*80*2 + 1]); }
  //}
  //else
  //{ gotoxy(1,1); textattr(15);
  //  cprintf("\nChyba: SetScr: Buffer neni %2d inicializovan !",ScrBuf_ix-1);
  //}
}
