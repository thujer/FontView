//-------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// +---------------------------------------------------------------------+ //
// |                      Detekce prikazu a parametru                    | //
// |                         -------------------                         | //
// |                       Verze 1.27 Build 060829                       | //
// |                            by Tomas Hujer                           | //
// |                            (c) 2005-2006                            | //
// |                                                                     | //
// +---------------------------------------------------------------------+ //
// |  Detekuje sekvencni prikazy podle tabulky prikazu v parametru       | //
// |  CMD_TestProc(InstID, *CMDTab, Get_Char). CMDTab je pointer na tab. | //
// |  Get_Char je promenna kde se sekvencne objevuje testovany znak.     | //
// |  Funkce prochazi tabulkou a pri kazdem znaku se rozhoduje podle     | //
// |  kodu a parametru v tabulce jaka bude nasledujici akce, pokud       | //
// |  sekvence znaku prosla cestu v tabulce az ridicimu znaku ExitCode,  | //
// |  je vracena hodnota parametru. Podle navratove hodnoty mohou byt    | //
// |  v hlavnim programu rozliseny jednotlive prikazy.                   | //
// +---------------------------------------------------------------------+ //
//                                                                         //
//   Version history:                                                      //
//    1.20    ...                                                          //
//                                                                         //
//    1.22   050525   Pridany funkce IsHex a CMD_GetParHex                 //
//                                                                         //
//    1.23   050609   Pridany cGoNodFar3 (index + 768)                     //
//                            cGoNodFar4 (index + 1024)                    //
//                                                                         //
//    1.25   060603   cmd_def.k prejmenovan na cmd.def                     //
//                                                                         //
//    1.26   060608   - upraveno pro minimalni hw naroky                   //
//                                                                         //
//                    - prepsano na moznost vice nezavislych instanci      //
//                                                                         //
//    1.27   060829   - opravena chyba nulovani indexu v bufferu           //
//                      parametru chyba mohla zpusobovat ztratu vsech      //
//                      parametru v lokalnim bufferu parametru.            //
//                                                                         //
//                    - opravena redukce mezer mezi parametry              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//


//#define CMD_DEBUG_MODE

#include "cmd.inc"       // Definice konstant pro makra
#include "shorttyp.h"


// Pozor: navrzeno max. pro indexaci parametru do 256 byte !

uchar CMD_Par[cCMD_ParBufSize * cCMD_NoInstance];             // Buffer parametru pro vsechny instance

uchar CMD_Inst[cCMDInstSize * cCMD_NoInstance];               // Pole promennych pro vsechny instance detekce
// uchar       CMD_NumOfPar;      // Pocet parametru
// uchar       CMD_WaitRet;       //
// uchar       CMD_Space;         // Detekce a vynechani mezer
// uchar       bCMD_WaitPar;      // Flag cekani na parametr
// uint        CMDTab_ix;         // Index v tabulce prikazu

//bit  bCMD_DebugMsgs;   // Povoleni ladicich zprav

#define CMD_ParIx     CMD_Par[ID_Inst * cCMD_ParBufSize]            // Asociace indexu s nultym bytem v bufferu
#define CMD_NumOfPar  CMD_Inst[((ID_Inst * cCMDInstSize) + 0)]      // Odkaz na Pocet parametru pro instanci ID_Inst
#define CMD_WaitRet   CMD_Inst[((ID_Inst * cCMDInstSize) + 1)]      //
#define CMD_Space     CMD_Inst[((ID_Inst * cCMDInstSize) + 2)]      // Detekce a vynechani mezer
#define CMD_WaitPar   CMD_Inst[((ID_Inst * cCMDInstSize) + 3)]      // Flag cekani na parametr


//----------------------------------------------------------------------------------

// Prevede znak na velky
uchar UpChar(uchar Char)
{
  if((Char>='a') && (Char<='z')) return(Char & (255-32));
  else return(Char);
}


// Je-li znak cislo, pismeno, nebo znamenko vrati 1
uchar IsPrint(uchar Char)
{
  if(((Char >= 'A') && (Char <= 'Z')) ||
     ((Char >= 'a') && (Char <= 'z')) ||
     ((Char >= '0') && (Char <= '9')) ||
     (Char=='+') || (Char=='-') || (Char=='#'))
     return(1);
  else return(0);
}


// Je-li znak cislice vrati 1
uchar IsNum(uchar Char)
{
  if((Char >= '0') && (Char <= '9')) return(1); else return(0);
}


// Je-li znak hexa cislice vrati 1
uchar IsHex(uchar Char)
{
  if(((Char >= '0') && (Char <= '9')) ||
     ((UpChar(Char) >= 'A') && (UpChar(Char) <= 'F'))) return(1); else return(0);
}


// Vynuluje buffer na ukazateli
void ResetBuffer(uchar *Buf, uint Bytes)
{
  uint i;

  for(i=0; i<Bytes; i++)
     Buf[i]=0;
}


#ifdef CMD_DEBUG_MODE
// Vrati nazev makra jako ukazatel na text
uchar *ActionText(uchar Action)
{ switch(Action)
  { case cGoNod    : return("GoNod");
    case cExitCode : return("ExitCode");
    case cWaitPar  : return("WaitPar");
    case cResetIx  : return("ResetIx");
    case cExitRet  : return("ExitRet");
    case cResetRun : return("RstRun");
    case cGoNodFar : return("GoNodFar");
    case cGoNodFar2: return("GoNodFar2");
    case cGoNodFar3: return("GoNodFar3");
    case cGoNodFar4: return("GoNodFar4");
    case cRptRstIx : return("RptRstIx");
  }
  return("Undefined");
}
#endif


// Zapis do indexu v tabulce prikazu pro instanci ID_Inst
void CMD_Tab_ix_Write(uchar ID_Inst, uint Tab_ix)
{
  CMD_Inst[((ID_Inst * cCMDInstSize) + 4)] = ((Tab_ix >> 8) & 0xFF);
  CMD_Inst[((ID_Inst * cCMDInstSize) + 5)] = (Tab_ix & 0xFF);
}



// Cteni indexu z tabulky prikazu pro instanci ID_Inst
uint CMD_Tab_ix_Read(uchar ID_Inst)
{
  uint Tab_ix;

  Tab_ix  =  CMD_Inst[((ID_Inst * cCMDInstSize) + 4)] << 8;
  Tab_ix |=  CMD_Inst[((ID_Inst * cCMDInstSize) + 5)];

  return(Tab_ix);
}


// Vrati pocet prvku v bufferu parametru
uchar CMD_GetNumOfPar(uchar ID_Inst)
{
  return(CMD_NumOfPar);
}


// Zjisti offset bufferu parametru dane instance
uchar CMD_GetInstParOffs(uchar ID_Inst)
{
  return (ID_Inst * cCMD_ParBufSize);
}


// Vrati ukazatel na n-ty parametr
uchar *CMD_GetPar(uchar ID_Inst, uchar Index)
{
  uchar i;
  uchar ct;
  uchar Inst_Offs;

  Inst_Offs = CMD_GetInstParOffs(ID_Inst);                           // Zjisti offset bufferu parametru pro danou instanci

  if(CMD_Tab_ix_Read(ID_Inst) >= cCMD_ParBufSize)                    // Pokud je index v tab.prik. vetsi nez velikost oddilu instance
  {
    CMD_Tab_ix_Write(ID_Inst, (cCMD_ParBufSize-1));                  // Sniz index o 1 a zapis ho zpet
  }

  i=1;                                                               // Nastav index prvniho paramatru
  ct=0;                                                              // Nuluj citac parametru
  while((i < CMD_ParIx) && (ct!=Index))                              // Citej parametry dokud neni cislo parametru stejne jako index
  {
    if(ct==Index) break;                                             // Pokud je cislo parametru stejne jako index, parametr nalezen, konci
    if(!CMD_Par[Inst_Offs + i]) ct++;                                // Pokud je nalezena #0, pokracuj s dalsim parametrem
    i++;                                                             // Inc index v bufferu
  }
  return(CMD_Par + Inst_Offs + i);                                   // Vrat skutecnou adresu parametru
}


// Nulovani poctu parametru instance ID_Inst
// Uzitecne v pripade, kdy nebyl detekovan zadny parametr a v bufferu zustal predchozi
void CMD_ClearPar(uchar ID_Inst)
{
  CMD_NumOfPar = 0;
}


// Vrati cislo zkonvertovane z n-teho parametru
uint CMD_GetParNum(uchar ID_Inst, uchar Index, uchar Omez)
{
  uchar Num[5];
  uchar IxNum,IxPar;
  uint  DestInt;
  uint  Nasob;
  uchar Ch;

  for(IxNum=0; IxNum<5; IxNum++) Num[IxNum]=0; // Nuluj buffer

  // Nacti cislice do bufferu
  IxNum=0;
  IxPar=0;
  while(IxNum<5)                                   // 5 cislic (Dec Int)
  {
    Ch = CMD_GetPar(ID_Inst, Index)[IxPar];
    if(!Ch) break;

    if(!IsNum(Ch))
    {
      IxPar++;
    }
    else
    {
      Num[IxNum]=Ch; IxNum++; IxPar++;
    }
  }
  // IxNum...PocetCislic
  if((Omez) && (Omez<IxNum)) IxNum=Omez;  // Omezeni poctu cislic

  // Vypocitej integer ze znaku v parametru
  Nasob=1;                          // Nastav pocatecniho nasobitele
  DestInt=0;                        // Nuluj vysledne cislo
  while(IxNum)
  {
    DestInt+= (Num[IxNum-1]-'0') * Nasob; // pocitej
    Nasob*=10;
    IxNum--;
  }
  return(DestInt);
}


// Vrati hexa cislo zkonvertovane z n-teho parametru
uint CMD_GetParHex(uchar ID_Inst, uchar Index, uchar Omez)
{
  #define HexMaxSize 8

  uchar Num[HexMaxSize];
  uchar IxNum,IxPar;
  uint  DestInt;
  uint  Nasob;
  uchar Ch;

  for(IxNum=0; IxNum < HexMaxSize; IxNum++)
  {
    Num[IxNum]=0; // Nuluj buffer
  }

  // Nacti cislice do bufferu
  IxNum=0;
  IxPar=0;
  while(IxNum < HexMaxSize)
  {
    Ch = CMD_GetPar(ID_Inst, Index)[IxPar];
    if(!Ch) break;

    if(!IsHex(Ch))
    {
      IxPar++;
    }
    else
    {
      Num[IxNum]=UpChar(Ch);
      IxNum++;
      IxPar++;
    }
  }
  // IxNum...PocetCislic
  if((Omez) && (Omez < IxNum))
  {
    IxNum=Omez;  // Omezeni poctu cislic
  }

  // Vypocitej integer ze znaku v parametru
  Nasob=1;                          // Nastav pocatecniho nasobitele
  DestInt=0;                        // Nuluj vysledne cislo
  while(IxNum)
  {
    if(Num[IxNum-1]<='9')
    {
      DestInt+= (Num[IxNum-1]-'0') * Nasob; // pocitej
    }
    else
    { DestInt+= ((Num[IxNum-1]-'A')+10) * Nasob; // pocitej
    }
    Nasob*=16;
    IxNum--;
  }
  return(DestInt);
}



// Porovnava znak v Get_Char se znaky v tabulce CMDTab, ridi se makry v tabulce
uchar CMD_TestProc(uchar ID_Inst, uchar *CMDTab, uchar Get_Char)
{
  uchar Repeat;
  uchar Out;

  uint  lCMDTab_ix;
  uchar lCMD_NumOfPar;
  uchar lCMD_WaitRet;
  uchar lCMD_Space;
  uchar lCMD_WaitPar;
  uchar lCMD_ParIx;

  // Zkopiruj data instance do lokalnich promennych
  lCMD_ParIx    = CMD_ParIx;                 // Nacti index z nulteho byte v lokalnim bufferu parametru
  lCMD_NumOfPar = CMD_NumOfPar;              // Pocet parametru pro instanci ID_Inst
  lCMD_WaitRet  = CMD_WaitRet;               //
  lCMD_Space    = CMD_Space;                 // Detekce a vynechani mezer
  lCMD_WaitPar  = CMD_WaitPar;               // Flag cekani na parametr
  lCMDTab_ix    = CMD_Tab_ix_Read(ID_Inst);  // Nacti index v tabulce prikazu pro aktualni instanci

  #define CMD_Char        CMDTab[lCMDTab_ix]
  #define CMD_Yes_Action  CMDTab[lCMDTab_ix+1]
  #define CMD_Yes_Par     CMDTab[lCMDTab_ix+2]
  #define CMD_No_Action   CMDTab[lCMDTab_ix+3]
  #define CMD_No_Par      CMDTab[lCMDTab_ix+4]

  Repeat=1;
  Out=0;

  #ifdef CMD_DEBUG_MODE
  printf("\r\nTest char %c, adress of CMD table: %p\r\n",Get_Char,CMDTab);
  #endif

  if(!lCMD_WaitPar)                           // Pokud se neceka na parametr
  {
    while(Repeat)                            // Dokud nastaveno repeat, provadej...
    {
      Repeat=0;

      #ifdef CMD_DEBUG_MODE
      printf(" '%c' '%c'  %4d   ", Get_Char,CMDTab[lCMDTab_ix],lCMDTab_ix);
      printf(" %7s, %3d,  ", ActionText(CMD_Yes_Action),CMD_Yes_Par);
      printf(" %7s, %3d\r\n", ActionText(CMD_No_Action),CMD_No_Par);
      #endif

      if(UpChar(Get_Char) == CMDTab[lCMDTab_ix])        // Znak se shoduje
      {
        #ifdef CMD_DEBUG_MODE
        printf("%c",CMDTab[lCMDTab_ix]);
        #endif

        switch(CMD_Yes_Action)
        {
          case cGoNod:     lCMDTab_ix += CMD_Yes_Par;                                // Pripocti hodnotu param.Yes
                           break;

          case cExitCode:  Out=CMD_Yes_Par;                                          // Vrat hodnotu Yes
                           lCMDTab_ix=0;                                             // Nuluj lokalni index v tab prikazu

                           //lCMD_NumOfPar=0;
                           // Zde nenulovat pocet parametru !!! Zustanou tak pouzitelne parametry
                           // z predchoziho prikazu, v pripade potreby.
                           //
                           // Napr. po prichodu OK z modemu !
                           //
                           break;

          case cWaitPar:   lCMD_WaitPar=1;                                           // Nastav lokalni flag cekani na parametr
                           lCMD_WaitRet=CMD_Yes_Par;                                 // Nastav lokalni hodnotu pro vraceni po ukonceni
                           lCMDTab_ix=0;                                             // Nuluj lokalni index v tab.
                           lCMD_Space=0;                                             // Nuluj vynechani mezer
                           lCMD_ParIx=1;                                             // Nastav index parametru
                           lCMD_NumOfPar=0;                                          // Nuluj lokalni pocet parametru

                           // Vymaz pocet byte jedne instance v bufferu parametru jedne instance
                           ResetBuffer((CMD_Par + (ID_Inst * cCMD_ParBufSize) + 1), cCMD_ParBufSize);

                           // POZOR: Mozna chyba, dojde k vymazani bufferu parametru vsech instanci
                           //ResetBuffer(CMD_Par+1,sizeof(CMD_Par)-1);                 // Nuluj lokalni buffer parametru
                           break;

          case cResetIx:   lCMDTab_ix=0;                                              // Nuluj lokalni index v tab. prikazu
                           break;

          case cExitRet:   Out=CMD_Yes_Par;                                           // Nastav navratovou hodnotu Yes
                           lCMD_ParIx=1;                                              // Nuluj lokalni index parametru
                           break;

          case cResetRun:  lCMDTab_ix=0;                                              // Nuluj lokalni index v tab. prikazu
                           lCMD_ParIx=1;                                              // Nuluj lokalni index parametru
                           break;

          case cGoNodFar:  lCMDTab_ix+=(CMD_Yes_Par + 0x100);                         // Skok na hodnotu Yes+0x100
                           break;

          case cGoNodFar2: lCMDTab_ix+=(CMD_Yes_Par + 0x200);                         // Skok na hodnotu Yes+0x200
                           break;

          case cGoNodFar3: lCMDTab_ix+=(CMD_Yes_Par + 0x300);                         // Skok na hodnotu Yes+0x300
                           break;

          case cGoNodFar4: lCMDTab_ix+=(CMD_Yes_Par + 0x400);                         // Skok na hodnotu Yes+0x400
                           break;

          case cRptRstIx:  lCMDTab_ix=0;                                              // Nuluj lokalni index v tab. prikazu
                           lCMD_ParIx=1;                                              // Nuluj lokalni index v bufferu parametru
                           Repeat=1;                                                  // Nastav flag opakovani smycky
                           break;
        }
      }
      else
      { // Zde se znak neshoduje
        switch(CMD_No_Action)
        {
          case cGoNod:     lCMDTab_ix += CMD_No_Par;                       // Pripocti hodnotu par.No
                           Repeat = 1;
                           break;

          case cExitCode:  Out = CMD_No_Par;
                           lCMDTab_ix = 0;
                           break;

          case cWaitPar:   lCMD_WaitPar = 1;                               // Nastav lokalni flag cekani na parametr
                           lCMD_WaitRet = CMD_No_Par;                      // Nastav navratovou hodnotu
                           lCMDTab_ix = 0;                                 // Nuluj lokalni index v tab.
                           lCMD_Space = 0;                                 // Nuluj vynechani mezer
                           lCMD_ParIx = 1;                                 // Nastav index parametru
                           lCMD_NumOfPar = 0;                              // Nuluj lokalni pocet parametru

                           // Vymaz pocet byte jedne instance v bufferu parametru jedne instance
                           ResetBuffer((CMD_Par + (ID_Inst * cCMD_ParBufSize) + 1), cCMD_ParBufSize);

                           //ResetBuffer(CMD_Par,sizeof(CMD_Par));
                           break;


          case cResetIx:   lCMDTab_ix=0;
                           lCMD_ParIx=1;
                           Repeat=0;
                           break;

          case cExitRet:   Out=CMD_No_Par;
                           break;

          case cResetRun:  lCMDTab_ix=0;
                           break;

          case cGoNodFar:  lCMDTab_ix+=(CMD_No_Par + 0x100);
                           Repeat=1;
                           break;

          case cGoNodFar2: lCMDTab_ix+=(CMD_No_Par + 0x200);
                           Repeat=1;
                           break;

          case cGoNodFar3: lCMDTab_ix+=(CMD_No_Par + 0x300);
                           Repeat=1;
                           break;

          case cGoNodFar4: lCMDTab_ix+=(CMD_No_Par + 0x400);
                           Repeat=1;
                           break;

          case cRptRstIx:  lCMDTab_ix=0;
                           lCMD_ParIx=1;
                           Repeat=1;
                           break;
        }
      }
    }
  }
  else  // Ukladej parametr
  {
    #ifdef CMD_DEBUG_MODE
    //LED3=~LED3;
    #endif

    switch(Get_Char)
    {
      case ';':                                   // Konec ukladani parametru
      case '@':
      case '*':
      case '#':
      case 10:
      case 13:
      //case ',':
      //case ' ':
                if(lCMD_ParIx < cCMD_ParBufSize)  // Pokud je index parametru mensi nez velikost lokalniho bufferu pro parametry
                {
                  CMD_Par[(ID_Inst * cCMD_ParBufSize) + lCMD_ParIx] = 0;             // Zapis nulu na na konec
                  lCMD_ParIx++;

                  #ifdef CMD_DEBUG_MODE
                  printf("\r\nParIx:%i\r\n",lCMD_ParIx);
                  #endif

                  if(lCMD_ParIx > 2) lCMD_NumOfPar++;
                }
                lCMDTab_ix = 0;
                lCMD_WaitPar = 0;
                lCMD_Space = 0;
                Out = lCMD_WaitRet;
                break;
      case '/':                                      // Dalsi parametr
      case '.':
      case ':':
      case ',':
      case ' ':
                if(lCMD_ParIx < sizeof(CMD_Par))   // Pokud je jeste misto v bufferu
                {
                  if(lCMD_ParIx > 1)               // a index je vetsi nez 1
                  {
                    if(!lCMD_Space)                // a znak neni mezera
                    {
                      CMD_Par[(ID_Inst * cCMD_ParBufSize) + lCMD_ParIx] = 0;       // Zapis do bufferu konec x.parametru
                      lCMD_ParIx++;                // Dalsi znak
                      lCMD_NumOfPar++;             // Inc pocet parametru
                      lCMD_Space=1;                // Nastav flag redukce mezer
                    }
                  }
                  else lCMD_NumOfPar=0;            // jinak nuluj pocet parametru
                }
                break;
      case '"': break;
      default:  // POZOR: velikost jedne instance
                //if(lCMD_ParIx < sizeof(CMD_Par))
                if(lCMD_ParIx < cCMD_ParBufSize)
                {
                  if(IsPrint(Get_Char))                         // Pokud je znak printable
                  {
                    CMD_Par[(ID_Inst * cCMD_ParBufSize) + lCMD_ParIx] = Get_Char;      // Zapis znak do lokalniho bufferu parametru
                  }
                  else
                  {
                    CMD_Par[(ID_Inst * cCMD_ParBufSize) + lCMD_ParIx] = 0;
                    lCMDTab_ix = 0;
                    lCMD_WaitPar = 0;
                    Out = lCMD_WaitRet;
                  }
                  lCMD_ParIx++;
                  lCMD_Space=0;
                }
                break;
    }
  }

  // Zkopiruj lokalni promenne do dat instance
  CMD_ParIx       = lCMD_ParIx;           // Index parametru
  CMD_NumOfPar    = lCMD_NumOfPar;        // Pocet parametru pro instanci ID_Inst
  CMD_WaitRet     = lCMD_WaitRet;         //
  CMD_Space       = lCMD_Space;           // Detekce a vynechani mezer
  CMD_WaitPar     = lCMD_WaitPar;         // Flag cekani na parametr
  CMD_Tab_ix_Write(ID_Inst, lCMDTab_ix);  // Zapis index v tabulce prikazu pro aktualni instanci

  return(Out);
}


void CMD_Init()
{
  uchar i;

  for(i=0; i<sizeof(CMD_Par); i++) CMD_Par[i]=0;
  for(i=0; i<sizeof(CMD_Inst); i++) CMD_Inst[i]=0;
}




#ifdef CMD_DEBUG_MODE
void main()
{
  uchar CMD;

  printf("TEXT PARALELNI DETEKCE PRIKAZU\r\n--------------------------------\r\nAlt+X...Exit\r\n\n");
  CMD_Init();

  while(!kbALT_X)
  {
    if(kbhit())
    {
      GetKey();
                 //uchar ID_Inst, uchar *CMDTab, uchar Get_Char
      CMD = CMD_TestProc(0, cCMDTab, Ascii);
      switch(CMD)
      {
        case cCMD_CPBR:  printf("CPBR Detected!\r\n"); break;
      }

      //CMD = CMD_TestProc(1, NodeTab2, Ascii);
      //switch(CMD)
      //{
      //  case cCMD_DRAGO:   DRAGO(); break;
      //  case cCMD_INFLAM:  INFLAM(); break;
      //}

    }

    //HCOM_Proc();
  }
}
#endif

