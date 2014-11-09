#include "shorttyp.h"

uchar UpChar(uchar Char);
uchar IsPrint(uchar Char);
uchar IsNum(uchar Char);
uchar IsHex(uchar Char);
void ResetBuffer(uchar *Buf, uint Bytes);
void CMD_Tab_ix_Write(uchar ID_Inst, uint Tab_ix);
uint CMD_Tab_ix_Read(uchar ID_Inst);
uchar CMD_GetNumOfPar(uchar ID_Inst);
uchar CMD_GetInstParOffs(uchar ID_Inst);
uchar *CMD_GetPar(uchar ID_Inst, uchar Index);
void CMD_ClearPar(uchar ID_Inst);
uint CMD_GetParNum(uchar ID_Inst, uchar Index, uchar Omez);
uint CMD_GetParHex(uchar ID_Inst, uchar Index, uchar Omez);
uchar CMD_TestProc(uchar ID_Inst, uchar *CMDTab, uchar Get_Char);
void CMD_Init();
