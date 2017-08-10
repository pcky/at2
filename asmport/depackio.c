#include "asmport.h"
#include "fpc.h"
#include "defs.h"

/*
SYSTEM____FILLCHAR_formal_LONGINT_BYTE

Local variables:

var_ctrl_mask
var_ctrl_bits
var_ibuf_idx
var_ibuf_end
var_obuf_idx
var_obuf_src
var_command
var_count
var_offs

var_work_mem
var_ibufCount
var_ibufSize
var_input_size
var_output_size
var_input_ptr
var_output_ptr
var_work_ptr

var_le76
var_le77
var_le6a
var_le6c
var_le6e
var_le70
var_le72
var_le74
var_le78,
var_le7a_0
var_le7a_2
var_le7a_4
var_le7a_6
var_le7a_8
var_le82a
var_le82b

var_BitValue
var_CopyBits
var_CopyMin

var_dad
var_frq
var_leftC
var_rghtC
var_ibitCount
var_ibutBuffer
var_obufCount
var_index

PASCAL
const
  WORKMEM_SIZE = 64*1024;

var
  ibuf_idx,ibuf_end,obuf_idx,obuf_src: Pointer;
  ctrl_bits,ctrl_mask,
  command,count,offs: Word;

var
  work_mem: array[0..PRED(WORKMEM_SIZE)] of Byte;
  ibufCount,ibufSize: Word;
  input_size,output_size: Word;
  input_ptr,output_ptr,work_ptr: Pointer;

var
  le76,le77: Byte;
  le6a,le6c,le6e,le70,le72,le74,le78,
  le7a_0,le7a_2,le7a_4,le7a_6,le7a_8,le82a,le82b: Word;

const
  MAXFREQ       = 2000;
  MINCOPY       = 3;
  MAXCOPY       = 255;
  COPYRANGES    = 6;
  TERMINATE     = 256;
  FIRSTCODE     = 257;
  ROOT          = 1;
  CODESPERRANGE = MAXCOPY-MINCOPY+1;
  MAXCHAR       = FIRSTCODE+COPYRANGES*CODESPERRANGE-1;
  SUCCMAX       = MAXCHAR+1;
  TWICEMAX      = 2*MAXCHAR+1;
  MAXBUF        = PRED(64*1024);
  MAXDISTANCE   = 21389;
  MAXSIZE       = 21389+MAXCOPY;

const
  BitValue: array[1..14] of Word = (1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192);
  CopyBits: array[0..PRED(COPYRANGES)] of Word = (4,6,8,10,12,14);
  CopyMin:  array[0..PRED(COPYRANGES)] of Word = (0,16,80,336,1360,5456);

var
  leftC,rghtC: array[0..MAXCHAR] of Word;
  dad,frq: array[0..TWICEMAX] of Word;
  index,ibitCount,ibitBuffer,obufCount: Word;
*/

int DEPACKIO____RDC_DECODE()
{
  unsigned int v0; // ecx@2
  unsigned __int8 v1; // al@12
  const void *v2; // esi@12
  unsigned __int8 v3; // al@13
  const void *v4; // esi@13
  int result; // eax@14

  var_ctrl_mask = 0;
  var_ibuf_end = (unsigned __int16)var_input_size + var_input_ptr;
  var_ibuf_idx = var_input_ptr;
  var_obuf_idx = var_output_ptr;
  while ( 1 )
  {
    HIWORD(v0) = 0;
    if ( var_ibuf_idx >= (unsigned int)var_ibuf_end )
      break;
    var_ctrl_mask = (unsigned __int16)var_ctrl_mask >> 1;
    if ( !var_ctrl_mask )
    {
      var_ctrl_bits = *(_WORD *)var_ibuf_idx;
      var_ibuf_idx += 2;
      var_ctrl_mask = -32768;
    }
    if ( (unsigned __int16)var_ctrl_mask & (unsigned __int16)var_ctrl_bits )
    {
      var_command = (unsigned __int8)(*(_BYTE *)var_ibuf_idx >> 4);
      var_count = *(_BYTE *)var_ibuf_idx++ & 0xF;
      if ( var_command )
      {
        if ( var_command == 1 )
        {
          var_count += 16 * *(_BYTE *)var_ibuf_idx++;
          var_count += 19;
          LOWORD(v0) = var_count;
          memset((void *)var_obuf_idx, *(_BYTE *)var_ibuf_idx++, v0);
          var_obuf_idx += (unsigned __int16)var_count;
        }
        else if ( var_command == 2 )
        {
          var_offs = var_count + 3;
          var_offs = 16 * *(_BYTE *)var_ibuf_idx++ + var_count + 3;
          v1 = *(_BYTE *)var_ibuf_idx++;
          var_count = v1 + 16;
          LOWORD(v0) = var_offs;
          var_obuf_src = var_obuf_idx - v0;
          v2 = (const void *)(var_obuf_idx - v0);
          LOWORD(v0) = v1 + 16;
          qmemcpy((void *)var_obuf_idx, v2, v0);
          var_obuf_idx += (unsigned __int16)var_count;
        }
        else
        {
          var_offs = var_count + 3;
          v3 = *(_BYTE *)var_ibuf_idx;
          var_offs = 16 * *(_BYTE *)var_ibuf_idx++ + var_count + 3;
          LOWORD(v0) = 16 * v3 + var_count + 3;
          var_obuf_src = var_obuf_idx - v0;
          v4 = (const void *)(var_obuf_idx - v0);
          LOWORD(v0) = var_command;
          qmemcpy((void *)var_obuf_idx, v4, v0);
          var_obuf_idx += (unsigned __int16)var_command;
        }
      }
      else
      {
        var_count += 3;
        LOWORD(v0) = var_count;
        memset((void *)var_obuf_idx, *(_BYTE *)var_ibuf_idx++, v0);
        var_obuf_idx += (unsigned __int16)var_count;
      }
    }
    else
    {
      *(_BYTE *)var_obuf_idx++ = *(_BYTE *)var_ibuf_idx++;
    }
  }
  result = var_obuf_idx - var_output_ptr;
  var_output_size = var_obuf_idx - var_output_ptr;
  return result;
}

short DEPACKIO____RDC_DECOMPRESS_formal_formal_WORD__WORD(unsigned char *a3, unsigned char *a2, short a1)
{
  var_input_ptr = a3;
  var_output_ptr = a2;
  var_input_size = a1;
  DEPACKIO____RDC_DECODE();
  return var_output_size;
}

char DEPACKIO____GETCHAR(int a1)
{
  char result; // al@2

  LOWORD(a1) = var_ibufCount;
  if ( (unsigned __int16)var_ibufCount < (unsigned __int16)var_ibufSize )
  {
    result = *(_BYTE *)(var_input_ptr + a1);
    ++var_ibufCount;
  }
  return result;
}

char DEPACKIO____PUTCHAR(char result, int a2)
{
  LOWORD(a2) = var_output_size;
  *(_BYTE *)(var_output_ptr + a2) = result;
  var_output_size = a2 + 1;
  return result;
}

unsigned char DEPACKIO____LZSS_DECODE()
{
  int v0; // ebx@1
  unsigned __int16 v1; // dx@1
  int v2; // edi@1
  __int16 v3; // dx@2
  unsigned __int8 result; // al@3
  char v5; // cf@3
  char v6; // ch@9
  int v7; // ebx@11
  char v8; // al@11
  char v9; // cl@11

  var_ibufCount = 0;
  var_ibufSize = var_input_size;
  var_output_size = 0;
  v0 = 0;
  v1 = 0;
  v2 = 4078;
  while ( 1 )
  {
    while ( 1 )
    {
      v3 = v1 >> 1;
      if ( !HIBYTE(v3) )
      {
        result = DEPACKIO____GETCHAR(v0);
        if ( v5 )
          return result;
        LOBYTE(v3) = result;
      }
      if ( !(v3 & 1) )
        break;
      result = DEPACKIO____GETCHAR(v0);
      if ( v5 )
        return result;
      *(_BYTE *)(v2 + var_work_ptr) = result;
      v2 = ((_WORD)v2 + 1) & 0xFFF;
      DEPACKIO____PUTCHAR(result, v0);
    }
    result = DEPACKIO____GETCHAR(v0);
    if ( v5 )
      break;
    result = DEPACKIO____GETCHAR(v0);
    if ( v5 )
      break;
    BYTE1(v0) = result >> 4;
    LOBYTE(v0) = v6;
    do
    {
      v7 = v0 & 0xFFF;
      v8 = *(_BYTE *)(var_work_ptr + v7);
      *(_BYTE *)(v2 + var_work_ptr) = v8;
      v2 = ((_WORD)v2 + 1) & 0xFFF;
      DEPACKIO____PUTCHAR(v8, v7);
      v0 = v7 + 1;
    }
    while ( v9 != 1 );
  }
  return result;
}

short DEPACKIO____LZSS_DECOMPRESS_formal_formal_WORD__WORD(unsigned char *a3, unsigned char *a2, short a1)
{
  var_input_ptr = a3;
  var_output_ptr = a2;
  var_work_ptr = (int)&var_work_mem;
  var_input_size = a1;
  SYSTEM____FILLCHAR_formal_LONGINT_BYTE(0, 0x10000);
  DEPACKIO____LZSS_DECODE();
  return var_output_size;
}

short DEPACKIO____NEXTCODE(int a1, int a2)
{
  __int16 v2; // ax@1
  unsigned __int16 v3; // ax@1
  __int16 v4; // t2@1
  char v5; // cf@1
  __int16 v6; // ax@1
  unsigned __int8 v7; // dl@1

  v2 = __CFADD__(var_le78, var_le82a) + var_le82b;
  LOWORD(a2) = var_le82a;
  var_le82a += var_le78;
  v4 = v2;
  v3 = var_le82b;
  var_le82b = v4;
  LOWORD(a1) = a2 & 7;
  v5 = v3 & 1;
  v3 >>= 1;
  LOWORD(a2) = __RCR__(a2, v5);
  LOWORD(a2) = __RCR__(a2, v3 & 1);
  LOWORD(a2) = __RCR__(a2, (v3 >> 1) & 1);
  v6 = *(_WORD *)(a2 + var_input_ptr);
  v7 = *(_BYTE *)(a2 + var_input_ptr + 2);
  if ( (_WORD)a1 )
  {
    do
    {
      v5 = v7 & 1;
      v7 >>= 1;
      v6 = __RCR__(v6, v5);
      --a1;
    }
    while ( a1 );
  }
  LOWORD(a2) = 2 * (var_le78 - 9);
  return *(__int16 *)((char *)&var_le7a_0 + a2) & v6;
}

short DEPACKIO____LZW_DECODE__WORD()
{
  int a1;
  int v1; // ebx@1
  int v2; // ecx@1
  _BYTE *v3; // edi@1
  __int16 result; // ax@2
  int v5; // ecx@2
  char v6; // al@8
  char v7; // al@9
  int v8; // ST00_4@11
  int v9; // esi@11

  v1 = 0;
  v2 = 0;
  var_le72 = 0;
  var_le78 = 9;
  var_le70 = 258;
  var_le74 = 512;
  v3 = (_BYTE *)var_output_ptr;
  var_le6a = 0;
  var_le6c = 0;
  var_le6e = 0;
  var_le76 = 0;
  var_le77 = 0;
  var_le82a = 0;
  var_le82b = 0;
  var_le7a_0 = 511;
  var_le7a_2 = 1023;
  var_le7a_4 = 2047;
  var_le7a_6 = 4095;
  var_le7a_8 = 0x1FFF;
  while ( 1 )
  {
    result = DEPACKIO____NEXTCODE(v2, v1);
    if ( result == 257 )
      break;
    if ( result == 256 )
    {
      var_le78 = 9;
      var_le74 = 512;
      var_le70 = 258;
      var_le6a = DEPACKIO____NEXTCODE(v5, v1);
      var_le6c = var_le6a;
      var_le77 = var_le6a;
      var_le76 = var_le6a;
      *v3++ = var_le6a;
    }
    else
    {
      var_le6a = result;
      var_le6e = result;
      if ( (unsigned __int16)result >= (unsigned __int16)var_le70 )
      {
        var_le6a = var_le6c;
        ++var_le72;
      }
      while ( (unsigned __int16)var_le6a > 0xFFu )
      {
        a1 = var_work_ptr;
        LOWORD(v1) = 3 * var_le6a;
        v6 = *(_BYTE *)(v1 + var_work_ptr + 2);
        ++var_le72;
        var_le6a = *(_WORD *)(v1 + var_work_ptr);
      }
      v7 = var_le6a;
      var_le76 = var_le6a;
      var_le77 = var_le6a;
      v2 = (unsigned __int16)++var_le72;
      if ( var_le72 )
      {
        do
        {
          *v3++ = v7;
          --v2;
        }
        while ( v2 );
      }
      var_le72 = 0;
      v8 = a1;
      LOWORD(v1) = 3 * var_le70;
      v9 = var_work_ptr;
      *(_BYTE *)(v1 + var_work_ptr + 2) = var_le77;
      *(_WORD *)(v1 + v9) = var_le6c;
      ++var_le70;
      a1 = v8;
      var_le6c = var_le6e;
      LOWORD(v1) = var_le70;
      if ( var_le70 >= var_le74 && var_le78 != 14 )
      {
        ++var_le78;
        var_le74 *= 2;
      }
    }
  }
  var_output_size = 257;
  return result;
}

short DEPACKIO____LZW_DECOMPRESS_formal_formal__WORD(unsigned char *a3, unsigned char *a2)
{
  var_input_ptr = a3;
  var_output_ptr = a2;
  var_work_ptr = (int)&var_work_mem;
  DEPACKIO____LZW_DECODE__WORD();
  return var_output_size;
}

short DEPACKIO____INITTREE()
{
  signed int v0; // edi@1
  __int16 v1; // ax@2
  signed int v2; // ST00_4@2
  signed int v3; // ST00_4@4
  __int16 result; // ax@4

  v0 = 2;
  do
  {
    v1 = (unsigned __int16)v0 / 2u;
    v2 = v0;
    LOWORD(v0) = 2 * v0;
    *(__int16 *)((char *)&var_dad + v0) = v1;
    *(__int16 *)((char *)&var_frq + v0) = 1;
    HIWORD(v0) = HIWORD(v2);
    LOWORD(v0) = v2 + 1;
  }
  while ( (unsigned __int16)(v2 + 1) <= 0xDDDu );
  LOWORD(v0) = 1;
  do
  {
    v3 = v0;
    LOWORD(v0) = 2 * v0;
    *(__int16 *)((char *)&var_leftC + v0) = v0;
    result = v0 + 1;
    *(__int16 *)((char *)&var_rghtC + v0) = v0 + 1;
    HIWORD(v0) = HIWORD(v3);
    LOWORD(v0) = v3 + 1;
  }
  while ( (unsigned __int16)(v3 + 1) <= 0x6EEu );
  return result;
}

void DEPACKIO____UPDATEFREQ_WORD_WORD(short a1, short a2)
{
  int v2; // ecx@1
  int v3; // edi@1
  __int16 v4; // bx@2
  __int16 v5; // bx@2
  __int16 v6; // dx@2
  __int16 v7; // ax@3
  __int16 *v8; // edi@8
  __int16 *v9; // esi@8
  unsigned __int16 v10; // ax@9

  HIWORD(v2) = 0;
  HIWORD(v3) = 0;
  do
  {
    LOWORD(v3) = 2 * a2;
    v4 = *(__int16 *)((char *)&var_frq + v3);
    LOWORD(v3) = 2 * a1;
    v5 = *(__int16 *)((char *)&var_frq + v3) + v4;
    LOWORD(v3) = 2 * a2;
    v6 = *(__int16 *)((char *)&var_dad + v3);
    LOWORD(v3) = 2 * *(__int16 *)((char *)&var_dad + v3);
    *(__int16 *)((char *)&var_frq + v3) = v5;
    a2 = v6;
    if ( v6 != 1 )
    {
      LOWORD(v3) = 2 * v6;
      LOWORD(v3) = *(__int16 *)((char *)&var_dad + v3);
      v7 = v3;
      LOWORD(v3) = 2 * v3;
      if ( v6 == *(__int16 *)((char *)&var_leftC + v3) )
      {
        LOWORD(v3) = 2 * v7;
        a1 = *(__int16 *)((char *)&var_rghtC + v3);
      }
      else
      {
        LOWORD(v3) = 2 * v7;
        a1 = *(__int16 *)((char *)&var_leftC + v3);
      }
    }
  }
  while ( v6 != 1 );
  LOWORD(v3) = 2;
  if ( *(__int16 *)((char *)&var_frq + v3) == 2000 )
  {
    LOWORD(v2) = 3549;
    v9 = &var_frq + 1;
    v8 = &var_frq + 1;
    do
    {
      v10 = *v9;
      ++v9;
      *v8 = v10 >> 1;
      ++v8;
      --v2;
    }
    while ( v2 );
  }
}

void DEPACKIO____UPDATEMODEL_WORD(short a1)
{
  int v1; // ecx@1
  __int16 v2; // bx@1
  int v3; // edi@1
  __int16 v4; // dx@2
  __int16 v5; // si@2
  __int16 v6; // ST08_2@4
  __int16 v7; // dx@4
  __int16 v8; // ax@5
  int v9; // edi@5
  __int16 v10; // si@6
  int v11; // edi@8
  unsigned __int16 v12; // cx@12
  __int16 v13; // cx@13
  unsigned __int16 v14; // di@16

  HIWORD(v1) = 0;
  v2 = a1 + 1775;
  ++*(__int16 *)((char *)&var_frq + (unsigned __int16)(2 * (a1 + 1775)));
  v3 = (unsigned __int16)(2 * (a1 + 1775));
  if ( *(__int16 *)((char *)var_dad + (unsigned __int16)v3) != 1 )
  {
    v4 = *(__int16 *)((char *)var_dad + v3);
    LOWORD(v1) = 2 * v4;
    v5 = *(__int16 *)((char *)var_leftC + v1);
    if ( v5 == v2 )
    {
      LOWORD(v3) = 2 * v4;
      v5 = *(__int16 *)((char *)var_rghtC + v3);
    }
    v6 = v4;
    DEPACKIO____UPDATEFREQ_WORD_WORD(v5, v2);
    v7 = v6;
    do
    {
      v8 = *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * v7));
      v9 = (unsigned __int16)(2 * *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * v7)));
      if ( *(__int16 *)((char *)var_leftC + (unsigned __int16)v9) == v7 )
        v10 = *(__int16 *)((char *)var_rghtC + v9);
      else
        v10 = *(__int16 *)((char *)var_leftC + v9);
      HIWORD(v11) = 0;
      if ( *(unsigned __int16 *)((char *)&var_frq + (unsigned __int16)(2 * v2)) > *(unsigned __int16 *)((char *)&var_frq + (unsigned __int16)(2 * v10)) )
      {
        LOWORD(v11) = 2 * v8;
        if ( *(__int16 *)((char *)var_leftC + v11) == v7 )
        {
          LOWORD(v11) = 2 * v8;
          *(__int16 *)((char *)var_rghtC + v11) = v2;
        }
        else
        {
          *(__int16 *)((char *)var_leftC + (unsigned __int16)(2 * v8)) = v2;
        }
        v12 = 2 * v7;
        if ( *(__int16 *)((char *)var_leftC + (unsigned __int16)(2 * v7)) == v2 )
        {
          *(__int16 *)((char *)var_leftC + v12) = v10;
          v13 = *(__int16 *)((char *)var_rghtC + v12);
        }
        else
        {
          *(__int16 *)((char *)var_rghtC + v12) = v10;
          v13 = *(__int16 *)((char *)var_leftC + v12);
        }
        *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * v10)) = v7;
        *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * v2)) = v8;
        DEPACKIO____UPDATEFREQ_WORD_WORD(v13, v10);
        v2 = v10;
      }
      v14 = 2 * v2;
      v2 = *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * v2));
      v7 = *(__int16 *)((char *)var_dad + (unsigned __int16)(2 * *(__int16 *)((char *)var_dad + v14)));
    }
    while ( v7 != 1 );
  }
}

short DEPACKIO____INPUTCODE_WORD__WORD(unsigned short a1)
{
  __int16 v1; // bx@1
  unsigned __int16 v2; // cx@1

  v1 = 0;
  v2 = 1;
  do
  {
    if ( var_ibitCount )
    {
      --var_ibitCount;
    }
    else
    {
      if ( var_ibufCount == -1 )
        var_ibufCount = 0;
      var_ibitBuffer = *(_WORD *)((unsigned __int16)(2 * var_ibufCount++) + var_input_ptr);
      var_ibitCount = 15;
    }
    if ( (unsigned __int16)var_ibitBuffer > 0x7FFFu )
      v1 |= *(__int16 *)((char *)&var_BitValue + (unsigned __int16)(2 * (v2 - 1)));
    var_ibitBuffer *= 2;
    ++v2;
  }
  while ( v2 <= a1 );
  return v1;
}

short DEPACKIO____UNCOMPRESS__WORD()
{
  signed int v0; // ebx@1
  signed __int16 v1; // dx@1
  __int16 v2; // cx@1
  int v3; // eax@1

  v0 = 1;
  v1 = var_ibitCount;
  v2 = var_ibitBuffer;
  v3 = (unsigned __int16)var_ibufCount;
  do
  {
    if ( v1 )
    {
      --v1;
    }
    else
    {
      if ( (_WORD)v3 == -1 )
        LOWORD(v3) = 0;
      LOWORD(v3) = 2 * v3;
      v2 = *(_WORD *)(v3 + var_input_ptr);
      LOWORD(v3) = ((unsigned __int16)v3 >> 1) + 1;
      v1 = 15;
    }
    if ( (unsigned __int16)v2 <= 0x7FFFu )
      LOWORD(v0) = var_leftC[v0];
    else
      LOWORD(v0) = var_rghtC[v0];
    v2 *= 2;
  }
  while ( (signed __int16)v0 <= 1774 );
  LOWORD(v0) = v0 - 1775;
  var_ibitCount = v1;
  var_ibitBuffer = v2;
  var_ibufCount = v3;
  DEPACKIO____UPDATEMODEL_WORD(v0);
  return v0;
}

short DEPACKIO____SIXPACK_DECODE()
{
  int v0; // ebx@1
  __int16 result; // ax@1
  unsigned __int16 v2; // ax@8
  unsigned __int16 v3; // si@8
  unsigned __int16 v4; // ax@8
  int v5; // ecx@8
  unsigned __int16 v6; // dx@10
  _BYTE *v7; // edi@11
  int v8; // [sp-4h] [bp-10h]@11

  var_ibitCount = 0;
  var_ibitBuffer = 0;
  var_obufCount = 0;
  var_ibufCount = 0;
  HIWORD(v0) = 0;
  var_count = 0;
  DEPACKIO____INITTREE();
  for ( result = DEPACKIO____UNCOMPRESS__WORD(); result != 256; result = DEPACKIO____UNCOMPRESS__WORD() )
  {
    if ( (unsigned __int16)result >= 0x100u )
    {
      v2 = result - 257;
      var_index = v2 / 0xFDu;
      v3 = v2 + 3 - 253 * (v2 / 0xFDu);
      v4 = *(__int16 *)((char *)&var_CopyMin + (unsigned __int16)(2 * (v2 / 0xFDu)))
         + v3
         + DEPACKIO____INPUTCODE_WORD__WORD(*(__int16 *)((char *)&var_CopyBits + (unsigned __int16)(2 * (v2 / 0xFDu))));
      LOWORD(v0) = var_count;
      LOWORD(v5) = var_count - v4;
      if ( (unsigned __int16)var_count < v4 )
        LOWORD(v5) = v5 + 21644;
      v6 = 0;
      do
      {
        v8 = v0;
        LOWORD(v0) = var_obufCount;
        v7 = (_BYTE *)(v0 + var_output_ptr);
        HIWORD(v0) = HIWORD(v8);
        *v7 = *(_BYTE *)(v5 + var_work_ptr);
        if ( ++var_obufCount == -1 )
        {
          var_output_size = -1;
          var_obufCount = 0;
        }
        *(_BYTE *)(v8 + var_work_ptr) = *(_BYTE *)(v5 + var_work_ptr);
        LOWORD(v0) = v8 + 1;
        if ( (_WORD)v8 == 21643 )
          LOWORD(v0) = 0;
        LOWORD(v5) = v5 + 1;
        if ( (_WORD)v5 == 21644 )
          LOWORD(v5) = 0;
        ++v6;
      }
      while ( v6 < v3 );
      var_count += v3;
      if ( (unsigned __int16)var_count >= 0x548Cu )
        var_count -= 21644;
    }
    else
    {
      LOWORD(v0) = var_obufCount;
      *(_BYTE *)(v0 + var_output_ptr) = result;
      if ( ++var_obufCount == -1 )
      {
        var_output_size = -1;
        var_obufCount = 0;
      }
      LOWORD(v0) = var_count;
      *(_BYTE *)(v0 + var_work_ptr) = result;
      if ( ++var_count == 21644 )
        var_count = 0;
    }
  }
  var_output_size = var_obufCount;
  return result;
}

short DEPACKIO____SIXPACK_DECOMPRESS_formal_formal_WORD__WORD(unsigned char *a3, unsigned char *a2, short a1)
{
  var_input_ptr = a3;
  var_output_ptr = a2;
  var_work_ptr = (int)&var_work_mem;
  var_input_size = a1;
  DEPACKIO____SIXPACK_DECODE();
  return var_output_size;
}

int DEPACKIO____APACK_DECOMPRESS_formal_formal__LONGWORD(char *a2, unsigned char *a1)
{
  char *v2; // esi@1
  _BYTE *v3; // edi@1
  char v4; // dl@1
  unsigned __int8 v5; // cf@3
  char v6; // dl@4
  char v7; // tt@4
  unsigned __int8 v8; // cf@6
  char v9; // dl@7
  char v10; // tt@7
  unsigned int v11; // eax@9
  unsigned __int8 v12; // cf@9
  char v13; // dl@10
  char v14; // tt@10
  unsigned int v15; // ecx@12
  unsigned __int8 v16; // cf@13
  char v17; // dl@14
  char v18; // tt@14
  char v19; // tt@15
  signed int v20; // ecx@18
  unsigned __int8 v21; // cf@19
  char v22; // dl@19
  char v23; // dl@20
  char v24; // tt@20
  unsigned __int8 v25; // cf@21
  char v26; // dl@22
  char v27; // tt@22
  int v28; // ecx@24
  unsigned __int8 v29; // cf@26
  char v30; // dl@26
  char v31; // dl@27
  char v32; // tt@27
  unsigned __int8 v33; // cf@28
  char v34; // dl@29
  char v35; // tt@29
  unsigned __int8 v36; // cf@32
  unsigned __int32 v37; // eax@32
  unsigned __int8 v38; // cf@35
  char v39; // dl@35
  char v40; // dl@36
  char v41; // tt@36
  unsigned __int8 v42; // cf@37
  char v43; // dl@38
  char v44; // tt@38
  unsigned int v46; // [sp+10h] [bp-8h]@45

  v2 = a2;
  v3 = a1;
  v4 = -128;
LABEL_2:
  *v3++ = *v2++;
  while ( 1 )
  {
    v5 = __CFADD__(v4, v4);
    v4 *= 2;
    if ( !v4 )
    {
      v6 = *v2++;
      v7 = v5 + v6;
      v5 = __CFADD__(v5, v6) | __CFADD__(v6, v5 + v6);
      v4 = v6 + v7;
    }
    if ( !v5 )
      goto LABEL_2;
    v8 = __CFADD__(v4, v4);
    v4 *= 2;
    if ( !v4 )
    {
      v9 = *v2++;
      v10 = v8 + v9;
      v8 = __CFADD__(v8, v9) | __CFADD__(v9, v8 + v9);
      v4 = v9 + v10;
    }
    if ( !v8 )
    {
      v20 = 1;
      do
      {
        v21 = __CFADD__(v4, v4);
        v22 = 2 * v4;
        if ( !v22 )
        {
          v23 = *v2++;
          v24 = v21 + v23;
          v21 = __CFADD__(v21, v23) | __CFADD__(v23, v21 + v23);
          v22 = v23 + v24;
        }
        v20 += v21 + v20;
        v25 = __CFADD__(v22, v22);
        v4 = 2 * v22;
        if ( !v4 )
        {
          v26 = *v2++;
          v27 = v25 + v26;
          v25 = __CFADD__(v25, v26) | __CFADD__(v26, v25 + v26);
          v4 = v26 + v27;
        }
      }
      while ( v25 );
      v28 = v20 - 2;
      if ( !v28 )
      {
        v15 = 1;
        do
        {
          v29 = __CFADD__(v4, v4);
          v30 = 2 * v4;
          if ( !v30 )
          {
            v31 = *v2++;
            v32 = v29 + v31;
            v29 = __CFADD__(v29, v31) | __CFADD__(v31, v29 + v31);
            v30 = v31 + v32;
          }
          v15 += v29 + v15;
          v33 = __CFADD__(v30, v30);
          v4 = 2 * v30;
          if ( !v4 )
          {
            v34 = *v2++;
            v35 = v33 + v34;
            v33 = __CFADD__(v33, v34) | __CFADD__(v34, v33 + v34);
            v4 = v34 + v35;
          }
        }
        while ( v33 );
        goto LABEL_46;
      }
      v37 = (v28 - 1) << 8;
      LOBYTE(v37) = *v2++;
      v15 = 1;
      do
      {
        v38 = __CFADD__(v4, v4);
        v39 = 2 * v4;
        if ( !v39 )
        {
          v40 = *v2++;
          v41 = v38 + v40;
          v38 = __CFADD__(v38, v40) | __CFADD__(v40, v38 + v40);
          v39 = v40 + v41;
        }
        v15 += v38 + v15;
        v42 = __CFADD__(v39, v39);
        v4 = 2 * v39;
        if ( !v4 )
        {
          v43 = *v2++;
          v44 = v42 + v43;
          v42 = __CFADD__(v42, v43) | __CFADD__(v43, v42 + v43);
          v4 = v43 + v44;
        }
      }
      while ( v42 );
      if ( v37 < 0x7D00 )
      {
        if ( BYTE1(v37) >= 5u )
        {
LABEL_44:
          ++v15;
          goto LABEL_45;
        }
        if ( v37 > 0x7F )
        {
LABEL_45:
          _InterlockedExchange((volatile signed __int32 *)&v46, v37);
LABEL_46:
          v11 = v46;
          goto LABEL_47;
        }
      }
LABEL_43:
      ++v15;
      goto LABEL_44;
    }
    v11 = 0;
    v12 = __CFADD__(v4, v4);
    v4 *= 2;
    if ( !v4 )
    {
      v13 = *v2++;
      v14 = v12 + v13;
      v12 = __CFADD__(v12, v13) | __CFADD__(v13, v12 + v13);
      v4 = v13 + v14;
    }
    if ( !v12 )
      break;
    v15 = 1;
    LOBYTE(v11) = 16;
    do
    {
      v16 = __CFADD__(v4, v4);
      v4 *= 2;
      if ( !v4 )
      {
        v17 = *v2++;
        v18 = v16 + v17;
        v16 = __CFADD__(v16, v17) | __CFADD__(v17, v16 + v17);
        v4 = v17 + v18;
      }
      v19 = v16 + v11;
      v5 = __CFADD__(v16, (_BYTE)v11) | __CFADD__((_BYTE)v11, v16 + v11);
      LOBYTE(v11) = v11 + v19;
    }
    while ( !v5 );
    if ( (_BYTE)v11 )
    {
LABEL_47:
      qmemcpy(v3, &v3[-v11], v15);
      v3 += v15;
    }
    else
    {
      *v3++ = 0;
    }
  }
  LOBYTE(v11) = *v2++;
  v36 = v11 & 1;
  v37 = v11 >> 1;
  if ( v37 )
  {
    v15 = v36;
    goto LABEL_43;
  }
  return v3 - a1;
}