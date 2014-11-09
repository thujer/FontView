
#include "shorttyp.h"

typedef struct
{
  uchar index;
  uchar *ptr;
  uchar cursor_attr;
  uchar back_attr;
  uchar lines;
} tstr_Menu;


typedef struct
{
  uchar far *file_buffer;
  uchar      filename_ix;
  uint       file_length;
  uint       file_bytes_read;
  FILE      *file_stream;
  uchar      file_type;
} tstr_OpenFile;


typedef struct
{
  uchar far *file_buffer;
  uchar      filename_ix;
  FILE      *file_stream;
  uchar      file_name[20];
  uchar      file_type;
} tstr_ExportFile;


typedef struct
{
  uchar xres;
  uchar yres;
  uint  no_fonts;
  uint  size;
  uint  char_base;
  uint  whole_size;
  uchar file_name[20];
  uchar far *font_buffer;
} tstr_Font;

