#define uchar unsigned char

#define Cursor_Buffers 7

// Ukladani pozice kurzoru - automaticky index
uchar Cursor_X_a[Cursor_Buffers];
uchar Cursor_Y_a[Cursor_Buffers];
uchar Cursor_ix=0;

// Ukladani pozice kurzoru - pevny index
uchar Cursor_X[Cursor_Buffers];
uchar Cursor_Y[Cursor_Buffers];

//------------------------------------------------------------------------
// Zapise do Cursor_X_a[] a Cursor_Y_a[] aktualni pozici kurzoru
//------------------------------------------------------------------------
void SaveCursorPosAuto()
{ uchar _x,_y;

  if(Cursor_ix < Cursor_Buffers)
  { asm{ MOV  AH,3          // Nacteni pozice kurzoru
         MOV  BH,0
         INT  0x10
         MOV  _x,DL
         MOV  _y,DH
       }

    Cursor_X_a[Cursor_ix]=_x;
    Cursor_Y_a[Cursor_ix]=_y;
    Cursor_ix++;
  }
  else Cursor_ix=Cursor_Buffers-1;
}


void SaveCursorPos(uchar B)
{ uchar _x,_y;

  if(B < Cursor_Buffers)
  { asm{ MOV  AH,3          // Nacteni pozice kurzoru
         MOV  BH,0
         INT  0x10
         MOV  _x,DL
         MOV  _y,DH
       }

    Cursor_X[B]=_x;
    Cursor_Y[B]=_y;
    Cursor_ix++;
  }
  else cprintf("Prekroceni maximalni mozne pozice pro ulozeni pozice kurzoru!");
}


//------------------------------------------------------------------------
// Nastavi kurzor na pozici zapsanou v Cursor_X_a[] a Cursor_Y_a[]
//------------------------------------------------------------------------
void LoadCursorPosAuto()
{ uchar _x,_y;

  if(Cursor_ix < Cursor_Buffers)
  { if(Cursor_ix) Cursor_ix--;
    _x=Cursor_X_a[Cursor_ix];
    _y=Cursor_Y_a[Cursor_ix];

    asm{ MOV  AH,2
         MOV  BH,0
         MOV  DL,_x
         MOV  DH,_y
         INT  0x10
       }
  } else Cursor_ix=Cursor_Buffers-1;
}


//------------------------------------------------------------------------
// Nastavi kurzor na pozici zapsanou v promennych Cursor_X[] a Cursor_Y[]
//------------------------------------------------------------------------
void LoadCursorPos(uchar B)
{ uchar _x,_y;

  if(B < Cursor_Buffers)
  { _x=Cursor_X[B];
    _y=Cursor_Y[B];

    asm{ MOV  AH,2
         MOV  BH,0
         MOV  DL,_x
         MOV  DH,_y
         INT  0x10
       }
  }
  else cprintf("Prekroceni maximalni mozne pozice pro obnoveni pozice kurzoru!");
}


//-----------------------------------------------------------------------
// Nastavi neviditelny textovy kurzor
//------------------------------------------------------------------------
void HideTextCursor()
{ asm{ MOV AH,1
       MOV CH,20h
       MOV CL,20h
       INT 10h
     }
}


//-----------------------------------------------------------------------
// Nastavi viditelny textovy kurzor
//------------------------------------------------------------------------
void ShowTextCursor()
{ asm{ MOV AH,1
       MOV CH,15
       MOV CL,16
       INT 10h
     }
}


//------------------------------------------------------------------------
// Nastavi normalni textovy kurzor ( blikajici podtrzitko )
//------------------------------------------------------------------------
void NormalTextCursor()
{
  asm{
  MOV AH,1
  MOV CH,15
  MOV CL,16
  INT 10h }
}



//------------------------------------------------------------------------
// Nastavi textovy kurzor pro insert mod  ( blikajici plny znak )
//------------------------------------------------------------------------
void InsertTextCursor()
{
  asm{
  MOV AH,1
  MOV CH,0
  MOV CL,16
  INT 10h }
}
