#ifndef __asmhelper_h__
#define __asmhelper_h__

#include <windows.h>
#include <stdio.h>

extern  unsigned long long AsmTestAccessTime(PBYTE Address); // rcx
extern  void AsmMovsDW(PDWORD dest, PDWORD src, unsigned int size_dw); //rcx, rdx, r8

extern  void AsmBlockMove2Move(PDWORD pmid, PDWORD pbuf, unsigned int size_dw); //rcx, rdx, r8


#endif
 

