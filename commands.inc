#include "cmd.def"

//----------------------------------------------------
// Tabulka prikazu pro HCOM
uchar code cCMDTab_HCOM[]={/*

 Char | Action    Par  | Action    Par  |  Offset
      | <-----(=)----> | <-----(!=)---> |
                                           */
   0,     cGoNod,               5,     cGoNod,   5,    // 0

 // +CLIP: XX
 '+',     cGoNod,               5,     cGoNod, 140,    // 5                    <-+
 'C',     cGoNod,               5,   cResetIx,   0,    // 10                     |
 'L',     cGoNod,               5,     cGoNod,  20,    // 15               <-+   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 20                 |   |
 'P',     cGoNod,               5,   cResetIx,   0,    // 25                 |   |
 ':',   cWaitPar,       cCMD_CLIP,   cResetIx,   0,    // 30                 |   |
                                                       //                    |   |
 // +CMGR: XX                                                                |   |
 'M',     cGoNod,               5,     cGoNod,  35,    // 35               <-+   |
 'G',     cGoNod,               5,     cGoNod,  15,    // 40           <-+   |   |
 'R',     cGoNod,               5,  cGoNodFar,  54,    // 45             |   |   |  <-+
 ':',   cWaitPar,       cCMD_CMGR,   cResetIx,   0,    // 50             |   |   |    |
                                                       //                |   |   |    |
 // +CMTI: XX                                                            |   |   |    |
 'T',     cGoNod,               5,  cRptRstIx,   0,    // 55           <-+   |   |    |
 'I',     cGoNod,               5,   cResetIx,   0,    // 60                 |   |    |
 ':',   cWaitPar,       cCMD_CMTI,   cResetIx,   0,    // 65                 |   |    |
                                                       //                    |   |    |
 // +COPS: XX                                                                |   |    |
 'O',     cGoNod,               5,     cGoNod,  20,    // 70               <-+   |    |
 'P',     cGoNod,               5,   cResetIx,   0,    // 75                 |   |    |
 'S',     cGoNod,               5,   cResetIx,   0,    // 80                 |   |    |
 ':',   cWaitPar,       cCMD_COPS,   cResetIx,   0,    // 85                 |   |    |
                                                       //                    |   |    |
 // +CPIN: XX                                                                |   |    |
 'P',     cGoNod,               5,     cGoNod, 225,    // 90               <-+   |    |
 'I',     cGoNod,               5,     cGoNod,  15,    // 95           <-+   |   |    |
 'N',     cGoNod,               5,   cResetIx,   0,    // 100            |   |   |    |
 ':',   cWaitPar,       cCMD_CPIN,   cResetIx,   0,    // 105            |   |   |    |
                                                       //                |   |   |    |
 // +CPBR: 11,"+420xxxxxxxxx",145,"TH"                                   |   |   |    |
 'B',     cGoNod,               5,   cResetIx,   0,    // 110          <-+   |   |    |
 'R',     cGoNod,               5,   cResetIx,   0,    // 115                |   |    |
 ':',   cWaitPar,       cCMD_CPBR,   cResetIx,   0,    // 120                |   |    |
                                                       //                    |   |    |
 //                                                                          |   |    |
 'X',     cGoNod,               5,     cGoNod, 190,    // 125                |   |    |
 'X',     cGoNod,               5,   cResetIx,   0,    // 130                |   |    |
 'X',     cGoNod,               5,   cResetIx,   0,    // 135                |   |    |
 ':',   cWaitPar,        cCMD_CSQ,   cResetIx,   0,    // 140                |   |    |
                                                       //                    |   |    |
 // >_                                                                       |   |    |
 '>',     cGoNod,               5,     cGoNod,  10,    // 145                | <-+    |
 ' ',  cExitCode,     cCMD_SMS_ED,   cResetIx,   0,    // 150                |   |    |
                                                       //                    |   |    |
 // M20<CR>                                                                  |   |    |
 'M',     cGoNod,               5,     cGoNod,  20,    // 155                | <-+    |
 '2',     cGoNod,               5,   cResetIx,   0,    // 160                |   |    |
 '0',     cGoNod,               5,   cResetIx,   0,    // 165                |   |    |
  13,  cExitCode,        cCMD_M20,   cResetIx,   0,    // 170                |   |    |
                                                       //                    |   |    |
 // OK<CR>                                                                   |   |    |
 'O',     cGoNod,               5,     cGoNod,  15,    // 175                | <-+    |
 'K',     cGoNod,               5,   cResetIx,   0,    // 180                |   |    |
  13,  cExitCode,         cCMD_OK,   cResetIx,   0,    // 185                |   |    |
                                                       //                    |   |    |
                                                       //                    |   |    |
 // RING<CR>                                                                 |   |    |
 'R',     cGoNod,               5,     cGoNod,  25,    // 190                | <-+    |
 'I',     cGoNod,               5,   cResetIx,   0,    // 195                |   |    |
 'N',     cGoNod,               5,   cResetIx,   0,    // 200                |   |    |
 'G',     cGoNod,               5,   cResetIx,   0,    // 205                |   |    |
  13,  cExitCode,       cCMD_RING,   cResetIx,   0,    // 210                |   |    |
                                                       //                    |   |    |
 // TC35<CR>                                                                 |   |    |
 'T',     cGoNod,               5,     cGoNod,  25,    // 215                | <-+    |
 'C',     cGoNod,               5,   cResetIx,   0,    // 220                |   |    |
 '3',     cGoNod,               5,   cResetIx,   0,    // 225                |   |    |
 '5',     cGoNod,               5,   cResetIx,   0,    // 230                |   |    |
  13,  cExitCode,       cCMD_TC35,   cResetIx,   0,    // 235                |   |    |
                                                       //                    |   |    |
 // ERROR                                                                    |   |    |
 'E',     cGoNod,               5,     cGoNod,  25,    // 240                | <-+    |
 'R',     cGoNod,               5,   cResetIx,   0,    // 245                |   |    |
 'R',     cGoNod,               5,   cResetIx,   0,    // 250                |   |    |
 'O',     cGoNod,               5,   cResetIx,   0,    // 255                |   |    |
 'R',  cExitCode,      cCMD_ERROR,   cResetIx,   0,    // 260                |   |    |
                                                       //                    |   |    |
 // NO CARRIER                                                               |   |    |
 'N',     cGoNod,               5,     cGoNod,  65,    // 265                | <-+    |
 'O',     cGoNod,               5,   cResetIx,   0,    // 270                |   |    |
 ' ',     cGoNod,               5,   cResetIx,   0,    // 275                |   |    |
 'C',     cGoNod,               5,   cResetIx,   0,    // 280                |   |    |
 'A',     cGoNod,               5,   cResetIx,   0,    // 285                |   |    |
 'R',     cGoNod,               5,   cResetIx,   0,    // 290                |   |    |
 'R',     cGoNod,               5,   cResetIx,   0,    // 295                |   |    |
 'I',     cGoNod,               5,   cResetIx,   0,    // 300                |   |    |
 'E',     cGoNod,               5,   cResetIx,   0,    // 305                |   |    |
 'R',  cExitCode,  cCMD_NOCARRIER,   cResetIx,   0,    // 310                |   |    |
                                                       //                    |   |    |
 // +CBC:                                                                    |   |    |
 'B',     cGoNod,               5,   cResetIx,   0,    // 315              <-+   |    |
 'C',     cGoNod,               5,   cResetIx,   0,    // 320                    |    |
 ':',   cWaitPar,        cCMD_CBC,   cResetIx,   0,    // 325                    |    |
                                                       //                        |    |
 // BUSY\r                                                                       |    |
 'B',     cGoNod,               5,   cResetIx,   0,    // 330                  <-+    |
 'U',     cGoNod,               5,   cResetIx,   0,    // 335                         |
 'S',     cGoNod,               5,   cResetIx,   0,    // 340                         |
 'Y',     cGoNod,               5,   cResetIx,   0,    // 345                         |
  13,  cExitCode,       cCMD_BUSY,   cResetIx,   0,    // 350                         |
                                                       //                             |
 // +CMGL: XX                                                                         |
 'L',     cGoNod,               5,     cGoNod,  35,    // 355                       <-+
 ':',   cWaitPar,       cCMD_CMGL,   cResetIx,   0,    // 360
};




uchar code cCMDTab_SCOM[]={/*

 Char |  Action             Par   | Action     Par |  Offset   |           Map
      | <-------(=)-------------> | <-----(!=)---> |           |
                                           */
   0,     cGoNod,               5,     cGoNod,   5,    // 0

 // VARS
 'V',     cGoNod,               5,     cGoNod,  20,    // 5                    <-+
 'A',     cGoNod,               5,   cResetIx,   0,    // 10                     |
 'R',     cGoNod,               5,   cResetIx,   0,    // 15                     |
 'S',  cExitCode,        cCMD_VAR,   cResetIx,   0,    // 20                     |
                                                       //                        |
 // PINS 1 1                                                                     |
 'P',     cGoNod,               5,     cGoNod,  20,    // 25                   <-+
 'I',     cGoNod,               5,     cGoNod, 190,    // 30       <-+           |
 'N',     cGoNod,               5,   cResetIx,   0,    // 35         |           |
 'S',   cWaitPar,       cCMD_PINS,   cResetIx,   0,    // 40         |           |
                                                       //            |           |
 // HELP                                                             |           |
 'H',     cGoNod,               5,     cGoNod,  20,    // 45         |         <-+
 'E',     cGoNod,               5,   cResetIx,   0,    // 50         |           |
 'L',     cGoNod,               5,     cGoNod,  85,    // 55         |     <-+   |
 'P',  cExitCode,       cCMD_HELP,   cResetIx,   0,    // 60         |       |   |
                                                       //            |       |   |
 // DBGMSG                                                           |       |   |
 'D',     cGoNod,               5,     cGoNod,  30,    // 65         |       | <-+
 'B',     cGoNod,               5,   cResetIx,   0,    // 70         |       |   |
 'G',     cGoNod,               5,   cResetIx,   0,    // 75         |       |   |
 'M',     cGoNod,               5,   cResetIx,   0,    // 80         |       |   |
 'S',     cGoNod,               5,   cResetIx,   0,    // 85         |       |   |
 'G',  cExitCode,     cCMD_DBGMSG,   cResetIx,   0,    // 90         |       |   |
                                                       //            |       |   |
 // BRIDGE                                                           |       |   |
 'B',     cGoNod,               5,     cGoNod,  55,    // 95         |       | <-+
 'R',     cGoNod,               5,     cGoNod,  25,    // 100        | <-+   |   |
 'I',     cGoNod,               5,   cResetIx,   0,    // 105        |   |   |   |
 'D',     cGoNod,               5,   cResetIx,   0,    // 110        |   |   |   |
 'G',     cGoNod,               5,   cResetIx,   0,    // 115        |   |   |   |
 'E',  cExitCode,     cCMD_BRIDGE,   cResetIx,   0,    // 120        |   |   |   |
                                                       //            |   |   |   |
 // BATT                                                             |   |   |   |
 'A',     cGoNod,               5,   cResetIx,   0,    // 125        | <-+   |   |
 'T',     cGoNod,               5,   cResetIx,   0,    // 130        |       |   |
 'T',  cExitCode,       cCMD_BATT,   cResetIx,   0,    // 135        |       |   |
                                                       //            |       |   |
 // HEAR                                                             |       |   |
 'A',     cGoNod,               5,   cResetIx,   0,    // 140        |     <-+   |
 'R',  cExitCode,       cCMD_HEAR,   cResetIx,   0,    // 145        |           |
                                                       //            |           |
 // TIMES                                                            |           |
 'T',     cGoNod,               5,     cGoNod,  25,    // 150        |         <-+
 'I',     cGoNod,               5,   cResetIx,   0,    // 155        |           |
 'M',     cGoNod,               5,   cResetIx,   0,    // 160        |           |
 'E',     cGoNod,               5,   cResetIx,   0,    // 165        |           |
 'S',  cExitCode,      cCMD_TIMES,   cResetIx,   0,    // 170        |           |
                                                       //            |           |
 // CHECK                                                            |           |
 'C',     cGoNod,               5,     cGoNod,  25,    // 175        |         <-+
 'H',     cGoNod,               5,   cResetIx,   0,    // 180        |           |
 'E',     cGoNod,               5,   cResetIx,   0,    // 185        |           |
 'C',     cGoNod,               5,   cResetIx,   0,    // 190        |           |
 'K',  cExitCode,      cCMD_CHECK,   cResetIx,   0,    // 195        |           |
                                                       //            |           |
 // SCOM xxx                                                         |           |
 'S',     cGoNod,               5,     cGoNod,  85,    // 200        |         <-+
 'C',     cGoNod,               5,     cGoNod,  35,    // 205        | <-+       |
 'O',     cGoNod,               5,   cResetIx,   0,    // 210        |   |       |
 'M',   cWaitPar,       cCMD_SCOM,   cResetIx,   0,    // 215        |   |       |
                                                       //            |   |       |
 // PROCS                                                            |   |       |
 'R',     cGoNod,               5,     cGoNod,  30,    // 220      <-+   |       |
 'O',     cGoNod,               5,   cResetIx,   0,    // 225        |   |       |
 'C',     cGoNod,               5,   cResetIx,   0,    // 230        |   |       |
 'S',   cWaitPar,      cCMD_PROCS,   cResetIx,   0,    // 235        |   |       |
                                                       //            |   |       |
 // SND Pars                                                         |   |       |
 'N',     cGoNod,               5,   cResetIx,   0,    // 240        | <-+       |
 'D',   cWaitPar,        cCMD_SND,   cResetIx,   0,    // 245        |           |
                                                       //            |           |
 // PARALYSE                                                         |           |
 'A',     cGoNod,               5,   cResetIx,   0,    // 250      <-+           |
 'R',     cGoNod,               5,   cResetIx,   0,    // 255                    |
 'A',     cGoNod,               5,   cResetIx,   0,    // 260                    |
 'L',     cGoNod,               5,   cResetIx,   0,    // 265                    |
 'Y',     cGoNod,               5,   cResetIx,   0,    // 270                    |
 'S',     cGoNod,               5,   cResetIx,   0,    // 275                    |
 'E',   cWaitPar,   cCMD_PARALYSE,   cResetIx,   0,    // 280                    |
                                                       //                        |
 // REP                                                                          |
 'R',     cGoNod,               5,   cResetIx,   0,    // 285                  <-+
 'E',     cGoNod,               5,   cResetIx,   0,    // 290
 'P',   cWaitPar,        cCMD_REP,   cResetIx,   0,    // 295
};





//----------------------------------------------------
// Tabulka prikazu pro SMS
uchar code cCMDTab_SMS[]={/*

 Char | Action    Par  | Action    Par  |  Offset
      | <-----(=)----> | <-----(!=)---> |
                                           */
   0,     cGoNod,               5,     cGoNod,   5,    // 0

 // PINS xxx xxx
 'P',     cGoNod,               5,   cResetIx,   0,    // 5                    <-+
 'I',     cGoNod,               5,   cResetIx,   0,    // 10                     |
 'N',     cGoNod,               5,   cResetIx,   0,    // 15               <-+   |
 'S',   cWaitPar,   cCMD_SMS_PINS,   cResetIx,   0,    // 20                 |   |
                                                       //                    |   |


};