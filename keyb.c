//#define KeyboardTest
//-------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// +---------------------------------------------------------------------+ //
// |             Zjisteni Scan a Ascii kodu stisknutych klaves           | //
// |                         -------------------                         | //
// |                       Verze 1.05 Build 061109                       | //
// |                           by Thomas Hudger                          | //
// |                            (c) 2005-2006                            | //
// |                                                                     | //
// +---------------------------------------------------------------------+ //
// |                                                                     | //
// +---------------------------------------------------------------------+ //
//                                                                         //
//   Version history:                                                      //
//    1.05    ...                                                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//


#ifdef CompileObj
#include <dos.h>
#endif

#ifdef KeyboardTest
#include <dos.h>
#include <conio.h>
#endif

#include "shorttyp.h"
#include "keyb.h"


//------------------------------------------------------------------------
// Zjisti stisknutou klavesu, pokud neni klavesa stisknuta, ceka.
//------------------------------------------------------------------------
void GetKey()
{ uchar S,A;
  asm{ MOV AH,0x10
       INT 0x16
       MOV A,AL
       MOV S,AH
     }
  Ascii=A;
  Scan=S;
}


//------------------------------------------------------------------------
// Vraci 128, pokud je zapnuty prepisovaci mod  ( stisknuto Insert )
//------------------------------------------------------------------------
uchar Insert()
{ return(peekb(0,0x0417) & 128);
}


//------------------------------------------------------------------------
// Vraci stav klaves SHIFT, 0=zadny
//------------------------------------------------------------------------
uchar Shift()
{ return(peekb(0,0x0417) & 3);
}



#ifdef KeyboardTest
void main()
{ while(1)
  { if(kbhit())
    { GetKey();
      cprintf("Ascii: %Xh",Ascii);
      cprintf("        Scan: %Xh",Scan);
      cprintf("        Char: %c \r\n",Ascii);
      if(kbESC) break;
    }
  }
}
#endif
