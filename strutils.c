
#include "shorttyp.h"


/*========================================================================*/
/*                         Prevod znaku na velky                          */
/*------------------------------------------------------------------------*/
uchar Str_UpCase(uchar Ch)
{ if((Ch>='a') && (Ch<='z')) Ch&=(255-32);
  return(Ch);
}


/*========================================================================*/
/*                         Porovnani retezce                              */
/*------------------------------------------------------------------------*/
/* Pokud si Bytes znaku v retezcich Ptr1 a Ptr2 odpovida vrati 1          */
/*------------------------------------------------------------------------*/
uchar Str_Cmp(uchar *Ptr1, uchar *Ptr2, uchar Bytes)
{ uchar i;
  uchar Out;

  Out=1;
  for(i=0; i<Bytes; i++) if(Str_UpCase(Ptr1[i])!=Str_UpCase(Ptr2[i])) Out=0;
  return(Out);
}


/*========================================================================*/
/*                        Kopirovani pameti                               */
/*------------------------------------------------------------------------*/
/* Kopirovani Bytes byte z Source do Dest                                 */
/*------------------------------------------------------------------------*/
void Str_MemCopy(uchar *Source, uchar *Dest, uchar Bytes)
{ uchar i;
  for(i=0; i<Bytes; i++) Dest[i]=Source[i];
}


// Je-li znak cislo, pismeno, nebo znamenko vrati 1
uchar Str_IsPrint(uchar Char)
{
  if(((Char >= 'A') && (Char <= 'Z')) ||
     ((Char >= 'a') && (Char <= 'z')) ||
     ((Char >= '0') && (Char <= '9')) ||
     (Char=='+') || (Char=='-') || (Char=='#') ||
     (Char==' ') || (Char=='(') || (Char==')') ||
     (Char=='.'))
     return(1);
  else return(0);
}


// Je-li znak cislice vrati 1
uchar Str_IsNum(uchar Char)
{
  if(((Char >= '0') && (Char <= '9')) || (Char=='+')) return(1); else return(0);
}


// Vrati 1 pokud je znak hexacislo
uchar Str_IsHex(uchar Ch)
{
  if(((Ch>='A') && (Ch<='F')) ||
     ((Ch>='a') && (Ch<='f')) ||
     ((Ch>='0') && (Ch<='9')))
  {
    return(1);
  }
  else
  {
    return(0);
  }
}

// Vrati velikost retezce
uchar Str_StrSize(uchar *Str)
{ uchar i;
  i=0;
  while((Str_IsPrint(Str[i])) && (i<255)) i++;
  return(i);
}

// Vrati velikost cisla v retezci
uchar Str_StrNumSize(uchar *StrNum)
{ uchar i;
  i=0;
  while((Str_IsNum(StrNum[i])) && (i<255)) i++;
  return(i);
}

void Str_ResetBuffer(uchar *Buf, uchar Size)
{
  uchar i;

  for(i=0; i<Size; i++) Buf[i]=0;
}

