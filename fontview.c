/// *---------------------------------------*
/// |                                       |
#define SWTitle   "FontViewer"
#define Version     "1.07a"
/// |                                       |
/// |          Copyright (c) 2006           |
/// |             Thomas Hudger             |
/// |                                       |
/// *---------------------------------------*
#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <memory.h>

#define CompileObj

#include "cmd.h"
#include "cmd.inc"
#include "keyb.h"
#include "strutils.h"
#include "curutils.h"
#include "gsscr.h"
#include "scrutils.h"
#include "fontview.h"

#define DATATYPE_UNKNOWN 0
#define DATATYPE_BIN     1
#define DATATYPE_ASM     2
#define DATATYPE_C       3

#define DownMenu_main "~Alt-X~ Exit ~F2~ Export font file ~F3~ Read & preview ~F6~ Change filename"

tstr_Menu       Menu;
tstr_OpenFile   OpenFile;
tstr_ExportFile ExportFile;
tstr_Font       Font;

uint       Main_Proc;
uchar      ExitSignal;
uchar      CMD;

#define    cDATATYPE_NODETECT  0
#define    cDATATYPE_BINARY    1
#define    cDATATYPE_TEXT      2

#define    FONT_SPACE_ALLOCATE 8192




uchar cCMDTab[]={/*

 Char | Action    Par  | Action    Par  |  Offset
      | <-----(=)----> | <-----(!=)---> |
                                           */
 // 0x..
 '0',     cGoNod,               5,     cGoNod,  10,    // 10
 'X',   cWaitPar,        cCMD_HEX,   cResetIx,   0,    // 15

 'D',     cGoNod,               5,   cResetIx,   0,    // 20
 'B',   cWaitPar,        cCMD_ASM,   cResetIx,   0,    // 25

};


void main_init()
{
  Main_Proc = 0;
  ExitSignal = 0;
}


void main_destruct()
{
  if(Font.font_buffer != OpenFile.file_buffer)                     // pokud nebyl fontbuf presmerovan na filebuf
  {
    farfree(Font.font_buffer);                            // uvolni pamet
  }

  textattr(7);
  cprintf("\r\nApplication process terminated\r\n");
}


void file_console_def()
{
  FBorder.PosX = 2;
  FBorder.PosY = 3;
  FBorder.Width = 30;
  FBorder.Height = 17;
  FBorder.Color = 16+15;
  FBorder.Shadow = 1;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back";
  FBorder.Title = "File console";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}



void font_console_def()
{
  FBorder.PosX = 38;
  FBorder.PosY = 3;
  FBorder.Width = 35;
  FBorder.Height = 17;
  FBorder.Color = 16+15;
  FBorder.Shadow = 1;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back";
  FBorder.Title = "Font console";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}


void font_preview_def()
{
  FBorder.PosX = 30;
  FBorder.PosY = 2;
  FBorder.Width = 30;
  FBorder.Height = 20;
  FBorder.Color = 16+15;
  FBorder.Shadow = 0;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back ~UP~ rol up ~DOWN~ rol down ~/~,~*~ x size ~+~,~-~ y size";
  FBorder.Title = "Font preview";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}


void font_select_filename_def()
{
  FBorder.PosX = 30;
  FBorder.PosY = 10;
  FBorder.Width = 20;
  FBorder.Height = 1;
  FBorder.Color = 16+15;
  FBorder.Shadow = 1;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back ~ENTER~ Open typed file";
  FBorder.Title = "Load font file";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}


void font_export_filename_def()
{
  FBorder.PosX = 30;
  FBorder.PosY = 10;
  FBorder.Width = 20;
  FBorder.Height = 1;
  FBorder.Color = 16+15;
  FBorder.Shadow = 1;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back ~ENTER~ Export file";
  FBorder.Title = "Export font file";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}


void font_export_format_def()
{
  FBorder.PosX = 30;
  FBorder.PosY = 15;
  FBorder.Width = 13;
  FBorder.Height = 3;
  FBorder.Color = 16+15;
  FBorder.Shadow = 1;
  FBorder.Type = 0;
  FBorder.DownMenu = "~ESC~ Back ~ENTER~ Export file";
  FBorder.Title = "Data format";
  FBorder.Window = 1;
  FBorder.ShowHint = 0;
}


long filesize(FILE *stream)
{
   long curpos, length;

   curpos = ftell(stream);
   fseek(stream, 0L, SEEK_END);
   length = ftell(stream);
   fseek(stream, curpos, SEEK_SET);
   return(length);

}

void LoadFontFile()
{

  if ((OpenFile.file_stream = fopen(Font.file_name, "r")) != NULL)
  {
    cprintf("Open file %s\r\n", Font.file_name );
    rewind(OpenFile.file_stream);

    OpenFile.file_length = filesize(OpenFile.file_stream);
    cprintf("File length: %u bytes\r\n", OpenFile.file_length);

    OpenFile.file_buffer = 0 ;
    OpenFile.file_buffer = (char far *) farmalloc(OpenFile.file_length);
    cprintf("Alocating memory on %p\r\n", OpenFile.file_buffer);

    if(OpenFile.file_buffer)
    {
      cprintf("Bytes %i\r\n", OpenFile.file_length);

      OpenFile.file_bytes_read = fread(OpenFile.file_buffer, OpenFile.file_length, 1, OpenFile.file_stream);
      cprintf("Reading file: %u bytes\r\n", OpenFile.file_bytes_read);
    }
    else
    {
      cprintf("Can't alocated memory !\r\n");
    }
    cprintf("Closing file: %i\r\n", fclose(OpenFile.file_stream));

  }
  else
  {
    cprintf("Error open \"%s\" !\r\n",Font.file_name);
  }
}


void DetectDataType()
{
  uchar i;

  cprintf("Detecting data type ... ");
  OpenFile.file_type = cDATATYPE_TEXT;
  for(i=0; i<30; i++)
  {
    if((OpenFile.file_buffer[i] < 30) & (OpenFile.file_buffer[i]!=0x0A) & (OpenFile.file_buffer[i]!=0x0D))
    {
      OpenFile.file_type = cDATATYPE_BINARY;
    }
  }
  if(OpenFile.file_type == cDATATYPE_BINARY)
  {
    cprintf("BIN");
  }
  else
  {
    cprintf("TEXT");
  }
  cprintf("\r\n");
}


void RecognizeData()
{
  uint i;
  uint par;
  uint par_size;
  uint byte_index;
  uchar NumOfPar;
  uchar Break;

  Break = 0;
  byte_index = 0;

  if(OpenFile.file_type == cDATATYPE_TEXT)
  {
    Font.font_buffer = (char far *) farmalloc(FONT_SPACE_ALLOCATE);                      // allocate buffer

    for(i=0; i<OpenFile.file_length; i++)
    //for(i=0; i<400; i++)
    {
       //switch(OpenFile.file_type)
       //{
       //  case cDATATYPE_BINARY: cprintf("%.2X ",OpenFile.file_buffer[i]); break;
       //  case cDATATYPE_TEXT:   cprintf("%c",OpenFile.file_buffer[i]);    break;
       //}

       CMD = CMD_TestProc(cCMD_InstID_HEX, cCMDTab, OpenFile.file_buffer[i]);
       if((CMD == cCMD_HEX) || (CMD == cCMD_ASM))
       {
         cprintf("\r\n%.3i:",byte_index);

         NumOfPar = CMD_GetNumOfPar(cCMD_InstID_HEX);
         for(par=0; par <= NumOfPar; par++)
         {
           if(byte_index < FONT_SPACE_ALLOCATE)
           {
             par_size = Str_StrSize(CMD_GetPar(cCMD_InstID_HEX, par));

             if(CMD == cCMD_HEX)
             {
               if(!par)
                 par_size+=2;
             }

             //cprintf("<%i:%s>", par_size, CMD_GetPar(cCMD_InstID_HEX, par));

             switch(par_size)
             {
               // 00H  Asm source
               case 3: Font.font_buffer[byte_index] = CMD_GetParHex(cCMD_InstID_HEX, par, 3);
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       //printf("<2b>");
                       break;

               // 0x00 C source
               case 4: Font.font_buffer[byte_index] = CMD_GetParHex(cCMD_InstID_HEX, par, 4);
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       //printf("<2b>");
                       break;
               // 0x0000
               case 6: Font.font_buffer[byte_index] = CMD_GetParHex(cCMD_InstID_HEX, par, 6) & 0xFF;
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 6) & 0xFF00) >> 8;
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       //printf("<4b>");
                       break;

               // 0x000000
               case 8: Font.font_buffer[byte_index] = CMD_GetParHex(cCMD_InstID_HEX, par, 8) & 0xFF;
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 8) & 0xFF00) >> 8;
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 8) & 0xFF0000) >> 16;
                       cprintf("%.2X ", Font.font_buffer[byte_index]);
                       byte_index++;
                       //printf("<6b>");
                       break;

               //0x00000000
               case 10: Font.font_buffer[byte_index] = CMD_GetParHex(cCMD_InstID_HEX, par, 10) & 0xFF;
                        cprintf("%.2X ", Font.font_buffer[byte_index]);
                        byte_index++;
                        Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 10) & 0xFF00) >> 8;
                        cprintf("%.2X ", Font.font_buffer[byte_index]);
                        byte_index++;
                        Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 10) & 0xFF0000) >> 16;
                        cprintf("%.2X ", Font.font_buffer[byte_index]);
                        byte_index++;
                        Font.font_buffer[byte_index] = (CMD_GetParHex(cCMD_InstID_HEX, par, 10) & 0xFF000000) >> 24;
                        cprintf("%.2X ", Font.font_buffer[byte_index]);
                        byte_index++;
                        //printf("<8b>");
                        break;
             }
           }
           else
           {
             cprintf("\r\n\nNeed alocate more space !\r\n");
             Break = 1;
             break;
           }
         }
       }
       if(Break) break;
    }
    Font.whole_size = byte_index;         // Zkopiruj pocet byte celkem
  }

  if(OpenFile.file_type == cDATATYPE_BINARY)        // Binary source
  {
    Font.font_buffer = OpenFile.file_buffer;
  }
}


void ShowFontPreview()
{
  uchar x;
  uchar y;
  uint index;
  uchar bit;
  uint b;

  Font.size = Font.xres * Font.yres;                         // vypocet poctu bitu na jeden font
  Font.no_fonts = Font.whole_size / Font.size / 8;           // Vypocet poctu fontu v souboru
  cprintf("File: %s, x:%i, y:%i\r\n",Font.file_name, Font.xres, Font.yres);

  cprintf("ID:%.3i  size:%3i\r\n ", (Font.char_base / Font.yres), Font.size);
  for(index=0; index < Font.size; index++)
  {
      //(index % Font.xres)       ... pozice x
      //(index / Font.yres)       ... pozice y
      //(index / 8)               ... byte
      //(index % 8)               ... bit
      // (1 << (7-(index % 8)))   ... bitova maska

      //b = Font.font_buffer[Font.char_base + (index / (Font.yres * (Font.xres/8)))];

      cprintf("%c", (Font.font_buffer[Font.char_base + (index / 8)] & (1 << (7-(index % 8))))?'±':'.');

      if((index % Font.xres)==(Font.xres-1)) cprintf("\r\n ");
  }
  //gotoxy(2, FBorder.PosY + FBorder.Height);
  //printf("[%i x %i]",xres,yres);
}

void menu_def(uchar *MenuStrings, uchar cursor_attr, uchar back_attr)
{
  Menu.index = 0;
  Menu.ptr = MenuStrings;
  Menu.cursor_attr = cursor_attr;
  Menu.back_attr = back_attr;
  Menu.lines = 0;
}


void menu_refresh()
{
  uint i;
  uint scan;

  i=0;
  scan=0;
  textattr(Menu.back_attr);
  clrscr();
  do
  {
    if(scan == Menu.index)
    {
      textattr(Menu.cursor_attr);
    }
    else
    {
      textattr(Menu.back_attr);
    }

    if(Menu.ptr[i]==',')
    {
      scan++;
      cprintf("\r\n");
    }
    else
    {
      putch(Menu.ptr[i]);
    }

    i++;
  } while(Menu.ptr[i]);

  Menu.lines = scan;
}



void menu_select()
{
  if(kbUP)
  {
    if(Menu.index)
    {
      Menu.index--;                        // cursor up
    }
    else
    {
      Menu.index=Menu.lines;               // cursor roll down
    }
  }

  if(kbDOWN)
  {
    if(Menu.index < Menu.lines)
    {
      Menu.index++;                  // cursor down
    }
    else
    {
      Menu.index=0;                  // cursor roll up
    }
  }
  menu_refresh();
}


void InputLine(uchar *Buf, uchar Bytes, uchar *Index)
{
  if(kbBACKSPACE)
  {
    if(*Index)
    {
      (*Index)--;
      Buf[*Index] = 0;
      clrscr();
      cprintf("%s",Buf);
    }
  }
  else
  {
    if(Str_IsPrint(Ascii))
    {
      if((*Index) < (Bytes-1))
      {
        Buf[*Index] = Ascii;
        (*Index)++;
        putch(Ascii);
      }
    }

    //cprintf("c:%i ",Ascii);
  }

  if(kbENTER)
  {
    if(*Index < Bytes)
    {
      Buf[*Index] = 0;
    }
    else
    {
      Buf[(*Index) - 1] = 0;
    }

  }
}


void Export_File(uchar file_type)
{
  uint i;

  if ((ExportFile.file_stream = fopen(ExportFile.file_name, "wt")) == NULL)
  {
    fprintf(stderr, "Cannot open output file.\n");
  }
  else
  {
    switch(file_type)
    {
      case DATATYPE_BIN:
          i=0;
          while (i < Font.whole_size)
          {
            fputc(Font.font_buffer[i], ExportFile.file_stream);
            i++;
          }
          break;

      case DATATYPE_ASM:
          fprintf(ExportFile.file_stream, "\r\nasm\r\n{\r\n    DB  ", Font.font_buffer[i]);

          i=0;
          while (i < Font.whole_size)
          {
            if((!(i % 8)) & (i>1))
            {
              fprintf(ExportFile.file_stream, "\n    DB  %.2Xh, ", Font.font_buffer[i]);
            }
            else
            {
              if(i == (Font.whole_size - 1))
              {
                fprintf(ExportFile.file_stream, "%.2Xh", Font.font_buffer[i]);
              }
              else
              {
                fprintf(ExportFile.file_stream, "%.2Xh,", Font.font_buffer[i]);
              }
            }
            i++;
          }
          fprintf(ExportFile.file_stream, "\r\n}\r\n");
          break;

      case DATATYPE_C:
          fprintf(ExportFile.file_stream, "\r\nfont[] = {\r\n    ", Font.font_buffer[i]);

          i=0;
          while (i < Font.whole_size)
          {
            if((!(i % 8)) & (i>1))
            {
              fprintf(ExportFile.file_stream, "\n    0x%.2X, ", Font.font_buffer[i]);
            }
            else
            {
              if(i == (Font.whole_size - 1))
              {
                fprintf(ExportFile.file_stream, "0x%.2X", Font.font_buffer[i]);
              }
              else
              {
                fprintf(ExportFile.file_stream, "0x%.2X, ", Font.font_buffer[i]);
              }
            }
            i++;
          }
          fprintf(ExportFile.file_stream, "\r\n};\r\n");
          break;
    }
      fclose(ExportFile.file_stream);
  }
}


void main_proc()
{

  #define cInit          0
  #define cMain          1
  #define cSelectFile    10
  #define cFileLoad      20
  #define cFontPreview   30
  #define cFontConsole   40
  #define cFileConsole   50
  #define cFileExport    60
  #define cExit          0xFF

  switch(Main_Proc)
  {
    case cInit:
      Scr_Init();
      Scr_InitScreen(SWTitle);
      CMD_Init();
      Main_Proc = cMain;
      break;

    //--------------------------------------
    case cMain:
      Scr_InitDownMenu(DownMenu_main);
      Main_Proc++;
      break;

    case (cMain+1):
      if(kbhit())
      {
        GetKey();
        if(kbF2)    Main_Proc = cFileExport;
        if(kbF3)    Main_Proc = cFileLoad;
        if(kbF6)    Main_Proc = cSelectFile;
        if(kbALT_X) Main_Proc = cExit;
      }
      break;

    //--------------------------------------
    case cSelectFile:
      font_select_filename_def();
      Scr_DrawFrame();
      Main_Proc++;
      //OpenFile.filename_ix=0;
      cprintf("%s",Font.file_name);
      gotoxy(OpenFile.filename_ix+1,1);
      break;

    case cSelectFile+1:
      if(kbhit())
      {
        GetKey();
        InputLine(Font.file_name, sizeof(Font.file_name), &OpenFile.filename_ix);

        if(kbENTER)
        {
          font_select_filename_def();
          Scr_DestructFrame();
          Main_Proc = cFileLoad;
        }

        if((kbESC) || (kbALT_X))  Main_Proc++;
      }
      break;

    case cSelectFile+2:
      font_select_filename_def();
      Scr_DestructFrame();
      Main_Proc = cMain;
      break;

    //--------------------------------------
    case cFileLoad:
      if(!Font.file_name[0])
      {
        Main_Proc = cSelectFile;
      }
      else
      {
        file_console_def();
        Scr_DrawFrame();
        Main_Proc = (cFileLoad+2);
      }
      break;

    case (cFileLoad+1):
      file_console_def();
      Scr_DestructFrame();
      farfree(OpenFile.file_buffer);
      Scr_DrawFrame();
      Main_Proc++;
      break;

    case (cFileLoad+2):
      LoadFontFile();
      DetectDataType();
      Main_Proc = cFontPreview;
      break;

    //--------------------------------------
    case cFontPreview:
      font_console_def();
      Scr_DrawFrame();
      RecognizeData();
      Main_Proc++;
      break;

    case (cFontPreview+1):
      font_preview_def();
      Scr_DrawFrame();

      Font.xres = 16;
      Font.yres = 16;

      Main_Proc++;
      break;

    case (cFontPreview+2):
      clrscr();
      ShowFontPreview();
      Main_Proc++;
      break;

    case (cFontPreview+3):
      if(kbhit())
      {
        GetKey();
        //putch(Ascii);
        if(kbUP)    { Main_Proc = (cFontPreview+2);  if(Font.char_base < Font.whole_size) Font.char_base += 1; }
        if(kbDOWN)  { Main_Proc = (cFontPreview+2);  if(Font.char_base >= 1) Font.char_base -= 1; }
        if(kbLEFT)  { Main_Proc = (cFontPreview+2);  if(Font.char_base >= Font.yres) Font.char_base -= (Font.size / 8); }
        if(kbRIGHT) { Main_Proc = (cFontPreview+2);  if(Font.char_base < Font.whole_size) Font.char_base += (Font.size / 8); }

        if(kbPlus)  { Main_Proc = (cFontPreview+2);  Font.yres += 1; }
        if(kbMinus) { Main_Proc = (cFontPreview+2);  if(Font.yres > 1) Font.yres -= 1; }
        if(kbStar)  { Main_Proc = (cFontPreview+2);  Font.xres += 1; }
        if(kbSlash) { Main_Proc = (cFontPreview+2);  if(Font.xres > 1) Font.xres -= 1; }

        if((kbESC) || (kbALT_X))  Main_Proc++;
      }
      break;

    case (cFontPreview+4):
      font_preview_def();
      Scr_RefreshFrame();
      Scr_DestructFrame();
      Main_Proc = cFontConsole;
      break;


    //--------------------------------------
    case cFontConsole:
      if(kbhit())
      {
        GetKey();
        //putch(Ascii);
        if(kbF3) Main_Proc = (cFontPreview+1);
        if((kbESC) || (kbALT_X))  Main_Proc++;
      }
      break;

    case (cFontConsole+1):
      //cprintf("Destroying frame\r\n");
      font_console_def();
      Scr_RefreshFrame();
      Scr_DestructFrame();
      Main_Proc = cFileConsole;
      break;

    //--------------------------------------
    case cFileConsole:
      if(kbhit())
      {
        GetKey();
        if(kbF3) Main_Proc = (cFileLoad+1);
        if((kbESC) || (kbALT_X))  Main_Proc++;
      }
      break;

    case (cFileConsole+1):
      file_console_def();
      Scr_RefreshFrame();
      //cprintf("Destroying frame\r\n");
      //cprintf("Freeing memory\r\n");
      Scr_DestructFrame();
      Main_Proc++;
      break;

    case (cFileConsole+2):
      farfree(OpenFile.file_buffer);
      Main_Proc = cMain;
      break;

    //--------------------------------------
    case cFileExport:

      if(!OpenFile.file_stream)
      {
        Scr_Confirm(1, 33, 2, "Chyba", "Neni otevren zadny soubor !", 32+15, 1);
        Main_Proc = cMain;
      }
      else
      {
        font_export_filename_def();
        Scr_DrawFrame();
        Main_Proc++;
      }
      break;

    case (cFileExport+1):
      if(kbhit())
      {
        GetKey();
        InputLine(ExportFile.file_name, sizeof(ExportFile.file_name), &ExportFile.filename_ix);

        if(kbENTER)
        {
          Main_Proc = (cFileExport+3);
        }

        if((kbESC) || (kbALT_X))  Main_Proc++;
      }
      break;

    case (cFileExport+2):
      font_export_filename_def();
      Scr_DestructFrame();

      Main_Proc = cMain;
      break;

    case (cFileExport+3):
      font_export_format_def();
      Scr_DrawFrame();
      menu_def("     C++     ,    Binary   ,  Assembler  ", 10, 7+16);
      menu_refresh();
      Main_Proc++;
      break;

    case (cFileExport+4):
      if(kbhit())
      {
        GetKey();
        menu_select();

        if(kbENTER)
        {
          Main_Proc++;
        }

        if((kbESC) || (kbALT_X))  Main_Proc=(cFileExport+2);
      }
      break;

    case (cFileExport+5):
      font_export_format_def();
      Scr_DestructFrame();

      font_export_filename_def();
      Scr_DestructFrame();

      switch(Menu.index)
      {
        case 0: Export_File(DATATYPE_C);   break;
        case 1: Export_File(DATATYPE_BIN); break;
        case 2: Export_File(DATATYPE_ASM); break;
      }

      Scr_Confirm(1, 33, 2, "Udalost", "Soubor fontu ulozen.", 16+15, 1);

      Main_Proc = cMain;
      break;

    //--------------------------------------
    case cExit:
      ExitSignal = 1;
      gotoxy(1,25);
      textattr(15);
      cprintf("\r\n\nSend exit signal...");
      Main_Proc = cMain;
      break;

    //--------------------------------------
    default:
      Scr_Confirm(1, 32, 2, "Error", "Volany proces neexistuje !", 64+15, 1);
      Main_Proc = cMain;
      break;

  }

}


void main()
{
  main_init();

  while(!ExitSignal)
  {
    main_proc();
  }

  main_destruct();

  cprintf("\r\n\nCopyright (c) 2006 by Thomas Hudger\r\n\n");
}
