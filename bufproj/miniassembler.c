/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Bob Dondero and Catherine Yu and Joseph Chen               */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, unsigned int uiImmed)
{
   unsigned int uiInstr;
   unsigned int uiDisp;
         
   uiInstr = 0x52800000;
   
   uiInstr |= uiReg;

   uiDisp = uiImmed;
   uiDisp = uiDisp << 5;
   uiDisp &= 0x001fffe0;
   uiInstr |= uiDisp;
   
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;
   unsigned int uiDispLo;
   unsigned int uiDispHi;
         
   uiInstr = 0x10000000;
   
   uiInstr |= uiReg;
    
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   uiDispLo = uiDisp & 0x3;
   uiDispLo = uiDispLo << 29;
   uiInstr |= uiDispLo;
   
   uiDispHi = uiDisp >> 2;
   uiDispHi = uiDispHi << 5;
   uiDispHi &= 0x00ffffe0;
   uiInstr |= uiDispHi;
   
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_strb(unsigned int uiFromReg, unsigned
                                int uiToReg)
{
   unsigned int uiInstr;
   unsigned int uiToRegShift;
         
   uiInstr = 0x39000000;
   
   uiInstr |= uiFromReg;
   uiToRegShift = uiToReg << 5;
   uiInstr |= uiToRegShift;
   
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   signed int uiDisp;
         
   uiInstr = 0x14000000;
   
   uiDisp = (ulAddr - ulAddrOfThisInstr);
   uiDisp >>= 2;
   uiDisp &= 0x03ffffff;
   uiInstr |= uiDisp;
   
   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_bl(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   signed int uiDisp;
         
   uiInstr = 0x94000000;
   
   uiDisp = (ulAddr - ulAddrOfThisInstr);
   uiDisp >>= 2;
   uiDisp &= 0x03ffffff;
   uiInstr |= uiDisp;
   
   return uiInstr;
}
