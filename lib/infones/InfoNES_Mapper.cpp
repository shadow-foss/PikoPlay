/*===================================================================*/
/*                                                                   */
/*  InfoNES_Mapper.cpp : InfoNES Mapper Function                     */
/*                                                                   */
/*  2000/05/16  InfoNES Project ( based on NesterJ and pNesX )       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_Mapper.h"
#include "K6502.h"
#include <pico.h>

/*-------------------------------------------------------------------*/
/*  Mapper resources                                                 */
/*-------------------------------------------------------------------*/

/* Disk System RAM */
BYTE *DRAM = nullptr;  // PikoPlay: only mapper 235 uses it; that mapper is refused

/*-------------------------------------------------------------------*/
/*  Table of Mapper initialize function                              */
/*-------------------------------------------------------------------*/

struct MapperTable_tag MapperTable[] =
    {
        {0, Map0_Init},
        {1, Map1_Init},
        {2, Map2_Init},
        {3, Map3_Init},
        {4, Map4_Init},
        //        {5, Map5_Init},
//{6, Map6_Init},  // PikoPlay: removed (RAM)
        {7, Map7_Init},
        {8, Map8_Init},
        {9, Map9_Init},
        {10, Map10_Init},
        {11, Map11_Init},
        {13, Map13_Init},
        {15, Map15_Init},
        {16, Map16_Init},
        {17, Map17_Init},
        {18, Map18_Init},
//{19, Map19_Init},  // PikoPlay: removed (RAM)
        {21, Map21_Init},
        {22, Map22_Init},
        {23, Map23_Init},
        {24, Map24_Init},
        {25, Map25_Init},
        {26, Map26_Init},
        {32, Map32_Init},
        {33, Map33_Init},
        {34, Map34_Init},
        {40, Map40_Init},
        {41, Map41_Init},
        {42, Map42_Init},
        {43, Map43_Init},
        {44, Map44_Init},
        {45, Map45_Init},
        {46, Map46_Init},
        {47, Map47_Init},
        {48, Map48_Init},
        {49, Map49_Init},
        {50, Map50_Init},
        {51, Map51_Init},
        {57, Map57_Init},
        {58, Map58_Init},
        {60, Map60_Init},
        {61, Map61_Init},
        {62, Map62_Init},
        {64, Map64_Init},
        {65, Map65_Init},
        {66, Map66_Init},
        {67, Map67_Init},
        {68, Map68_Init},
        {69, Map69_Init},
        {70, Map70_Init},
        {71, Map71_Init},
        {72, Map72_Init},
        {73, Map73_Init},
        {74, Map74_Init},
        {75, Map75_Init},
        {76, Map76_Init},
        {77, Map77_Init},
        {78, Map78_Init},
        {79, Map79_Init},
        {80, Map80_Init},
        {82, Map82_Init},
        {83, Map83_Init},
        //  {  85, Map85_Init  },
        {86, Map86_Init},
        {87, Map87_Init},
        {88, Map88_Init},
        {89, Map89_Init},
        {90, Map90_Init},
        {91, Map91_Init},
        {92, Map92_Init},
        {93, Map93_Init},
        {94, Map94_Init},
        {95, Map95_Init},
        {96, Map96_Init},
        {97, Map97_Init},
        {100, Map100_Init},
        {101, Map101_Init},
        {105, Map105_Init},
        {107, Map107_Init},
        {108, Map108_Init},
        {109, Map109_Init},
        {110, Map110_Init},
        {112, Map112_Init},
        {113, Map113_Init},
        {114, Map114_Init},
        {115, Map115_Init},
        {116, Map116_Init},
        {117, Map117_Init},
        {118, Map118_Init},
        {119, Map119_Init},
        {122, Map122_Init},
        {133, Map133_Init},
        {134, Map134_Init},
        {135, Map135_Init},
        {140, Map140_Init},
        {151, Map151_Init},
        {160, Map160_Init},
        {180, Map180_Init},
        {181, Map181_Init},
        {182, Map182_Init},
        {183, Map183_Init},
        {185, Map185_Init},
        {187, Map187_Init},
//{188, Map188_Init},  // PikoPlay: removed (RAM)
        {189, Map189_Init},
        {191, Map191_Init},
        {193, Map193_Init},
        {194, Map194_Init},
        {200, Map200_Init},
        {201, Map201_Init},
        {202, Map202_Init},
        {222, Map222_Init},
        {225, Map225_Init},
        {226, Map226_Init},
        {227, Map227_Init},
        {228, Map228_Init},
        {229, Map229_Init},
        {230, Map230_Init},
        {231, Map231_Init},
        {232, Map232_Init},
        {233, Map233_Init},
        {234, Map234_Init},
        {235, Map235_Init},
        {236, Map236_Init},
        {240, Map240_Init},
        {241, Map241_Init},
        {242, Map242_Init},
        {243, Map243_Init},
        {244, Map244_Init},
        {245, Map245_Init},
        {246, Map246_Init},
        {248, Map248_Init},
        {249, Map249_Init},
        {251, Map251_Init},
        {252, Map252_Init},
        {255, Map255_Init},
        {-1, NULL}};

/*-------------------------------------------------------------------*/
/*  body of Mapper functions                                         */
/*-------------------------------------------------------------------*/

/*===================================================================*/
/*                                                                   */
/*                            Mapper 0                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 0                                              */
/*-------------------------------------------------------------------*/
void Map0_Init()
{
  /* Initialize Mapper */
  MapperInit = Map0_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  if (NesHeader.byRomSize > 1)
  {
    ROMBANK0 = ROMPAGE(0);
    ROMBANK1 = ROMPAGE(1);
    ROMBANK2 = ROMPAGE(2);
    ROMBANK3 = ROMPAGE(3);
  }
  else if (NesHeader.byRomSize > 0)
  {
    ROMBANK0 = ROMPAGE(0);
    ROMBANK1 = ROMPAGE(1);
    ROMBANK2 = ROMPAGE(0);
    ROMBANK3 = ROMPAGE(1);
  }
  else
  {
    ROMBANK0 = ROMPAGE(0);
    ROMBANK1 = ROMPAGE(0);
    ROMBANK2 = ROMPAGE(0);
    ROMBANK3 = ROMPAGE(0);
  }

  /* Set PPU Banks */
  if (NesHeader.byVRomSize > 0)
  {
    for (int nPage = 0; nPage < 8; ++nPage)
      PPUBANK[nPage] = VROMPAGE(nPage);
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring(1, 1);
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write Function                                          */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_Write)(WORD wAddr, BYTE byData)
{
  /*
 *  Dummy Write to Mapper
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to SRAM Function                                  */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_Sram)(WORD wAddr, BYTE byData)
{
  /*
 *  Dummy Write to Sram
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_Apu)(WORD wAddr, BYTE byData)
{
  /*
 *  Dummy Write to Apu
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Read from APU Function                                  */
/*-------------------------------------------------------------------*/
BYTE __not_in_flash_func(Map0_ReadApu)(WORD wAddr)
{
  /*
 *  Dummy Read from Apu
 *
 */
  return (wAddr >> 8);
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 V-Sync Function                                         */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_VSync)()
{
  /*
 *  Dummy Callback at VSync
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_HSync)()
{
/*
 *  Dummy Callback at HSync
 *
 */
#if 0 
  // Frame IRQ
  FrameStep += STEP_PER_SCANLINE;
  if ( FrameStep > STEP_PER_FRAME && FrameIRQ_Enable )
  {
    FrameStep %= STEP_PER_FRAME;
    IRQ_REQ;
    APU_Reg[ 0x4015 ] |= 0x40;
  }
#endif
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 PPU Function                                            */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_PPU)(WORD wAddr)
{
  /*
 *  Dummy Callback at PPU
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Rendering Screen Function                               */
/*-------------------------------------------------------------------*/
void __not_in_flash_func(Map0_RenderScreen)(BYTE byMode)
{
  /*
 *  Dummy Callback at Rendering Screen
 *
 */
}
/*===================================================================*/
/*                                                                   */
/*                            Mapper 1                               */
/*                                                                   */
/*===================================================================*/

BYTE  Map1_Regs[ 4 ];
DWORD Map1_Cnt;
BYTE  Map1_Latch;
WORD  Map1_Last_Write_Addr;

enum Map1_Size_t
{
  Map1_SMALL,
  Map1_512K,
  Map1_1024K
};

Map1_Size_t Map1_Size;
DWORD Map1_256K_base;
DWORD Map1_swap;

// these are the 4 ROM banks currently selected
DWORD Map1_bank1;
DWORD Map1_bank2;
DWORD Map1_bank3;
DWORD Map1_bank4;

DWORD Map1_HI1;
DWORD Map1_HI2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 1                                              */
/*-------------------------------------------------------------------*/
void Map1_Init()
{
  DWORD size_in_K;

  /* Initialize Mapper */
  MapperInit = Map1_Init;

  /* Write to Mapper */
  MapperWrite = Map1_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  Map1_Cnt = 0;
  Map1_Latch = 0x00;

  Map1_Regs[ 0 ] = 0x0c;
  Map1_Regs[ 1 ] = 0x00;
  Map1_Regs[ 2 ] = 0x00;
  Map1_Regs[ 3 ] = 0x00;

  size_in_K = ( NesHeader.byRomSize << 1 ) * 8;

  if ( size_in_K == 1024 )
  {
    Map1_Size = Map1_1024K;
  } 
  else if(size_in_K == 512)
  {
    Map1_Size = Map1_512K;
  }
  else
  {
    Map1_Size = Map1_SMALL;
  }

  Map1_256K_base = 0; // use first 256K
  Map1_swap = 0;

  if( Map1_Size == Map1_SMALL )
  {
    // set two high pages to last two banks
    Map1_HI1 = ( NesHeader.byRomSize << 1 ) - 2;
    Map1_HI2 = ( NesHeader.byRomSize << 1 ) - 1;
  }
  else
  {
    // set two high pages to last two banks of current 256K region
    Map1_HI1 = ( 256 / 8 ) - 2;
    Map1_HI2 = ( 256 / 8 ) - 1;
  }

  // set CPU bank pointers
  Map1_bank1 = 0;
  Map1_bank2 = 1;
  Map1_bank3 = Map1_HI1;
  Map1_bank4 = Map1_HI2;

  /* Set ROM Banks */
  Map1_set_ROM_banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

void Map1_set_ROM_banks()
{
  ROMBANK0 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank1 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );  
  ROMBANK1 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank2 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank3 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank4 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 1 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map1_Write( WORD wAddr, BYTE byData )
{
/*
 * MMC1
 */
  DWORD dwRegNum;

  // if write is to a different reg, reset
  if( ( wAddr & 0x6000 ) != ( Map1_Last_Write_Addr & 0x6000 ) )
  {
    Map1_Cnt = 0;
    Map1_Latch = 0x00;
  }
  Map1_Last_Write_Addr = wAddr;

  // if bit 7 set, reset and return
  if ( byData & 0x80 )
  {
    Map1_Cnt = 0;
    Map1_Latch = 0x00;
    return;
  }

  if ( byData & 0x01 ) Map1_Latch |= ( 1 << Map1_Cnt );
  Map1_Cnt++;
  if( Map1_Cnt < 5 ) return;

  dwRegNum = ( wAddr & 0x7FFF ) >> 13;
  Map1_Regs[ dwRegNum ] = Map1_Latch;

  Map1_Cnt = 0;
  Map1_Latch = 0x00;

  switch( dwRegNum )
  {
    case 0:
      {
        // set mirroring
        if( Map1_Regs[0] & 0x02 )
        {
          if( Map1_Regs[0] & 0x01 )
          {
            InfoNES_Mirroring( 0 );
          }
          else
          {
            InfoNES_Mirroring( 1 );
          }
        }
        else
        {
          // one-screen mirroring
          if( Map1_Regs[0] & 0x01 )
          {
            InfoNES_Mirroring( 2 );
          }
          else
          {
            InfoNES_Mirroring( 3 );
          }
        }
      }
      break;

    case 1:
      {
        BYTE byBankNum = Map1_Regs[1];

        if ( Map1_Size == Map1_1024K )
        {
          if ( Map1_Regs[0] & 0x10 )
          {
            if ( Map1_swap )
            {
              Map1_256K_base = (Map1_Regs[1] & 0x10) >> 4;
              if(Map1_Regs[0] & 0x08)
              {
                Map1_256K_base |= ((Map1_Regs[2] & 0x10) >> 3);
              }
              Map1_set_ROM_banks();
              Map1_swap = 0;
            }
            else
            {
              Map1_swap = 1;
            }
          }
          else
          {
            // use 1st or 4th 256K banks
            Map1_256K_base = ( Map1_Regs[1] & 0x10 ) ? 3 : 0;
            Map1_set_ROM_banks();
          }
        }
        else if((Map1_Size == Map1_512K) && (!NesHeader.byVRomSize))
        {
          Map1_256K_base = (Map1_Regs[1] & 0x10) >> 4;
          Map1_set_ROM_banks();
        }
        else if ( NesHeader.byVRomSize )
        {
          // set VROM bank at $0000
          if ( Map1_Regs[0] & 0x10 )
          {
            // swap 4K
            byBankNum <<= 2;
            PPUBANK[ 0 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 1 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 2 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 3 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
            InfoNES_SetupChr();
          }
          else
          {
            // swap 8K
            byBankNum <<= 2;
            PPUBANK[ 0 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 1 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 2 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 3 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 4 ] = VROMPAGE( (byBankNum+4) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 5 ] = VROMPAGE( (byBankNum+5) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 6 ] = VROMPAGE( (byBankNum+6) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 7 ] = VROMPAGE( (byBankNum+7) % (NesHeader.byVRomSize << 3) );
            InfoNES_SetupChr();
          }
        }
      }
      break;

    case 2:
      {
        BYTE byBankNum = Map1_Regs[2];

        if((Map1_Size == Map1_1024K) && (Map1_Regs[0] & 0x08))
        {
          if(Map1_swap)
          {
            Map1_256K_base =  (Map1_Regs[1] & 0x10) >> 4;
            Map1_256K_base |= ((Map1_Regs[2] & 0x10) >> 3);
            Map1_set_ROM_banks();
            Map1_swap = 0;
          }
          else
          {
            Map1_swap = 1;
          }
        }

        if(!NesHeader.byVRomSize) 
        {
          if ( Map1_Regs[ 0 ] & 0x10 )
          {
            byBankNum <<= 2;
#if 0
            PPUBANK[ 4 ] = VRAMPAGE0( byBankNum+0 );
            PPUBANK[ 5 ] = VRAMPAGE0( byBankNum+1 );
            PPUBANK[ 6 ] = VRAMPAGE0( byBankNum+2 );
            PPUBANK[ 7 ] = VRAMPAGE0( byBankNum+3 );
#else
            PPUBANK[ 4 ] = CRAMPAGE( byBankNum+0 );
            PPUBANK[ 5 ] = CRAMPAGE( byBankNum+1 );
            PPUBANK[ 6 ] = CRAMPAGE( byBankNum+2 );
            PPUBANK[ 7 ] = CRAMPAGE( byBankNum+3 );
#endif
            InfoNES_SetupChr();
            break;
          }
        }

        // set 4K VROM bank at $1000
        if(Map1_Regs[0] & 0x10)
        {
          // swap 4K
          byBankNum <<= 2;
          PPUBANK[ 4 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 5 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 6 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 7 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
          InfoNES_SetupChr(); 
        }
      }
      break;

    case 3:
      {
        BYTE byBankNum = Map1_Regs[3];

        // set ROM bank
        if ( Map1_Regs[0] & 0x08 )
        {
          // 16K of ROM
          byBankNum <<= 1;

          if ( Map1_Regs[0] & 0x04 )
          {
            // 16K of ROM at $8000
            Map1_bank1 = byBankNum;
            Map1_bank2 = byBankNum+1;
            Map1_bank3 = Map1_HI1;
            Map1_bank4 = Map1_HI2;
          }
          else
          {
            // 16K of ROM at $C000
            if(Map1_Size == Map1_SMALL)
            {
              Map1_bank1 = 0;
              Map1_bank2 = 1;
              Map1_bank3 = byBankNum;
              Map1_bank4 = byBankNum+1;
            }
          }
        }
        else
        {
          // 32K of ROM at $8000
          byBankNum <<= 1;

          Map1_bank1 = byBankNum;
          Map1_bank2 = byBankNum+1;
          if(Map1_Size == Map1_SMALL)
          {
            Map1_bank3 = byBankNum+2;
            Map1_bank4 = byBankNum+3;
          }
        }
        Map1_set_ROM_banks();
      }
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 2 (UNROM)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 2                                              */
/*-------------------------------------------------------------------*/
void Map2_Init()
{
  /* Initialize Mapper */
  MapperInit = Map2_Init;

  /* Write to Mapper */
  MapperWrite = Map2_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 2 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map2_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  byData %= NesHeader.byRomSize;
  byData <<= 1;

  ROMBANK0 = ROMPAGE( byData );
  ROMBANK1 = ROMPAGE( byData + 1 );
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 3 (VROM Switch)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 3                                              */
/*-------------------------------------------------------------------*/
void Map3_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map3_Init;

  /* Write to Mapper */
  MapperWrite = Map3_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  if ( ( NesHeader.byRomSize << 1 ) > 2 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 2 );
    ROMBANK3 = ROMPAGE( 3 );    
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 0 );
    ROMBANK3 = ROMPAGE( 1 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  /* "DragonQuest" doesn't run if IRQ isn't made to occur in CLI */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 3 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map3_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBase;

  /* Set PPU Banks */
  byData %= NesHeader.byVRomSize;
  dwBase = ( (DWORD)byData ) << 3;

  PPUBANK[ 0 ] = VROMPAGE( dwBase + 0 );
  PPUBANK[ 1 ] = VROMPAGE( dwBase + 1 );
  PPUBANK[ 2 ] = VROMPAGE( dwBase + 2 );
  PPUBANK[ 3 ] = VROMPAGE( dwBase + 3 );
  PPUBANK[ 4 ] = VROMPAGE( dwBase + 4 );
  PPUBANK[ 5 ] = VROMPAGE( dwBase + 5 );
  PPUBANK[ 6 ] = VROMPAGE( dwBase + 6 );
  PPUBANK[ 7 ] = VROMPAGE( dwBase + 7 );

  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 4 (MMC3)                             */
/*                                                                   */
/*===================================================================*/

BYTE  Map4_Regs[ 8 ];
DWORD Map4_Rom_Bank;
DWORD Map4_Prg0, Map4_Prg1;
DWORD Map4_Chr01, Map4_Chr23;
DWORD Map4_Chr4, Map4_Chr5, Map4_Chr6, Map4_Chr7;

#define Map4_Chr_Swap()    ( Map4_Regs[ 0 ] & 0x80 )
#define Map4_Prg_Swap()    ( Map4_Regs[ 0 ] & 0x40 )

BYTE Map4_IRQ_Enable;
BYTE Map4_IRQ_Cnt;
BYTE Map4_IRQ_Latch;
BYTE Map4_IRQ_Request;
BYTE Map4_IRQ_Present;
BYTE Map4_IRQ_Present_Vbl;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 4                                              */
/*-------------------------------------------------------------------*/
void Map4_Init()
{
  /* Initialize Mapper */
  MapperInit = Map4_Init;

  /* Write to Mapper */
  MapperWrite = Map4_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map4_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map4_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map4_Prg0 = 0;
  Map4_Prg1 = 1;
  Map4_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map4_Chr01 = 0;
    Map4_Chr23 = 2;
    Map4_Chr4  = 4;
    Map4_Chr5  = 5;
    Map4_Chr6  = 6;
    Map4_Chr7  = 7;
    Map4_Set_PPU_Banks();
  } else {
    Map4_Chr01 = Map4_Chr23 = 0;
    Map4_Chr4 = Map4_Chr5 = Map4_Chr6 = Map4_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map4_IRQ_Enable = 0;
  Map4_IRQ_Cnt = 0;
  Map4_IRQ_Latch = 0;
  Map4_IRQ_Request = 0;
  Map4_IRQ_Present = 0;
  Map4_IRQ_Present_Vbl = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map4_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map4_Regs[ 0 ] = byData;
      Map4_Set_PPU_Banks();
      Map4_Set_CPU_Banks();
      break;

    case 0x8001:
      Map4_Regs[ 1 ] = byData;
      dwBankNum = Map4_Regs[ 1 ];

      switch ( Map4_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map4_Chr01 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map4_Chr23 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr4 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr5 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr6 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr7 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map4_Prg0 = dwBankNum;
          Map4_Set_CPU_Banks();
          break;

        case 0x07:
          Map4_Prg1 = dwBankNum;
          Map4_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map4_Regs[ 2 ] = byData;

      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map4_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map4_Regs[ 4 ] = byData;
      Map4_IRQ_Latch = byData;
      break;

    case 0xc001:
      Map4_Regs[ 5 ] = byData;
      if ( PPU_Scanline < 240 )
      {
          Map4_IRQ_Cnt |= 0x80;
          Map4_IRQ_Present = 0xff;
      } else {
          Map4_IRQ_Cnt |= 0x80;
          Map4_IRQ_Present_Vbl = 0xff;
          Map4_IRQ_Present = 0;
      }
      break;

    case 0xe000:
      Map4_Regs[ 6 ] = byData;
      Map4_IRQ_Enable = 0;
			Map4_IRQ_Request = 0;
      break;

    case 0xe001:
      Map4_Regs[ 7 ] = byData;
      Map4_IRQ_Enable = 1;
			Map4_IRQ_Request = 0;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map4_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( 0 <= PPU_Scanline && PPU_Scanline <= 239 ) && 
       ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) )
  {
		if( Map4_IRQ_Present_Vbl ) {
			Map4_IRQ_Cnt = Map4_IRQ_Latch;
			Map4_IRQ_Present_Vbl = 0;
		}
		if( Map4_IRQ_Present ) {
			Map4_IRQ_Cnt = Map4_IRQ_Latch;
			Map4_IRQ_Present = 0;
		} else if( Map4_IRQ_Cnt > 0 ) {
			Map4_IRQ_Cnt--;
		}

		if( Map4_IRQ_Cnt == 0 ) {
			if( Map4_IRQ_Enable ) {
				Map4_IRQ_Request = 0xFF;
			}
			Map4_IRQ_Present = 0xFF;
		}
	}
	if( Map4_IRQ_Request  ) {
		IRQ_REQ;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Set CPU Banks Function                                  */
/*-------------------------------------------------------------------*/
void Map4_Set_CPU_Banks()
{
  if ( Map4_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map4_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map4_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map4_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map4_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Set PPU Banks Function                                  */
/*-------------------------------------------------------------------*/
void Map4_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map4_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map4_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map4_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map4_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map4_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map4_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map4_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map4_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map4_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map4_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map4_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map4_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map4_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map4_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map4_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map4_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map4_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
  else
  {
    if ( Map4_Chr_Swap() )
    { 
#if 0
      PPUBANK[ 0 ] = VRAMPAGE0( 0 );
      PPUBANK[ 1 ] = VRAMPAGE0( 1 );
      PPUBANK[ 2 ] = VRAMPAGE0( 2 );
      PPUBANK[ 3 ] = VRAMPAGE0( 3 );
      PPUBANK[ 4 ] = VRAMPAGE1( 0 );
      PPUBANK[ 5 ] = VRAMPAGE1( 1 );
      PPUBANK[ 6 ] = VRAMPAGE1( 2 );
      PPUBANK[ 7 ] = VRAMPAGE1( 3 );
#else
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
#endif
      InfoNES_SetupChr();
    } else {
#if 0
      PPUBANK[ 0 ] = VRAMPAGE1( 0 );
      PPUBANK[ 1 ] = VRAMPAGE1( 1 );
      PPUBANK[ 2 ] = VRAMPAGE1( 2 );
      PPUBANK[ 3 ] = VRAMPAGE1( 3 );
      PPUBANK[ 4 ] = VRAMPAGE0( 0 );
      PPUBANK[ 5 ] = VRAMPAGE0( 1 );
      PPUBANK[ 6 ] = VRAMPAGE0( 2 );
      PPUBANK[ 7 ] = VRAMPAGE0( 3 );
#else
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
#endif
      InfoNES_SetupChr();
    }
  }    
}
// Mapper 005: removed (needs more RAM than the RP2040 has)
// Mapper 006: removed (needs more RAM than the RP2040 has)
/*===================================================================*/
/*                                                                   */
/*                        Mapper 7 (AOROM)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 7                                              */
/*-------------------------------------------------------------------*/
void Map7_Init()
{
  /* Initialize Mapper */
  MapperInit = Map7_Init;

  /* Write to Mapper */
  MapperWrite = Map7_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 7 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map7_Write( WORD wAddr, BYTE byData )
{
  BYTE byBank;

  /* Set ROM Banks */
  byBank = ( byData & 0x07 ) << 2;
  byBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byBank );
  ROMBANK1 = ROMPAGE( byBank + 1 );
  ROMBANK2 = ROMPAGE( byBank + 2 );
  ROMBANK3 = ROMPAGE( byBank + 3 );

  /* Name Table Mirroring */
  InfoNES_Mirroring( byData & 0x10 ? 2 : 3 );
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 8 (FFE F3xxx)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 8                                              */
/*-------------------------------------------------------------------*/
void Map8_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map8_Init;

  /* Write to Mapper */
  MapperWrite = Map8_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 8 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map8_Write( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = ( byData & 0xf8 ) >> 3;
  BYTE byChrBank = byData & 0x07;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank + 0 );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 9 (MMC2)                               */
/*                                                                   */
/*===================================================================*/

struct Map9_Latch 
{
  BYTE lo_bank;
  BYTE hi_bank;
  BYTE state;
};

struct Map9_Latch latch1;
struct Map9_Latch latch2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 9                                              */
/*-------------------------------------------------------------------*/
void Map9_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map9_Init;

  /* Write to Mapper */
  MapperWrite = Map9_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map9_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMLASTPAGE( 2 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Init Latch Selector */
  latch1.state = 0xfe;
  latch1.lo_bank = 0;
  latch1.hi_bank = 0;
  latch2.state = 0xfe;
  latch2.lo_bank = 0;
  latch2.hi_bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 9 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map9_Write( WORD wAddr, BYTE byData )
{
  WORD wMapAddr;

  wMapAddr = wAddr & 0xf000;
  switch ( wMapAddr )
  {
    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0xb000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch1.lo_bank = byData;

      if (0xfd == latch1.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xc000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch1.hi_bank = byData;

      if (0xfe == latch1.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xd000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch2.lo_bank = byData;

      if (0xfd == latch2.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 );    
        InfoNES_SetupChr();
      }
      break;

    case 0xe000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch2.hi_bank = byData;

      if (0xfe == latch2.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 ); 
        InfoNES_SetupChr();
      }
      break;

    case 0xf000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x01 ? 0 : 1);
      break;
  }  
}

/*-------------------------------------------------------------------*/
/*  Mapper 9 PPU Function                                            */
/*-------------------------------------------------------------------*/
void Map9_PPU( WORD wAddr )
{
  /* Control Latch Selector */ 
  switch ( wAddr & 0x3ff0 )
  {
    case 0x0fd0:
      /* Latch Control */
      latch1.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch1.lo_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch1.lo_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch1.lo_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch1.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;

    case 0x0fe0:
      /* Latch Control */
      latch1.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch1.hi_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch1.hi_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch1.hi_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch1.hi_bank + 3 );     
      InfoNES_SetupChr();      
      break;

    case 0x1fd0:
      /* Latch Control */
      latch2.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch2.lo_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch2.lo_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch2.lo_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch2.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;      

    case 0x1fe0:
      /* Latch Control */
      latch2.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch2.hi_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch2.hi_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch2.hi_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch2.hi_bank + 3 );     
      InfoNES_SetupChr();            
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 10 (MMC4)                              */
/*                                                                   */
/*===================================================================*/

struct Map10_Latch 
{
  BYTE lo_bank;
  BYTE hi_bank;
  BYTE state;
};

struct Map10_Latch latch3;    // Latch Selector #1
struct Map10_Latch latch4;    // Latch Selector #2

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 10                                             */
/*-------------------------------------------------------------------*/
void Map10_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map10_Init;

  /* Write to Mapper */
  MapperWrite = Map10_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map10_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Init Latch Selector */
  latch3.state = 0xfe;
  latch3.lo_bank = 0;
  latch3.hi_bank = 0;
  latch4.state = 0xfe;
  latch4.lo_bank = 0;
  latch4.hi_bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map10_Write( WORD wAddr, BYTE byData )
{
  WORD wMapAddr;

  wMapAddr = wAddr & 0xf000;
  switch ( wMapAddr )
  {
    case 0xa000:
      /* Set ROM Banks */
      byData %= NesHeader.byRomSize;
      byData <<= 1;
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    case 0xb000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.lo_bank = byData;

      if (0xfd == latch3.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xc000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.hi_bank = byData;

      if (0xfe == latch3.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xd000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.lo_bank = byData;

      if (0xfd == latch4.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 );    
        InfoNES_SetupChr();
      }
      break;

    case 0xe000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.hi_bank = byData;

      if (0xfe == latch4.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 ); 
        InfoNES_SetupChr();
      }
      break;

    case 0xf000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x01 ? 0 : 1);
      break;
  }  
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 PPU Function                                           */
/*-------------------------------------------------------------------*/
void Map10_PPU( WORD wAddr )
{
  /* Control Latch Selector */ 
  switch ( wAddr & 0x3ff0 )
  {
    case 0x0fd0:
      /* Latch Control */
      latch3.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch3.lo_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch3.lo_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch3.lo_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch3.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;

    case 0x0fe0:
      /* Latch Control */
      latch3.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch3.hi_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch3.hi_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch3.hi_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch3.hi_bank + 3 );     
      InfoNES_SetupChr();      
      break;

    case 0x1fd0:
      /* Latch Control */
      latch4.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch4.lo_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch4.lo_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch4.lo_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch4.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;      

    case 0x1fe0:
      /* Latch Control */
      latch4.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch4.hi_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch4.hi_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch4.hi_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch4.hi_bank + 3 );     
      InfoNES_SetupChr();            
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 11 (Color Dreams)                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 11                                             */
/*-------------------------------------------------------------------*/
void Map11_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map11_Init;

  /* Write to Mapper */
  MapperWrite = Map11_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 11 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map11_Write( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = ( byData & 0x01 ) << 2;
  BYTE byChrBank = ( ( byData & 0x70 ) >> 4 ) << 3;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( byPrgBank + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ( byPrgBank + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( byPrgBank + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( byPrgBank + 3 ) % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ( byChrBank + 0 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 1 ] = VROMPAGE( ( byChrBank + 1 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 2 ] = VROMPAGE( ( byChrBank + 2 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 3 ] = VROMPAGE( ( byChrBank + 3 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 4 ] = VROMPAGE( ( byChrBank + 4 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 5 ] = VROMPAGE( ( byChrBank + 5 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 6 ] = VROMPAGE( ( byChrBank + 6 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 7 ] = VROMPAGE( ( byChrBank + 7 ) % ( NesHeader.byVRomSize << 3 ) );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 13 : CPROM                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 13                                             */
/*-------------------------------------------------------------------*/
void Map13_Init()
{
  /* Initialize Mapper */
  MapperInit = Map13_Init;

  /* Write to Mapper */
  MapperWrite = Map13_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = CRAMPAGE( 0 );
  PPUBANK[ 1 ] = CRAMPAGE( 1 );
  PPUBANK[ 2 ] = CRAMPAGE( 2 );
  PPUBANK[ 3 ] = CRAMPAGE( 3 );
  PPUBANK[ 4 ] = CRAMPAGE( 0 );
  PPUBANK[ 5 ] = CRAMPAGE( 1 );
  PPUBANK[ 6 ] = CRAMPAGE( 2 );
  PPUBANK[ 7 ] = CRAMPAGE( 3 );
  InfoNES_SetupChr();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 13 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map13_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE((((byData&0x30)>>2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((byData&0x30)>>2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((byData&0x30)>>2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((byData&0x30)>>2)+3) % (NesHeader.byRomSize<<1));

  /* Set PPU Banks */
  PPUBANK[ 4 ] = CRAMPAGE(((byData&0x03)<<2)+0);
  PPUBANK[ 5 ] = CRAMPAGE(((byData&0x03)<<2)+1);
  PPUBANK[ 6 ] = CRAMPAGE(((byData&0x03)<<2)+2);
  PPUBANK[ 7 ] = CRAMPAGE(((byData&0x03)<<2)+3);
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 15 (100-in-1)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 15                                             */
/*-------------------------------------------------------------------*/
void Map15_Init()
{
  /* Initialize Mapper */
  MapperInit = Map15_Init;

  /* Write to Mapper */
  MapperWrite = Map15_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 15 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map15_Write( WORD wAddr, BYTE byData )
{
  BYTE byBank;

  switch ( wAddr )
  {
    case 0x8000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x20 ? 0 : 1);
      
      /* Set ROM Banks */
      byBank = byData & 0x1f;
      byBank %= ( NesHeader.byRomSize << 1 );
      byBank <<= 1;

      ROMBANK0 = ROMPAGE( byBank );
      ROMBANK1 = ROMPAGE( byBank + 1 );
      ROMBANK2 = ROMPAGE( byBank + 2 );
      ROMBANK3 = ROMPAGE( byBank + 3 );
      break;

    case 0x8001:
      /* Set ROM Banks */
      byData &= 0x3f;
      byData %= ( NesHeader.byRomSize << 1 );
      byData <<= 1;

      ROMBANK2 = ROMPAGE( byData );
      ROMBANK3 = ROMPAGE( byData + 1 );
      break;

    case 0x8002:
      /* Set ROM Banks */
      byBank = byData & 0x3f; 
      byBank %= ( NesHeader.byRomSize << 1 );
      byBank <<= 1;
      byBank += ( byData & 0x80 ? 1 : 0 );

      ROMBANK0 = ROMPAGE( byBank );
      ROMBANK1 = ROMPAGE( byBank );
      ROMBANK2 = ROMPAGE( byBank );
      ROMBANK3 = ROMPAGE( byBank );
      break;

    case 0x8003:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x20 ? 0 : 1);
      
      /* Set ROM Banks */
      byData &= 0x1f;
      byData %= ( NesHeader.byRomSize << 1 );
      byData <<= 1;

      ROMBANK2 = ROMPAGE( byData );
      ROMBANK3 = ROMPAGE( byData + 1 );
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 16 (Bandai Mapper)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map16_Regs[3];

BYTE  Map16_IRQ_Enable;
DWORD Map16_IRQ_Cnt;
DWORD Map16_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 16                                             */
/*-------------------------------------------------------------------*/
void Map16_Init()
{
  /* Initialize Mapper */
  MapperInit = Map16_Init;

  /* Write to Mapper */
  MapperWrite = Map16_Write;

  /* Write to SRAM */
  MapperSram = Map16_Write;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map16_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize State Flag */
  Map16_Regs[ 0 ] = 0;
  Map16_Regs[ 1 ] = 0;
  Map16_Regs[ 2 ] = 0;

  Map16_IRQ_Enable = 0;
  Map16_IRQ_Cnt = 0;
  Map16_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 16 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map16_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0x000f )
  {
    case 0x0000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0004:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0005:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0006:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0007:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0008:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    case 0x0009:
      switch ( byData & 0x03 )
      {
        case 0x00:
          InfoNES_Mirroring( 1 );
          break;

        case 0x01:
          InfoNES_Mirroring( 0 );
          break;    

        case 0x02:
          InfoNES_Mirroring( 3 );
          break;

        case 0x03:
          InfoNES_Mirroring( 2 );
          break; 
      }
      break;

      case 0x000a:
        Map16_IRQ_Enable = byData & 0x01;
        Map16_IRQ_Cnt = Map16_IRQ_Latch;
        break;

      case 0x000b:
        Map16_IRQ_Latch = ( Map16_IRQ_Latch & 0xff00 ) | byData;
        break;

      case 0x000c:
        Map16_IRQ_Latch = ( (DWORD)byData << 8 ) | ( Map16_IRQ_Latch & 0x00ff );
        break;

      case 0x000d:
        /* Write Protect */
        break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 16 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map16_HSync()
{
  if ( Map16_IRQ_Enable )
  {
    /* Normal IRQ */
    if ( Map16_IRQ_Cnt <= 114 )
    {
      IRQ_REQ;
      Map16_IRQ_Cnt = 0;
      Map16_IRQ_Enable = 0;
    } else {
      Map16_IRQ_Cnt -= 114;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                 Mapper 17 (FFE F8 Series)                         */
/*                                                                   */
/*===================================================================*/

BYTE  Map17_IRQ_Enable;
DWORD Map17_IRQ_Cnt;
DWORD Map17_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 17                                             */
/*-------------------------------------------------------------------*/
void Map17_Init()
{
  /* Initialize Mapper */
  MapperInit = Map17_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map17_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map17_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize State Registers */
  Map17_IRQ_Enable = 0;
  Map17_IRQ_Cnt = 0;
  Map17_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map17_Apu( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x42fe:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring( 3 );
      } else {
        InfoNES_Mirroring( 2 );
      }
      break;

    case 0x42ff:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }
      break;

    case 0x4501:
      Map17_IRQ_Enable = 0;
      break;

    case 0x4502:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0xff00 ) | byData;
      break;

    case 0x4503:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0x00ff ) | ( (DWORD)byData << 8 );
      Map17_IRQ_Cnt = Map17_IRQ_Latch;
      Map17_IRQ_Enable = 1;
      break;

    case 0x4504:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x4505:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x4506:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0x4507:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK3 = ROMPAGE( byData );
      break;

    case 0x4510:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4511:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4512:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4513:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4514:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4515:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4516:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4517:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map17_HSync()
{
  if ( Map17_IRQ_Enable )
  {
    if ( Map17_IRQ_Cnt >= 0xffff - 113 )
    {
      IRQ_REQ;
      Map17_IRQ_Cnt = 0;
      Map17_IRQ_Enable = 0;
    } else {
      Map17_IRQ_Cnt += 113;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 18 (Jaleco SS8806)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map18_Regs[11];
BYTE Map18_IRQ_Enable;
WORD Map18_IRQ_Latch;
WORD Map18_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 18                                             */
/*-------------------------------------------------------------------*/
void Map18_Init()
{
  /* Initialize Mapper */
  MapperInit = Map18_Init;

  /* Write to Mapper */
  MapperWrite = Map18_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map18_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize Regs */
  for ( int i = 0; i < sizeof( Map18_Regs ); i++ )
  {
    Map18_Regs[ i ] = 0;
  }
  Map18_IRQ_Enable = 0;
  Map18_IRQ_Latch = 0;
  Map18_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 18 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map18_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      Map18_Regs[ 0 ] = ( Map18_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK0 = ROMPAGE( Map18_Regs[ 0 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      Map18_Regs[ 0 ] = ( Map18_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK0 = ROMPAGE( Map18_Regs[ 0 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8002:
      Map18_Regs[ 1 ] = ( Map18_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK1 = ROMPAGE( Map18_Regs[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8003:
      Map18_Regs[ 1 ] = ( Map18_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK1 = ROMPAGE( Map18_Regs[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9000:
      Map18_Regs[ 2 ] = ( Map18_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK2 = ROMPAGE( Map18_Regs[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9001:
      Map18_Regs[ 2 ] = ( Map18_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK2 = ROMPAGE( Map18_Regs[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    /* Set PPU Banks */
    case 0xA000:
      Map18_Regs[ 3 ]  = ( Map18_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map18_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xA001:
      Map18_Regs[ 3 ] = ( Map18_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map18_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xA002:
      Map18_Regs[ 4 ]  = ( Map18_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map18_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xA003:
      Map18_Regs[ 4 ] = ( Map18_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map18_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xB000:
      Map18_Regs[ 5 ]  = ( Map18_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map18_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xB001:
      Map18_Regs[ 5 ] = ( Map18_Regs[ 5 ] &0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map18_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xB002:
      Map18_Regs[ 6 ]  = ( Map18_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map18_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xB003:
      Map18_Regs[ 6 ] = ( Map18_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map18_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xC000:
      Map18_Regs[ 7 ]  = ( Map18_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map18_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xC001:
      Map18_Regs[ 7 ] = ( Map18_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map18_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xC002:
      Map18_Regs[ 8 ]  = ( Map18_Regs[ 8 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map18_Regs[ 8 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xC003:
      Map18_Regs[ 8 ] = ( Map18_Regs[ 8 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map18_Regs[ 8 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xD000:
      Map18_Regs[ 9 ]  = ( Map18_Regs[ 9 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map18_Regs[ 9 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xD001:
      Map18_Regs[ 9 ] = ( Map18_Regs[ 9 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map18_Regs[ 9 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xD002:
      Map18_Regs[ 10 ]  = ( Map18_Regs[ 10 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map18_Regs[ 10 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xD003:
      Map18_Regs[ 10 ] = ( Map18_Regs[ 10 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map18_Regs[ 10 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xE000:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xfff0 ) | ( byData & 0x0f );
      break;

    case 0xE001:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xff0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xE002:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xf0ff ) | ( ( byData & 0x0f ) << 8 );
      break;

    case 0xE003:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0x0fff ) | ( ( byData & 0x0f ) << 12 );
      break;

    case 0xF000:
      if ( ( byData & 0x01 ) == 0 )
      {
        Map18_IRQ_Cnt = 0;
      } else {
        Map18_IRQ_Cnt = Map18_IRQ_Latch;
      }
      break;

    case 0xF001:
      Map18_IRQ_Enable = byData & 0x01;
      break;

    /* Name Table Mirroring */
    case 0xF002:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 1:
          InfoNES_Mirroring( 1 );   /* Vertical */            
          break;
        case 2:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
      }    
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 18 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map18_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map18_IRQ_Enable )
  {
    if ( Map18_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map18_IRQ_Cnt = 0;
      Map18_IRQ_Enable = 0;
    } else {
      Map18_IRQ_Cnt -= 113;
    }
  }
}
// Mapper 019: removed (needs more RAM than the RP2040 has)
/*===================================================================*/
/*                                                                   */
/*                   Mapper 21 (Konami VRC4 2A)                      */
/*                                                                   */
/*===================================================================*/

BYTE Map21_Regs[ 10 ];
BYTE Map21_IRQ_Enable;
BYTE Map21_IRQ_Cnt;
BYTE Map21_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 21                                             */
/*-------------------------------------------------------------------*/
void Map21_Init()
{
  /* Initialize Mapper */
  MapperInit = Map21_Init;

  /* Write to Mapper */
  MapperWrite = Map21_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map21_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map21_Regs[ nPage ] = nPage;
  }
  Map21_Regs[ 8 ] = 0;

  Map21_IRQ_Enable = 0;
  Map21_IRQ_Cnt = 0;
  Map21_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 21 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map21_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf00f )
  {
    /* Set ROM Banks */
    case 0x8000:
      if ( Map21_Regs[ 8 ] & 0x02 )
      {
        byData %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byData );
      } else {
        byData %= ( NesHeader.byRomSize << 1 );
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0xa000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    /* Name Table Mirroring */
    case 0x9000:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 1 );
          break;

        case 1:
          InfoNES_Mirroring( 0 );
          break;
       
        case 2:
          InfoNES_Mirroring( 3 );
          break;

        case 3:
          InfoNES_Mirroring( 2 );
          break; 
      }
      break;

    case 0x9002:
      Map21_Regs[ 8 ] = byData;
      break;

    case 0xb000:
      Map21_Regs[ 0 ] = ( Map21_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map21_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
      Map21_Regs[ 0 ] = ( Map21_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map21_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
    case 0xb004:
      Map21_Regs[ 1 ] = ( Map21_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map21_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
    case 0xb006:
      Map21_Regs[ 1 ] = ( Map21_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map21_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map21_Regs[ 2 ] = ( Map21_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map21_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc002:
      Map21_Regs[ 2 ] = ( Map21_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map21_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc004:
      Map21_Regs[ 3 ] = ( Map21_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map21_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc003:
    case 0xc006:
      Map21_Regs[ 3 ] = ( Map21_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map21_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd000:
      Map21_Regs[ 4 ] = ( Map21_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map21_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd002:
      Map21_Regs[ 4 ] = ( Map21_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map21_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd001:
    case 0xd004:
      Map21_Regs[ 5 ] = ( Map21_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map21_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd003:
    case 0xd006:
      Map21_Regs[ 5 ] = ( Map21_Regs[ 5 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map21_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe000:
      Map21_Regs[ 6 ] = ( Map21_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map21_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe002:
      Map21_Regs[ 6 ] = ( Map21_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map21_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe001:
    case 0xe004:
      Map21_Regs[ 7 ] = ( Map21_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map21_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe003:
    case 0xe006:
      Map21_Regs[ 7 ] = ( Map21_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map21_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xf000:
      Map21_IRQ_Latch = ( Map21_IRQ_Latch & 0xf0 ) | ( byData & 0x0f );
      break;

    case 0xf002:
      Map21_IRQ_Latch = ( Map21_IRQ_Latch & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xf003:
      if ( Map21_IRQ_Enable & 0x01 )
      {
        Map21_IRQ_Enable |= 0x02;
      } else {
        Map21_IRQ_Enable &= 0x01;
      }
      break;

    case 0xf004:
      Map21_IRQ_Enable = byData & 0x03;
      if ( Map21_IRQ_Enable & 0x02 )
      {
        Map21_IRQ_Cnt = Map21_IRQ_Latch;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 21 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map21_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map21_IRQ_Enable & 0x02 )
  {
    if ( Map21_IRQ_Cnt == 0xff )
    {
      Map21_IRQ_Cnt = Map21_IRQ_Latch;
      
      if ( Map21_IRQ_Enable & 0x01 )
      {
        Map21_IRQ_Enable |= 0x02;
      } else {
        Map21_IRQ_Enable &= 0x01;
      }
      IRQ_REQ;
    } else {
      Map21_IRQ_Cnt++;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 22 (Konami VRC2 type A)                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 22                                             */
/*-------------------------------------------------------------------*/
void Map22_Init()
{
  /* Initialize Mapper */
  MapperInit = Map22_Init;

  /* Write to Mapper */
  MapperWrite = Map22_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 22 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map22_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x9000:
      /* Name Table Mirroring */
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 1:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 2:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2000 */
          break;
        case 3:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2400 */
          break;
      }
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;
    
    case 0xb000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xb001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
    
    case 0xc000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
          
    case 0xd000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
          
    case 0xe000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 23 (Konami VRC2 type B)                   */
/*                                                                   */
/*===================================================================*/

BYTE Map23_Regs[ 9 ];

BYTE Map23_IRQ_Enable;
BYTE Map23_IRQ_Cnt;
BYTE Map23_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 23                                             */
/*-------------------------------------------------------------------*/
void Map23_Init()
{
  /* Initialize Mapper */
  MapperInit = Map23_Init;

  /* Write to Mapper */
  MapperWrite = Map23_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize State Flag */
  Map23_Regs[ 0 ] = 0;
  Map23_Regs[ 1 ] = 1;
  Map23_Regs[ 2 ] = 2;
  Map23_Regs[ 3 ] = 3;
  Map23_Regs[ 4 ] = 4;
  Map23_Regs[ 5 ] = 5;
  Map23_Regs[ 6 ] = 6;
  Map23_Regs[ 7 ] = 7;
  Map23_Regs[ 8 ] = 0;

  Map23_IRQ_Enable = 0;
  Map23_IRQ_Cnt = 0;
  Map23_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 23 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map23_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
    case 0x8004:
    case 0x8008:
    case 0x800c:
      byData %= ( NesHeader.byRomSize << 1 );

      if ( Map23_Regs[ 8 ] )
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0x9000:
      switch ( byData & 0x03 )
      {
       case 0x00:
          InfoNES_Mirroring( 1 );
          break;
        case 0x01:
          InfoNES_Mirroring( 0 ); 
          break;
        case 0x02:
          InfoNES_Mirroring( 3 );  
          break;
        case 0x03:
          InfoNES_Mirroring( 2 );  
          break;
      }
      break;

    case 0x9008:
      Map23_Regs[ 8 ] = byData & 0x02;
      break;

    case 0xa000:
    case 0xa004:
    case 0xa008:
    case 0xa00c:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xb000:
      Map23_Regs[ 0 ] = ( Map23_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map23_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
    case 0xb004:
      Map23_Regs[ 0 ] = ( Map23_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map23_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
    case 0xb008:
      Map23_Regs[ 1 ] = ( Map23_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map23_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
    case 0xb00c:
      Map23_Regs[ 1 ] = ( Map23_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map23_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map23_Regs[ 2 ] = ( Map23_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map23_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc004:
      Map23_Regs[ 2 ] = ( Map23_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map23_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc002:
    case 0xc008:
      Map23_Regs[ 3 ] = ( Map23_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map23_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc003:
    case 0xc00c:
      Map23_Regs[ 3 ] = ( Map23_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map23_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd000:
      Map23_Regs[ 4 ] = ( Map23_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map23_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd001:
    case 0xd004:
      Map23_Regs[ 4 ] = ( Map23_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map23_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd002:
    case 0xd008:
      Map23_Regs[ 5 ] = ( Map23_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map23_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd003:
    case 0xd00c:
      Map23_Regs[ 5 ] = ( Map23_Regs[ 5 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map23_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;   
      
    case 0xe000:
      Map23_Regs[ 6 ] = ( Map23_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map23_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe001:
    case 0xe004:
      Map23_Regs[ 6 ] = ( Map23_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map23_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe002:
    case 0xe008:
      Map23_Regs[ 7 ] = ( Map23_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map23_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe003:
    case 0xe00c:
      Map23_Regs[ 7 ] = ( Map23_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map23_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xf000:
      Map23_IRQ_Latch = ( Map23_IRQ_Latch & 0xf0 ) | ( byData & 0x0f );
      break;

    case 0xf004:
      Map23_IRQ_Latch = ( Map23_IRQ_Latch & 0xf0 ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xf008:
      Map23_IRQ_Enable = byData & 0x03;
      if ( Map23_IRQ_Enable & 0x02 )
      {
        Map23_IRQ_Cnt = Map23_IRQ_Latch;
      }
      break;

    case 0xf00c:
      if ( Map23_IRQ_Enable & 0x01 )
      {
        Map23_IRQ_Enable |= 0x02;
      } else {
        Map23_IRQ_Enable &= 0x01;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 23 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map23_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map23_IRQ_Enable & 0x02 )
  {
    if ( Map23_IRQ_Cnt == 0xff )
    {
      IRQ_REQ;

      Map23_IRQ_Cnt = Map23_IRQ_Latch;
      if ( Map23_IRQ_Enable & 0x01 )
      {
        Map23_IRQ_Enable |= 0x02;
      } else {
        Map23_IRQ_Enable &= 0x01;
      }
    } else {
      Map23_IRQ_Cnt++;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 24 (Konami VRC6)                          */
/*                                                                   */
/*===================================================================*/

BYTE Map24_IRQ_Count;
BYTE Map24_IRQ_State;
BYTE Map24_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 24                                             */
/*-------------------------------------------------------------------*/
void Map24_Init()
{
  /* Initialize Mapper */
  MapperInit = Map24_Init;

  /* Write to Mapper */
  MapperWrite = Map24_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map24_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 24 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map24_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( ( byData + 0 ) % ( NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( ( byData + 1 ) % ( NesHeader.byRomSize << 1) );
      break;

    case 0xb003:
      /* Name Table Mirroring */
      switch ( byData & 0x0c )
      {
        case 0x00:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 0x04:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 0x08:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
        case 0x0c:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2400 */
          break;
      }
      break;

	  case 0xC000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
		  break;

	  case 0xD000:
      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD002:
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE000:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE001:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE002:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE003:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xF000:
			Map24_IRQ_Latch = byData;
	  	break;

	  case 0xF001:
			Map24_IRQ_State = byData & 0x03;
			if(Map24_IRQ_State & 0x02)
			{
				Map24_IRQ_Count = Map24_IRQ_Latch;
			}
		  break;

	  case 0xF002:
			if(Map24_IRQ_State & 0x01)
			{
				Map24_IRQ_State |= 0x02;
			}
			else
			{
				Map24_IRQ_State &= 0x01;
			}
		break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 24 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map24_HSync()
{
/*
 *  Callback at HSync
 *
 */
	if(Map24_IRQ_State & 0x02)
	{
	  if(Map24_IRQ_Count == 0xFF)
		{
			IRQ_REQ;
			Map24_IRQ_Count = Map24_IRQ_Latch;
		}
		else
		{
			Map24_IRQ_Count++;
		}
	}
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 25 (Konami VRC4 type B)                   */
/*                                                                   */
/*===================================================================*/

BYTE Map25_Bank_Selector;
BYTE Map25_VBank[16];

BYTE Map25_IRQ_Count;
BYTE Map25_IRQ_State;
BYTE Map25_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 25                                             */
/*-------------------------------------------------------------------*/
void Map25_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map25_Init;

  /* Write to Mapper */
  MapperWrite = Map25_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map25_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Reset VBank Registers */
  for (nPage = 0; nPage < 16; nPage++)
    Map25_VBank[ nPage ] = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map25_Write( WORD wAddr, BYTE byData )
{
  int nBank;

  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      if ( Map25_Bank_Selector ) 
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0x9000:
      /* Name Table Mirroring */
      switch (byData & 0x03)
      {
        case 0:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 1:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 2:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2000 */
          break;
        case 3:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2400 */
          break;
      }
      break;

    case 0x9002:
      /* TODO: SaveRAM Toggle */
      /* $8000 Switching Mode */
      Map25_Bank_Selector = byData & 0x02;
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;
    
    default:
      /* Set PPU Banks */
      switch ( wAddr & 0xfffc)
      {
        case 0xb000:
          Map25_VBank[ 0 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 0 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;
  
        case 0xc000:
          Map25_VBank[ 4 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 2 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xd000:
          Map25_VBank[ 8 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 4 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xe000:
          Map25_VBank[ 12 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 6 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xf000:
          switch ( wAddr & 0x0003 )
          {
            case 0:
            case 1:
              Map25_IRQ_Latch = byData;
              break;
            case 2:
              Map25_IRQ_State = ( byData & 0x01 ) ? Map25_IRQ_State : 0x00;
              Map25_IRQ_State = ( byData & 0x02 ) ? 0x01 : Map25_IRQ_State;
              Map25_IRQ_Count = Map25_IRQ_Latch;
              break;
            case 3:
              Map25_IRQ_State = ( Map25_IRQ_State << 1 ) | ( Map25_IRQ_State & 1 );
              break;
          }
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 Sync VROM Function                                     */
/*-------------------------------------------------------------------*/
void Map25_Sync_Vrom( int nBank )
{
  BYTE byValue;
  
  byValue  = Map25_VBank[ ( nBank << 1 ) - ( nBank & 0x01 ) ];
  byValue |= Map25_VBank[ ( nBank << 1 ) - ( nBank & 0x01 ) + 2] << 4;
  byValue %= ( NesHeader.byVRomSize << 3 );
  PPUBANK[ nBank ] = VROMPAGE( byValue );
  InfoNES_SetupChr();
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map25_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( Map25_IRQ_State & 0x02 ) && ( ++Map25_IRQ_Count == 0 ) )
  {
    IRQ_REQ;
    Map25_IRQ_Count = Map25_IRQ_Latch;
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 26 (Konami VRC 6V)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map26_IRQ_Enable;
BYTE Map26_IRQ_Cnt;
BYTE Map26_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 26                                             */
/*-------------------------------------------------------------------*/
void Map26_Init()
{
  /* Initialize Mapper */
  MapperInit = Map26_Init;

  /* Write to Mapper */
  MapperWrite = Map26_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map26_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map26_IRQ_Enable = 0;
  Map26_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 26 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map26_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      byData <<= 1;      
      byData %= ( NesHeader.byRomSize << 1 );
      
      ROMBANK0 = ROMPAGE( byData + 0 );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    /* Name Table Mirroring */
    case 0xb003:  
      switch ( byData & 0x7f )
      {
        case 0x08:
        case 0x2c:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2400 */
          break;
        case 0x20:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 0x24:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 0x28:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
      }
      break;

    /* Set ROM Banks */
    case 0xc000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );      
      InfoNES_SetupChr();
      break;

    /* Set PPU Bank */
    case 0xd000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    /* Set IRQ Registers */
    case 0xf000:
      Map26_IRQ_Latch = byData;
      break;

    case 0xf001:
      Map26_IRQ_Enable = byData & 0x01;
      break;

    case 0xf002:
      Map26_IRQ_Enable = byData & 0x03;

      if ( Map26_IRQ_Enable & 0x02 )
      {
        Map26_IRQ_Cnt = Map26_IRQ_Latch;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 26 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map26_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map26_IRQ_Enable & 0x03 )
  {
    if ( Map26_IRQ_Cnt >= 0xfe )
    {
      IRQ_REQ;
      Map26_IRQ_Cnt = Map26_IRQ_Latch;
      Map26_IRQ_Enable = 0;
    } else {
      Map26_IRQ_Cnt++;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 32 (Irem G-101)                      */
/*                                                                   */
/*===================================================================*/

BYTE Map32_Saved;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 32                                             */
/*-------------------------------------------------------------------*/
void Map32_Init()
{
  /* Initialize Mapper */
  MapperInit = Map32_Init;

  /* Write to Mapper */
  MapperWrite = Map32_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Initialize state flag */
  Map32_Saved = 0x00;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 32 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map32_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf000 )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );

      if ( Map32_Saved & 0x02 ) 
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;
      
    case 0x9000:
      Map32_Saved = byData;
      
      // Name Table Mirroring
      InfoNES_Mirroring( byData & 0x01 );
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xb000:
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ wAddr & 0x0007 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    default:
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                 Mapper 33 (Taito TC0190/TC0350)                   */
/*                                                                   */
/*===================================================================*/

BYTE Map33_Regs[ 8 ];
BYTE Map33_Switch;

BYTE Map33_IRQ_Enable;
BYTE Map33_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 33                                             */
/*-------------------------------------------------------------------*/
void Map33_Init()
{
  /* Initialize Mapper */
  MapperInit = Map33_Init;

  /* Write to Mapper */
  MapperWrite = Map33_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
      Map33_Regs[ nPage ] = nPage;
    }
    InfoNES_SetupChr();
  } 
  else 
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      Map33_Regs[ nPage ] = 0;
    }
  }

  /* Initialize State Registers */
  Map33_Switch = 0;
  Map33_IRQ_Enable = 0;
  Map33_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map33_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  switch ( wAddr )
  {
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      Map33_Regs[ 0 ] = byData * 2;
      Map33_Regs[ 1 ] = byData * 2 + 1;

      PPUBANK[ 0 ] = VROMPAGE( Map33_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map33_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x8003:
      Map33_Regs[ 2 ] = byData * 2;
      Map33_Regs[ 3 ] = byData * 2 + 1;

      PPUBANK[ 2 ] = VROMPAGE( Map33_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map33_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa000:
      Map33_Regs[ 4 ] = byData;
      PPUBANK[ 4 ] = VROMPAGE( Map33_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa001:
      Map33_Regs[ 5 ] = byData;
      PPUBANK[ 5 ] = VROMPAGE( Map33_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa002:
      Map33_Regs[ 6 ] = byData;
      PPUBANK[ 6 ] = VROMPAGE( Map33_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa003:
      Map33_Regs[ 7 ] = byData;
      PPUBANK[ 7 ] = VROMPAGE( Map33_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map33_IRQ_Cnt = byData;
      break;

    case 0xc001:
    case 0xc002:
    case 0xe001:
    case 0xe002:
      if ( Map33_IRQ_Cnt == byData )
      {
        Map33_IRQ_Enable = 0xff;
      } else {
        Map33_IRQ_Enable = byData;
      }
      break;

    case 0xe000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map33_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map33_IRQ_Enable )
  {
    if ( Map33_IRQ_Enable == 0xff )
    {
      if ( PPU_Scanline == (WORD)( 0xff - Map33_IRQ_Cnt ) )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      }
    } else {
      if ( Map33_IRQ_Cnt == 0xff )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      } else {
        Map33_IRQ_Cnt++;
      }
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 34 (Nina-1)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 34                                             */
/*-------------------------------------------------------------------*/
void Map34_Init()
{
  /* Initialize Mapper */
  MapperInit = Map34_Init;

  /* Write to Mapper */
  MapperWrite = Map34_Write;

  /* Write to SRAM */
  MapperSram = Map34_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 34 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map34_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  byData <<= 2;
  byData %= ( NesHeader.byRomSize << 1);

  ROMBANK0 = ROMPAGE( byData );
  ROMBANK1 = ROMPAGE( byData + 1 );
  ROMBANK2 = ROMPAGE( byData + 2 );
  ROMBANK3 = ROMPAGE( byData + 3 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 34 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map34_Sram( WORD wAddr, BYTE byData )
{
  switch(wAddr)
  {
    /* Set ROM Banks */
    case 0x7ffd:
      byData <<= 2;
      byData %= ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      ROMBANK2 = ROMPAGE( byData + 2 );
      ROMBANK3 = ROMPAGE( byData + 3 );
      break;

    /* Set PPU Banks */
    case 0x7ffe:
      byData <<= 2;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byData + 3 );
      InfoNES_SetupChr();
      break;

    /* Set PPU Banks */
    case 0x7fff:
      byData <<= 2;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 4 ] = VROMPAGE( byData );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
      PPUBANK[ 7 ] = VROMPAGE( byData + 3 );
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 40 (SMB2J)                           */
/*                                                                   */
/*===================================================================*/

BYTE  Map40_IRQ_Enable;
DWORD Map40_Line_To_IRQ;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 40                                             */
/*-------------------------------------------------------------------*/
void Map40_Init()
{
  /* Initialize Mapper */
  MapperInit = Map40_Init;

  /* Write to Mapper */
  MapperWrite = Map40_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map40_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 6 );

  /* Initialize IRQ Registers */
  Map40_IRQ_Enable = 0;
  Map40_Line_To_IRQ = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 4 );
  ROMBANK1 = ROMPAGE( 5 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 7 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 40 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map40_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xe000 )
  {
    case 0x8000:
      Map40_IRQ_Enable = 0;
      break;

    case 0xa000:
      Map40_IRQ_Enable = 1;
      Map40_Line_To_IRQ = 37;
      break;

    case 0xc000:
      break;

    case 0xe000:
      /* Set ROM Banks */
      ROMBANK2 = ROMPAGE ( ( byData & 0x07 ) % ( NesHeader.byRomSize << 1 ) );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 40 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map40_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map40_IRQ_Enable )
  {
    if ( ( --Map40_Line_To_IRQ ) <= 0 )
    {
      IRQ_REQ;
    }
  }
}

/* End of InfoNES_Mapper_40.cpp */
/*===================================================================*/
/*                                                                   */
/*                        Mapper 41 ()                               */
/*                                                                   */
/*===================================================================*/

BYTE Map41_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 41                                             */
/*-------------------------------------------------------------------*/
void Map41_Init()
{
  /* Initialize Mapper */
  MapperInit = Map41_Init;

  /* Write to Mapper */
  MapperWrite = Map41_Write;

  /* Write to SRAM */
  MapperSram = Map41_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 41 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map41_Write( WORD wAddr, BYTE byData )
{
  /* Set PPU Banks */
  if ( Map41_Regs[ 0 ] )
  {
    BYTE byChrBank;
    
    byChrBank = Map41_Regs[ 1 ] | ( byData & 0x0003 );
    byChrBank <<= 3;
    byChrBank %= ( NesHeader.byVRomSize << 3 );

    PPUBANK[ 0 ] = VROMPAGE( byChrBank );
    PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
    PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
    PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
    PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
    PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
    PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
    PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );

    InfoNES_SetupChr();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 41 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map41_Sram( WORD wAddr, BYTE byData )
{
  BYTE byBank;

  if ( wAddr < 0x6800 )
  {
    byData = ( BYTE )( wAddr & 0xff );

    /* Set CPU Banks */
    byBank = ( byData & 0x07 ) << 2;
    byBank %= ( NesHeader.byRomSize << 1 );

    ROMBANK0 = ROMPAGE( byBank );
    ROMBANK1 = ROMPAGE( byBank + 1 );
    ROMBANK2 = ROMPAGE( byBank + 2 );
    ROMBANK3 = ROMPAGE( byBank + 3 );

    Map41_Regs[ 0 ] = ( byData & 0x04 );
    Map41_Regs[ 1 ] = ( byData & 0x18 ) >> 1;

    /* Name Table Mirroring */
    if ( byData & 0x20 )
    {
      InfoNES_Mirroring( 0 );         /* Horizontal */
    } else {
      InfoNES_Mirroring( 1 );         /* Vertical */
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 42 (Pirates)                           */
/*                                                                   */
/*===================================================================*/

BYTE Map42_IRQ_Cnt;
BYTE Map42_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 42                                             */
/*-------------------------------------------------------------------*/
void Map42_Init()
{
  /* Initialize Mapper */
  MapperInit = Map42_Init;

  /* Write to Mapper */
  MapperWrite = Map42_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map42_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 3 );
  ROMBANK1 = ROMLASTPAGE( 2 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 42 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map42_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xe003 )
  {
    /* Set ROM Banks */
    case 0xe000:
      SRAMBANK = ROMPAGE( ( byData & 0x0f ) % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xe001:
      if ( byData & 0x08 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      break;

    case 0xe002:
      if ( byData & 0x02 )
      {
        Map42_IRQ_Enable = 1;
      } else {
        Map42_IRQ_Enable = 0;
        Map42_IRQ_Cnt = 0;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 42 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map42_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map42_IRQ_Enable )
  {
    if ( Map42_IRQ_Cnt < 215 )
    {
      Map42_IRQ_Cnt++;
    }
    if ( Map42_IRQ_Cnt == 215 )
    {
      IRQ_REQ;
      Map42_IRQ_Enable = 0;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                         Mapper 43 (SMB2J)                         */
/*                                                                   */
/*===================================================================*/

DWORD Map43_IRQ_Cnt;
BYTE Map43_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 43                                             */
/*-------------------------------------------------------------------*/
void Map43_Init()
{
  /* Initialize Mapper */
  MapperInit = Map43_Init;

  /* Write to Mapper */
  MapperWrite = Map43_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map43_Apu;

  /* Read from APU */
  MapperReadApu = Map43_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map43_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 2 );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 1 );
  ROMBANK1 = ROMPAGE( 0 );
  ROMBANK2 = ROMPAGE( 4 );
  ROMBANK3 = ROMPAGE( 9 );

  /* Initialize State Registers */
	Map43_IRQ_Enable = 1;
	Map43_IRQ_Cnt = 0;

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
BYTE Map43_ReadApu( WORD wAddr )
{
  if ( 0x5000 <= wAddr && wAddr < 0x6000 ) 
  {
    return ROM[ 0x2000*8 + 0x1000 + (wAddr - 0x5000) ];
  }
  return (BYTE)(wAddr >> 8);
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map43_Apu( WORD wAddr, BYTE byData )
{
	if( (wAddr&0xF0FF) == 0x4022 ) 
  {
		switch( byData&0x07 ) 
    {
			case	0x00:
			case	0x02:
			case	0x03:
			case	0x04:
        ROMBANK2 = ROMPAGE( 4 );
				break;
			case	0x01:
        ROMBANK2 = ROMPAGE( 3 );
				break;
			case	0x05:
        ROMBANK2 = ROMPAGE( 7 );
				break;
			case	0x06:
        ROMBANK2 = ROMPAGE( 5 );
				break;
			case	0x07:
        ROMBANK2 = ROMPAGE( 6 );
				break;
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map43_Write( WORD wAddr, BYTE byData )
{
	if( wAddr == 0x8122 ) {
		if( byData&0x03 ) {
			Map43_IRQ_Enable = 1;
		} else {
			Map43_IRQ_Cnt = 0;
			Map43_IRQ_Enable = 0;
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map43_HSync()
{
	if( Map43_IRQ_Enable ) 
  {
		Map43_IRQ_Cnt += 114;
		if( Map43_IRQ_Cnt >= 4096 ) {
			Map43_IRQ_Cnt -= 4096;
			IRQ_REQ;
		}
	}
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 44 (Nin1)                            */
/*                                                                   */
/*===================================================================*/

BYTE  Map44_Regs[ 8 ];
DWORD Map44_Rom_Bank;
DWORD Map44_Prg0, Map44_Prg1;
DWORD Map44_Chr01, Map44_Chr23;
DWORD Map44_Chr4, Map44_Chr5, Map44_Chr6, Map44_Chr7;

#define Map44_Chr_Swap()    ( Map44_Regs[ 0 ] & 0x80 )
#define Map44_Prg_Swap()    ( Map44_Regs[ 0 ] & 0x40 )

BYTE Map44_IRQ_Enable;
BYTE Map44_IRQ_Cnt;
BYTE Map44_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 44                                             */
/*-------------------------------------------------------------------*/
void Map44_Init()
{
  /* Initialize Mapper */
  MapperInit = Map44_Init;

  /* Write to Mapper */
  MapperWrite = Map44_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map44_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map44_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map44_Rom_Bank = 0;
  Map44_Prg0 = 0;
  Map44_Prg1 = 1;
  Map44_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map44_Chr01 = 0;
    Map44_Chr23 = 2;
    Map44_Chr4  = 4;
    Map44_Chr5  = 5;
    Map44_Chr6  = 6;
    Map44_Chr7  = 7;
    Map44_Set_PPU_Banks();
  } else {
    Map44_Chr01 = Map44_Chr23 = 0;
    Map44_Chr4 = Map44_Chr5 = Map44_Chr6 = Map44_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map44_IRQ_Enable = 0;
  Map44_IRQ_Cnt = 0;
  Map44_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map44_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map44_Regs[ 0 ] = byData;
      Map44_Set_PPU_Banks();
      Map44_Set_CPU_Banks();
      break;

    case 0x8001:
      Map44_Regs[ 1 ] = byData;
      dwBankNum = Map44_Regs[ 1 ];

      switch ( Map44_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map44_Chr01 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map44_Chr23 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr4 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr5 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr6 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr7 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map44_Prg0 = dwBankNum;
          Map44_Set_CPU_Banks();
          break;

        case 0x07:
          Map44_Prg1 = dwBankNum;
          Map44_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map44_Regs[ 2 ] = byData;

      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map44_Rom_Bank = byData & 0x07;
      if ( Map44_Rom_Bank == 7 )
      {
        Map44_Rom_Bank = 6;
      }
      Map44_Set_CPU_Banks();
      Map44_Set_PPU_Banks();
      break;

    case 0xc000:
      Map44_Regs[ 4 ] = byData;
      Map44_IRQ_Cnt = Map44_Regs[ 4 ];
      break;

    case 0xc001:
      Map44_Regs[ 5 ] = byData;
      Map44_IRQ_Latch = Map44_Regs[ 5 ];
      break;

    case 0xe000:
      Map44_Regs[ 6 ] = byData;
      Map44_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map44_Regs[ 7 ] = byData;
      Map44_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map44_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map44_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map44_IRQ_Cnt-- ) )
        {
          Map44_IRQ_Cnt = Map44_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map44_Set_CPU_Banks()
{
  if ( Map44_Prg_Swap() )
  {
    ROMBANK0 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    ROMBANK0 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map44_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map44_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 45 (Pirates)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map45_Regs[7];
DWORD Map45_P[4], Map45_Prg0, Map45_Prg1, Map45_Prg2, Map45_Prg3;
DWORD Map45_C[8], Map45_Chr0, Map45_Chr1, Map45_Chr2, Map45_Chr3;
DWORD Map45_Chr4, Map45_Chr5, Map45_Chr6, Map45_Chr7;

BYTE Map45_IRQ_Enable;
BYTE Map45_IRQ_Cnt;
BYTE Map45_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 45                                             */
/*-------------------------------------------------------------------*/
void Map45_Init()
{
	/* Initialize Mapper */
	MapperInit = Map45_Init;

	/* Write to Mapper */
	MapperWrite = Map45_Write;

	/* Write to SRAM */
	MapperSram = Map45_Sram;

	/* Write to APU */
	MapperApu = Map0_Apu;

	/* Read from APU */
	MapperReadApu = Map0_ReadApu;

	/* Callback at VSync */
	MapperVSync = Map0_VSync;

	/* Callback at HSync */
	MapperHSync = Map45_HSync;

	/* Callback at PPU */
	MapperPPU = Map0_PPU;

	/* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
	MapperRenderScreen = Map0_RenderScreen;

	/* Set SRAM Banks */
	SRAMBANK = SRAM;

	/* Set ROM Banks */
	Map45_Prg0 = 0;
	Map45_Prg1 = 1;
	Map45_Prg2 = NesHeader.byRomSize * 2 - 2;
	Map45_Prg3 = NesHeader.byRomSize * 2 - 1;

	ROMBANK0 = ROMPAGE(Map45_Prg0);
	Map45_P[0] = Map45_Prg0;
	ROMBANK1 = ROMPAGE(Map45_Prg1);
	Map45_P[1] = Map45_Prg1;
	ROMBANK2 = ROMPAGE(Map45_Prg2);
	Map45_P[2] = Map45_Prg2;
	ROMBANK3 = ROMPAGE(Map45_Prg3);
	Map45_P[3] = Map45_Prg3;

	/* Set PPU Banks */
	Map45_Chr0 = 0;
	Map45_C[0] = Map45_Chr0;
	Map45_Chr1 = 1;
	Map45_C[1] = Map45_Chr1;
	Map45_Chr2 = 2;
	Map45_C[2] = Map45_Chr2;
	Map45_Chr3 = 3;
	Map45_C[3] = Map45_Chr3;
	Map45_Chr4 = 4;
	Map45_C[4] = Map45_Chr4;
	Map45_Chr5 = 5;
	Map45_C[5] = Map45_Chr5;
	Map45_Chr6 = 6;
	Map45_C[6] = Map45_Chr6;
	Map45_Chr7 = 7;
	Map45_C[7] = Map45_Chr7;

	for (int nPage = 0; nPage < 8; ++nPage)
	{
		PPUBANK[nPage] = VROMPAGE(nPage);
	}
	InfoNES_SetupChr();

	/* Initialize IRQ Registers */
	Map45_IRQ_Enable = 0;
	Map45_IRQ_Cnt = 0;
	Map45_IRQ_Latch = 0;

	Map45_Regs[0] = Map45_Regs[1] = Map45_Regs[2] = Map45_Regs[3] = 0;
	Map45_Regs[4] = Map45_Regs[5] = Map45_Regs[6] = 0;

	/* Set up wiring of the interrupt pin */
	K6502_Set_Int_Wiring(1, 1);
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map45_Sram(WORD wAddr, BYTE byData)
{
	if (wAddr == 0x6000)
	{
		Map45_Regs[Map45_Regs[5]] = byData;
		Map45_Regs[5] = (Map45_Regs[5] + 1) & 0x03;
		Map45_Set_CPU_Bank4((BYTE)Map45_Prg0);
		Map45_Set_CPU_Bank5((BYTE)Map45_Prg1);
		Map45_Set_CPU_Bank6((BYTE)Map45_Prg2);
		Map45_Set_CPU_Bank7((BYTE)Map45_Prg3);
		Map45_Set_PPU_Banks();
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map45_Write(WORD wAddr, BYTE byData)
{
	DWORD swap;

	switch (wAddr & 0xE001)
	{
	case 0x8000:
		if ((byData & 0x40) != (Map45_Regs[6] & 0x40))
		{
			swap = Map45_Prg0;
			Map45_Prg0 = Map45_Prg2;
			Map45_Prg2 = swap;
			swap = Map45_P[0];
			Map45_P[0] = Map45_P[2];
			Map45_P[2] = swap;
			ROMBANK0 = ROMPAGE(Map45_P[0] % (NesHeader.byRomSize << 1));
			ROMBANK2 = ROMPAGE(Map45_P[2] % (NesHeader.byRomSize << 1));
		}
		if (NesHeader.byRomSize > 0)
		{
			if ((byData & 0x80) != (Map45_Regs[6] & 0x80))
			{
				swap = Map45_Chr4;
				Map45_Chr4 = Map45_Chr0;
				Map45_Chr0 = swap;
				swap = Map45_Chr5;
				Map45_Chr5 = Map45_Chr1;
				Map45_Chr1 = swap;
				swap = Map45_Chr6;
				Map45_Chr6 = Map45_Chr2;
				Map45_Chr2 = swap;
				swap = Map45_Chr7;
				Map45_Chr7 = Map45_Chr3;
				Map45_Chr3 = swap;
				swap = Map45_C[4];
				Map45_C[4] = Map45_C[0];
				Map45_C[0] = swap;
				swap = Map45_C[5];
				Map45_C[5] = Map45_C[1];
				Map45_C[1] = swap;
				swap = Map45_C[6];
				Map45_C[6] = Map45_C[2];
				Map45_C[2] = swap;
				swap = Map45_C[7];
				Map45_C[7] = Map45_C[3];
				Map45_C[3] = swap;

				PPUBANK[0] = VROMPAGE(Map45_C[0] % (NesHeader.byVRomSize << 3));
				PPUBANK[1] = VROMPAGE(Map45_C[1] % (NesHeader.byVRomSize << 3));
				PPUBANK[2] = VROMPAGE(Map45_C[2] % (NesHeader.byVRomSize << 3));
				PPUBANK[3] = VROMPAGE(Map45_C[3] % (NesHeader.byVRomSize << 3));
				PPUBANK[4] = VROMPAGE(Map45_C[4] % (NesHeader.byVRomSize << 3));
				PPUBANK[5] = VROMPAGE(Map45_C[5] % (NesHeader.byVRomSize << 3));
				PPUBANK[6] = VROMPAGE(Map45_C[6] % (NesHeader.byVRomSize << 3));
				PPUBANK[7] = VROMPAGE(Map45_C[7] % (NesHeader.byVRomSize << 3));
				InfoNES_SetupChr();
			}
		}
		Map45_Regs[6] = byData;
		break;

	case 0x8001:
		switch (Map45_Regs[6] & 0x07)
		{
		case 0x00:
			Map45_Chr0 = (byData & 0xFE) + 0;
			Map45_Chr1 = (byData & 0xFE) + 1;
			Map45_Set_PPU_Banks();
			break;

		case 0x01:
			Map45_Chr2 = (byData & 0xFE) + 0;
			Map45_Chr3 = (byData & 0xFE) + 1;
			Map45_Set_PPU_Banks();
			break;

		case 0x02:
			Map45_Chr4 = byData;
			Map45_Set_PPU_Banks();
			break;

		case 0x03:
			Map45_Chr5 = byData;
			Map45_Set_PPU_Banks();
			break;

		case 0x04:
			Map45_Chr6 = byData;
			Map45_Set_PPU_Banks();
			break;

		case 0x05:
			Map45_Chr7 = byData;
			Map45_Set_PPU_Banks();
			break;

		case 0x06:
			if (Map45_Regs[6] & 0x40)
			{
				Map45_Prg2 = byData & 0x3F;
				Map45_Set_CPU_Bank6(byData);
			}
			else
			{
				Map45_Prg0 = byData & 0x3F;
				Map45_Set_CPU_Bank4(byData);
			}
			break;

		case 0x07:
			Map45_Prg1 = byData & 0x3F;
			Map45_Set_CPU_Bank5(byData);
			break;
		}
		break;

	case 0xA000:
		if (byData & 0x01)
		{
			InfoNES_Mirroring(0);
		}
		else
		{
			InfoNES_Mirroring(1);
		}
		break;

	case 0xC000:
		Map45_IRQ_Cnt = byData;
		break;

	case 0xC001:
		Map45_IRQ_Latch = byData;
		break;

	case 0xE000:
		Map45_IRQ_Enable = 0;
		break;

	case 0xE001:
		Map45_IRQ_Enable = 1;
		break;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map45_HSync()
{
	/*
 *  Callback at HSync
 *
 */
	if (Map45_IRQ_Enable)
	{
		if (0 <= PPU_Scanline && PPU_Scanline <= 239)
		{
			if (PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP)
			{
				if (!(Map45_IRQ_Cnt--))
				{
					Map45_IRQ_Cnt = Map45_IRQ_Latch;
					IRQ_REQ;
				}
			}
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/

void Map45_Set_CPU_Bank4(BYTE byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
	ROMBANK0 = ROMPAGE(byData % (NesHeader.byRomSize << 1));
	Map45_P[0] = byData;
}

void Map45_Set_CPU_Bank5(BYTE byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
	ROMBANK1 = ROMPAGE(byData % (NesHeader.byRomSize << 1));
	Map45_P[1] = byData;
}

void Map45_Set_CPU_Bank6(BYTE byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
	ROMBANK2 = ROMPAGE(byData % (NesHeader.byRomSize << 1));
	Map45_P[2] = byData;
}

void Map45_Set_CPU_Bank7(BYTE byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
	ROMBANK3 = ROMPAGE(byData % (NesHeader.byRomSize << 1));
	Map45_P[3] = byData;
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map45_Set_PPU_Banks()
{
	BYTE table[16] =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
	Map45_C[0] = Map45_Chr0;
	Map45_C[1] = Map45_Chr1;
	Map45_C[2] = Map45_Chr2;
	Map45_C[3] = Map45_Chr3;
	Map45_C[4] = Map45_Chr4;
	Map45_C[5] = Map45_Chr5;
	Map45_C[6] = Map45_Chr6;
	Map45_C[7] = Map45_Chr7;
	for (BYTE i = 0; i < 8; i++)
	{
		Map45_C[i] &= table[Map45_Regs[2] & 0x0F];
		Map45_C[i] |= Map45_Regs[0] & 0xff;
		Map45_C[i] += (BYTE)(Map45_Regs[2] & 0x10) << 4;
	}
	if (Map45_Regs[6] & 0x80)
	{
		PPUBANK[0] = VROMPAGE(Map45_C[4] % (NesHeader.byVRomSize << 3));
		PPUBANK[1] = VROMPAGE(Map45_C[5] % (NesHeader.byVRomSize << 3));
		PPUBANK[2] = VROMPAGE(Map45_C[6] % (NesHeader.byVRomSize << 3));
		PPUBANK[3] = VROMPAGE(Map45_C[7] % (NesHeader.byVRomSize << 3));
		PPUBANK[4] = VROMPAGE(Map45_C[0] % (NesHeader.byVRomSize << 3));
		PPUBANK[5] = VROMPAGE(Map45_C[1] % (NesHeader.byVRomSize << 3));
		PPUBANK[6] = VROMPAGE(Map45_C[2] % (NesHeader.byVRomSize << 3));
		PPUBANK[7] = VROMPAGE(Map45_C[3] % (NesHeader.byVRomSize << 3));
		InfoNES_SetupChr();
	}
	else
	{
		PPUBANK[0] = VROMPAGE(Map45_C[0] % (NesHeader.byVRomSize << 3));
		PPUBANK[1] = VROMPAGE(Map45_C[1] % (NesHeader.byVRomSize << 3));
		PPUBANK[2] = VROMPAGE(Map45_C[2] % (NesHeader.byVRomSize << 3));
		PPUBANK[3] = VROMPAGE(Map45_C[3] % (NesHeader.byVRomSize << 3));
		PPUBANK[4] = VROMPAGE(Map45_C[4] % (NesHeader.byVRomSize << 3));
		PPUBANK[5] = VROMPAGE(Map45_C[5] % (NesHeader.byVRomSize << 3));
		PPUBANK[6] = VROMPAGE(Map45_C[6] % (NesHeader.byVRomSize << 3));
		PPUBANK[7] = VROMPAGE(Map45_C[7] % (NesHeader.byVRomSize << 3));
		InfoNES_SetupChr();
	}
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 46 (Color Dreams)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map46_Regs[ 4 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 46                                             */
/*-------------------------------------------------------------------*/
void Map46_Init()
{
  /* Initialize Mapper */
  MapperInit = Map46_Init;

  /* Write to Mapper */
  MapperWrite = Map46_Write;

  /* Write to SRAM */
  MapperSram = Map46_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  Map46_Regs[ 0 ] = Map46_Regs[ 1 ] = Map46_Regs[ 2 ] = Map46_Regs[ 3 ] = 0;
  Map46_Set_ROM_Banks();

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map46_Sram( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  Map46_Regs[ 0 ] = byData & 0x0f;
  Map46_Regs[ 1 ] = ( byData & 0xf0 ) >> 4;
  Map46_Set_ROM_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map46_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  Map46_Regs[ 2 ] = byData & 0x01;
  Map46_Regs[ 3 ] = ( byData & 0x70 ) >> 4;
  Map46_Set_ROM_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Setup ROM Banks Function                               */
/*-------------------------------------------------------------------*/
void Map46_Set_ROM_Banks()
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );  
  ROMBANK1 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 3 ) % ( NesHeader.byRomSize << 1 ) ); 

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 1 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 2 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 3 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 4 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 4 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 5 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 5 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 6 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 6 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 7 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 7 ) % ( NesHeader.byVRomSize << 3 ) ); 
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 47 (MMC)                             */
/*                                                                   */
/*===================================================================*/

BYTE  Map47_Regs[ 8 ];
DWORD Map47_Rom_Bank;
DWORD Map47_Prg0, Map47_Prg1;
DWORD Map47_Chr01, Map47_Chr23;
DWORD Map47_Chr4, Map47_Chr5, Map47_Chr6, Map47_Chr7;

#define Map47_Chr_Swap()    ( Map47_Regs[ 0 ] & 0x80 )
#define Map47_Prg_Swap()    ( Map47_Regs[ 0 ] & 0x40 )

BYTE Map47_IRQ_Enable;
BYTE Map47_IRQ_Cnt;
BYTE Map47_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 47                                             */
/*-------------------------------------------------------------------*/
void Map47_Init()
{
  /* Initialize Mapper */
  MapperInit = Map47_Init;

  /* Write to Mapper */
  MapperWrite = Map47_Write;

  /* Write to SRAM */
  MapperSram = Map47_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map47_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map47_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map47_Rom_Bank = 0;
  Map47_Prg0 = 0;
  Map47_Prg1 = 1;
  Map47_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map47_Chr01 = 0;
    Map47_Chr23 = 2;
    Map47_Chr4  = 4;
    Map47_Chr5  = 5;
    Map47_Chr6  = 6;
    Map47_Chr7  = 7;
    Map47_Set_PPU_Banks();
  } else {
    Map47_Chr01 = Map47_Chr23 = 0;
    Map47_Chr4 = Map47_Chr5 = Map47_Chr6 = Map47_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map47_IRQ_Enable = 0;
  Map47_IRQ_Cnt = 0;
  Map47_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Sram( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x6000:
      Map47_Rom_Bank = byData & 0x01;
      Map47_Set_CPU_Banks();
      Map47_Set_PPU_Banks();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map47_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map47_Regs[ 0 ] = byData;
      Map47_Set_PPU_Banks();
      Map47_Set_CPU_Banks();
      break;

    case 0x8001:
      Map47_Regs[ 1 ] = byData;
      dwBankNum = Map47_Regs[ 1 ];

      switch ( Map47_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map47_Chr01 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map47_Chr23 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr4 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr5 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr6 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr7 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map47_Prg0 = dwBankNum;
          Map47_Set_CPU_Banks();
          break;

        case 0x07:
          Map47_Prg1 = dwBankNum;
          Map47_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map47_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map47_Regs[ 4 ] = byData;
      Map47_IRQ_Cnt = Map47_Regs[ 4 ];
      break;

    case 0xc001:
      Map47_Regs[ 5 ] = byData;
      Map47_IRQ_Latch = Map47_Regs[ 5 ];
      break;

    case 0xe000:
      Map47_Regs[ 6 ] = byData;
      Map47_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map47_Regs[ 7 ] = byData;
      Map47_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map47_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map47_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map47_IRQ_Cnt ) )
        {
          Map47_IRQ_Cnt = Map47_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Set_CPU_Banks()
{
  if ( Map47_Prg_Swap() )
  {
    ROMBANK0 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    ROMBANK0 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map47_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 48 (Taito TC0190V)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map48_Regs[ 1 ];
BYTE Map48_IRQ_Enable;
BYTE Map48_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 48                                             */
/*-------------------------------------------------------------------*/
void Map48_Init()
{
  /* Initialize Mapper */
  MapperInit = Map48_Init;

  /* Write to Mapper */
  MapperWrite = Map48_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map48_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map48_Regs[ 0 ] = 0;
  Map48_IRQ_Enable = 0;
  Map48_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map48_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Name Table Mirroring */ 
      if ( !Map48_Regs[ 0 ] )
      {
        if ( byData & 0x40 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      /* Set ROM Banks */
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;  
 
    /* Set PPU Banks */
    case 0x8002:
      PPUBANK[ 0 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x8003:
      PPUBANK[ 2 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa000:
      PPUBANK[ 4 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa001:
      PPUBANK[ 5 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa002:
      PPUBANK[ 6 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa003:
      PPUBANK[ 7 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map48_IRQ_Cnt = byData;
      break;

    case 0xc001:
      Map48_IRQ_Enable = byData & 0x01;
      break;

    case 0xe000:
      /* Name Table Mirroring */ 
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      Map48_Regs[ 0 ] = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map48_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map48_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( Map48_IRQ_Cnt == 0xff )
        {
          IRQ_REQ;
          Map48_IRQ_Enable = 0;
        } else {
          Map48_IRQ_Cnt++;
        }
      }
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 49 (Nin1)                            */
/*                                                                   */
/*===================================================================*/

BYTE  Map49_Regs[ 3 ];
DWORD Map49_Prg0, Map49_Prg1;
DWORD Map49_Chr01, Map49_Chr23;
DWORD Map49_Chr4, Map49_Chr5, Map49_Chr6, Map49_Chr7;

BYTE Map49_IRQ_Enable;
BYTE Map49_IRQ_Cnt;
BYTE Map49_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 49                                             */
/*-------------------------------------------------------------------*/
void Map49_Init()
{
  /* Initialize Mapper */
  MapperInit = Map49_Init;

  /* Write to Mapper */
  MapperWrite = Map49_Write;

  /* Write to SRAM */
  MapperSram = Map49_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map49_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  Map49_Prg0 = 0;
  Map49_Prg1 = 1;
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );
  
  /* Set PPU Banks */
  Map49_Chr01 = 0;
  Map49_Chr23 = 2;
  Map49_Chr4  = 4;
  Map49_Chr5  = 5;
  Map49_Chr6  = 6;
  Map49_Chr7  = 7;

  for ( int nPage = 0; nPage < 8; ++nPage )
  {
    PPUBANK[ nPage ] = VROMPAGE( nPage );
  }
  InfoNES_SetupChr();
  
  /* Initialize IRQ Registers */
  Map49_Regs[ 0 ] = Map49_Regs[ 1 ] = Map49_Regs[ 2 ] = 0;
  Map49_IRQ_Enable = 0;
  Map49_IRQ_Cnt = 0;
  Map49_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Sram( WORD wAddr, BYTE byData )
{
  if ( Map49_Regs[ 2 ] & 0x80 )
  {
    Map49_Regs[ 1 ] = byData;
    Map49_Set_CPU_Banks();
    Map49_Set_PPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map49_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      if ( ( byData & 0x40 ) != ( Map49_Regs[ 0 ] & 0x40 ) )
      {
        Map49_Set_CPU_Banks();
      }
      if ( ( byData & 0x80 ) != ( Map49_Regs[ 0 ] & 0x80 ) )
      {
        Map49_Regs[ 0 ] = byData;
        Map49_Set_PPU_Banks();
      }
      Map49_Regs[ 0 ] = byData;
      break;

    case 0x8001:
      switch ( Map49_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          Map49_Chr01 = byData & 0xfe;
          Map49_Set_PPU_Banks();
          break;

        case 0x01:
          Map49_Chr23 = byData & 0xfe;
          Map49_Set_PPU_Banks();
          break;

        case 0x02:
          Map49_Chr4 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x03:
          Map49_Chr5 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x04:
          Map49_Chr6 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x05:
          Map49_Chr7 = byData;
          Map49_Set_PPU_Banks();
          break;

        /* Set ROM Banks */
        case 0x06:
          Map49_Prg0 = byData;
          Map49_Set_CPU_Banks();
          break;

        case 0x07:
          Map49_Prg1 = byData;
          Map49_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map49_Regs[ 2 ] = byData;
      break;

    case 0xc000:
      Map49_IRQ_Cnt = byData;
      break;

    case 0xc001:
      Map49_IRQ_Latch = byData;
      break;

    case 0xe000:
      Map49_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map49_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map49_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map49_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map49_IRQ_Cnt-- ) )
        {
          Map49_IRQ_Cnt = Map49_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Set_CPU_Banks()
{
  DWORD dwBank0, dwBank1, dwBank2, dwBank3;

  if ( Map49_Regs[ 1 ] & 0x01 )
  {
    if ( Map49_Regs[ 0 ] & 0x40 )
    {
      dwBank0 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0e ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank1 = ( Map49_Prg1 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank2 = ( Map49_Prg0 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank3 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
    } else {
      dwBank0 = ( Map49_Prg0 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank1 = ( Map49_Prg1 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank2 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0e ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank3 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
    }
  } else {
    dwBank0 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 0;
    dwBank1 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 1;
    dwBank2 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 2;
    dwBank3 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 3;
  }

  /* Set ROM Banks */ 
  ROMBANK0 = ROMPAGE( dwBank0 % ( NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( dwBank1 % ( NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( dwBank2 % ( NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( dwBank3 % ( NesHeader.byRomSize << 1) );
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Set_PPU_Banks()
{
  Map49_Chr01 = ( Map49_Chr01 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr23 = ( Map49_Chr23 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr4 = ( Map49_Chr4 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr5 = ( Map49_Chr5 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr6 = ( Map49_Chr6 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr7 = ( Map49_Chr7 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );

  /* Set PPU Banks */ 
  if ( Map49_Regs[ 0 ] & 0x80 )
  { 
    PPUBANK[ 0 ] = VROMPAGE( Map49_Chr4 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( Map49_Chr5 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( Map49_Chr6 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( Map49_Chr7 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ( Map49_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( Map49_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ( Map49_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( Map49_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = VROMPAGE( ( Map49_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ( Map49_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ( Map49_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ( Map49_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( Map49_Chr4 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( Map49_Chr5 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( Map49_Chr6 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( Map49_Chr7 % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 50 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map50_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 50                                             */
/*-------------------------------------------------------------------*/
void Map50_Init()
{
  /* Initialize Mapper */
  MapperInit = Map50_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map50_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map50_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 15 % ( NesHeader.byRomSize << 1 ) );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 8 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( 9 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( 0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( 11 % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }    
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map50_IRQ_Enable = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 50 Write to Apu Function                                  */
/*-------------------------------------------------------------------*/
void Map50_Apu( WORD wAddr, BYTE byData )
{
  if ( ( wAddr & 0xE060 ) == 0x4020 )
  {
    if( wAddr & 0x0100 )
    {
      Map50_IRQ_Enable = byData & 0x01;
    }
    else
    {
      BYTE byDummy;

      byDummy = ( byData & 0x08 ) | ( ( byData & 0x01 ) << 2 ) | ( ( byData & 0x06 ) >> 1 );
      ROMBANK2 = ROMPAGE( byDummy % ( NesHeader.byRomSize << 1 ) );
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 50 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map50_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map50_IRQ_Enable )
  {
    if ( PPU_Scanline == 21 )
    {
      IRQ_REQ;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 51 : 11-in-1                           */
/*                                                                   */
/*===================================================================*/

int     Map51_Mode, Map51_Bank;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 51                                             */
/*-------------------------------------------------------------------*/
void Map51_Init()
{
  /* Initialize Mapper */
  MapperInit = Map51_Init;

  /* Write to Mapper */
  MapperWrite = Map51_Write;

  /* Write to SRAM */
  MapperSram = Map51_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  Map51_Bank = 0;
  Map51_Mode = 1;

  /* Set ROM Banks */
  Map51_Set_CPU_Banks();

  /* Set PPU Banks */
  for ( int nPage = 0; nPage < 8; ++nPage )
    PPUBANK[ nPage ] = CRAMPAGE( nPage );
  InfoNES_SetupChr();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map51_Write( WORD wAddr, BYTE byData )
{
  Map51_Bank = (byData & 0x0f) << 2;
  if( 0xC000 <= wAddr && wAddr <= 0xDFFF ) {
    Map51_Mode = (Map51_Mode & 0x01) | ((byData & 0x10) >> 3);
  }
  Map51_Set_CPU_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Sram( WORD wAddr, BYTE byData )
{
  if( wAddr>=0x6000 ) {
    Map51_Mode = ((byData & 0x10) >> 3) | ((byData & 0x02) >> 1);
    Map51_Set_CPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Set_CPU_Banks()
{
  switch(Map51_Mode) {
  case 0:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x2c|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x0c|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x0c|3) % (NesHeader.byRomSize<<1));
    break;
  case 1:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  case 2:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x2e|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x02|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x02|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x0e|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x0e|3) % (NesHeader.byRomSize<<1));
    break;
  case 3:
    InfoNES_Mirroring( 0 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  }
}

/*===================================================================*/
/*                                                                   */
/*                           Mapper 57                               */
/*                                                                   */
/*===================================================================*/

BYTE	Map57_Reg;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 57                                             */
/*-------------------------------------------------------------------*/
void Map57_Init()
{
  /* Initialize Mapper */
  MapperInit = Map57_Init;

  /* Write to Mapper */
  MapperWrite = Map57_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Registers */
  Map57_Reg = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 57 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map57_Write( WORD wAddr, BYTE byData )
{
  BYTE byChr;

  switch( wAddr ) {
  case	0x8000:
  case	0x8001:
  case	0x8002:
  case	0x8003:
    if( byData & 0x40 ) {
      byChr = (byData&0x03)+((Map57_Reg&0x10)>>1)+(Map57_Reg&0x07);

      PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
    break;
  case	0x8800:
    Map57_Reg = byData;
    
    if( byData & 0x80 ) {
      ROMBANK0 = ROMPAGE((((byData & 0x40)>>6)*4+8+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData & 0x40)>>6)*4+8+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData & 0x40)>>6)*4+8+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData & 0x40)>>6)*4+8+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((((byData & 0x60)>>5)*2+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData & 0x60)>>5)*2+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData & 0x60)>>5)*2+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData & 0x60)>>5)*2+1) % (NesHeader.byRomSize<<1));
    }
    
    byChr = (byData&0x07)+((byData&0x10)>>1);
    
    PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();

    if( byData & 0x08 ) InfoNES_Mirroring( 0 );
    else		InfoNES_Mirroring( 1 );
    
    break;
  }
}

/*===================================================================*/
/*                                                                   */
/*                           Mapper 58                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 58                                             */
/*-------------------------------------------------------------------*/
void Map58_Init()
{
  /* Initialize Mapper */
  MapperInit = Map58_Init;

  /* Write to Mapper */
  MapperWrite = Map58_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 58 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map58_Write( WORD wAddr, BYTE byData )
{
  if( wAddr & 0x40 ) {
    ROMBANK0 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE((((wAddr&0x06)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x06)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x06)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x06)<<1)+3) % (NesHeader.byRomSize<<1));
  }

  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE(((wAddr&0x38)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((wAddr&0x38)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((wAddr&0x38)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((wAddr&0x38)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((wAddr&0x38)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((wAddr&0x38)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((wAddr&0x38)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((wAddr&0x38)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }

  if( byData & 0x02 ) InfoNES_Mirroring( 1 );
  else		      InfoNES_Mirroring( 0 );
}
/*===================================================================*/
/*                                                                   */
/*                            Mapper 60                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 60                                             */
/*-------------------------------------------------------------------*/
void Map60_Init()
{
  /* Initialize Mapper */
  MapperInit = Map60_Init;

  /* Write to Mapper */
  MapperWrite = Map60_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 60 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map60_Write( WORD wAddr, BYTE byData )
{
  if( wAddr & 0x80 ) {
    ROMBANK0 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x70)>>3)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x70)>>3)+3) % (NesHeader.byRomSize<<1));
  }
  

  PPUBANK[ 0 ] = VROMPAGE((((wAddr&0x07)<<3)+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE((((wAddr&0x07)<<3)+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE((((wAddr&0x07)<<3)+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE((((wAddr&0x07)<<3)+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE((((wAddr&0x07)<<3)+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE((((wAddr&0x07)<<3)+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE((((wAddr&0x07)<<3)+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE((((wAddr&0x07)<<3)+7) % (NesHeader.byVRomSize<<3));
  InfoNES_SetupChr();
  
  if( byData & 0x08 ) InfoNES_Mirroring( 0 );
  else		      InfoNES_Mirroring( 1 );
}
/*===================================================================*/
/*                                                                   */
/*                            Mapper 61                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 61                                             */
/*-------------------------------------------------------------------*/
void Map61_Init()
{
  /* Initialize Mapper */
  MapperInit = Map61_Init;

  /* Write to Mapper */
  MapperWrite = Map61_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 61 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map61_Write( WORD wAddr, BYTE byData )
{
	BYTE byBank;

	switch( wAddr & 0x30 ) {
		case	0x00:
		case	0x30:
			ROMBANK0 = ROMPAGE((((wAddr&0x0F)<<2)+0) % (NesHeader.byRomSize<<1));
			ROMBANK1 = ROMPAGE((((wAddr&0x0F)<<2)+1) % (NesHeader.byRomSize<<1));
			ROMBANK2 = ROMPAGE((((wAddr&0x0F)<<2)+2) % (NesHeader.byRomSize<<1));
			ROMBANK3 = ROMPAGE((((wAddr&0x0F)<<2)+3) % (NesHeader.byRomSize<<1));
			break;
		case	0x10:
		case	0x20:
			byBank = ((wAddr & 0x0F)<<1)|((wAddr&0x20)>>4);

			ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			break;
	}

	if( wAddr & 0x80 ) InfoNES_Mirroring( 0 );
	else		   InfoNES_Mirroring( 1 );
}
/*===================================================================*/
/*                                                                   */
/*                            Mapper 62                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 62                                             */
/*-------------------------------------------------------------------*/
void Map62_Init()
{
  /* Initialize Mapper */
  MapperInit = Map62_Init;

  /* Write to Mapper */
  MapperWrite = Map62_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 62 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map62_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xFF00 ) {
  case	0x8100:
    ROMBANK0 = ROMPAGE((byData+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((byData+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x8500:
    ROMBANK0 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
    break;
  case	0x8700:
    ROMBANK1 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
    break;

  default:
    PPUBANK[ 0 ] = VROMPAGE((byData+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((byData+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((byData+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((byData+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((byData+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((byData+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE((byData+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((byData+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                    Mapper 64 (Tengen RAMBO-1)                     */
/*                                                                   */
/*===================================================================*/

BYTE Map64_Cmd;
BYTE Map64_Prg;
BYTE Map64_Chr;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 64                                             */
/*-------------------------------------------------------------------*/
void Map64_Init()
{
  /* Initialize Mapper */
  MapperInit = Map64_Init;

  /* Write to Mapper */
  MapperWrite = Map64_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 0 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMLASTPAGE( 0 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize state flag */
  Map64_Cmd = 0x00;
  Map64_Prg = 0x00;
  Map64_Chr = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 64 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map64_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set state flag */
      Map64_Cmd = byData & 0x0f;
      Map64_Prg = ( byData & 0x40 ) >> 6;
      Map64_Chr = ( byData & 0x80 ) >> 7;
      break;

    case 0x8001:
      switch ( Map64_Cmd )
      {
        case 0x00:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 4 ] = VROMPAGE( byData );
            PPUBANK[ 5 ] = VROMPAGE( byData + 1 );      
          } else {
            PPUBANK[ 0 ] = VROMPAGE( byData );
            PPUBANK[ 1 ] = VROMPAGE( byData + 1 );  
          } 
          InfoNES_SetupChr();
          break;

        case 0x01:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 6 ] = VROMPAGE( byData );
            PPUBANK[ 7 ] = VROMPAGE( byData + 1 );      
          } else {
            PPUBANK[ 2 ] = VROMPAGE( byData );
            PPUBANK[ 3 ] = VROMPAGE( byData + 1 );  
          } 
          InfoNES_SetupChr();
          break;

        case 0x02:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 0 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 4 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x03:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 1 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 5 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x04:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 2 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 6 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x05:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 3 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 7 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x06:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK1 = ROMPAGE( byData );
          } else {
            ROMBANK0 = ROMPAGE( byData );
          } 
          break;

        case 0x07:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK2 = ROMPAGE( byData );
          } else {
            ROMBANK1 = ROMPAGE( byData );
          } 
          break;

        case 0x08:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 1 ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        case 0x09:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 3 ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        case 0x0f:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK0 = ROMPAGE( byData );
          } else {
            ROMBANK2 = ROMPAGE( byData );
          } 
          break;
      }
      break;

    default:
      switch ( wAddr & 0xf000 )
      {
        case 0xa000:
          /* Name Table Mirroring */
          InfoNES_Mirroring( byData & 0x01 );
          break;

        default:
          break;
      }
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 65 (Irem H3001)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map65_IRQ_Enable;
DWORD Map65_IRQ_Cnt;
DWORD Map65_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 65                                             */
/*-------------------------------------------------------------------*/
void Map65_Init()
{
  /* Initialize Mapper */
  MapperInit = Map65_Init;

  /* Write to Mapper */
  MapperWrite = Map65_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map65_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 65 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map65_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0x9000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }
      break;

    case 0x9003:
      Map65_IRQ_Enable = byData & 0x80;
      break;

    case 0x9004:
      Map65_IRQ_Cnt = Map65_IRQ_Latch;
      break;

    case 0x9005:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0x00ff ) | ((DWORD)byData << 8 );
      break;

    case 0x9006:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0xff00 ) | (DWORD)byData;
      break;

    /* Set PPU Banks */
    case 0xb000:
      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb004:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb005:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb006:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb007:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    /* Set ROM Banks */
    case 0xa000:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0xc000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 65 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map65_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map65_IRQ_Enable )
  {
    if ( Map65_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map65_IRQ_Enable = 0;
      Map65_IRQ_Cnt = 0xffff;
    } else {
      Map65_IRQ_Cnt -= 113;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 66 (GNROM)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 66                                             */
/*-------------------------------------------------------------------*/
void Map66_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map66_Init;

  /* Write to Mapper */
  MapperWrite = Map66_Write;

  /* Write to SRAM */
  MapperSram = Map66_Write;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 0 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 66 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map66_Write( WORD wAddr, BYTE byData )
{
  BYTE byRom;
  BYTE byVRom;

  byRom  = ( byData >> 4 ) & 0x0F;
  byVRom = byData & 0x0F;

  /* Set ROM Banks */
  byRom <<= 1;
  byRom %= NesHeader.byRomSize;
  byRom <<= 1;

  ROMBANK0 = ROMPAGE( byRom );
  ROMBANK1 = ROMPAGE( byRom + 1 );
  ROMBANK2 = ROMPAGE( byRom + 2 );
  ROMBANK3 = ROMPAGE( byRom + 3 );

  /* Set PPU Banks */
  byVRom <<= 3;
  byVRom %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byVRom );
  PPUBANK[ 1 ] = VROMPAGE( byVRom + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byVRom + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byVRom + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byVRom + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byVRom + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byVRom + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byVRom + 7 );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*               Mapper 67 (Sunsoft Mapper #3)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map67_IRQ_Enable;
BYTE Map67_IRQ_Cnt;
BYTE Map67_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 67                                             */
/*-------------------------------------------------------------------*/
void Map67_Init()
{
  /* Initialize Mapper */
  MapperInit = Map67_Init;

  /* Write to Mapper */
  MapperWrite = Map67_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map67_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( 0 );
  PPUBANK[ 1 ] = VROMPAGE( 1 );
  PPUBANK[ 2 ] = VROMPAGE( 2 );
  PPUBANK[ 3 ] = VROMPAGE( 3 );
  PPUBANK[ 4 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 4 );
  PPUBANK[ 5 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 3 );
  PPUBANK[ 6 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 2 );
  PPUBANK[ 7 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 1 );
  InfoNES_SetupChr();

  /* Initialize IRQ Registers */
  Map67_IRQ_Enable = 0;
  Map67_IRQ_Cnt = 0;
  Map67_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map67_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf800 )
  {
    /* Set PPU Banks */
    case 0x8800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0x9800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 2 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xa800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 4 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xb800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 6 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xc800:
      Map67_IRQ_Cnt = Map67_IRQ_Latch;
      Map67_IRQ_Latch = byData;
      break;

    case 0xd800:
      Map67_IRQ_Enable = byData & 0x10;
      break;

    case 0xe800:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 1 );
          break;
        case 1:
          InfoNES_Mirroring( 0 );
          break;
        case 2:
          InfoNES_Mirroring( 3 );
          break;
        case 3:
          InfoNES_Mirroring( 2 );
          break;
      }
      break;

    /* Set ROM Banks */
    case 0xf800:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byData + 0 );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map67_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map67_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( --Map67_IRQ_Cnt == 0xf6 )
        {
          Map67_IRQ_Cnt = Map67_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                Mapper 68 (Sunsoft Mapper #4)                      */
/*                                                                   */
/*===================================================================*/

BYTE Map68_Regs[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 68                                              */
/*-------------------------------------------------------------------*/
void Map68_Init()
{
  /* Initialize Mapper */
  MapperInit = Map68_Init;

  /* Write to Mapper */
  MapperWrite = Map68_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize state flag */
  for ( int i = 0; i < 4; i++ )
    Map68_Regs[ i ] = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 68 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map68_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr )
  {
    case 0x8000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0x9000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 2 ] = VROMPAGE( byData );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xA000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 4 ] = VROMPAGE( byData );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xB000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 6 ] = VROMPAGE( byData );
      PPUBANK[ 7 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xC000:  
      Map68_Regs[ 2 ] = byData;
      Map68_SyncMirror();
      break;

    case 0xD000:  
      Map68_Regs[ 3 ] = byData;
      Map68_SyncMirror();
      break;

    case 0xE000:  
      Map68_Regs[ 0 ] = ( byData & 0x10 ) >> 4;
      Map68_Regs[ 1 ] = byData & 0x03;
      Map68_SyncMirror();
      break;

    case 0xF000:  
      /* Set ROM Banks */
      byData %= NesHeader.byRomSize;
      byData <<= 1;
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 68 Sub Function                                           */
/*-------------------------------------------------------------------*/
void Map68_SyncMirror( void )
{
  if ( Map68_Regs[ 0 ] )
  {
    switch( Map68_Regs[ 1 ] )
    {
      case 0x00:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;

      case 0x01:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;

      case 0x02:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        break;

      case 0x03:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;
    }
    InfoNES_SetupChr();
  } 
  else 
  {
    InfoNES_Mirroring( Map68_Regs[ 1 ] );
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 69 (Sunsoft FME-7)                        */
/*                                                                   */
/*===================================================================*/

BYTE  Map69_IRQ_Enable;
DWORD Map69_IRQ_Cnt;
BYTE  Map69_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 69                                             */
/*-------------------------------------------------------------------*/
void Map69_Init()
{
  /* Initialize Mapper */
  MapperInit = Map69_Init;

  /* Write to Mapper */
  MapperWrite = Map69_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map69_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Reg */
  Map69_IRQ_Enable = 0;
  Map69_IRQ_Cnt    = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 69 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map69_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map69_Regs[ 0 ] = byData & 0x0f;
      break;

    case 0xA000:
      switch ( Map69_Regs[ 0 ] )
      {
        /* Set PPU Banks */
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ Map69_Regs[ 0 ] ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        /* Set ROM Banks */
#if 0
        case 0x08:
          if ( !( byData & 0x40 ) )
          {
            byData %= ( NesHeader.byRomSize << 1 );
            SRAMBANK = ROMPAGE( byData );
          }
          break;
#endif

        case 0x09:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK0 = ROMPAGE( byData );
          break;

        case 0x0a:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK1 = ROMPAGE( byData );
          break;

        case 0x0b:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK2 = ROMPAGE( byData );
          break;

        /* Name Table Mirroring */
        case 0x0c:  
          switch ( byData & 0x03 )
          {
            case 0:
              InfoNES_Mirroring( 1 );   /* Vertical */
              break;
            case 1:
              InfoNES_Mirroring( 0 );   /* Horizontal */
              break;
            case 2:
              InfoNES_Mirroring( 3 );   /* One Screen 0x2400 */
              break;
            case 3:
              InfoNES_Mirroring( 2 );   /* One Screen 0x2000 */
              break;
          }
          break;

        case 0x0d:
          Map69_IRQ_Enable = byData;
          break;

        case 0x0e:
          Map69_IRQ_Cnt = ( Map69_IRQ_Cnt & 0xff00) | (DWORD)byData;
          break;

        case 0x0f:
          Map69_IRQ_Cnt = ( Map69_IRQ_Cnt & 0x00ff) | ( (DWORD)byData << 8 );
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 69 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map69_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map69_IRQ_Enable )
  {
    if ( Map69_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map69_IRQ_Cnt = 0;
    } else {
      Map69_IRQ_Cnt -= 113;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                 Mapper 70 (74161/32 Bandai)                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 70                                             */
/*-------------------------------------------------------------------*/
void Map70_Init()
{
  /* Initialize Mapper */
  MapperInit = Map70_Init;

  /* Write to Mapper */
  MapperWrite = Map70_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 70 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map70_Write( WORD wAddr, BYTE byData )
{
  BYTE byChrBank = byData & 0x0f;
  BYTE byPrgBank = ( byData & 0x70 ) >> 4;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr();

  /* Name Table Mirroring */
  if ( byData & 0x80 )
  {
    InfoNES_Mirroring( 2 );
  } else {
    InfoNES_Mirroring( 3 );
  }
}
/*===================================================================*/
/*                                                                   */
/*            Mapper 71 (Camerica Custom Mapper)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 71                                             */
/*-------------------------------------------------------------------*/
void Map71_Init()
{
  /* Initialize Mapper */
  MapperInit = Map71_Init;

  /* Write to Mapper */
  MapperWrite = Map71_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 71 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map71_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf000 )
  {
    case 0x9000:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring( 2 );
      } else {
        InfoNES_Mirroring( 3 );
      }
      break;

    /* Set ROM Banks */
    case 0xc000:
    case 0xd000:
    case 0xe000:
    case 0xf000:
      ROMBANK0 = ROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*           Mapper 72 (Jaleco Early Mapper #0)                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 72                                             */
/*-------------------------------------------------------------------*/
void Map72_Init()
{
  /* Initialize Mapper */
  MapperInit = Map72_Init;

  /* Write to Mapper */
  MapperWrite = Map72_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 72 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map72_Write( WORD wAddr, BYTE byData )
{
  BYTE byBank = byData & 0x0f;

  if ( byData & 0x80 )
  {
    /* Set ROM Banks */
    byBank <<= 1;
    byBank %= ( NesHeader.byRomSize << 1 );
    ROMBANK0 = ROMPAGE( byBank );
    ROMBANK1 = ROMPAGE( byBank + 1 );
  } else 
  if ( byData & 0x40 )
  {
    /* Set PPU Banks */
    byBank <<= 3;
    byBank %= ( NesHeader.byVRomSize << 3 );
    PPUBANK[ 0 ] = VROMPAGE( byBank );
    PPUBANK[ 1 ] = VROMPAGE( byBank + 1 );
    PPUBANK[ 2 ] = VROMPAGE( byBank + 2 );
    PPUBANK[ 3 ] = VROMPAGE( byBank + 3 );
    PPUBANK[ 4 ] = VROMPAGE( byBank + 4 );
    PPUBANK[ 5 ] = VROMPAGE( byBank + 5 );
    PPUBANK[ 6 ] = VROMPAGE( byBank + 6 );
    PPUBANK[ 7 ] = VROMPAGE( byBank + 7 );
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 73 (Konami VRC 3)                      */
/*                                                                   */
/*===================================================================*/

BYTE  Map73_IRQ_Enable;
DWORD Map73_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 73                                             */
/*-------------------------------------------------------------------*/
void Map73_Init()
{
  /* Initialize Mapper */
  MapperInit = Map73_Init;

  /* Write to Mapper */
  MapperWrite = Map73_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map73_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize IRQ Registers */
  Map73_IRQ_Enable = 0;
  Map73_IRQ_Cnt = 0;  

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 73 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map73_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xfff0 ) | ( byData & 0x0f );
      break;

    case 0x9000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xff0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xa000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xf0ff ) | ( ( byData & 0x0f ) << 8 );
      break;

    case 0xb000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0x0fff ) | ( ( byData & 0x0f ) << 12 );
      break;

    case 0xc000:
      Map73_IRQ_Enable = byData;
      break;

    /* Set ROM Banks */
    case 0xf000:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 73 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map73_HSync()
{
/*
 *  Callback at HSync
 *
 */
#if 1
  if ( Map73_IRQ_Enable & 0x02 )
  {
    if ( ( Map73_IRQ_Cnt += STEP_PER_SCANLINE ) > 0xffff )
    {
      Map73_IRQ_Cnt &= 0xffff;
      IRQ_REQ;
      Map73_IRQ_Enable = 0;
    }
  }
#else
  if ( Map73_IRQ_Enable )
  {
    if ( Map73_IRQ_Cnt > 0xffff - 114 )
    {
      IRQ_REQ;
      Map73_IRQ_Enable = 0;
    } else {
      Map73_IRQ_Cnt += 114;
    }
  }
#endif
}
/*===================================================================*/
/*                                                                   */
/*         Mapper 74 : Metal Max (Zhong Zhuang Ji Bing               */
/*                                                                   */
/*===================================================================*/

BYTE  Map74_Regs[ 8 ];
DWORD Map74_Rom_Bank;
DWORD Map74_Prg0, Map74_Prg1;
DWORD Map74_Chr01, Map74_Chr23;
DWORD Map74_Chr4, Map74_Chr5, Map74_Chr6, Map74_Chr7;

#define Map74_Chr_Swap()    ( Map74_Regs[ 0 ] & 0x80 )
#define Map74_Prg_Swap()    ( Map74_Regs[ 0 ] & 0x40 )

BYTE Map74_IRQ_Enable;
BYTE Map74_IRQ_Cnt;
BYTE Map74_IRQ_Latch;
BYTE Map74_IRQ_Request;
BYTE Map74_IRQ_Present;
BYTE Map74_IRQ_Present_Vbl;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 74                                             */
/*-------------------------------------------------------------------*/
void Map74_Init()
{
  /* Initialize Mapper */
  MapperInit = Map74_Init;

  /* Write to Mapper */
  MapperWrite = Map74_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map74_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map74_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map74_Prg0 = 0;
  Map74_Prg1 = 1;
  Map74_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map74_Chr01 = 0;
    Map74_Chr23 = 2;
    Map74_Chr4  = 4;
    Map74_Chr5  = 5;
    Map74_Chr6  = 6;
    Map74_Chr7  = 7;
    Map74_Set_PPU_Banks();
  } else {
    Map74_Chr01 = Map74_Chr23 = 0;
    Map74_Chr4 = Map74_Chr5 = Map74_Chr6 = Map74_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map74_IRQ_Enable = 0;
  Map74_IRQ_Cnt = 0;
  Map74_IRQ_Latch = 0;
  Map74_IRQ_Request = 0;
  Map74_IRQ_Present = 0;
  Map74_IRQ_Present_Vbl = 0;

  /* VRAM Write Enabled */
  byVramWriteEnable = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map74_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map74_Regs[ 0 ] = byData;
      Map74_Set_PPU_Banks();
      Map74_Set_CPU_Banks();
      break;

    case 0x8001:
      Map74_Regs[ 1 ] = byData;
      dwBankNum = Map74_Regs[ 1 ];

      switch ( Map74_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map74_Chr01 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map74_Chr23 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr4 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr5 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr6 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr7 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map74_Prg0 = dwBankNum;
          Map74_Set_CPU_Banks();
          break;

        case 0x07:
          Map74_Prg1 = dwBankNum;
          Map74_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map74_Regs[ 2 ] = byData;

      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map74_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map74_Regs[ 4 ] = byData;
      Map74_IRQ_Latch = byData;
      break;

    case 0xc001:
      Map74_Regs[ 5 ] = byData;
      if ( PPU_Scanline < 240 )
      {
          Map74_IRQ_Cnt |= 0x80;
          Map74_IRQ_Present = 0xff;
      } else {
          Map74_IRQ_Cnt |= 0x80;
          Map74_IRQ_Present_Vbl = 0xff;
          Map74_IRQ_Present = 0;
      }
      break;

    case 0xe000:
      Map74_Regs[ 6 ] = byData;
      Map74_IRQ_Enable = 0;
			Map74_IRQ_Request = 0;
      break;

    case 0xe001:
      Map74_Regs[ 7 ] = byData;
      Map74_IRQ_Enable = 1;
			Map74_IRQ_Request = 0;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map74_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( 0 <= PPU_Scanline && PPU_Scanline <= 239 ) && 
       ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) )
  {
		if( Map74_IRQ_Present_Vbl ) {
			Map74_IRQ_Cnt = Map74_IRQ_Latch;
			Map74_IRQ_Present_Vbl = 0;
		}
		if( Map74_IRQ_Present ) {
			Map74_IRQ_Cnt = Map74_IRQ_Latch;
			Map74_IRQ_Present = 0;
		} else if( Map74_IRQ_Cnt > 0 ) {
			Map74_IRQ_Cnt--;
		}

		if( Map74_IRQ_Cnt == 0 ) {
			if( Map74_IRQ_Enable ) {
				Map74_IRQ_Request = 0xFF;
			}
			Map74_IRQ_Present = 0xFF;
		}
	}
	if( Map74_IRQ_Request  ) {
		IRQ_REQ;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map74_Set_CPU_Banks()
{
  if ( Map74_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map74_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map74_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map74_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map74_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map74_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map74_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map74_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map74_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map74_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map74_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map74_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map74_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map74_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map74_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map74_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map74_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map74_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map74_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map74_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map74_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map74_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map74_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
  else
  {
    if ( Map74_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
      InfoNES_SetupChr();
    }
  }    
}

/*===================================================================*/
/*                                                                   */
/*           Mapper 75 (Konami VRC 1 and Jaleco SS8805)              */
/*                                                                   */
/*===================================================================*/

BYTE Map75_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 75                                             */
/*-------------------------------------------------------------------*/
void Map75_Init()
{
  /* Initialize Mapper */
  MapperInit = Map75_Init;

  /* Write to Mapper */
  MapperWrite = Map75_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize State Flag */
  Map75_Regs[ 0 ] = 0;
  Map75_Regs[ 1 ] = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 75 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map75_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xf000 )
  {
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x9000:
      /* Set Mirroring */
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }

      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x02 ) << 3 );
      PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );

      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x04 ) << 2 );
      PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;

    /* Set ROM Banks */
    case 0xA000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    /* Set ROM Banks */
    case 0xC000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0xE000:
      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x10 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;

    case 0xF000:
      /* Set PPU Banks */
      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x10 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                    Mapper 76 (Namcot 109)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map76_Reg;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 76                                             */
/*-------------------------------------------------------------------*/
void Map76_Init()
{
  /* Initialize Mapper */
  MapperInit = Map76_Init;

  /* Write to Mapper */
  MapperWrite = Map76_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 76 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map76_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map76_Reg = byData;
      break;

    case 0x8001:
      switch ( Map76_Reg & 0x07 )
      {
        case 0x02:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 0 ] = VROMPAGE( byData );
          PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x03:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 2 ] = VROMPAGE( byData );
          PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x04:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 4 ] = VROMPAGE( byData );
          PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x05:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 6 ] = VROMPAGE( byData );
          PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x06:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK0 = ROMPAGE( byData );
          break;

        case 0x07:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK1 = ROMPAGE( byData );
          break;
      }
      break;
  }  
}
/*===================================================================*/
/*                                                                   */
/*                Mapper 77  (Irem Early Mapper #0)                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 77                                             */
/*-------------------------------------------------------------------*/
void Map77_Init()
{
  /* Initialize Mapper */
  MapperInit = Map77_Init;

  /* Write to Mapper */
  MapperWrite = Map77_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* VRAM Write Enabled */
  byVramWriteEnable = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 77 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map77_Write( WORD wAddr, BYTE byData )
{
  BYTE byRomBank = byData & 0x07;
  BYTE byChrBank = ( byData & 0xf0 ) >> 4;

  /* Set ROM Banks */
  byRomBank <<= 2;
  byRomBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byRomBank );
  ROMBANK1 = ROMPAGE( byRomBank + 1 );
  ROMBANK2 = ROMPAGE( byRomBank + 2 );
  ROMBANK3 = ROMPAGE( byRomBank + 3 );

  /* Set PPU Banks */
  byChrBank <<= 1;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 78 (74161/32 Irem)                    */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 78                                             */
/*-------------------------------------------------------------------*/
void Map78_Init()
{
  /* Initialize Mapper */
  MapperInit = Map78_Init;

  /* Write to Mapper */
  MapperWrite = Map78_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 78 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map78_Write( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = byData & 0x0f;
  BYTE byChrBank = ( byData & 0xf0 ) >> 4;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );
  ROMBANK0 = ROMPAGE( byPrgBank );
  ROMBANK1 = ROMPAGE( byPrgBank + 1);

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );
  PPUBANK[ 0 ] = VROMPAGE( byChrBank );  
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 ); 
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 ); 
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 ); 
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 ); 
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 ); 
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 ); 
  InfoNES_SetupChr();  

  /* Set Name Table Mirroring */
  if ( ( wAddr & 0xfe00 ) != 0xfe00 )
  {
    if ( byData & 0x08 )
    {
      InfoNES_Mirroring( 2 );
    } else {
      InfoNES_Mirroring( 3 );
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*   Mapper 79 (American Video Entertainment/Sachen Custom Mapper)   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 79                                             */
/*-------------------------------------------------------------------*/
void Map79_Init()
{
  /* Initialize Mapper */
  MapperInit = Map79_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map79_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 79 Write to Apu Function                                  */
/*-------------------------------------------------------------------*/
void Map79_Apu( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = ( byData & 0x08 ) >> 3;
  BYTE byChrBank = byData & 0x07;
 
  /* Set ROM Banks */
  byPrgBank <<= 2;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank + 0 );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );
  ROMBANK2 = ROMPAGE( byPrgBank + 2 );
  ROMBANK3 = ROMPAGE( byPrgBank + 3 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize  << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 80 (X1-005)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 80                                             */
/*-------------------------------------------------------------------*/
void Map80_Init()
{
  /* Initialize Mapper */
  MapperInit = Map80_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map80_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 80 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map80_Sram( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set PPU Banks */
    case 0x7ef0:
      byData &= 0x7f;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0x7ef1:
      byData &= 0x7f;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 2 ] = VROMPAGE( byData );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;
  
    case 0x7ef2:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;      

    case 0x7ef3:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;  

    case 0x7ef4:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 

    case 0x7ef5:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 

    /* Name Table Mirroring */
    case 0x7ef6:
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }

    /* Set ROM Banks */
    case 0x7efa:
    case 0x7efb:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x7efc:
    case 0x7efd:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x7efe:
    case 0x7eff:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 82 (Taito X1-17)                    */
/*                                                                   */
/*===================================================================*/

BYTE Map82_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 82                                             */
/*-------------------------------------------------------------------*/
void Map82_Init()
{
  /* Initialize Mapper */
  MapperInit = Map82_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map82_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 82 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map82_Sram( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set PPU Banks */
    case 0x7ef0:
      byData &= 0xfe;
      byData %= ( NesHeader.byVRomSize << 3 );

      if ( Map82_Regs[ 0 ] )
      {
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      } else {
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      }
      InfoNES_SetupChr();
      break;

    case 0x7ef1:
      byData &= 0xfe;
      byData %= ( NesHeader.byVRomSize << 3 );

      if ( Map82_Regs[ 0 ] )
      {
        PPUBANK[ 6 ] = VROMPAGE( byData );
        PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
      } else {
        PPUBANK[ 2 ] = VROMPAGE( byData );
        PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      }
      InfoNES_SetupChr();
      break;
  
    case 0x7ef2:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 4 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 0 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;    
      
    case 0x7ef3:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 5 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 1 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    case 0x7ef4:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 6 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 2 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    case 0x7ef5:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 7 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 3 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    /* Name Table Mirroring */
    case 0x7ef6:
      Map82_Regs[ 0 ] = byData & 0x02;
      
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }

    /* Set ROM Banks */
    case 0x7efa:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x7efb:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x7efc:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 83 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map83_Regs[3];
DWORD Map83_Chr_Bank;
DWORD Map83_IRQ_Cnt;
BYTE Map83_IRQ_Enabled;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 83                                             */
/*-------------------------------------------------------------------*/
void Map83_Init()
{
  /* Initialize Mapper */
  MapperInit = Map83_Init;

  /* Write to Mapper */
  MapperWrite = Map83_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map83_Apu;

  /* Read from APU */
  MapperReadApu = Map83_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map83_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  Map83_Regs[0] = Map83_Regs[1] = Map83_Regs[2] = 0;

  /* Set ROM Banks */
  if ( ( NesHeader.byRomSize << 1 ) >= 32 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 30 );
    ROMBANK3 = ROMPAGE( 31 );
    Map83_Regs[ 1 ] = 0x30;
  }
  else
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map83_IRQ_Enabled = 0;
  Map83_IRQ_Cnt = 0;
  Map83_Chr_Bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
BYTE Map83_ReadApu( WORD wAddr )
{
  if ( ( wAddr & 0x5100 ) == 0x5100 )
  {
    return Map83_Regs[2];
  }
  else
  {
    return wAddr >> 8;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map83_Apu( WORD wAddr, BYTE byData )
{
  switch(wAddr)
  {
    case 0x5101:
    case 0x5102:
    case 0x5103:
      Map83_Regs[2] = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map83_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  switch( wAddr )
  {
    case 0x8000:
    case 0xB000:
    case 0xB0FF:
    case 0xB1FF:
      Map83_Regs[0] = byData;
      Map83_Chr_Bank = (byData & 0x30) << 4;

      ROMBANK0 = ROMPAGE( (byData*2+0) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (byData*2+1) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData&0x30)|0x0F)*2+0) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData&0x30)|0x0F)*2+1) % (NesHeader.byRomSize << 1) );
      break;

    case 0x8100:
      if ( NesHeader.byVRomSize <= 32 )
      {
	Map83_Regs[1] = byData;
      }
      if ((byData & 0x03) == 0x00)
      {
	InfoNES_Mirroring( 1 );
      }
      else if((byData & 0x03) == 0x01)
      {
	InfoNES_Mirroring( 0 );
      }
      else if((byData & 0x03) == 0x02)
      {
	InfoNES_Mirroring( 3 );
      }
      else
      {
	InfoNES_Mirroring( 2 );
      }
      break;

    case 0x8200:
      Map83_IRQ_Cnt = ( Map83_IRQ_Cnt & 0xFF00 ) | (DWORD)byData;
      break;

    case 0x8201:
      Map83_IRQ_Cnt = ( Map83_IRQ_Cnt & 0x00FF ) | ((DWORD)byData << 8);
      Map83_IRQ_Enabled = byData;
      break;

    case 0x8300:
      ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8301:
      ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8302:
      ROMBANK2 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8310:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 0 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      { 
	PPUBANK[ 0 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 1 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8311:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 1 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 2 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 3 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8312:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 2 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8313:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 3 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8314:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      { 
	PPUBANK[ 4 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8315:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 5 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8316:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 6 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8317:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 7 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ( (Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8318:
      ROMBANK0 = ROMPAGE( (((Map83_Regs[0]&0x30)|byData)*2+0) % (NesHeader.byRomSize << 1) );	
      ROMBANK1 = ROMPAGE( (((Map83_Regs[0]&0x30)|byData)*2+1) % (NesHeader.byRomSize << 1) );	
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map83_HSync()
{
  if (Map83_IRQ_Enabled)
  {
    if (Map83_IRQ_Cnt <= 114)
    {
      IRQ_REQ;
      Map83_IRQ_Enabled = 0;
    }
    else
    {
      Map83_IRQ_Cnt -= 114;
    }
  }
}
// Mapper 085: removed (needs more RAM than the RP2040 has)
/*===================================================================*/
/*                                                                   */
/*                        Mapper 86 (Jaleco)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 86                                             */
/*-------------------------------------------------------------------*/
void Map86_Init()
{
  /* Initialize Mapper */
  MapperInit = Map86_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map86_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 86 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map86_Sram( WORD wAddr, BYTE byData )
{
  BYTE byChrBank;
  BYTE byPrgBank;

  switch ( wAddr )
  {
    case 0x6000:
      byChrBank = byData & 0x03 | ( byData & 0x40 ) >> 4;
      byPrgBank = ( byData & 0x30 ) >> 4;

      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 87 (74161/32)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 87                                             */
/*-------------------------------------------------------------------*/
void Map87_Init()
{
  /* Initialize Mapper */
  MapperInit = Map87_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map87_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 87 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map87_Sram( WORD wAddr, BYTE byData )
{
  BYTE byChrBank;

  switch ( wAddr )
  {
    case 0x6000:
      byChrBank = ( byData & 0x02 ) >> 1;
      byChrBank <<= 3;
      byChrBank %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 88 (Namco 118)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map88_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 88                                             */
/*-------------------------------------------------------------------*/
void Map88_Init()
{
  /* Initialize Mapper */
  MapperInit = Map88_Init;

  /* Write to Mapper */
  MapperWrite = Map88_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 88 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map88_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map88_Regs[ 0 ] = byData;
      break;

    case 0x8001:
      switch ( Map88_Regs[ 0 ] & 0x07 )
      { 
        case 0x00:
          PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xfe ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xfe ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x01:
          PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xfe ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xfe ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x02:
          PPUBANK[ 4 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x03:
          PPUBANK[ 5 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x04:
          PPUBANK[ 6 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x05:
          PPUBANK[ 7 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x06:
          ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        case 0x07:
          ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;
      }
      break;

    case 0xc000:
      if ( byData ) 
      {
        InfoNES_Mirroring( 2 );
      } else {
        InfoNES_Mirroring( 3 );
      }
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 89 (Sunsoft)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 89                                             */
/*-------------------------------------------------------------------*/
void Map89_Init()
{
  /* Initialize Mapper */
  MapperInit = Map89_Init;

  /* Write to Mapper */
  MapperWrite = Map89_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );

    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 89 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map89_Write( WORD wAddr, BYTE byData )
{
  if ( ( wAddr & 0xFF00 ) == 0xC000 )
  {
    BYTE byPrgBank = (byData & 0x70) >> 4;
    BYTE byChrBank = ((byData & 0x80) >> 4) | (byData & 0x07);

    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE( (byPrgBank*2+0) % (NesHeader.byRomSize << 1) );
    ROMBANK1 = ROMPAGE( (byPrgBank*2+1) % (NesHeader.byRomSize << 1) );

    PPUBANK[ 0 ] = VROMPAGE( (byChrBank*8+0) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 1 ] = VROMPAGE( (byChrBank*8+1) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 2 ] = VROMPAGE( (byChrBank*8+2) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 3 ] = VROMPAGE( (byChrBank*8+3) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 4 ] = VROMPAGE( (byChrBank*8+4) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 5 ] = VROMPAGE( (byChrBank*8+5) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 6 ] = VROMPAGE( (byChrBank*8+6) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 7 ] = VROMPAGE( (byChrBank*8+7) % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();

    if ( byData & 0x08 )
    {
      InfoNES_Mirroring( 2 );
    }
    else
    {
      InfoNES_Mirroring( 3 );
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 90 (PC-JY-??)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map90_Prg_Reg[ 4 ];
BYTE Map90_Chr_Low_Reg[ 8 ];
BYTE Map90_Chr_High_Reg[ 8 ];
BYTE Map90_Nam_Low_Reg[ 4 ];
BYTE Map90_Nam_High_Reg[ 4 ];

BYTE Map90_Prg_Bank_Size;
BYTE Map90_Prg_Bank_6000;
BYTE Map90_Prg_Bank_E000;
BYTE Map90_Chr_Bank_Size;
BYTE Map90_Mirror_Mode;
BYTE Map90_Mirror_Type;

DWORD Map90_Value1;
DWORD Map90_Value2;

BYTE Map90_IRQ_Enable;
BYTE Map90_IRQ_Cnt;
BYTE Map90_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 90                                             */
/*-------------------------------------------------------------------*/
void Map90_Init()
{
  /* Initialize Mapper */
  MapperInit = Map90_Init;

  /* Write to Mapper */
  MapperWrite = Map90_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map90_Apu;

  /* Read from APU */
  MapperReadApu = Map90_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map90_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 3 );
  ROMBANK1 = ROMLASTPAGE( 2 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map90_IRQ_Cnt = 0;
  Map90_IRQ_Latch = 0;
  Map90_IRQ_Enable = 0;

  for ( BYTE byPage = 0; byPage < 4; byPage++ )
  {
    Map90_Prg_Reg[ byPage ] = ( NesHeader.byRomSize << 1 ) - 4 + byPage;
    Map90_Nam_Low_Reg[ byPage ] = 0;
    Map90_Nam_High_Reg[ byPage ] = 0;
    Map90_Chr_Low_Reg[ byPage ] = byPage;
    Map90_Chr_High_Reg[ byPage ] = 0;
    Map90_Chr_Low_Reg[ byPage + 4 ] = byPage + 4;
    Map90_Chr_High_Reg[ byPage + 4 ] = 0;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map90_Apu( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    case 0x5000:
      Map90_Value1 = byData;
      break;

    case 0x5001:
      Map90_Value2 = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
BYTE Map90_ReadApu( WORD wAddr )
{
  if ( wAddr == 0x5000 )
  {
    return ( BYTE )( ( Map90_Value1 * Map90_Value2 ) & 0x00ff );
  } else {
    return ( BYTE )( wAddr >> 8 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map90_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
      Map90_Prg_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Prg_Banks();
      break;
    
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0x9003:
    case 0x9004:
    case 0x9005:
    case 0x9006:
    case 0x9007:
      Map90_Chr_Low_Reg[ wAddr & 0x07 ] = byData;
      Map90_Sync_Chr_Banks();
      break;

    case 0xa000:
    case 0xa001:
    case 0xa002:
    case 0xa003:
    case 0xa004:
    case 0xa005:
    case 0xa006:
    case 0xa007:
      Map90_Chr_High_Reg[ wAddr & 0x07 ] = byData;
      Map90_Sync_Chr_Banks();
      break;

    case 0xb000:
    case 0xb001:
    case 0xb002:
    case 0xb003:
      Map90_Nam_Low_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Mirror();
      break;

    case 0xb004:
    case 0xb005:
    case 0xb006:
    case 0xb007:
      Map90_Nam_High_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Mirror();
      break;

    case 0xc002:
      Map90_IRQ_Enable = 0;
      break;

    case 0xc003:
    case 0xc004:
      if ( Map90_IRQ_Enable == 0 )
      {
        Map90_IRQ_Enable = 1;
        Map90_IRQ_Cnt = Map90_IRQ_Latch;
      }
      break;

    case 0xc005:
      Map90_IRQ_Cnt = byData;
      Map90_IRQ_Latch = byData;
      break;

    case 0xd000:
      Map90_Prg_Bank_6000 = byData & 0x80;
      Map90_Prg_Bank_E000 = byData & 0x04;
      Map90_Prg_Bank_Size = byData & 0x03;
      Map90_Chr_Bank_Size = ( byData & 0x18 ) >> 3;
      Map90_Mirror_Mode = byData & 0x20;
      
      Map90_Sync_Prg_Banks();
      Map90_Sync_Chr_Banks();
      Map90_Sync_Mirror();
      break;

    case 0xd001:
      Map90_Mirror_Type = byData & 0x03;
      Map90_Sync_Mirror();
      break;

    case 0xd003:
      /* Bank Page*/
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map90_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
  {
    if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
    {
      if ( Map90_IRQ_Cnt == 0 )
      {
        if ( Map90_IRQ_Enable )
        {
          IRQ_REQ;
        }
        Map90_IRQ_Latch = 0;
        Map90_IRQ_Enable = 0;
      } else {
        Map90_IRQ_Cnt--;
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Mirror Function                                   */
/*-------------------------------------------------------------------*/
void Map90_Sync_Mirror( void )
{
  BYTE byPage;
  DWORD dwNamBank[ 4 ];

  for ( byPage = 0; byPage < 4; byPage++ )
  {
    dwNamBank[ byPage ] = 
      ( (DWORD)Map90_Nam_High_Reg[ byPage ] << 8 ) | (DWORD)Map90_Nam_Low_Reg[ byPage ];
  }

  if ( Map90_Mirror_Mode )
  {
    for ( byPage = 0; byPage < 4; byPage++ )
    {
      if ( !Map90_Nam_High_Reg[ byPage ] && ( Map90_Nam_Low_Reg[ byPage ] == byPage ) )
      {
        Map90_Mirror_Mode = 0;
      }
    }

    if ( Map90_Mirror_Mode )
    {
        PPUBANK[ NAME_TABLE0 ] = VROMPAGE( dwNamBank[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE1 ] = VROMPAGE( dwNamBank[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE2 ] = VROMPAGE( dwNamBank[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE3 ] = VROMPAGE( dwNamBank[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
    }
  } else {
    switch ( Map90_Mirror_Type )
    {
      case 0x00:
        InfoNES_Mirroring( 1 );
        break;

      case 0x01:
        InfoNES_Mirroring( 0 );
        break;

      default:
        InfoNES_Mirroring( 3 );
        break;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Char Banks Function                               */
/*-------------------------------------------------------------------*/
void Map90_Sync_Chr_Banks( void )
{
  BYTE byPage;
  DWORD dwChrBank[ 8 ];

  for ( byPage = 0; byPage < 8; byPage++ )
  {
    dwChrBank[ byPage ] =
      ( (DWORD)Map90_Chr_High_Reg[ byPage ] << 8 ) | (DWORD)Map90_Chr_Low_Reg[ byPage ];
  }

  switch ( Map90_Chr_Bank_Size )
  {
    case 0:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 1:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 2:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 2 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 2 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 6 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 6 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    default:
      PPUBANK[ 0 ] = VROMPAGE( dwChrBank[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( dwChrBank[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( dwChrBank[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( dwChrBank[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( dwChrBank[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( dwChrBank[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( dwChrBank[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( dwChrBank[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Program Banks Function                            */
/*-------------------------------------------------------------------*/
void Map90_Sync_Prg_Banks( void )
{
  switch ( Map90_Prg_Bank_Size )
  {
    case 0:
      ROMBANK0 = ROMLASTPAGE( 3 );
      ROMBANK1 = ROMLASTPAGE( 2 );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );      
      break;

    case 1:
      ROMBANK0 = ROMPAGE( ( ( Map90_Prg_Reg[ 1 ] << 1 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( Map90_Prg_Reg[ 1 ] << 1 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );      
      break;

    case 2:
      if ( Map90_Prg_Bank_E000 )
      {
        ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );   
      } else {
        if ( Map90_Prg_Bank_6000 )
        {
          SRAMBANK = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );
        }
        ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMLASTPAGE( 0 );  
      }
      break;

    default:
      /* 8k in reverse mode? */
      ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) ); 
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 91 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 91                                             */
/*-------------------------------------------------------------------*/
void Map91_Init()
{
  /* Initialize Mapper */
  MapperInit = Map91_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map91_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 1 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 91 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map91_Sram( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF00F)
  {
    /* Set PPU Banks */
    case 0x6000:
      PPUBANK[ 0 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6001:
      PPUBANK[ 2 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6002:
      PPUBANK[ 4 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6003:
      PPUBANK[ 6 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    /* Set CPU Banks */
    case 0x7000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;

    case 0x7001:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*              Mapper 92 (Jaleco Early Mapper #1)                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 92                                             */
/*-------------------------------------------------------------------*/
void Map92_Init()
{
  /* Initialize Mapper */
  MapperInit = Map92_Init;

  /* Write to Mapper */
  MapperWrite = Map92_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 92 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map92_Write( WORD wAddr, BYTE byData )
{
  BYTE byRomBank;
  BYTE byChrBank;
  
  byData = (BYTE)wAddr & 0xff;
  byRomBank = ( byData & 0x0f ) << 1;
  byChrBank = byData & 0x0f;

  if ( wAddr >= 0x9000 )
  {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xd0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byRomBank );
        ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;
        
      /* Set PPU Banks */
      case 0xe0:
        byChrBank <<= 3;
        byChrBank %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 0 ] = VROMPAGE( byChrBank );
        PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
        PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
        PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
        PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
        PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
        InfoNES_SetupChr();
        break;
    }
  } else {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xb0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byRomBank );
        ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;

      /* Set PPU Banks */
      case 0x70:
        byChrBank <<= 3;
        byChrBank %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 0 ] = VROMPAGE( byChrBank );
        PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
        PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
        PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
        PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
        PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
        InfoNES_SetupChr();
        break;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 93 (74161/32)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 93                                             */
/*-------------------------------------------------------------------*/
void Map93_Init()
{
  /* Initialize Mapper */
  MapperInit = Map93_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map93_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 93 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map93_Sram( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  if ( wAddr == 0x6000 )
  {
    byData <<= 1;
    byData %= ( NesHeader.byRomSize << 1 );
  
    ROMBANK0 = ROMPAGE( byData );
    ROMBANK1 = ROMPAGE( byData + 1 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 94 (74161/32 Capcom)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 94                                             */
/*-------------------------------------------------------------------*/
void Map94_Init()
{
  /* Initialize Mapper */
  MapperInit = Map94_Init;

  /* Write to Mapper */
  MapperWrite = Map94_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 94 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map94_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xfff0 )
  {
    /* Set ROM Banks */
    case 0xff00:
      byData = ( byData & 0x1c ) >> 2;
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 95 (Namco 1??)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map95_Regs[ 1 ];
DWORD Map95_Prg0, Map95_Prg1;
DWORD Map95_Chr01, Map95_Chr23;
DWORD Map95_Chr4, Map95_Chr5, Map95_Chr6, Map95_Chr7;

#define Map95_Chr_Swap()    ( Map95_Regs[ 0 ] & 0x80 )
#define Map95_Prg_Swap()    ( Map95_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 95                                             */
/*-------------------------------------------------------------------*/
void Map95_Init()
{
  /* Initialize Mapper */
  MapperInit = Map95_Init;

  /* Write to Mapper */
  MapperWrite = Map95_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  Map95_Regs[ 0 ] = 0;

  /* Set ROM Banks */
  Map95_Prg0 = 0;
  Map95_Prg1 = 1;
  Map95_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map95_Chr01 = 0;
    Map95_Chr23 = 2;
    Map95_Chr4  = 4;
    Map95_Chr5  = 5;
    Map95_Chr6  = 6;
    Map95_Chr7  = 7;
    Map95_Set_PPU_Banks();
  } else {
    Map95_Chr01 = Map95_Chr23 = 0;
    Map95_Chr4 = Map95_Chr5 = Map95_Chr6 = Map95_Chr7 = 0;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map95_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map95_Regs[ 0 ] = byData;
      Map95_Set_PPU_Banks();
      Map95_Set_CPU_Banks();
      break;

    case 0x8001:
      if ( Map95_Regs[ 0 ] <= 0x05 )
      {
        if ( byData & 0x20 ) 
        {
          InfoNES_Mirroring( 2 );
        } else {
          InfoNES_Mirroring( 3 );
        }
        byData &= 0x1f;
      }

      dwBankNum = byData;

      switch ( Map95_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr01 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr23 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr4 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr5 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr6 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr7 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map95_Prg0 = dwBankNum;
          Map95_Set_CPU_Banks();
          break;

        case 0x07:
          Map95_Prg1 = dwBankNum;
          Map95_Set_CPU_Banks();
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_CPU_Banks()
{
  if ( Map95_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map95_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 96 : Bandai 74161                        */
/*                                                                   */
/*===================================================================*/

BYTE	Map96_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 96                                             */
/*-------------------------------------------------------------------*/
void Map96_Init()
{
  /* Initialize Mapper */
  MapperInit = Map96_Init;

  /* Write to Mapper */
  MapperWrite = Map96_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map96_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  Map96_Reg[0] = Map96_Reg[1] = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map96_Set_Banks();
  InfoNES_Mirroring( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map96_Write( WORD wAddr, BYTE byData )
{
  ROMBANK0 = ROMPAGE((((byData & 0x03)<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((byData & 0x03)<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((byData & 0x03)<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((byData & 0x03)<<2)+3) % (NesHeader.byRomSize<<1));
  
  Map96_Reg[0] = (byData & 0x04) >> 2;
  Map96_Set_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 PPU Function                                           */
/*-------------------------------------------------------------------*/
void Map96_PPU( WORD wAddr )
{
  if( (wAddr & 0xF000) == 0x2000 ) {
    Map96_Reg[1] = (wAddr>>8)&0x03;
    Map96_Set_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 Set Banks Function                                     */
/*-------------------------------------------------------------------*/
void Map96_Set_Banks()
{
  PPUBANK[ 0 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+0);
  PPUBANK[ 1 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+1);
  PPUBANK[ 2 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+2);
  PPUBANK[ 3 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+3);
  PPUBANK[ 4 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+0);
  PPUBANK[ 5 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+1);
  PPUBANK[ 6 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+2);
  PPUBANK[ 7 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+3);
  InfoNES_SetupChr();
}

/*===================================================================*/
/*                                                                   */
/*                  Mapper 97 (74161/32 Irem)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 97                                             */
/*-------------------------------------------------------------------*/
void Map97_Init()
{
  /* Initialize Mapper */
  MapperInit = Map97_Init;

  /* Write to Mapper */
  MapperWrite = Map97_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 1 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 97 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map97_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  if ( wAddr < 0xc000 )
  {
    BYTE byPrgBank = byData & 0x0f;

    byPrgBank <<= 1;
    byPrgBank %= ( NesHeader.byRomSize << 1 );

    ROMBANK2 = ROMPAGE( byPrgBank );
    ROMBANK3 = ROMPAGE( byPrgBank + 1 );

    if ( ( byData & 0x80 ) == 0 )
    {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 100 : Nestile MMC 3                      */
/*                                                                   */
/*===================================================================*/

BYTE	Map100_Reg[8];
BYTE	Map100_Prg0, Map100_Prg1, Map100_Prg2, Map100_Prg3;
BYTE	Map100_Chr0, Map100_Chr1, Map100_Chr2, Map100_Chr3; 
BYTE	Map100_Chr4, Map100_Chr5, Map100_Chr6, Map100_Chr7;

BYTE	Map100_IRQ_Enable;
BYTE	Map100_IRQ_Cnt;
BYTE	Map100_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 100                                            */
/*-------------------------------------------------------------------*/
void Map100_Init()
{
  /* Initialize Mapper */
  MapperInit = Map100_Init;

  /* Write to Mapper */
  MapperWrite = Map100_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map100_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map100_Chr0 = 0;
    Map100_Chr1 = 1;
    Map100_Chr2 = 2;
    Map100_Chr3 = 3;
    Map100_Chr4 = 4;
    Map100_Chr5 = 5;
    Map100_Chr6 = 6;
    Map100_Chr7 = 7;
    Map100_Set_PPU_Banks();
  } else {
    Map100_Chr0 = Map100_Chr2 = Map100_Chr4 = Map100_Chr5 = Map100_Chr6 = Map100_Chr7 = 0;
    Map100_Chr1 = Map100_Chr3 = 1;
  }

  /* Set IRQ Registers */
  Map100_IRQ_Enable = 0;
  Map100_IRQ_Cnt = 0;
  Map100_IRQ_Latch = 0;
  for( int i = 0; i < 8; i++ ) { Map100_Reg[ i ] = 0x00; }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map100_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr & 0xE001 )
  {
    case 0x8000:
      Map100_Reg[0] = byData;
      break;

    case 0x8001:
      Map100_Reg[1] = byData;

      switch ( Map100_Reg[0] & 0xC7 ) {
        case 0x00:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr0 = byData&0xFE;
	    Map100_Chr1 = Map100_Chr0+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;
	  
        case 0x01:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr2 = byData&0xFE;
	    Map100_Chr3 = Map100_Chr2+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x02:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr4 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x03:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr5 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x04:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr6 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x05:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr7 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x06:
	  Map100_Prg0 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x07:
	  Map100_Prg1 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x46:
	  Map100_Prg2 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x47:
	  Map100_Prg3 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x80:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr4 = byData&0xFE;
	    Map100_Chr5 = Map100_Chr4+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x81:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr6 = byData&0xFE;
	    Map100_Chr7 = Map100_Chr6+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x82:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr0 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x83:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr1 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x84:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr2 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x85:
	  if( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr3 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

      }
      break;

    case 0xA000:
      Map100_Reg[2] = byData;
      if ( !ROM_FourScr )
      {
	if( byData & 0x01 ) InfoNES_Mirroring( 0 );
	else		    InfoNES_Mirroring( 1 );
      }
      break;

    case 0xA001:
      Map100_Reg[3] = byData;
      break;

    case 0xC000:
      Map100_Reg[4] = byData;
      Map100_IRQ_Cnt = byData;
      break;

    case 0xC001:
      Map100_Reg[5] = byData;
      Map100_IRQ_Latch = byData;
      break;

    case 0xE000:
      Map100_Reg[6] = byData;
      Map100_IRQ_Enable = 0;
      break;

    case 0xE001:
      Map100_Reg[7] = byData;
      Map100_IRQ_Enable = 0xFF;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map100_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map100_IRQ_Enable ) {
	if( !(Map100_IRQ_Cnt--) ) {
	  Map100_IRQ_Cnt = Map100_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map100_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE( Map100_Prg0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( Map100_Prg1 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( Map100_Prg2 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( Map100_Prg3 % ( NesHeader.byRomSize << 1 ) );
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map100_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
      PPUBANK[ 0 ] = VROMPAGE( Map100_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map100_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map100_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map100_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map100_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map100_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map100_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map100_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
  }
}

/*===================================================================*/
/*                                                                   */
/*                   Mapper 101 ()                                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 101                                            */
/*-------------------------------------------------------------------*/
void Map101_Init()
{
  /* Initialize Mapper */
  MapperInit = Map101_Init;

  /* Write to Mapper */
  MapperWrite = Map101_Write;

  /* Write to SRAM */
  MapperSram = Map101_Write;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 101 Write & Write to SRAM Function                        */
/*-------------------------------------------------------------------*/
void Map101_Write( WORD wAddr, BYTE byData )
{
  byData &= 0x03;
  byData <<= 3;
  byData %= ( NesHeader.byVRomSize << 3 );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( byData );
  PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byData + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byData + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byData + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byData + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byData + 7 );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*        Mapper 105 : Nintendo World Championship                   */
/*                                                                   */
/*===================================================================*/

BYTE	Map105_Init_State;
BYTE	Map105_Write_Count;
BYTE	Map105_Bits;
BYTE	Map105_Reg[4];

BYTE	Map105_IRQ_Enable;
int	Map105_IRQ_Counter;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 105                                            */
/*-------------------------------------------------------------------*/
void Map105_Init()
{
  /* Initialize Mapper */
  MapperInit = Map105_Init;

  /* Write to Mapper */
  MapperWrite = Map105_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map105_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map105_Reg[0] = 0x0C;
  Map105_Reg[1] = 0x00;
  Map105_Reg[2] = 0x00;
  Map105_Reg[3] = 0x10;
  
  Map105_Bits = 0;
  Map105_Write_Count = 0;
  
  Map105_IRQ_Counter = 0;
  Map105_IRQ_Enable = 0;
  Map105_Init_State = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 105 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map105_Write( WORD wAddr, BYTE byData )
{
  WORD reg_num = (wAddr & 0x7FFF) >> 13;

  if( byData & 0x80 ) {
    Map105_Bits = Map105_Write_Count = 0;
    if( reg_num == 0 ) {
      Map105_Reg[reg_num] |= 0x0C;
    }
  } else {
    Map105_Bits |= (byData & 1) << Map105_Write_Count++;
    if( Map105_Write_Count == 5) {
      Map105_Reg[reg_num] = Map105_Bits & 0x1F;
      Map105_Bits = Map105_Write_Count = 0;
    }
  }

  if( Map105_Reg[0] & 0x02 ) {
    if( Map105_Reg[0] & 0x01 ) {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
  } else {
    if( Map105_Reg[0] & 0x01 ) {
      InfoNES_Mirroring( 3 );
    } else {
      InfoNES_Mirroring( 2 );
    }
  }
  
  switch( Map105_Init_State ) {
  case 0:
  case 1:
    Map105_Init_State++;
    break;

  case 2:
    if(Map105_Reg[1] & 0x08) {
      if (Map105_Reg[0] & 0x08) {
	if (Map105_Reg[0] & 0x04) {
	  ROMBANK0 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK1 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK2 = ROMPAGE( 30 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK3 = ROMPAGE( 31 % ( NesHeader.byRomSize << 1 ) );
	} else {
	  ROMBANK0 = ROMPAGE( 16 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK1 = ROMPAGE( 17 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK2 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK3 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	}
      } else {
	ROMBANK0 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK1 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK2 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 18) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK3 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 19) % ( NesHeader.byRomSize << 1 ) );
      }
    } else {
      ROMBANK0 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 3) % ( NesHeader.byRomSize << 1 ) );
    }
    
    if( Map105_Reg[1] & 0x10 ) {
      Map105_IRQ_Counter = 0;
      Map105_IRQ_Enable = 0;
    } else {
      Map105_IRQ_Enable = 1;
    }
    break;

  default:
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 105 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map105_HSync()
{
  if( !PPU_Scanline ) {
    if( Map105_IRQ_Enable ) {
      Map105_IRQ_Counter += 29781;
    }
    if( ((Map105_IRQ_Counter | 0x21FFFFFF) & 0x3E000000) == 0x3E000000 ) {
      IRQ_REQ;
    }
  }
}

/*===================================================================*/
/*                                                                   */
/*             Mapper 107 : Magic Dragon Mapper                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 107                                            */
/*-------------------------------------------------------------------*/
void Map107_Init()
{
  /* Initialize Mapper */
  MapperInit = Map107_Init;

  /* Write to Mapper */
  MapperWrite = Map107_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 107 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map107_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE( ( ((byData&0x07)<<3) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ( ((byData&0x07)<<3) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ( ((byData&0x07)<<3) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ( ((byData&0x07)<<3) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ( ((byData&0x07)<<3) + 4 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( ((byData&0x07)<<3) + 5 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ( ((byData&0x07)<<3) + 6 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( ((byData&0x07)<<3) + 7 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                            Mapper 108                             */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 108                                            */
/*-------------------------------------------------------------------*/
void Map108_Init()
{
  /* Initialize Mapper */
  MapperInit = Map108_Init;

  /* Write to Mapper */
  MapperWrite = Map108_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0xC % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( 0xD % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( 0xE % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( 0xF % ( NesHeader.byRomSize << 1 ) );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 108 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map108_Write( WORD wAddr, BYTE byData )
{
  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
/*===================================================================*/
/*                                                                   */
/*          Mapper 109 : SACHEN The Great Wall SA-019                */
/*                                                                   */
/*===================================================================*/

BYTE	Map109_Reg;
BYTE	Map109_Chr0, Map109_Chr1, Map109_Chr2, Map109_Chr3;
BYTE	Map109_Chrmode0, Map109_Chrmode1;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 109                                            */
/*-------------------------------------------------------------------*/
void Map109_Init()
{
  /* Initialize Mapper */
  MapperInit = Map109_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map109_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Initialize Registers */
  Map109_Reg = 0;
  Map109_Chr0 = 0;
  Map109_Chr1 = 0;
  Map109_Chr2 = 0;
  Map109_Chr3 = 0;
  Map109_Chrmode0 = 0;
  Map109_Chrmode1 = 0;

  /* Set PPU Banks */
  Map109_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 109 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map109_Apu( WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map109_Reg = byData;
    break;
  case	0x4101:
    switch( Map109_Reg ) {
    case 0:
      Map109_Chr0 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 1:
      Map109_Chr1 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 2:
      Map109_Chr2 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 3:
      Map109_Chr3 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 4:
      Map109_Chrmode0 = byData & 0x01;
      Map109_Set_PPU_Banks();
      break;
    case 5:
      ROMBANK0 = ROMPAGE( ( ( byData & 0x07 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( byData & 0x07 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ( ( byData & 0x07 ) + 2 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( ( byData & 0x07 ) + 3 ) % ( NesHeader.byRomSize << 1 ) );
      break;
    case 6:
      Map109_Chrmode1 = byData & 0x07;
      Map109_Set_PPU_Banks();
      break;
    case 7:
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 109 Set Bank PPU Function                                 */
/*-------------------------------------------------------------------*/
void Map109_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE((Map109_Chr0) % (NesHeader.byVRomSize<<3) );
    PPUBANK[ 1 ] = VROMPAGE((Map109_Chr1|((Map109_Chrmode1<<3)&0x8)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((Map109_Chr2|((Map109_Chrmode1<<2)&0x8)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((Map109_Chr3|((Map109_Chrmode1<<1)&0x8)|(Map109_Chrmode0*0x10)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((NesHeader.byVRomSize<<3)-4);
    PPUBANK[ 5 ] = VROMPAGE((NesHeader.byVRomSize<<3)-3);
    PPUBANK[ 6 ] = VROMPAGE((NesHeader.byVRomSize<<3)-2);
    PPUBANK[ 7 ] = VROMPAGE((NesHeader.byVRomSize<<3)-1);
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                          Mapper 110                               */
/*                                                                   */
/*===================================================================*/

BYTE	Map110_Reg0, Map110_Reg1;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 110                                            */
/*-------------------------------------------------------------------*/
void Map110_Init()
{
  /* Initialize Mapper */
  MapperInit = Map110_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map110_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize Registers */
  Map110_Reg0 = 0;
  Map110_Reg1 = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 110 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map110_Apu( WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map110_Reg1 = byData & 0x07;
    break;
  case	0x4101:
    switch( Map110_Reg1 ) {
    case	5:
      ROMBANK0 = ROMPAGE( ((byData << 2) + 0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ((byData << 2) + 1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ((byData << 2) + 2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ((byData << 2) + 3) % ( NesHeader.byRomSize << 1 ) );
      break;

    case	0:
      Map110_Reg0 = byData & 0x01;
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	2:
      Map110_Reg0 = byData;
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	4:
      Map110_Reg0 = Map110_Reg0 | (byData<<1);
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	6:
      Map110_Reg0 = Map110_Reg0 | (byData<<2);
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 112 (Pirates)                       */
/*                                                                   */
/*===================================================================*/

BYTE  Map112_Regs[8];
DWORD Map112_Prg0,Map112_Prg1;
DWORD Map112_Chr01,Map112_Chr23,Map112_Chr4,Map112_Chr5,Map112_Chr6,Map112_Chr7;

BYTE  Map112_IRQ_Enable;  /* IRQs enabled */
BYTE  Map112_IRQ_Cnt;     /* IRQ scanline counter, decreasing */
BYTE  Map112_IRQ_Latch;   /* IRQ scanline counter latch */

#define Map112_Chr_Swap()  ( Map112_Regs[0] & 0x80 )
#define Map112_Prg_Swap()  ( Map112_Regs[0] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 112                                            */
/*-------------------------------------------------------------------*/
void Map112_Init()
{
  /* Initialize Mapper */
  MapperInit = Map112_Init;

  /* Write to Mapper */
  MapperWrite = Map112_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map112_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* clear registers FIRST!!! */
  for(int i = 0; i < 8; i++) 
  {
    Map112_Regs[i] = 0x00;
  }

  /* set CPU bank pointers */
  Map112_Prg0 = 0;
  Map112_Prg1 = 1;
  Map112_Set_CPU_Banks();

  /* set VROM banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map112_Chr01 = 0;
    Map112_Chr23 = 2;
    Map112_Chr4  = 4;
    Map112_Chr5  = 5;
    Map112_Chr6  = 6;
    Map112_Chr7  = 7;
    Map112_Set_PPU_Banks();
  }
  else
  {
    Map112_Chr01 = Map112_Chr23 = Map112_Chr4 = Map112_Chr5 = Map112_Chr6 = Map112_Chr7 = 0;
  }

  Map112_IRQ_Enable = 0;
  Map112_IRQ_Cnt = 0;
  Map112_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map112_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  /* Set ROM Banks */
  switch( wAddr & 0xE001 )
  {
    case 0x8000:
      Map112_Regs[0] = byData;
      Map112_Set_PPU_Banks();
      Map112_Set_CPU_Banks();
      break;

    case 0xA000:
      Map112_Regs[1] = byData;
      dwBankNum = Map112_Regs[1];
      switch ( Map112_Regs[0] & 0x07 )
      {
        case 0x02:
	  if ( NesHeader.byVRomSize > 0 )
	  {
	    dwBankNum &= 0xfe;
	    Map112_Chr01 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x03:
	  if ( NesHeader.byVRomSize > 0 )
	  {
	    dwBankNum &= 0xfe;
	    Map112_Chr23 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x04:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr4 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x05:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr5 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x06:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr6 = dwBankNum;
	    Map112_Set_PPU_Banks();
          }
	  break;

        case 0x07:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr7 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x00:
	  Map112_Prg0 = dwBankNum;
	  Map112_Set_CPU_Banks();
	  break;

        case 0x01:
	  Map112_Prg1 = dwBankNum;
	  Map112_Set_CPU_Banks();
	  break;
      }
      break;

    case 0x8001:
      Map112_Regs[2] = byData;
      if ( !ROM_FourScr )
      {
	if ( byData & 0x01 )
        {
	  InfoNES_Mirroring( 1 );   /* Vertical */
	}
	else
        {
	  InfoNES_Mirroring( 0 );   /* Horizontal */
	}
      }
      break;

    case 0xA001:
      Map112_Regs[3] = byData;
      break;

    case 0xC000:
      Map112_Regs[4] = byData;
      Map112_IRQ_Cnt = Map112_Regs[4];
      break;

    case 0xC001:
      Map112_Regs[5] = byData;
      Map112_IRQ_Latch = Map112_Regs[5];
      break;

    case 0xE000:
      Map112_Regs[6] = byData;
      Map112_IRQ_Enable = 0;

      if ( byData )
      {
	InfoNES_Mirroring( 0 );   /* Horizontal */
      }
      else
      {
	InfoNES_Mirroring( 1 );   /* Vertical */
      }
      break;

    case 0xE001:
      Map112_Regs[7] = byData;
      Map112_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map112_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map112_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map112_IRQ_Cnt-- ) )
        {
          Map112_IRQ_Cnt = Map112_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map112_Set_CPU_Banks()
{
  /* Set ROM Banks */
  if ( Map112_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map112_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map112_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
  else
  {
    ROMBANK0 = ROMPAGE( Map112_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map112_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map112_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map112_Chr_Swap() )
    {
      PPUBANK[ 0 ] = VROMPAGE( Map112_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map112_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map112_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map112_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map112_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map112_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map112_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map112_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } 
    else 
    {
      PPUBANK[ 0 ] = VROMPAGE( ( Map112_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map112_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map112_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map112_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map112_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map112_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map112_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map112_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                    Mapper 113 (PC-Sachen/Hacker)                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 113                                            */
/*-------------------------------------------------------------------*/
void Map113_Init()
{
  /* Initialize Mapper */
  MapperInit = Map113_Init;

  /* Write to Mapper */
  MapperWrite = Map113_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map113_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map113_Apu( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x4100:
    case 0x4111:
    case 0x4120:
    case 0x4900:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map113_Write( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x8008:
    case 0x8009:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 114 (PC-SuperGames)                  */
/*                                                                   */
/*===================================================================*/

BYTE  Map114_Regs[ 8 ];
DWORD Map114_Prg0, Map114_Prg1;
DWORD Map114_Chr01, Map114_Chr23;
DWORD Map114_Chr4, Map114_Chr5, Map114_Chr6, Map114_Chr7;

BYTE Map114_IRQ_Enable;
BYTE Map114_IRQ_Cnt;
BYTE Map114_IRQ_Latch;

#define Map114_Chr_Swap()    ( Map114_Regs[ 0 ] & 0x80 )
#define Map114_Prg_Swap()    ( Map114_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 114                                            */
/*-------------------------------------------------------------------*/
void Map114_Init()
{
  /* Initialize Mapper */
  MapperInit = Map114_Init;

  /* Write to Mapper */
  MapperWrite = Map114_Write;

  /* Write to SRAM */
  MapperSram = Map114_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map114_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++)
  {
    Map114_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map114_Prg0 = 0;
  Map114_Prg1 = 1;
  Map114_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map114_Chr01 = 0;
    Map114_Chr23 = 2;
    Map114_Chr4  = 4;
    Map114_Chr5  = 5;
    Map114_Chr6  = 6;
    Map114_Chr7  = 7;
    Map114_Set_PPU_Banks();
  } else {
    Map114_Chr01 = Map114_Chr23 = 0;
    Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map114_IRQ_Enable = 0;
  Map114_IRQ_Cnt = 0;
  Map114_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map114_Sram( WORD wAddr, BYTE byData )
{
  if ( ( wAddr == 0x6000 ) && ( byData == 0x00 ) )
  {
    /* Initialize State Registers */
    for ( int nPage = 0; nPage < 8; nPage++)
    {
      Map114_Regs[ nPage ] = 0x00;
    }

    /* Set ROM Banks */
    Map114_Prg0 = 0;
    Map114_Prg1 = 1;
    Map114_Set_CPU_Banks();

    /* Set PPU Banks */
    if ( NesHeader.byVRomSize > 0 )
    { 
      Map114_Chr01 = 0;
      Map114_Chr23 = 2;
      Map114_Chr4  = 4;
      Map114_Chr5  = 5;
      Map114_Chr6  = 6;
      Map114_Chr7  = 7;
      Map114_Set_PPU_Banks();
    } else {
      Map114_Chr01 = Map114_Chr23 = 0;
      Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
    }

    /* Initialize IRQ Registers */
    Map114_IRQ_Enable = 0;
    Map114_IRQ_Cnt = 0;
    Map114_IRQ_Latch = 0;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map114_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map114_Regs[ 0 ] = byData;
      Map114_Set_PPU_Banks();
      Map114_Set_CPU_Banks();
      break;

    case 0x8001:
      Map114_Regs[ 1 ] = byData;
      dwBankNum = Map114_Regs[ 1 ];

      switch ( Map114_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr01 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr23 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr4 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr5 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr6 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr7 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map114_Prg0 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;

        case 0x07:
          Map114_Prg1 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map114_Regs[ 2 ] = byData;

      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map114_Regs[ 3 ] = byData;

      if ( byData & 0x80 )
      {
        /* Enable Save RAM $6000-$7fff */
      } else {
        /* Disable Save RAM $6000-$7fff */
      }
      break;

    case 0xc000:
      Map114_Regs[ 4 ] = byData;
      Map114_IRQ_Cnt = Map114_Regs[ 4 ];
      break;

    case 0xc001:
      Map114_Regs[ 5 ] = byData;
      Map114_IRQ_Latch = Map114_Regs[ 5 ];
      break;

    case 0xe000:
      Map114_Regs[ 6 ] = byData;
      Map114_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map114_Regs[ 7 ] = byData;
      Map114_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map114_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map114_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map114_IRQ_Cnt-- ) )
        {
          Map114_IRQ_Cnt = Map114_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_CPU_Banks()
{
  if ( Map114_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map114_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map114_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map114_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map114_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map114_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map114_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map114_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map114_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map114_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map114_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map114_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map114_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map114_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*   Mapper 115 : CartSaint : Yuu Yuu Hakusho Final JusticePao(?)    */
/*                                                                   */
/*===================================================================*/

BYTE	Map115_Reg[8];
BYTE	Map115_Prg0, Map115_Prg1, Map115_Prg2, Map115_Prg3;
BYTE	Map115_Prg0L, Map115_Prg1L;
BYTE	Map115_Chr0, Map115_Chr1, Map115_Chr2, Map115_Chr3;
BYTE    Map115_Chr4, Map115_Chr5, Map115_Chr6, Map115_Chr7;

BYTE	Map115_IRQ_Enable;
BYTE	Map115_IRQ_Counter;
BYTE	Map115_IRQ_Latch;

BYTE	Map115_ExPrgSwitch;
BYTE	Map115_ExChrSwitch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 115                                            */
/*-------------------------------------------------------------------*/
void Map115_Init()
{
  /* Initialize Mapper */
  MapperInit = Map115_Init;

  /* Write to Mapper */
  MapperWrite = Map115_Write;

  /* Write to SRAM */
  MapperSram = Map115_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map115_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map115_Reg[i] = 0x00;
  }

  Map115_Prg0 = Map115_Prg0L = 0;
  Map115_Prg1 = Map115_Prg1L = 1;
  Map115_Prg2 = ( NesHeader.byRomSize << 1 ) - 2;
  Map115_Prg3 = ( NesHeader.byRomSize << 1 ) - 1;

  Map115_ExPrgSwitch = 0;
  Map115_ExChrSwitch = 0;

  /* Set ROM Banks */
  Map115_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    Map115_Chr0 = 0;
    Map115_Chr1 = 1;
    Map115_Chr2 = 2;
    Map115_Chr3 = 3;
    Map115_Chr4 = 4;
    Map115_Chr5 = 5;
    Map115_Chr6 = 6;
    Map115_Chr7 = 7;
    Map115_Set_PPU_Banks();
  } else {
    Map115_Chr0 = Map115_Chr2 = Map115_Chr4 = Map115_Chr5 = Map115_Chr6 = Map115_Chr7 = 0;
    Map115_Chr1 = Map115_Chr3 = 1;
  }

  Map115_IRQ_Enable = 0;	/* Disable */
  Map115_IRQ_Counter = 0;
  Map115_IRQ_Latch = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map115_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map115_Reg[0] = byData;
    Map115_Set_CPU_Banks();
    Map115_Set_PPU_Banks();
    break;
  case	0x8001:
    Map115_Reg[1] = byData;
    switch( Map115_Reg[0] & 0x07 ) {
    case	0x00:
      Map115_Chr0 = byData & 0xFE;
      Map115_Chr1 = Map115_Chr0+1;
      Map115_Set_PPU_Banks();
      break;
    case	0x01:
      Map115_Chr2 = byData & 0xFE;
      Map115_Chr3 = Map115_Chr2+1;
      Map115_Set_PPU_Banks();
      break;
    case	0x02:
      Map115_Chr4 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x03:
      Map115_Chr5 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x04:
      Map115_Chr6 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x05:
      Map115_Chr7 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x06:
      Map115_Prg0 = Map115_Prg0L = byData;
      Map115_Set_CPU_Banks();
      break;
    case	0x07:
      Map115_Prg1 = Map115_Prg1L = byData;
      Map115_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map115_Reg[2] = byData;
    if ( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map115_Reg[3] = byData;
    break;
  case	0xC000:
    Map115_Reg[4] = byData;
    Map115_IRQ_Counter = byData;
    Map115_IRQ_Enable = 0xFF;
    break;
  case	0xC001:
    Map115_Reg[5] = byData;
    Map115_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map115_Reg[6] = byData;
    Map115_IRQ_Enable = 0;
    break;
  case	0xE001:
    Map115_Reg[7] = byData;
    Map115_IRQ_Enable = 0xFF;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map115_Sram( WORD wAddr, BYTE byData )
{
  switch ( wAddr ) {
  case	0x6000:
    Map115_ExPrgSwitch = byData;
    Map115_Set_CPU_Banks();
    break;
  case	0x6001:
    Map115_ExChrSwitch = byData&0x1;
    Map115_Set_PPU_Banks();
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map115_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map115_IRQ_Enable ) {
	if( !(Map115_IRQ_Counter--) ) {
	  Map115_IRQ_Counter = Map115_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map115_Set_CPU_Banks()
{
  if( Map115_ExPrgSwitch & 0x80 ) {
    Map115_Prg0 = ((Map115_ExPrgSwitch<<1)&0x1e);
    Map115_Prg1 = Map115_Prg0+1;

    ROMBANK0 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( Map115_Prg0+2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( Map115_Prg1+2 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    Map115_Prg0 = Map115_Prg0L;
    Map115_Prg1 = Map115_Prg1L;
    if( Map115_Reg[0] & 0x40 ) {
      ROMBANK0 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
      ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
    } else {
      ROMBANK0 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
      ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map115_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    if( Map115_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr7) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr3) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}

/*===================================================================*/
/*                                                                   */
/*   Mapper 116 : CartSaint : Yuu Yuu AV Kyoretsuden                 */
/*                                                                   */
/*===================================================================*/

BYTE	Map116_Reg[8];
BYTE	Map116_Prg0, Map116_Prg1, Map116_Prg2, Map116_Prg3;
BYTE	Map116_Prg0L, Map116_Prg1L;
BYTE	Map116_Chr0, Map116_Chr1, Map116_Chr2, Map116_Chr3;
BYTE    Map116_Chr4, Map116_Chr5, Map116_Chr6, Map116_Chr7;

BYTE	Map116_IRQ_Enable;
BYTE	Map116_IRQ_Counter;
BYTE	Map116_IRQ_Latch;

BYTE	Map116_ExPrgSwitch;
BYTE	Map116_ExChrSwitch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 116                                            */
/*-------------------------------------------------------------------*/
void Map116_Init()
{
  /* Initialize Mapper */
  MapperInit = Map116_Init;

  /* Write to Mapper */
  MapperWrite = Map116_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map116_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map116_Reg[i] = 0x00;
  }

  Map116_Prg0 = Map116_Prg0L = 0;
  Map116_Prg1 = Map116_Prg1L = 1;
  Map116_Prg2 = ( NesHeader.byRomSize << 1 ) - 2;
  Map116_Prg3 = ( NesHeader.byRomSize << 1 ) - 1;

  Map116_ExPrgSwitch = 0;
  Map116_ExChrSwitch = 0;

  /* Set ROM Banks */
  Map116_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    Map116_Chr0 = 0;
    Map116_Chr1 = 1;
    Map116_Chr2 = 2;
    Map116_Chr3 = 3;
    Map116_Chr4 = 4;
    Map116_Chr5 = 5;
    Map116_Chr6 = 6;
    Map116_Chr7 = 7;
    Map116_Set_PPU_Banks();
  } else {
    Map116_Chr0 = Map116_Chr2 = Map116_Chr4 = Map116_Chr5 = Map116_Chr6 = Map116_Chr7 = 0;
    Map116_Chr1 = Map116_Chr3 = 1;
  }

  Map116_IRQ_Enable = 0;	/* Disable */
  Map116_IRQ_Counter = 0;
  Map116_IRQ_Latch = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map116_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map116_Reg[0] = byData;
    Map116_Set_CPU_Banks();
    Map116_Set_PPU_Banks();
    break;
  case	0x8001:
    Map116_Reg[1] = byData;
    switch( Map116_Reg[0] & 0x07 ) {
    case	0x00:
      Map116_Chr0 = byData & 0xFE;
      Map116_Chr1 = Map116_Chr0+1;
      Map116_Set_PPU_Banks();
      break;
    case	0x01:
      Map116_Chr2 = byData & 0xFE;
      Map116_Chr3 = Map116_Chr2+1;
      Map116_Set_PPU_Banks();
      break;
    case	0x02:
      Map116_Chr4 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x03:
      Map116_Chr5 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x04:
      Map116_Chr6 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x05:
      Map116_Chr7 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x06:
      Map116_Prg0 = byData;
      Map116_Set_CPU_Banks();
      break;
    case	0x07:
      Map116_Prg1 = byData;
      Map116_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map116_Reg[2] = byData;
    if ( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map116_Reg[3] = byData;
    break;
  case	0xC000:
    Map116_Reg[4] = byData;
    Map116_IRQ_Counter = byData;
    Map116_IRQ_Enable = 0xFF;
    break;
  case	0xC001:
    Map116_Reg[5] = byData;
    Map116_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map116_Reg[6] = byData;
    Map116_IRQ_Enable = 0;
    break;
  case	0xE001:
    Map116_Reg[7] = byData;
    Map116_IRQ_Enable = 0xFF;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map116_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map116_IRQ_Enable ) {
	if( !(Map116_IRQ_Counter--) ) {
	  Map116_IRQ_Counter = Map116_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map116_Set_CPU_Banks()
{
  if( Map116_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
    ROMBANK1 = ROMPAGE( Map116_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map116_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
  } else {
    ROMBANK0 = ROMPAGE( Map116_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map116_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
    ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map116_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    if( Map116_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr7) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr3) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}

/*===================================================================*/
/*                                                                   */
/*                    Mapper 117 (PC-Future)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map117_IRQ_Line;
BYTE Map117_IRQ_Enable1;
BYTE Map117_IRQ_Enable2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 117                                            */
/*-------------------------------------------------------------------*/
void Map117_Init()
{
  /* Initialize Mapper */
  MapperInit = Map117_Init;

  /* Write to Mapper */
  MapperWrite = Map117_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map117_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map117_IRQ_Line = 0;
  Map117_IRQ_Enable1 = 0;
  Map117_IRQ_Enable2 = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map117_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
  
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    /* Set PPU Banks */
    case 0xA000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA004:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA005:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA006:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA007:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc002:
    case 0xc003:
      Map117_IRQ_Enable1 = Map117_IRQ_Line = byData;
      break;

    case 0xe000:
      Map117_IRQ_Enable2 = byData & 0x01;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map117_HSync()
{
  if ( Map117_IRQ_Enable1 && Map117_IRQ_Enable2 )
  {
    if ( Map117_IRQ_Line == PPU_Scanline )
    {
      Map117_IRQ_Enable1 = 0x00;
      IRQ_REQ;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 118 (Others)                         */
/*                                                                   */
/*===================================================================*/

BYTE  Map118_Regs[ 8 ];
DWORD Map118_Prg0, Map118_Prg1;
DWORD Map118_Chr0, Map118_Chr1, Map118_Chr2, Map118_Chr3;
DWORD Map118_Chr4, Map118_Chr5, Map118_Chr6, Map118_Chr7;

BYTE Map118_IRQ_Enable;
BYTE Map118_IRQ_Cnt;
BYTE Map118_IRQ_Latch;

#define Map118_Chr_Swap()    ( Map118_Regs[ 0 ] & 0x80 )
#define Map118_Prg_Swap()    ( Map118_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 118                                            */
/*-------------------------------------------------------------------*/
void Map118_Init()
{
  /* Initialize Mapper */
  MapperInit = Map118_Init;

  /* Write to Mapper */
  MapperWrite = Map118_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map118_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++)
  {
    Map118_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map118_Prg0 = 0;
  Map118_Prg1 = 1;
  Map118_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map118_Chr0 = 0;
    Map118_Chr1 = 1;
    Map118_Chr2 = 2;
    Map118_Chr3 = 3;
    Map118_Chr4 = 4;
    Map118_Chr5 = 5;
    Map118_Chr6 = 6;
    Map118_Chr7 = 7;
    Map118_Set_PPU_Banks();
  } else {
    Map118_Chr0 = Map118_Chr1 = Map118_Chr2 = Map118_Chr3 = 0;
    Map118_Chr4 = Map118_Chr5 = Map118_Chr6 = Map118_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map118_IRQ_Enable = 0;
  Map118_IRQ_Cnt = 0;
  Map118_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map118_Write( WORD wAddr, BYTE byData )
{
  DWORD dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map118_Regs[ 0 ] = byData;
      Map118_Set_PPU_Banks();
      Map118_Set_CPU_Banks();
      break;

    case 0x8001:
      Map118_Regs[ 1 ] = byData;
      dwBankNum = Map118_Regs[ 1 ];

      /* Name Table Mirroring */
      if ( ( Map118_Regs[ 0 ] & 0x07 ) < 6 )
      {
        if ( byData & 0x80 )
        {
          InfoNES_Mirroring( 3 );
        } else {
          InfoNES_Mirroring( 2 );
        }
      }

      switch ( Map118_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map118_Chr0 = dwBankNum;
            Map118_Chr1 = dwBankNum + 1;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map118_Chr2 = dwBankNum;
            Map118_Chr3 = dwBankNum + 1;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr4 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr5 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr6 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr7 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map118_Prg0 = dwBankNum;
          Map118_Set_CPU_Banks();
          break;

        case 0x07:
          Map118_Prg1 = dwBankNum;
          Map118_Set_CPU_Banks();
          break;
      }
      break;

    case 0xc000:
      Map118_Regs[ 4 ] = byData;
      Map118_IRQ_Cnt = Map118_Regs[ 4 ];
      break;

    case 0xc001:
      Map118_Regs[ 5 ] = byData;
      Map118_IRQ_Latch = Map118_Regs[ 5 ];
      break;

    case 0xe000:
      Map118_Regs[ 6 ] = byData;
      Map118_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map118_Regs[ 7 ] = byData;
      Map118_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map118_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map118_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map118_IRQ_Cnt-- ) )
        {
          Map118_IRQ_Cnt = Map118_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map118_Set_CPU_Banks()
{
  if ( Map118_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map118_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map118_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map118_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map118_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map118_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map118_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map118_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map118_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map118_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map118_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map118_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map118_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map118_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map118_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( Map118_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map118_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map118_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map118_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map118_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map118_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map118_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map118_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 119 : TQ-ROM                           */
/*                                                                   */
/*===================================================================*/

BYTE	Map119_Reg[8];
BYTE	Map119_Prg0, Map119_Prg1;
BYTE	Map119_Chr01, Map119_Chr23, Map119_Chr4, Map119_Chr5, Map119_Chr6, Map119_Chr7;
BYTE	Map119_WeSram;

BYTE	Map119_IRQ_Enable;
BYTE	Map119_IRQ_Counter;
BYTE	Map119_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 119                                            */
/*-------------------------------------------------------------------*/
void Map119_Init()
{
  /* Initialize Mapper */
  MapperInit = Map119_Init;

  /* Write to Mapper */
  MapperWrite = Map119_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map119_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */ 
  for( int i = 0; i < 8; i++ ) {
    Map119_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map119_Prg0 = 0;
  Map119_Prg1 = 1;
  Map119_Set_CPU_Banks();

  /* Set PPU Banks */
  Map119_Chr01 = 0;
  Map119_Chr23 = 2;
  Map119_Chr4  = 4;
  Map119_Chr5  = 5;
  Map119_Chr6  = 6;
  Map119_Chr7  = 7;
  Map119_Set_PPU_Banks();

  Map119_WeSram  = 0;		// Disable
  Map119_IRQ_Enable = 0;	// Disable
  Map119_IRQ_Counter = 0;
  Map119_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map119_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map119_Reg[0] = byData;
    Map119_Set_CPU_Banks();
    Map119_Set_PPU_Banks();
    break;
  case	0x8001:
    Map119_Reg[1] = byData;
    
    switch( Map119_Reg[0] & 0x07 ) {
    case	0x00:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr01 = byData & 0xFE;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x01:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr23 = byData & 0xFE;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x02:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr4 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x03:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr5 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x04:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr6 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x05:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr7 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x06:
      Map119_Prg0 = byData;
      Map119_Set_CPU_Banks();
      break;
    case	0x07:
      Map119_Prg1 = byData;
      Map119_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map119_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map119_Reg[3] = byData;
    break;
  case	0xC000:
    Map119_Reg[4] = byData;
    Map119_IRQ_Counter = byData;
    break;
  case	0xC001:
    Map119_Reg[5] = byData;
    Map119_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map119_Reg[6] = byData;
    Map119_IRQ_Enable = 0;
    Map119_IRQ_Counter = Map119_IRQ_Latch;
    break;
  case	0xE001:
    Map119_Reg[7] = byData;
    Map119_IRQ_Enable = 1;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map119_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map119_IRQ_Enable ) {
	if( !(Map119_IRQ_Counter--) ) {
	  Map119_IRQ_Counter = Map119_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map119_Set_CPU_Banks()
{
  if( Map119_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map119_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map119_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map119_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map119_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map119_Set_PPU_Banks()
{
  if( Map119_Reg[0]&0x80 ) {
    if(Map119_Chr4&0x40)	PPUBANK[ 0 ] = CRAMPAGE(Map119_Chr4&0x07);
    else			PPUBANK[ 0 ] = VROMPAGE(Map119_Chr4 % (NesHeader.byRomSize<<1));
    if(Map119_Chr5&0x40)	PPUBANK[ 1 ] = CRAMPAGE(Map119_Chr5&0x07);
    else			PPUBANK[ 1 ] = VROMPAGE(Map119_Chr5 % (NesHeader.byRomSize<<1));
    if(Map119_Chr6&0x40)	PPUBANK[ 2 ] = CRAMPAGE(Map119_Chr6&0x07);
    else			PPUBANK[ 2 ] = VROMPAGE(Map119_Chr6 % (NesHeader.byRomSize<<1));
    if(Map119_Chr7&0x40)	PPUBANK[ 3 ] = CRAMPAGE(Map119_Chr7&0x07);
    else			PPUBANK[ 3 ] = VROMPAGE(Map119_Chr7 % (NesHeader.byRomSize<<1));
    
    if((Map119_Chr01+0)&0x40)   PPUBANK[ 4 ] = CRAMPAGE((Map119_Chr01+0)&0x07);
    else		        PPUBANK[ 4 ] = VROMPAGE((Map119_Chr01+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr01+1)&0x40)   PPUBANK[ 5 ] = CRAMPAGE((Map119_Chr01+1)&0x07);
    else		     	PPUBANK[ 5 ] = VROMPAGE((Map119_Chr01+1) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+0)&0x40)   PPUBANK[ 6 ] = CRAMPAGE((Map119_Chr23+0)&0x07);
    else			PPUBANK[ 6 ] = VROMPAGE((Map119_Chr23+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+1)&0x40)   PPUBANK[ 7 ] = CRAMPAGE((Map119_Chr23+1)&0x07);
    else			PPUBANK[ 7 ] = VROMPAGE((Map119_Chr23+1) % (NesHeader.byVRomSize<<3));
  } else {
    if((Map119_Chr01+0)&0x40)   PPUBANK[ 0 ] = CRAMPAGE((Map119_Chr01+0)&0x07);
    else			PPUBANK[ 0 ] = VROMPAGE((Map119_Chr01+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr01+1)&0x40)   PPUBANK[ 1 ] = CRAMPAGE((Map119_Chr01+1)&0x07);
    else			PPUBANK[ 1 ] = VROMPAGE((Map119_Chr01+1) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+0)&0x40)   PPUBANK[ 2 ] = CRAMPAGE((Map119_Chr23+0)&0x07);
    else		     	PPUBANK[ 2 ] = VROMPAGE((Map119_Chr23+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+1)&0x40)   PPUBANK[ 3 ] = CRAMPAGE((Map119_Chr23+1)&0x07);
    else		     	PPUBANK[ 3 ] = VROMPAGE((Map119_Chr23+1) % (NesHeader.byVRomSize<<3));
    
    if(Map119_Chr4&0x40)	PPUBANK[ 4 ] = CRAMPAGE(Map119_Chr4&0x07);
    else			PPUBANK[ 4 ] = VROMPAGE(Map119_Chr4 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr5&0x40)	PPUBANK[ 5 ] = CRAMPAGE(Map119_Chr5&0x07);
    else			PPUBANK[ 5 ] = VROMPAGE(Map119_Chr5 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr6&0x40)	PPUBANK[ 6 ] = CRAMPAGE(Map119_Chr6&0x07);
    else			PPUBANK[ 6 ] = VROMPAGE(Map119_Chr6 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr7&0x40)	PPUBANK[ 7 ] = CRAMPAGE(Map119_Chr7&0x07);
    else			PPUBANK[ 7 ] = VROMPAGE(Map119_Chr7 % (NesHeader.byVRomSize<<3));
  }
  InfoNES_SetupChr();
}

/*===================================================================*/
/*                                                                   */
/*                      Mapper 122 (Sunsoft)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 122                                            */
/*-------------------------------------------------------------------*/
void Map122_Init()
{
  /* Initialize Mapper */
  MapperInit = Map122_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map122_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 122 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map122_Sram( WORD wAddr, BYTE byData )
{
  if ( wAddr == 0x6000 )
  {
    BYTE byChrBank0 = byData & 0x07;
    BYTE byChrBank1 = ( byData & 0x70 ) >> 4;

    byChrBank0 = ( byChrBank0 << 2 ) % ( NesHeader.byVRomSize << 3 );
    byChrBank1 = ( byChrBank1 << 2 ) % ( NesHeader.byVRomSize << 3 );

    PPUBANK[ 0 ] = VROMPAGE( byChrBank0 + 0 );
    PPUBANK[ 1 ] = VROMPAGE( byChrBank0 + 1 );
    PPUBANK[ 2 ] = VROMPAGE( byChrBank0 + 2 );
    PPUBANK[ 3 ] = VROMPAGE( byChrBank0 + 3 );
    PPUBANK[ 4 ] = VROMPAGE( byChrBank1 + 0 );
    PPUBANK[ 5 ] = VROMPAGE( byChrBank1 + 1 );
    PPUBANK[ 6 ] = VROMPAGE( byChrBank1 + 2 );
    PPUBANK[ 7 ] = VROMPAGE( byChrBank1 + 3 );
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 133 : SACHEN CHEN                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 133                                            */
/*-------------------------------------------------------------------*/
void Map133_Init()
{
  /* Initialize Mapper */
  MapperInit = Map133_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map133_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 133 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map133_Apu( WORD wAddr, BYTE byData )
{
  if ( wAddr == 0x4120 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE( ((byData&0x04) + 0 ) % (NesHeader.byRomSize << 1) );
    ROMBANK1 = ROMPAGE( ((byData&0x04) + 1 ) % (NesHeader.byRomSize << 1) );
    ROMBANK2 = ROMPAGE( ((byData&0x04) + 2 ) % (NesHeader.byRomSize << 1) );
    ROMBANK3 = ROMPAGE( ((byData&0x04) + 3 ) % (NesHeader.byRomSize << 1) );
  
    /* Set PPU Banks */
    PPUBANK[ 0 ] = VROMPAGE( (((byData&0x03)<<3) + 0) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 1 ] = VROMPAGE( (((byData&0x03)<<3) + 1) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 2 ] = VROMPAGE( (((byData&0x03)<<3) + 2) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 3 ] = VROMPAGE( (((byData&0x03)<<3) + 3) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 4 ] = VROMPAGE( (((byData&0x03)<<3) + 4) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 5 ] = VROMPAGE( (((byData&0x03)<<3) + 5) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 6 ] = VROMPAGE( (((byData&0x03)<<3) + 6) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 7 ] = VROMPAGE( (((byData&0x03)<<3) + 7) % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
  }
  //Map133_Wram[ wAddr & 0x1fff ] = byData;
}
/*===================================================================*/
/*                                                                   */
/*                          Mapper 134                               */
/*                                                                   */
/*===================================================================*/

BYTE    Map134_Cmd, Map134_Prg, Map134_Chr;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 134                                            */
/*-------------------------------------------------------------------*/
void Map134_Init()
{
  /* Initialize Mapper */
  MapperInit = Map134_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map134_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 134 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map134_Apu( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0x4101 ) {
  case	0x4100:
    Map134_Cmd = byData & 0x07;
    break;
  case	0x4101:
    switch( Map134_Cmd ) {
    case 0:	
      Map134_Prg = 0;
      Map134_Chr = 3;
      break;
    case 4:
      Map134_Chr &= 0x3;
      Map134_Chr |= (byData & 0x07) << 2;
      break;
    case 5:
      Map134_Prg = byData & 0x07;
      break;
    case 6:
      Map134_Chr &= 0x1C;
      Map134_Chr |= byData & 0x3;
      break;
    case 7:
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ((Map134_Prg<<2) + 0 ) % (NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( ((Map134_Prg<<2) + 1 ) % (NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( ((Map134_Prg<<2) + 2 ) % (NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( ((Map134_Prg<<2) + 3 ) % (NesHeader.byRomSize << 1) );
  
  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ((Map134_Chr<<3) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( ((Map134_Chr<<3) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( ((Map134_Chr<<3) + 2) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( ((Map134_Chr<<3) + 3) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( ((Map134_Chr<<3) + 4) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( ((Map134_Chr<<3) + 5) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( ((Map134_Chr<<3) + 6) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( ((Map134_Chr<<3) + 7) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();

  //Map134_Wram[ wAddr & 0x1fff ] = byData;
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 135 : SACHEN CHEN                        */
/*                                                                   */
/*===================================================================*/

BYTE    Map135_Cmd;
BYTE	Map135_Chr0l, Map135_Chr1l, Map135_Chr0h, Map135_Chr1h, Map135_Chrch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 135                                            */
/*-------------------------------------------------------------------*/
void Map135_Init()
{
  /* Initialize Mapper */
  MapperInit = Map135_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map135_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  Map135_Cmd = 0;
  Map135_Chr0l = Map135_Chr1l = Map135_Chr0h = Map135_Chr1h = Map135_Chrch = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map135_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map135_Apu( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0x4101 ) {
  case	0x4100:
    Map135_Cmd = byData & 0x07;
    break;
  case	0x4101:
    switch( Map135_Cmd ) {
    case	0:
      Map135_Chr0l = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	1:
      Map135_Chr0h = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	2:
      Map135_Chr1l = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	3:
      Map135_Chr1h = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	4:
      Map135_Chrch = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	5:
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( (((byData%0x07)<<2) + 0 ) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (((byData%0x07)<<2) + 1 ) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData%0x07)<<2) + 2 ) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData%0x07)<<2) + 3 ) % (NesHeader.byRomSize << 1) );
      break;
    case	6:
      break;
    case	7:
      switch( (byData>>1)&0x03 ) {
      case	0: InfoNES_Mirroring( 2 ); break;
      case	1: InfoNES_Mirroring( 0  ); break;
      case	2: InfoNES_Mirroring( 1  ); break;
      case	3: InfoNES_Mirroring( 2 ); break;
      }
      break;
    }
    break;
  }
  //Map135_Wram[ wAddr & 0x1fff ] = byData;
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map135_Set_PPU_Banks()
{
  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                          Mapper 140                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 140                                            */
/*-------------------------------------------------------------------*/
void Map140_Init()
{
  /* Initialize Mapper */
  MapperInit = Map0_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map140_Sram;

  /* Write to APU */
  MapperApu = Map140_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 140 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map140_Sram( WORD wAddr, BYTE byData )
{
  Map140_Apu( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 140 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map140_Apu( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( (((byData&0xF0)>>2) + 0 ) % (NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( (((byData%0xF0)>>2) + 1 ) % (NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( (((byData%0xF0)>>2) + 2 ) % (NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( (((byData%0xF0)>>2) + 3 ) % (NesHeader.byRomSize << 1) );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( (((byData&0x0F)<<3) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( (((byData&0x0F)<<3) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( (((byData&0x0F)<<3) + 2) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( (((byData&0x0F)<<3) + 3) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( (((byData&0x0F)<<3) + 4) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( (((byData&0x0F)<<3) + 5) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( (((byData&0x0F)<<3) + 6) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( (((byData&0x0F)<<3) + 7) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 151 (VSUnisystem)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 151                                            */
/*-------------------------------------------------------------------*/
void Map151_Init()
{
  /* Initialize Mapper */
  MapperInit = Map151_Init;

  /* Write to Mapper */
  MapperWrite = Map151_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 151 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map151_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  switch( wAddr & 0xF000 )
  {
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xA000:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xC000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xE000:
      PPUBANK[ 0 ] = VROMPAGE( ( byData*4+0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( byData*4+1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( byData*4+2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( byData*4+3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xF000:
      PPUBANK[ 4 ] = VROMPAGE( ( byData*4+0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( byData*4+1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( byData*4+2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( byData*4+3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                        Mapper 160 (Pirates)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map160_IRQ_Enable;
BYTE Map160_IRQ_Cnt;
BYTE Map160_IRQ_Latch;
BYTE Map160_Refresh_Type;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 160                                            */
/*-------------------------------------------------------------------*/
void Map160_Init()
{
  /* Initialize Mapper */
  MapperInit = Map160_Init;

  /* Write to Mapper */
  MapperWrite = Map160_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map160_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize IRQ Registers */
  Map160_IRQ_Enable = 0;
  Map160_IRQ_Cnt = 0;
  Map160_IRQ_Latch = 0;
  Map160_Refresh_Type = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map160_Write( WORD wAddr, BYTE byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8002:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9000:
      if ( byData == 0x2b )
      {
        Map160_Refresh_Type = 1;      /* Title */
      }
      else if ( byData == 0xa8 )
      {
        Map160_Refresh_Type = 2;      /* Passwd */
      }
      else if ( byData == 0x1f )
      {
        Map160_Refresh_Type = 3;      /* Game Over */
      }
      else if ( byData == 0x7c )
      {
        Map160_Refresh_Type = 4;      /* Continue */
      }
      else if ( byData == 0x18 )
      {
        Map160_Refresh_Type = 5;      /* Roulette */
      }
      else if ( byData == 0x60 )
      {
        Map160_Refresh_Type = 6;      /* Congratulation */
      }     
      else
      {
        Map160_Refresh_Type = 0;   
      }

      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9002:
      if ( Map160_Refresh_Type == 2 && byData != 0xe8 )
      {
        Map160_Refresh_Type = 0;     /* Not Passwd */
      }
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9004:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  
    case 0x9005:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9006:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9007:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;      

    case 0xc000:
      Map160_IRQ_Cnt = Map160_IRQ_Latch;
      Map160_IRQ_Enable = Map160_IRQ_Latch;
      break;

    case 0xc001:
      Map160_IRQ_Latch = byData;
      break;

    case 0xc002:
      Map160_IRQ_Enable = 0;
      break;

    case 0xc003:
      Map160_IRQ_Cnt = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map160_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( PPU_Scanline == 0 || PPU_Scanline == 239 )
  {
    if ( Map160_Refresh_Type == 1 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 2 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 3 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x70 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x71 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x72 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x73 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 6 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x5c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x5d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 64 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x6c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x6d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x6e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x6f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 128 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x68 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x69 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x6a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x6b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 160 )
  {
    if ( Map160_Refresh_Type == 6 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x60 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x61 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( Map160_IRQ_Enable )
  {
    if ( Map160_IRQ_Cnt == 0xff )
    {
      IRQ_REQ;
      Map160_IRQ_Cnt = 0;
      Map160_IRQ_Enable = 0;
    } else {
      Map160_IRQ_Cnt++;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 180  (Nichibutsu)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 180                                            */
/*-------------------------------------------------------------------*/
void Map180_Init()
{
  /* Initialize Mapper */
  MapperInit = Map180_Init;

  /* Write to Mapper */
  MapperWrite = Map180_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 180 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map180_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */ 
  byData &= 0x07;
  byData <<= 1;
  byData %= ( NesHeader.byRomSize << 1 );
  ROMBANK2 = ROMPAGE( byData );
  ROMBANK3 = ROMPAGE( byData + 1 );
}
/*===================================================================*/
/*                                                                   */
/*            Mapper 181 : Hacker International Type2                */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 181                                            */
/*-------------------------------------------------------------------*/
void Map181_Init()
{
  /* Initialize Mapper */
  MapperInit = Map181_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map181_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 181 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map181_Apu( WORD wAddr, BYTE byData )
{
  if( wAddr == 0x4120 ) {
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( (((byData&0x08)>>1) + 0 ) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (((byData&0x08)>>1) + 1 ) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData&0x08)>>1) + 2 ) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData&0x08)>>1) + 3 ) % (NesHeader.byRomSize << 1) );

      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( (((byData&0x07)<<3) + 0) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 1 ] = VROMPAGE( (((byData&0x07)<<3) + 1) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 2 ] = VROMPAGE( (((byData&0x07)<<3) + 2) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 3 ] = VROMPAGE( (((byData&0x07)<<3) + 3) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 4 ] = VROMPAGE( (((byData&0x07)<<3) + 4) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 5 ] = VROMPAGE( (((byData&0x07)<<3) + 5) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 6 ] = VROMPAGE( (((byData&0x07)<<3) + 6) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 7 ] = VROMPAGE( (((byData&0x07)<<3) + 7) % (NesHeader.byVRomSize << 3) );
      InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 182  (Pirates)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map182_Regs[1];
BYTE Map182_IRQ_Enable;
BYTE Map182_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 182                                            */
/*-------------------------------------------------------------------*/
void Map182_Init()
{
  /* Initialize Mapper */
  MapperInit = Map182_Init;

  /* Write to Mapper */
  MapperWrite = Map182_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map182_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize IRQ registers */
  Map182_Regs[0] = 0;
  Map182_IRQ_Enable = 0;
  Map182_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map182_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF003 )
  {
    /* Name Table Mirroring */
    case 0x8001:
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 0 );   /* Horizontal */
      }
      else
      {
        InfoNES_Mirroring( 1 );   /* Vertical */
      }
      break;

    case 0xA000:
      Map182_Regs[0] = byData & 0x07;
      break;

    case 0xC000:
      switch( Map182_Regs[0] )
      { 
        /* Set PPU Banks */ 
        case 0x00:
          PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x01:
          PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x02:
          PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x03:
          PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        /* Set ROM Banks */ 
        case 0x04:
          ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        case 0x05:
          ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        /* Set PPU Banks */ 
        case 0x06:
          PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x07:
          PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;
      }
      break;

   case 0xE003:
     Map182_IRQ_Cnt = byData;
     Map182_IRQ_Enable = byData;
     break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map182_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map182_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 240 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map182_IRQ_Cnt ) )
        {
          Map182_IRQ_Cnt = 0;
          Map182_IRQ_Enable = 0;
          IRQ_REQ;
        }
      }
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                 Mapper 183 : Gimmick (Bootleg)                    */
/*                                                                   */
/*===================================================================*/

BYTE	Map183_Reg[8];
BYTE	Map183_IRQ_Enable;
int	Map183_IRQ_Counter;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 183                                            */
/*-------------------------------------------------------------------*/
void Map183_Init()
{
  /* Initialize Mapper */
  MapperInit = Map183_Init;

  /* Write to Mapper */
  MapperWrite = Map183_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map183_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map183_Reg[i] = i;
  }
  Map183_IRQ_Enable = 0;
  Map183_IRQ_Counter = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 183 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map183_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x8800:
    ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
  case	0xA800:
    ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
  case	0xA000:
    ROMBANK2 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
    
  case	0xB000:
    Map183_Reg[0] = (Map183_Reg[0]&0xF0)|(byData&0x0F);
    PPUBANK[ 0 ] = VROMPAGE( Map183_Reg[0] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB004:
    Map183_Reg[0] = (Map183_Reg[0]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 0 ] = VROMPAGE( Map183_Reg[0] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB008:
    Map183_Reg[1] = (Map183_Reg[1]&0xF0)|(byData&0x0F);
    PPUBANK[ 1 ] = VROMPAGE( Map183_Reg[1] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB00C:
    Map183_Reg[1] = (Map183_Reg[1]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 1 ] = VROMPAGE( Map183_Reg[1] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xC000:
    Map183_Reg[2] = (Map183_Reg[2]&0xF0)|(byData&0x0F);
    PPUBANK[ 2 ] = VROMPAGE( Map183_Reg[2] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC004:
    Map183_Reg[2] = (Map183_Reg[2]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 2 ] = VROMPAGE( Map183_Reg[2] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC008:
    Map183_Reg[3] = (Map183_Reg[3]&0xF0)|(byData&0x0F);
    PPUBANK[ 3 ] = VROMPAGE( Map183_Reg[3] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC00C:
    Map183_Reg[3] = (Map183_Reg[3]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 3 ] = VROMPAGE( Map183_Reg[3] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xD000:
    Map183_Reg[4] = (Map183_Reg[4]&0xF0)|(byData&0x0F);
    PPUBANK[ 4 ] = VROMPAGE( Map183_Reg[4] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD004:
    Map183_Reg[4] = (Map183_Reg[4]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 4 ] = VROMPAGE( Map183_Reg[4] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD008:
    Map183_Reg[5] = (Map183_Reg[5]&0xF0)|(byData&0x0F);
    PPUBANK[ 5 ] = VROMPAGE( Map183_Reg[5] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD00C:
    Map183_Reg[5] = (Map183_Reg[5]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 5 ] = VROMPAGE( Map183_Reg[5] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xE000:
    Map183_Reg[6] = (Map183_Reg[6]&0xF0)|(byData&0x0F);
    PPUBANK[ 6 ] = VROMPAGE( Map183_Reg[6] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE004:
    Map183_Reg[6] = (Map183_Reg[6]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 6 ] = VROMPAGE( Map183_Reg[6] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE008:
    Map183_Reg[7] = (Map183_Reg[3]&0xF0)|(byData&0x0F);
    PPUBANK[ 7 ] = VROMPAGE( Map183_Reg[7] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE00C:
    Map183_Reg[7] = (Map183_Reg[3]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 7 ] = VROMPAGE( Map183_Reg[7] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0x9008:
    if( byData == 1 ) {
      for( int i = 0; i < 8; i++ ) {
	Map183_Reg[i] = i;
      }
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( 0 );
      ROMBANK1 = ROMPAGE( 1 );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );

      /* Set PPU Banks */
      if ( NesHeader.byVRomSize > 0 ) {
	PPUBANK[ 0 ] = VROMPAGE( 0 );
	PPUBANK[ 1 ] = VROMPAGE( 1 );
	PPUBANK[ 2 ] = VROMPAGE( 2 );
	PPUBANK[ 3 ] = VROMPAGE( 3 );
	PPUBANK[ 4 ] = VROMPAGE( 4 );
	PPUBANK[ 5 ] = VROMPAGE( 5 );
	PPUBANK[ 6 ] = VROMPAGE( 6 );
	PPUBANK[ 7 ] = VROMPAGE( 7 );
	InfoNES_SetupChr();
      }
    }
    break;
    
  case	0x9800:
    if( byData == 0 )      InfoNES_Mirroring( 1 );
    else if( byData == 1 ) InfoNES_Mirroring( 0 );
    else if( byData == 2 ) InfoNES_Mirroring( 2 );
    else if( byData == 3 ) InfoNES_Mirroring( 3 );
    break;
    
  case	0xF000:
    Map183_IRQ_Counter = (Map183_IRQ_Counter&0xFF00)|byData;
    break;
  case	0xF004:
    Map183_IRQ_Counter = (Map183_IRQ_Counter&0x00FF)|(byData<<8);
    break;
  case	0xF008:
    Map183_IRQ_Enable = byData;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 183 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map183_HSync()
{
  if( Map183_IRQ_Enable & 0x02 ) {
    if( Map183_IRQ_Counter <= 113 ) {
      Map183_IRQ_Counter = 0;
      IRQ_REQ;
    } else {
      Map183_IRQ_Counter -= 113;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                         Mapper 185  (Tecmo)                       */
/*                                                                   */
/*===================================================================*/

BYTE Map185_Dummy_Chr_Rom[ 0x400 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 185                                            */
/*-------------------------------------------------------------------*/
void Map185_Init()
{
  /* Initialize Mapper */
  MapperInit = Map185_Init;

  /* Write to Mapper */
  MapperWrite = Map185_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Initialize Dummy VROM */
  for ( int nPage = 0; nPage < 0x400; nPage++ )
  {
    Map185_Dummy_Chr_Rom[ nPage ] = 0xff;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 185 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map185_Write( WORD wAddr, BYTE byData )
{
  /* Set PPU Banks */ 
  if ( byData & 0x03 )
  {
    PPUBANK[ 0 ] = VROMPAGE( 0 );
    PPUBANK[ 1 ] = VROMPAGE( 1 );
    PPUBANK[ 2 ] = VROMPAGE( 2 );
    PPUBANK[ 3 ] = VROMPAGE( 3 );
    PPUBANK[ 4 ] = VROMPAGE( 4 );
    PPUBANK[ 5 ] = VROMPAGE( 5 );
    PPUBANK[ 6 ] = VROMPAGE( 6 );
    PPUBANK[ 7 ] = VROMPAGE( 7 );
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 1 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 2 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 3 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 4 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 5 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 6 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 7 ] = Map185_Dummy_Chr_Rom;
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*            Mapper 187 : Street Fighter Zero 2 97                  */
/*                                                                   */
/*===================================================================*/

BYTE	Map187_Prg[4];
int	Map187_Chr[8];
BYTE	Map187_Bank[8];

BYTE	Map187_ExtMode;
BYTE	Map187_ChrMode;
BYTE	Map187_ExtEnable;

BYTE	Map187_IRQ_Enable;
BYTE	Map187_IRQ_Counter;
BYTE	Map187_IRQ_Latch;
BYTE	Map187_IRQ_Occur;
BYTE	Map187_LastWrite;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 187                                            */
/*-------------------------------------------------------------------*/
void Map187_Init()
{
  /* Initialize Mapper */
  MapperInit = Map187_Init;

  /* Write to Mapper */
  MapperWrite = Map187_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map187_Apu;

  /* Read from APU */
  MapperReadApu = Map187_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map187_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map187_Chr[i] = 0x00;
    Map187_Bank[i] = 0x00;
  }

  /* Set ROM Banks */
  Map187_Prg[0] = (NesHeader.byRomSize<<1)-4;
  Map187_Prg[1] = (NesHeader.byRomSize<<1)-3;
  Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
  Map187_Prg[3] = (NesHeader.byRomSize<<1)-1;
  Map187_Set_CPU_Banks();

  Map187_ExtMode = 0;
  Map187_ChrMode = 0;
  Map187_ExtEnable = 0;

  Map187_IRQ_Enable = 0;
  Map187_IRQ_Counter = 0;
  Map187_IRQ_Latch = 0;

  Map187_LastWrite = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map187_Write( WORD wAddr, BYTE byData )
{
  Map187_LastWrite = byData;
  switch( wAddr ) {
  case	0x8003:
    Map187_ExtEnable = 0xFF;
    Map187_ChrMode = byData;
    if( (byData&0xF0) == 0 ) {
      Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
      Map187_Set_CPU_Banks();
    }
    break;
    
  case	0x8000:
    Map187_ExtEnable = 0;
    Map187_ChrMode = byData;
    break;

  case	0x8001:
    if( !Map187_ExtEnable ) {
      switch( Map187_ChrMode & 7 ) {
      case	0:
	byData &= 0xFE;
	Map187_Chr[4] = (int)byData+0x100;
	Map187_Chr[5] = (int)byData+0x100+1;
	Map187_Set_PPU_Banks();
	break;
      case	1:
	byData &= 0xFE;
	Map187_Chr[6] = (int)byData+0x100;
	Map187_Chr[7] = (int)byData+0x100+1;
	Map187_Set_PPU_Banks();
	break;
      case	2:
	Map187_Chr[0] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	3:
	Map187_Chr[1] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	4:
	Map187_Chr[2] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	5:
	Map187_Chr[3] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	6:
	if( (Map187_ExtMode&0xA0)!=0xA0 ) {
	  Map187_Prg[0] = byData;
	  Map187_Set_CPU_Banks();
	}
	break;
      case	7:
	if( (Map187_ExtMode&0xA0)!=0xA0 ) {
	  Map187_Prg[1] = byData;
	  Map187_Set_CPU_Banks();
	}
	break;
      default:
	break;
      }
    } else {
      switch( Map187_ChrMode ) {
      case	0x2A:
	Map187_Prg[1] = 0x0F;
	break;
      case	0x28:
	Map187_Prg[2] = 0x17;
	break;
      case	0x26:
	break;
      default:
	break;
      }
      Map187_Set_CPU_Banks();
    }
    Map187_Bank[Map187_ChrMode&7] = byData;
    break;
    
  case	0xA000:
    if( byData & 0x01 ) {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    break;
    
  case	0xC000:
    Map187_IRQ_Counter = byData;
    Map187_IRQ_Occur = 0;
    break;
  case	0xC001:
    Map187_IRQ_Latch = byData;
    Map187_IRQ_Occur = 0;
    break;
  case	0xE000:
  case	0xE002:
    Map187_IRQ_Enable = 0;
    Map187_IRQ_Occur = 0;
    break;
  case	0xE001:
  case	0xE003:
    Map187_IRQ_Enable = 1;
    Map187_IRQ_Occur = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map187_Apu( WORD wAddr, BYTE byData )
{
  Map187_LastWrite = byData;
  if( wAddr == 0x5000 ) {
    Map187_ExtMode = byData;
    if( byData & 0x80 ) {
      if( byData & 0x20 ) {
	Map187_Prg[0] = ((byData&0x1E)<<1)+0;
	Map187_Prg[1] = ((byData&0x1E)<<1)+1;
	Map187_Prg[2] = ((byData&0x1E)<<1)+2;
	Map187_Prg[3] = ((byData&0x1E)<<1)+3;
      } else {
	Map187_Prg[2] = ((byData&0x1F)<<1)+0;
	Map187_Prg[3] = ((byData&0x1F)<<1)+1;
      }
    } else {
      Map187_Prg[0] = Map187_Bank[6];
      Map187_Prg[1] = Map187_Bank[7];
      Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
      Map187_Prg[3] = (NesHeader.byRomSize<<1)-1;
    }
    Map187_Set_CPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Read from APU Function                                */
/*-------------------------------------------------------------------*/
BYTE Map187_ReadApu( WORD wAddr )
{
  switch( Map187_LastWrite&0x03 ) {
  case 0:
    return 0x83;
  case 1:
    return 0x83;
  case 2:
    return 0x42;
  case 3:
    return 0x00;
  }
  return 0;
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map187_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map187_IRQ_Enable ) {
	if( !Map187_IRQ_Counter ) {
	  Map187_IRQ_Counter--;
	  Map187_IRQ_Enable = 0;
	  Map187_IRQ_Occur = 0xFF;
	} else {
	  Map187_IRQ_Counter--;
	}
      }
    }
  }
  if ( Map187_IRQ_Occur ) {
	  IRQ_REQ;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map187_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE(((Map187_Prg[0]<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((Map187_Prg[1]<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((Map187_Prg[2]<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((Map187_Prg[3]<<2)+3) % (NesHeader.byRomSize<<1));
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map187_Set_PPU_Banks()
{
  PPUBANK[ 0 ] = VROMPAGE(((Map187_Chr[0]<<3)+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE(((Map187_Chr[1]<<3)+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE(((Map187_Chr[2]<<3)+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE(((Map187_Chr[3]<<3)+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE(((Map187_Chr[4]<<3)+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE(((Map187_Chr[5]<<3)+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE(((Map187_Chr[6]<<3)+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE(((Map187_Chr[7]<<3)+7) % (NesHeader.byVRomSize<<3));
  InfoNES_SetupChr();
}
// Mapper 188: removed (needs more RAM than the RP2040 has)
/*===================================================================*/
/*                                                                   */
/*                       Mapper 189 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

BYTE Map189_Regs[ 1 ];
BYTE Map189_IRQ_Cnt;
BYTE Map189_IRQ_Latch;
BYTE Map189_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 189                                            */
/*-------------------------------------------------------------------*/
void Map189_Init()
{
  /* Initialize Mapper */
  MapperInit = Map189_Init;

  /* Write to Mapper */
  MapperWrite = Map189_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map189_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map189_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize IRQ registers */
  Map189_IRQ_Cnt = 0;
  Map189_IRQ_Latch = 0;
  Map189_IRQ_Enable = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map189_Apu( WORD wAddr, BYTE byData )
{
  if ( wAddr >= 0x4100 && wAddr <= 0x41FF )
  {
    byData = ( byData & 0x30 ) >> 4;
    ROMBANK0 = ROMPAGE( ( byData * 4 + 0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( byData * 4 + 1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( byData * 4 + 2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( byData * 4 + 3 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map189_Write( WORD wAddr, BYTE byData )
{
  /* Set ROM Banks */
  switch( wAddr )
  {
    case 0x8000:
      Map189_Regs[0] = byData;
      break;

    case 0x8001:
      switch( Map189_Regs[0] )
      {
        case 0x40:
	  PPUBANK[ 0 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
	  PPUBANK[ 1 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x41:
	  PPUBANK[ 2 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
	  PPUBANK[ 3 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x42:
	  PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x43:
	  PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x44:
	  PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x45:
	  PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x46:
	  ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
	  break;  

        case 0x47:
	  ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
	  break;  
      }
      break;

    case 0xC000:
      Map189_IRQ_Cnt = byData;
      break;

    case 0xC001:
      Map189_IRQ_Latch = byData;
      break;

    case 0xE000:
      Map189_IRQ_Enable = 0;
      break;

    case 0xE001:
      Map189_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map189_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map189_IRQ_Enable )
  {
    if ( 0 <= PPU_Scanline && PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map189_IRQ_Cnt ) )
        {
          Map189_IRQ_Cnt = Map189_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}
/*===================================================================*/
/*                                                                   */
/* Mapper 191 : SACHEN Super Cartridge Xin1 (Ver.1-9), Q-BOY Support */
/*                                                                   */
/*===================================================================*/

BYTE	Map191_Reg[8];
BYTE	Map191_Prg0, Map191_Prg1;
BYTE	Map191_Chr0, Map191_Chr1, Map191_Chr2, Map191_Chr3;
BYTE	Map191_Highbank;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 191                                            */
/*-------------------------------------------------------------------*/
void Map191_Init()
{
  /* Initialize Mapper */
  MapperInit = Map191_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map191_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map191_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map191_Prg0 = 0;
  // Map191_Prg1 = 1;
  Map191_Set_CPU_Banks();

  /* Set PPU Banks */
  Map191_Chr0 = 0;
  Map191_Chr1 = 0;
  Map191_Chr2 = 0;
  Map191_Chr3 = 0;
  Map191_Highbank = 0;
  Map191_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 191 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map191_Apu( WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map191_Reg[0]=byData;
    break;
  case	0x4101:
    Map191_Reg[1]=byData;
    switch( Map191_Reg[0] ) {
    case	0:
      Map191_Chr0=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	1:
      Map191_Chr1=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	2:
      Map191_Chr2=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	3:
      Map191_Chr3=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	4:
      Map191_Highbank=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	5:
      Map191_Prg0=byData&7;
      Map191_Set_CPU_Banks();
      break;
    case	7:
      if( byData & 0x02 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 191 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map191_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE( ((Map191_Prg0<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ((Map191_Prg0<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ((Map191_Prg0<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ((Map191_Prg0<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );
};

/*-------------------------------------------------------------------*/
/*  Mapper 191 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map191_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr0)<<2)+0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr0)<<2)+1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr1)<<2)+2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr1)<<2)+3 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr2)<<2)+0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr2)<<2)+1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr3)<<2)+2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr3)<<2)+3 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*         Mapper 193 : MEGA SOFT (NTDEC) : Fighting Hero            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 193                                            */
/*-------------------------------------------------------------------*/
void Map193_Init()
{
  /* Initialize Mapper */
  MapperInit = Map193_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map193_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 193 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map193_Sram( WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x6000:
    PPUBANK[ 0 ] = VROMPAGE( ((byData&0xfc) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ((byData&0xfc) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ((byData&0xfc) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ((byData&0xfc) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6001:
    PPUBANK[ 4 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6002:
    PPUBANK[ 6 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6003:
    ROMBANK0 = ROMPAGE( ((byData<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ((byData<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ((byData<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ((byData<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );
    break;
  }
}
/*===================================================================*/
/*                                                                   */
/*            Mapper 194 : Meikyuu Jiin Dababa                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 194                                            */
/*-------------------------------------------------------------------*/
void Map194_Init()
{
  /* Initialize Mapper */
  MapperInit = Map194_Init;

  /* Write to Mapper */
  MapperWrite = Map194_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 194 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map194_Write( WORD wAddr, BYTE byData )
{
  SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 200 (1200-in-1)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 200                                            */
/*-------------------------------------------------------------------*/
void Map200_Init()
{
  /* Initialize Mapper */
  MapperInit = Map200_Init;

  /* Write to Mapper */
  MapperWrite = Map200_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 200 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map200_Write( WORD wAddr, BYTE byData )
{
	/* Set ROM Banks */
  ROMBANK0 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE((((wAddr&0x07)<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE((((wAddr&0x07)<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE((((wAddr&0x07)<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE((((wAddr&0x07)<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE((((wAddr&0x07)<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE((((wAddr&0x07)<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE((((wAddr&0x07)<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE((((wAddr&0x07)<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();

	if ( wAddr & 0x01 ) {
		InfoNES_Mirroring( 1 );
	} else {
		InfoNES_Mirroring( 0 );
	}
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 201 (21-in-1)                            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 201                                            */
/*-------------------------------------------------------------------*/
void Map201_Init()
{
  /* Initialize Mapper */
  MapperInit = Map201_Init;

  /* Write to Mapper */
  MapperWrite = Map201_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 201 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map201_Write( WORD wAddr, BYTE byData )
{
	BYTE byBank = (BYTE)wAddr & 0x03;
	if (!(wAddr&0x08) )
		byBank = 0;

	/* Set ROM Banks */
  ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE(((byBank<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE(((byBank<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE(((byBank<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE(((byBank<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE(((byBank<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE(((byBank<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE(((byBank<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE(((byBank<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();
}

/*===================================================================*/
/*                                                                   */
/*                   Mapper 202 (150-in-1)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 202                                            */
/*-------------------------------------------------------------------*/
void Map202_Init()
{
  /* Initialize Mapper */
  MapperInit = Map202_Init;

  /* Write to Mapper */
  MapperWrite = Map202_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map202_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 12 );
  ROMBANK1 = ROMPAGE( 13 );
  ROMBANK2 = ROMPAGE( 14 );
  ROMBANK3 = ROMPAGE( 15 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map202_Apu( WORD wAddr, BYTE byData )
{
	Map202_WriteSub( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map202_Write( WORD wAddr, BYTE byData )
{
	Map202_WriteSub( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Sub Function                                    */
/*-------------------------------------------------------------------*/
void Map202_WriteSub( WORD wAddr, BYTE byData )
{
	int bank = (wAddr>>1) & 0x07;

	/* Set ROM Banks */
	ROMBANK0 = ROMPAGE(((bank<<1)+0) % (NesHeader.byRomSize<<1));
	ROMBANK1 = ROMPAGE(((bank<<1)+1) % (NesHeader.byRomSize<<1));

	if ((wAddr&0x0c) == 0x0c) {
		ROMBANK2 = ROMPAGE((((bank+1)<<1)+0) % (NesHeader.byRomSize<<1));
		ROMBANK3 = ROMPAGE((((bank+1)<<1)+1) % (NesHeader.byRomSize<<1));
	} else {		
		ROMBANK2 = ROMPAGE((((bank+0)<<1)+0) % (NesHeader.byRomSize<<1));
		ROMBANK3 = ROMPAGE((((bank+0)<<1)+1) % (NesHeader.byRomSize<<1));
	}	

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE(((bank<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE(((bank<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE(((bank<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE(((bank<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE(((bank<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE(((bank<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE(((bank<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE(((bank<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();

	if ( wAddr & 0x01 ) {
		InfoNES_Mirroring( 0 );
	} else {
		InfoNES_Mirroring( 1 );
	}
}

/*===================================================================*/
/*                                                                   */
/*                          Mapper 222                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 222                                            */
/*-------------------------------------------------------------------*/
void Map222_Init()
{
  /* Initialize Mapper */
  MapperInit = Map222_Init;

  /* Write to Mapper */
  MapperWrite = Map222_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 222 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map222_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xF003 ) {
  case	0x8000:
    ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xA000:
    ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xB000:
    PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xB002:
    PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xC000:
    PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xC002:
    PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xD000:
    PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xD002:
    PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xE000:
    PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xE002:
    PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 225 : 72-in-1                            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 225                                            */
/*-------------------------------------------------------------------*/
void Map225_Init()
{
  /* Initialize Mapper */
  MapperInit = Map225_Init;

  /* Write to Mapper */
  MapperWrite = Map225_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 225 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map225_Write( WORD wAddr, BYTE byData )
{
  BYTE byPrgBank = (wAddr & 0x0F80) >> 7;
  BYTE byChrBank = wAddr & 0x003F;
  
  PPUBANK[ 0 ] = VROMPAGE(((byChrBank<<3)+0) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 1 ] = VROMPAGE(((byChrBank<<3)+1) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 2 ] = VROMPAGE(((byChrBank<<3)+2) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 3 ] = VROMPAGE(((byChrBank<<3)+3) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 4 ] = VROMPAGE(((byChrBank<<3)+4) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 5 ] = VROMPAGE(((byChrBank<<3)+5) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 6 ] = VROMPAGE(((byChrBank<<3)+6) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 7 ] = VROMPAGE(((byChrBank<<3)+7) % ( NesHeader.byVRomSize << 3 ) );
  InfoNES_SetupChr();

  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
  
  if( wAddr & 0x1000 ) {
    // 16KBbank
    if( wAddr & 0x0040 ) {
      ROMBANK0 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
    } else {
      ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    }
  } else {
    ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 226 : 76-in-1                            */
/*                                                                   */
/*===================================================================*/

BYTE	Map226_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 226                                            */
/*-------------------------------------------------------------------*/
void Map226_Init()
{
  /* Initialize Mapper */
  MapperInit = Map226_Init;

  /* Write to Mapper */
  MapperWrite = Map226_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Initialize Registers */
  Map226_Reg[0] = 0;
  Map226_Reg[1] = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 226 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map226_Write( WORD wAddr, BYTE byData )
{
  if( wAddr & 0x0001 ) {
    Map226_Reg[1] = byData;
  } else {
    Map226_Reg[0] = byData;
  }
  
  if( Map226_Reg[0] & 0x40 ) {
    InfoNES_Mirroring( 1 );
  } else {
    InfoNES_Mirroring( 0 );
  }
  
  BYTE	byBank = ((Map226_Reg[0]&0x1E)>>1)|((Map226_Reg[0]&0x80)>>3)|((Map226_Reg[1]&0x01)<<5);
  
  if( Map226_Reg[0] & 0x20 ) {
    if( Map226_Reg[0] & 0x01 ) {
      ROMBANK0 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
/*===================================================================*/
/*                                                                   */
/*                    Mapper 227 : 1200-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 227                                            */
/*-------------------------------------------------------------------*/
void Map227_Init()
{
  /* Initialize Mapper */
  MapperInit = Map227_Init;

  /* Write to Mapper */
  MapperWrite = Map227_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 227 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map227_Write( WORD wAddr, BYTE byData )
{
  BYTE	byBank = ((wAddr&0x0100)>>4)|((wAddr&0x0078)>>3);

  if( wAddr & 0x0001 ) {
    ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
  } else {
    if( wAddr & 0x0004 ) {
      ROMBANK0 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    }
  }
  
  if( !(wAddr & 0x0080) ) {
    if( wAddr & 0x0200 ) {
      ROMBANK2 = ROMPAGE((((byBank&0x1C)<<2)+14) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byBank&0x1C)<<2)+15) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK2 = ROMPAGE((((byBank&0x1C)<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byBank&0x1C)<<2)+1) % (NesHeader.byRomSize<<1));
    }
  }
  if( wAddr & 0x0002 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 228 : Action 52                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 228                                            */
/*-------------------------------------------------------------------*/
void Map228_Init()
{
  /* Initialize Mapper */
  MapperInit = Map228_Init;

  /* Write to Mapper */
  MapperWrite = Map228_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 228 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map228_Write( WORD wAddr, BYTE byData )
{
  BYTE	byPrg = (wAddr&0x0780)>>7;

  switch( (wAddr&0x1800)>>11 ) {
  case	1:
    byPrg |= 0x10;
    break;
  case	3:
    byPrg |= 0x20;
    break;
  }

  if( wAddr & 0x0020 ) {
    byPrg <<= 1;
    if( wAddr & 0x0040 ) {
      byPrg++;
    }
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
  }

  BYTE byChr = ((wAddr&0x000F)<<2)|(byData&0x03);

  PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3)); 
  InfoNES_SetupChr();
  
  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 229 : 31-in-1                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 229                                            */
/*-------------------------------------------------------------------*/
void Map229_Init()
{
  /* Initialize Mapper */
  MapperInit = Map229_Init;

  /* Write to Mapper */
  MapperWrite = Map229_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 229 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map229_Write( WORD wAddr, BYTE byData )
{
  if( wAddr & 0x001E ) {
    BYTE byPrg = wAddr&0x001F;

    ROMBANK0 = ROMPAGE(((byPrg<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<1)+1) % (NesHeader.byRomSize<<1));

    BYTE byChr = wAddr & 0x0FFF;

    PPUBANK[ 0 ] = VROMPAGE(((byChr<<3) + 0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((byChr<<3) + 1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((byChr<<3) + 2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((byChr<<3) + 3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((byChr<<3) + 4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((byChr<<3) + 5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((byChr<<3) + 6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((byChr<<3) + 7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 2 );
    ROMBANK3 = ROMPAGE( 3 );

    PPUBANK[ 0 ] = VROMPAGE( 0 );
    PPUBANK[ 1 ] = VROMPAGE( 1 );
    PPUBANK[ 2 ] = VROMPAGE( 2 );
    PPUBANK[ 3 ] = VROMPAGE( 3 );
    PPUBANK[ 4 ] = VROMPAGE( 4 );
    PPUBANK[ 5 ] = VROMPAGE( 5 );
    PPUBANK[ 6 ] = VROMPAGE( 6 );
    PPUBANK[ 7 ] = VROMPAGE( 7 );
    InfoNES_SetupChr();
  }

  if( wAddr & 0x0020 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                       Mapper 230 : 22-in-1                        */
/*                                                                   */
/*===================================================================*/

BYTE Map230_RomSw;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 230                                            */
/*-------------------------------------------------------------------*/
void Map230_Init()
{
  /* Initialize Mapper */
  MapperInit = Map230_Init;

  /* Write to Mapper */
  MapperWrite = Map230_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  if( Map230_RomSw ) {
    Map230_RomSw = 0;
  } else {
    Map230_RomSw = 1;
  }

  /* Set ROM Banks */
  if( Map230_RomSw ) {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 14 );
    ROMBANK3 = ROMPAGE( 15 );
  } else {
    ROMBANK0 = ROMPAGE( 16 );
    ROMBANK1 = ROMPAGE( 17 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 230 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map230_Write( WORD wAddr, BYTE byData )
{
  if( Map230_RomSw ) {
    ROMBANK0 = ROMPAGE((((byData&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  } else {
    if( byData & 0x20 ) {
      ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+17) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+18) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+19) % (NesHeader.byRomSize<<1));
    }
    if( byData & 0x40 ) {
      InfoNES_Mirroring( 1 );
    } else {
      InfoNES_Mirroring( 0 );
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 231 : 20-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 231                                            */
/*-------------------------------------------------------------------*/
void Map231_Init()
{
  /* Initialize Mapper */
  MapperInit = Map231_Init;

  /* Write to Mapper */
  MapperWrite = Map231_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 231 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map231_Write( WORD wAddr, BYTE byData )
{
  BYTE	byBank = wAddr & 0x1E;

  if( wAddr & 0x0020 ) {
    ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<1)+3) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
  }

  if( wAddr & 0x0080 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 232 : Quattro Games                      */
/*                                                                   */
/*===================================================================*/

BYTE Map232_Regs[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 232                                            */
/*-------------------------------------------------------------------*/
void Map232_Init()
{
  /* Initialize Mapper */
  MapperInit = Map232_Init;

  /* Write to Mapper */
  MapperWrite = Map232_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize Registers */
  Map232_Regs[0] = 0x0C;
  Map232_Regs[1] = 0x00;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 232 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map232_Write( WORD wAddr, BYTE byData )
{
  if( wAddr == 0x9000 ) {
    Map232_Regs[0] = (byData & 0x18)>>1;
  } else if( wAddr >= 0xA000 && wAddr <= 0xFFFF ) {
    Map232_Regs[1] = byData & 0x03;
  }
  
  ROMBANK0= ROMPAGE((((Map232_Regs[0]|Map232_Regs[1])<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1= ROMPAGE((((Map232_Regs[0]|Map232_Regs[1])<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2= ROMPAGE((((Map232_Regs[0]|0x03)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK3= ROMPAGE((((Map232_Regs[0]|0x03)<<1)+1) % (NesHeader.byRomSize<<1));
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 233 : 42-in-1                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 233                                            */
/*-------------------------------------------------------------------*/
void Map233_Init()
{
  /* Initialize Mapper */
  MapperInit = Map233_Init;

  /* Write to Mapper */
  MapperWrite = Map233_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 233 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map233_Write( WORD wAddr, BYTE byData )
{
  if( byData & 0x20 ) {
    ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
    ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1)); 
    ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
  } else {
    ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+0) % (NesHeader.byRomSize<<1)); 
    ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+1) % (NesHeader.byRomSize<<1)); 
    ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+3) % (NesHeader.byRomSize<<1)); 
  }

  if( (byData&0xC0) == 0x00 ) {
    InfoNES_Mirroring( 5 );
  } else if( (byData&0xC0) == 0x40 ) {
    InfoNES_Mirroring( 1 );
  } else if( (byData&0xC0) == 0x80 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 2 );
  }
}
/*===================================================================*/
/*                                                                   */
/*                     Mapper 234 : Maxi-15                          */
/*                                                                   */
/*===================================================================*/

BYTE	Map234_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 234                                            */
/*-------------------------------------------------------------------*/
void Map234_Init()
{
  /* Initialize Mapper */
  MapperInit = Map234_Init;

  /* Write to Mapper */
  MapperWrite = Map234_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set Registers */
  Map234_Reg[0] = 0;
  Map234_Reg[1] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 234 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map234_Write( WORD wAddr, BYTE byData )
{
  if( wAddr >= 0xFF80 && wAddr <= 0xFF9F ) {
    if( !Map234_Reg[0] ) {
      Map234_Reg[0] = byData;
      Map234_Set_Banks();
    }
  }
  
  if( wAddr >= 0xFFE8 && wAddr <= 0xFFF7 ) {
    Map234_Reg[1] = byData;
    Map234_Set_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 234 Set Banks Function                                    */
/*-------------------------------------------------------------------*/
void Map234_Set_Banks()
{
  BYTE byPrg, byChr;

  if( Map234_Reg[0] & 0x80 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
  if( Map234_Reg[0] & 0x40 ) {
    byPrg = (Map234_Reg[0]&0x0E)|(Map234_Reg[1]&0x01);
    byChr = ((Map234_Reg[0]&0x0E)<<2)|((Map234_Reg[1]>>4)&0x07);
  } else {
    byPrg = Map234_Reg[0]&0x0F;
    byChr = ((Map234_Reg[0]&0x0F)<<2)|((Map234_Reg[1]>>4)&0x03);
  }
  
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3));
  InfoNES_SetupChr();
}


/*===================================================================*/
/*                                                                   */
/*                     Mapper 235 : 150-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 235                                            */
/*-------------------------------------------------------------------*/
void Map235_Init()
{
  /* Initialize Mapper */
  MapperInit = Map235_Init;

  /* Write to Mapper */
  MapperWrite = Map235_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 0x2000; i++ ) {
    DRAM[i] = 0xFF;
  }

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 235 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map235_Write( WORD wAddr, BYTE byData )
{
  BYTE	byPrg = ((wAddr&0x0300)>>3)|(wAddr&0x001F);
  BYTE	byBus = 0;

  if( (NesHeader.byRomSize<<1) == 64*2 ) {
    // 100-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byBus = 1; break;
    case	0x0300:	byBus = 1; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 128*2 ) {
    // 150-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byPrg = (byPrg&0x1F)|0x20; break;
    case	0x0300:	byBus = 1; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 192*2 ) {
    // 150-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byPrg = (byPrg&0x1F)|0x20; break;
    case	0x0300:	byPrg = (byPrg&0x1F)|0x40; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 256*2 ) {
  }
  
  if( wAddr & 0x0800 ) {
    if( wAddr & 0x1000 ) {
      ROMBANK0 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
  }
  
  if( byBus ) {
    ROMBANK0 = DRAM;
    ROMBANK1 = DRAM;
    ROMBANK2 = DRAM;
    ROMBANK3 = DRAM;
  }

  if( wAddr & 0x0400 ) {
    InfoNES_Mirroring( 3 );
  } else if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}

/*===================================================================*/
/*                                                                   */
/*                   Mapper 236 : 800-in-1                           */
/*                                                                   */
/*===================================================================*/

BYTE    Map236_Bank, Map236_Mode;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 236                                            */
/*-------------------------------------------------------------------*/
void Map236_Init()
{
  /* Initialize Mapper */
  MapperInit = Map236_Init;

  /* Write to Mapper */
  MapperWrite = Map236_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set Registers */
  Map236_Bank = Map236_Mode = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 236 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map236_Write( WORD wAddr, BYTE byData )
{
  if( wAddr >= 0x8000 && wAddr <= 0xBFFF ) {
    Map236_Bank = ((wAddr&0x03)<<4)|(Map236_Bank&0x07);
  } else {
    Map236_Bank = (wAddr&0x07)|(Map236_Bank&0x30);
    Map236_Mode = wAddr&0x30;
  }

  if( wAddr & 0x20 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }

  switch( Map236_Mode ) {
  case	0x00:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x10:
    Map236_Bank |= 0x37;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x20:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE((((Map236_Bank&0xFE)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((Map236_Bank&0xFE)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank&0xFE)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank&0xFE)<<1)+3) % (NesHeader.byRomSize<<1));
    break;
  case	0x30:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  }
}
/*===================================================================*/
/*                                                                   */
/*                  Mapper 240 : Gen Ke Le Zhuan                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 240                                            */
/*-------------------------------------------------------------------*/
void Map240_Init()
{
  /* Initialize Mapper */
  MapperInit = Map240_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map240_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 240 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map240_Apu( WORD wAddr, BYTE byData )
{
  if( wAddr>=0x4020 && wAddr<0x6000 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE((((byData&0xF0)>>2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0xF0)>>2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((byData&0xF0)>>2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((byData&0xF0)>>2)+3) % (NesHeader.byRomSize<<1));

    /* Set PPU Banks */
    PPUBANK[ 0 ] = VROMPAGE((((byData&0x0F)<<3)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((((byData&0x0F)<<3)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((((byData&0x0F)<<3)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((((byData&0x0F)<<3)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((((byData&0x0F)<<3)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((((byData&0x0F)<<3)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE((((byData&0x0F)<<3)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((((byData&0x0F)<<3)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 241 : Fon Serm Bon                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 241                                            */
/*-------------------------------------------------------------------*/
void Map241_Init()
{
  /* Initialize Mapper */
  MapperInit = Map241_Init;

  /* Write to Mapper */
  MapperWrite = Map241_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 241 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map241_Write( WORD wAddr, BYTE byData )
{
  if( wAddr == 0x8000 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((byData<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byData<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byData<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byData<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
/*===================================================================*/
/*                                                                   */
/*                Mapper 242 : Wai Xing Zhan Shi                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 242                                            */
/*-------------------------------------------------------------------*/
void Map242_Init()
{
  /* Initialize Mapper */
  MapperInit = Map242_Init;

  /* Write to Mapper */
  MapperWrite = Map242_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 242 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map242_Write( WORD wAddr, BYTE byData )
{
  if( wAddr & 0x01 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE((((wAddr&0xF8)>>1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0xF8)>>1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0xF8)>>1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0xF8)>>1)+3) % (NesHeader.byRomSize<<1));
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 243 (Pirates)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map243_Regs[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 243                                            */
/*-------------------------------------------------------------------*/
void Map243_Init()
{
  /* Initialize Mapper */
  MapperInit = Map243_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map243_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize state registers */
  Map243_Regs[0] = Map243_Regs[1] = Map243_Regs[2] = Map243_Regs[3] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 243 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map243_Apu( WORD wAddr, BYTE byData )
{
  if ( wAddr == 0x4100 )
  {
    Map243_Regs[0] = byData;
  }
  else if ( wAddr == 0x4101 )
  {
    switch ( Map243_Regs[0] & 0x07 )
    {
      case 0x02:
        Map243_Regs[1] = byData & 0x01;
        break;

      case 0x00:
      case 0x04:
      case 0x07:
        Map243_Regs[2] = ( byData & 0x01 ) << 1;
        break;

      /* Set ROM Banks */
      case 0x05:
        ROMBANK0 = ROMPAGE( ( byData * 4 + 0 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( ( byData * 4 + 1 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( ( byData * 4 + 2 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMPAGE( ( byData * 4 + 3 ) % ( NesHeader.byRomSize << 1 ) );
        break;

      case 0x06:
        Map243_Regs[3] = ( byData & 0x03 ) << 2;
        break;
    }

    /* Set PPU Banks */
    if ( ( NesHeader.byVRomSize << 3 ) <= 64 )
    {
      BYTE chr_bank = ( Map243_Regs[2] + Map243_Regs[3] ) >> 1;

      PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else
    {
      BYTE chr_bank = Map243_Regs[1] + Map243_Regs[2] + Map243_Regs[3];

      PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                          Mapper 244                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 244                                            */
/*-------------------------------------------------------------------*/
void Map244_Init()
{
  /* Initialize Mapper */
  MapperInit = Map244_Init;

  /* Write to Mapper */
  MapperWrite = Map244_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 244 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map244_Write( WORD wAddr, BYTE byData )
{
  if( wAddr>=0x8065 && wAddr<=0x80A4 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+3) % (NesHeader.byRomSize<<1));
  }
  
  if( wAddr>=0x80A5 && wAddr<=0x80E4 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
/*===================================================================*/
/*                                                                   */
/*               Mapper 245 : Yong Zhe Dou E Long                    */
/*                                                                   */
/*===================================================================*/

BYTE	Map245_Reg[8];
BYTE	Map245_Prg0, Map245_Prg1;
BYTE	Map245_Chr01, Map245_Chr23, Map245_Chr4, Map245_Chr5, Map245_Chr6, Map245_Chr7;
BYTE	Map245_WeSram;

BYTE	Map245_IRQ_Enable;
BYTE	Map245_IRQ_Counter;
BYTE	Map245_IRQ_Latch;
BYTE	Map245_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 245                                            */
/*-------------------------------------------------------------------*/
void Map245_Init()
{
  /* Initialize Mapper */
  MapperInit = Map245_Init;

  /* Write to Mapper */
  MapperWrite = Map245_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map245_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map245_Reg[i] = 0x00;
  }

  Map245_Prg0 = 0;
  Map245_Prg1 = 1;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  Map245_WeSram  = 0;		// Disable
  Map245_IRQ_Enable = 0;	// Disable
  Map245_IRQ_Counter = 0;
  Map245_IRQ_Latch = 0;
  Map245_IRQ_Request = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map245_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr&0xF7FF ) {
  case	0x8000:
    Map245_Reg[0] = byData;
    break;
  case	0x8001:
    Map245_Reg[1] = byData;
    switch( Map245_Reg[0] ) {
    case	0x00:
      Map245_Reg[3]=(byData & 2 )<<5;
      ROMBANK2 = ROMPAGE((0x3E|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x3F|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
      break;
    case	0x06:
      Map245_Prg0=byData;
      break;
    case	0x07:
      Map245_Prg1=byData;
      break;
    }
    ROMBANK0 = ROMPAGE((Map245_Prg0|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map245_Prg1|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
    break;
  case	0xA000:
    Map245_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    
    break;
  case	0xC000:
    Map245_Reg[4] = byData;
    Map245_IRQ_Counter = byData;
    Map245_IRQ_Request = 0;
    break;
  case	0xC001:
    Map245_Reg[5] = byData;
    Map245_IRQ_Latch = byData;
    Map245_IRQ_Request = 0;
    break;
  case	0xE000:
    Map245_Reg[6] = byData;
    Map245_IRQ_Enable = 0;
    Map245_IRQ_Request = 0;
    break;
  case	0xE001:
    Map245_Reg[7] = byData;
    Map245_IRQ_Enable = 1;
    Map245_IRQ_Request = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map245_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map245_IRQ_Enable && !Map245_IRQ_Request ) {
	if( PPU_Scanline == 0 ) {
	  if( Map245_IRQ_Counter ) {
	    Map245_IRQ_Counter--;
	  }
	}
	if( !(Map245_IRQ_Counter--) ) {
	  Map245_IRQ_Request = 0xFF;
	  Map245_IRQ_Counter = Map245_IRQ_Latch;
	}
      }
    }
  }
  if( Map245_IRQ_Request ) {
    IRQ_REQ;
  }
}

#if 0
/*-------------------------------------------------------------------*/
/*  Mapper 245 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void SetBank_CPU()
{
  ROMBANK0 = ROMPAGE( Map245_Prg0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( Map245_Prg1 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void SetBank_PPU()
{
  if( NesHeader.byVRomSize > 0 ) {
    if( Map245_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( Map245_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map245_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map245_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map245_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map245_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map245_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map245_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map245_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map245_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map245_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map245_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map245_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map245_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map245_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map245_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map245_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  } else {
    if( Map245_Reg[0] & 0x80 ) {
      PPUBANK[ 4 ] = CRAMPAGE( (Map245_Chr01+0)&0x07 );
      PPUBANK[ 5 ] = CRAMPAGE( (Map245_Chr01+1)&0x07 );
      PPUBANK[ 6 ] = CRAMPAGE( (Map245_Chr23+0)&0x07 );
      PPUBANK[ 7 ] = CRAMPAGE( (Map245_Chr23+1)&0x07 );
      PPUBANK[ 0 ] = CRAMPAGE( Map245_Chr4&0x07 );
      PPUBANK[ 1 ] = CRAMPAGE( Map245_Chr5&0x07 );
      PPUBANK[ 2 ] = CRAMPAGE( Map245_Chr6&0x07 );
      PPUBANK[ 3 ] = CRAMPAGE( Map245_Chr7&0x07 );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = CRAMPAGE( (Map245_Chr01+0)&0x07 );
      PPUBANK[ 1 ] = CRAMPAGE( (Map245_Chr01+1)&0x07 );
      PPUBANK[ 2 ] = CRAMPAGE( (Map245_Chr23+0)&0x07 );
      PPUBANK[ 3 ] = CRAMPAGE( (Map245_Chr23+1)&0x07 );
      PPUBANK[ 4 ] = CRAMPAGE( Map245_Chr4&0x07 );
      PPUBANK[ 5 ] = CRAMPAGE( Map245_Chr5&0x07 );
      PPUBANK[ 6 ] = CRAMPAGE( Map245_Chr6&0x07 );
      PPUBANK[ 7 ] = CRAMPAGE( Map245_Chr7&0x07 );
      InfoNES_SetupChr();
    }
  }
}
#endif
/*===================================================================*/
/*                                                                   */
/*                 Mapper 246 : Phone Serm Berm                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 246                                            */
/*-------------------------------------------------------------------*/
void Map246_Init()
{
  /* Initialize Mapper */
  MapperInit = Map246_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map246_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 246 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map246_Sram( WORD wAddr, BYTE byData )
{
  if( wAddr>=0x6000 && wAddr<0x8000 ) {
    switch( wAddr ) {
    case 0x6000:
      ROMBANK0 = ROMPAGE(((byData<<2)+0) % (NesHeader.byRomSize<<1));
      break;
    case 0x6001:
      ROMBANK1 = ROMPAGE(((byData<<2)+1) % (NesHeader.byRomSize<<1));
      break;
    case 0x6002:
      ROMBANK2 = ROMPAGE(((byData<<2)+2) % (NesHeader.byRomSize<<1));
      break;
    case 0x6003: 
      ROMBANK3 = ROMPAGE(((byData<<2)+3) % (NesHeader.byRomSize<<1));
      break;
    case 0x6004:
      PPUBANK[ 0 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 1 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6005:
      PPUBANK[ 2 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 3 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6006:
      PPUBANK[ 4 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 5 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6007:
      PPUBANK[ 6 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 7 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    default:
      SRAMBANK[wAddr&0x1FFF] = byData;
      break;
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                   Mapper 248 : Bao Qing Tian                      */
/*                                                                   */
/*===================================================================*/

BYTE	Map248_Reg[8];
BYTE	Map248_Prg0, Map248_Prg1;
BYTE	Map248_Chr01, Map248_Chr23, Map248_Chr4, Map248_Chr5, Map248_Chr6, Map248_Chr7;
BYTE	Map248_WeSram;

BYTE	Map248_IRQ_Enable;
BYTE	Map248_IRQ_Counter;
BYTE	Map248_IRQ_Latch;
BYTE	Map248_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 248                                            */
/*-------------------------------------------------------------------*/
void Map248_Init()
{
  /* Initialize Mapper */
  MapperInit = Map248_Init;

  /* Write to Mapper */
  MapperWrite = Map248_Write;

  /* Write to SRAM */
  MapperSram = Map248_Sram;

  /* Write to APU */
  MapperApu = Map248_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map248_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map248_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map248_Prg0 = 0;
  Map248_Prg1 = 1;
  Map248_Set_CPU_Banks();

  /* Set PPU Banks */
  Map248_Chr01 = 0;
  Map248_Chr23 = 2;
  Map248_Chr4  = 4;
  Map248_Chr5  = 5;
  Map248_Chr6  = 6;
  Map248_Chr7  = 7;
  Map248_Set_PPU_Banks();
  
  Map248_WeSram  = 0;		// Disable
  Map248_IRQ_Enable = 0;	// Disable
  Map248_IRQ_Counter = 0;
  Map248_IRQ_Latch = 0;
  Map248_IRQ_Request = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map248_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map248_Reg[0] = byData;
    Map248_Set_CPU_Banks();
    Map248_Set_PPU_Banks();
    break;
  case	0x8001:
    Map248_Reg[1] = byData;
    
    switch( Map248_Reg[0] & 0x07 ) {
    case	0x00:
      Map248_Chr01 = byData & 0xFE;
      Map248_Set_PPU_Banks();
      break;
    case	0x01:
      Map248_Chr23 = byData & 0xFE;
      Map248_Set_PPU_Banks();
      break;
    case	0x02:
      Map248_Chr4 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x03:
      Map248_Chr5 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x04:
      Map248_Chr6 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x05:
      Map248_Chr7 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x06:
      Map248_Prg0 = byData;
      Map248_Set_CPU_Banks();
      break;
    case	0x07:
      Map248_Prg1 = byData;
      Map248_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map248_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) {
	InfoNES_Mirroring( 0 );
      } else {
	InfoNES_Mirroring( 1 );
      }
    }
    break;
  case 0xC000:
    Map248_IRQ_Enable=0;
    Map248_IRQ_Latch=0xBE;
    Map248_IRQ_Counter =0xBE;
    break;
  case 0xC001:
    Map248_IRQ_Enable=1;
    Map248_IRQ_Latch=0xBE;
    Map248_IRQ_Counter=0xBE;
    break;
  }	
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map248_Sram( WORD wAddr, BYTE byData )
{
  ROMBANK0 = ROMPAGE(((byData<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byData<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map248_Apu( WORD wAddr, BYTE byData )
{
  Map248_Sram( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map248_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map248_IRQ_Enable ) {
	if( !(Map248_IRQ_Counter--) ) {
	  Map248_IRQ_Counter = Map248_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map248_Set_CPU_Banks()
{
  if( Map248_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE(Map248_Prg1 % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(Map248_Prg0 % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE(Map248_Prg0 % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(Map248_Prg1 % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map248_Set_PPU_Banks()
{
  if( NesHeader.byRomSize > 0 ) {
    if( Map248_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE(Map248_Chr4 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(Map248_Chr5 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(Map248_Chr6 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(Map248_Chr7 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE((Map248_Chr01+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE((Map248_Chr01+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE((Map248_Chr23+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE((Map248_Chr23+1) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE((Map248_Chr01+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE((Map248_Chr01+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE((Map248_Chr23+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE((Map248_Chr23+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(Map248_Chr4 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(Map248_Chr5 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(Map248_Chr6 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(Map248_Chr7 % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}
/*===================================================================*/
/*                                                                   */
/*                      Mapper 249 : MMC3                            */
/*                                                                   */
/*===================================================================*/

BYTE	Map249_Spdata;
BYTE	Map249_Reg[8];

BYTE	Map249_IRQ_Enable;
BYTE	Map249_IRQ_Counter;
BYTE	Map249_IRQ_Latch;
BYTE	Map249_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 249                                            */
/*-------------------------------------------------------------------*/
void Map249_Init()
{
  /* Initialize Mapper */
  MapperInit = Map249_Init;

  /* Write to Mapper */
  MapperWrite = Map249_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map249_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map249_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map249_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );



  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Registers */
  Map249_IRQ_Enable = 0;	// Disable
  Map249_IRQ_Counter = 0;
  Map249_IRQ_Latch = 0;
  Map249_IRQ_Request = 0;

  Map249_Spdata = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map249_Write( WORD wAddr, BYTE byData )
{
  BYTE	byM0,byM1,byM2,byM3,byM4,byM5,byM6,byM7;

  switch( wAddr&0xFF01 ) {
  case	  0x8000:
  case    0x8800:
    Map249_Reg[0] = byData;
    break;
  case	  0x8001:
  case    0x8801:
    switch( Map249_Reg[0] & 0x07 ) {
    case	0x00:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 0 ] = VROMPAGE((byData&0xFE) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE((byData|0x01) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x01:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 2 ] = VROMPAGE((byData&0xFE) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE((byData|0x01) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x02:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 4 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x03:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 5 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x04:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 6 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x05:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 7 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x06:
      if( Map249_Spdata == 1 ) {
	if( byData < 0x20 ) {
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=0;
	  byM6=0;
	  byM7=0;
	  byData=(byM7<<7)|(byM6<<6)|(byM5<<5)|(byM2<<4)|(byM1<<3)|(byM3<<2)|(byM4<<1)|byM0;
	} else {
	  byData=byData-0x20;
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=(byData&0x20)>>5;
	  byM6=(byData&0x40)>>6;
	  byM7=(byData&0x80)>>7;
	  byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
	}
      }
      ROMBANK0 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
      break;
    case	0x07:
      if( Map249_Spdata == 1 ) {
	if( byData < 0x20 ) {
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=0;
	  byM6=0;
	  byM7=0;
	  byData=(byM7<<7)|(byM6<<6)|(byM5<<5)|(byM2<<4)|(byM1<<3)|(byM3<<2)|(byM4<<1)|byM0;
	} else {
	  byData=byData-0x20;
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=(byData&0x20)>>5;
	  byM6=(byData&0x40)>>6;
	  byM7=(byData&0x80)>>7;
	  byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
	}
      }
      ROMBANK1 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
      break;
    }
    break;
  case	  0xA000:
  case    0xA800:
    Map249_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	  0xA001:
  case    0xA801:
    Map249_Reg[3] = byData;
    break;
  case	  0xC000:
  case    0xC800:
    Map249_Reg[4] = byData;
    Map249_IRQ_Counter = byData;
    Map249_IRQ_Request = 0;
    break;
  case	  0xC001:
  case    0xC801:
    Map249_Reg[5] = byData;
    Map249_IRQ_Latch = byData;
    Map249_IRQ_Request = 0;
    break;
  case	  0xE000:
  case    0xE800:
    Map249_Reg[6] = byData;
    Map249_IRQ_Enable = 0;
    Map249_IRQ_Request = 0;
    break;
  case	  0xE001:
  case    0xE801:
    Map249_Reg[7] = byData;
    Map249_IRQ_Enable = 1;
    Map249_IRQ_Request = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map249_Apu( WORD wAddr, BYTE byData )
{
  if( wAddr == 0x5000 ) {
    switch( byData ) {
    case	0x00:
      Map249_Spdata = 0;
      break;
    case	0x02:
      Map249_Spdata = 1;
      break;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map249_HSync()
{
  if( (PPU_Scanline >= 0 && PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map249_IRQ_Enable && !Map249_IRQ_Request ) {
	if( PPU_Scanline == 0 ) {
	  if( Map249_IRQ_Counter ) {
	    Map249_IRQ_Counter--;
	  }
	}
	if( !(Map249_IRQ_Counter--) ) {
	  Map249_IRQ_Request = 0xFF;
	  Map249_IRQ_Counter = Map249_IRQ_Latch;
	}
      }
    }
  }
  if( Map249_IRQ_Request ) {
    IRQ_REQ;
  }
}

/*===================================================================*/
/*                                                                   */
/*                          Mapper 251                               */
/*                                                                   */
/*===================================================================*/

BYTE	Map251_Reg[11];
BYTE	Map251_Breg[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 251                                            */
/*-------------------------------------------------------------------*/
void Map251_Init()
{
  /* Initialize Mapper */
  MapperInit = Map251_Init;

  /* Write to Mapper */
  MapperWrite = Map251_Write;

  /* Write to SRAM */
  MapperSram = Map251_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set Registers */
  InfoNES_Mirroring( 1 );

  int	i;
  for( i = 0; i < 11; i++ )
    Map251_Reg[i] = 0;
  for( i = 0; i < 4; i++ )
    Map251_Breg[i] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map251_Write( WORD wAddr, BYTE byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map251_Reg[8] = byData;
    Map251_Set_Banks();
    break;
  case	0x8001:
    Map251_Reg[Map251_Reg[8]&0x07] = byData;
    Map251_Set_Banks();
    break;
  case	0xA001:
    if( byData & 0x80 ) {
      Map251_Reg[ 9] = 1;
      Map251_Reg[10] = 0;
    } else {
      Map251_Reg[ 9] = 0;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map251_Sram( WORD wAddr, BYTE byData )
{
  if( (wAddr & 0xE001) == 0x6000 ) {
    if( Map251_Reg[9] ) {
      Map251_Breg[Map251_Reg[10]++] = byData;
      if( Map251_Reg[10] == 4 ) {
	Map251_Reg[10] = 0;
	Map251_Set_Banks();
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Set Banks Function                                    */
/*-------------------------------------------------------------------*/
void Map251_Set_Banks()
{
  int	nChr[6];
  int	nPrg[4];

  for( int i = 0; i < 6; i++ ) {
    nChr[i] = (Map251_Reg[i]|(Map251_Breg[1]<<4)) & ((Map251_Breg[2]<<4)|0x0F);
  }

  if( Map251_Reg[8] & 0x80 ) {
    PPUBANK[ 0 ] = VROMPAGE(nChr[2] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(nChr[3] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(nChr[4] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(nChr[5] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(nChr[0] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((nChr[0]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(nChr[1] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((nChr[1]+1) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = VROMPAGE(nChr[0] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((nChr[0]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(nChr[1] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((nChr[1]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(nChr[2] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(nChr[3] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(nChr[4] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(nChr[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }

  nPrg[0] = (Map251_Reg[6]&((Map251_Breg[3]&0x3F)^0x3F))|(Map251_Breg[1]);
  nPrg[1] = (Map251_Reg[7]&((Map251_Breg[3]&0x3F)^0x3F))|(Map251_Breg[1]);
  nPrg[2] = nPrg[3] =((Map251_Breg[3]&0x3F)^0x3F)|(Map251_Breg[1]);
  nPrg[2] &= (NesHeader.byRomSize<<1)-1;

  if( Map251_Reg[8] & 0x40 ) {
    ROMBANK0 = ROMPAGE(nPrg[2] % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(nPrg[1] % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(nPrg[0] % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(nPrg[3] % (NesHeader.byRomSize<<1));
  } else { 
    ROMBANK0 = ROMPAGE(nPrg[0] % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(nPrg[1] % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(nPrg[2] % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(nPrg[3] % (NesHeader.byRomSize<<1));
  }
}
/*===================================================================*/
/*                                                                   */
/*              Mapper 252 : Sangokushi - Chuugen no hasha           */
/*                                                                   */
/*===================================================================*/

BYTE	Map252_Reg[9];
BYTE	Map252_IRQ_Enable;
BYTE	Map252_IRQ_Counter;
BYTE	Map252_IRQ_Latch;
BYTE	Map252_IRQ_Occur;
int	Map252_IRQ_Clock;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 252                                            */
/*-------------------------------------------------------------------*/
void Map252_Init()
{
  /* Initialize Mapper */
  MapperInit = Map252_Init;

  /* Write to Mapper */
  MapperWrite = Map252_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map252_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 9; i++ ) {
    Map252_Reg[i] = i;
  }

  Map252_IRQ_Enable = 0;
  Map252_IRQ_Counter = 0;
  Map252_IRQ_Latch = 0;
  Map252_IRQ_Clock = 0;
  Map252_IRQ_Occur = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 252 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map252_Write( WORD wAddr, BYTE byData )
{
  if( (wAddr & 0xF000) == 0x8000 ) {
    ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize<<1));
    return;
  }
  if( (wAddr & 0xF000) == 0xA000 ) {
    ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize<<1));
    return;
  }
  switch( wAddr & 0xF00C ) {
  case 0xB000:
    Map252_Reg[0] = (Map252_Reg[0] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 0 ] = VROMPAGE(Map252_Reg[0] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;	
  case 0xB004:
    Map252_Reg[0] = (Map252_Reg[0] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 0 ] = VROMPAGE(Map252_Reg[0] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xB008:
    Map252_Reg[1] = (Map252_Reg[1] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 1 ] = VROMPAGE(Map252_Reg[1] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xB00C:
    Map252_Reg[1] = (Map252_Reg[1] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 1 ] = VROMPAGE(Map252_Reg[1] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xC000:
    Map252_Reg[2] = (Map252_Reg[2] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 2 ] = VROMPAGE(Map252_Reg[2] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC004:
    Map252_Reg[2] = (Map252_Reg[2] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 2 ] = VROMPAGE(Map252_Reg[2] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC008:
    Map252_Reg[3] = (Map252_Reg[3] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 3 ] = VROMPAGE(Map252_Reg[3] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC00C:
    Map252_Reg[3] = (Map252_Reg[3] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 3 ] = VROMPAGE(Map252_Reg[3] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xD000:
    Map252_Reg[4] = (Map252_Reg[4] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 4 ] = VROMPAGE(Map252_Reg[4] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD004:
    Map252_Reg[4] = (Map252_Reg[4] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 4 ] = VROMPAGE(Map252_Reg[4] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD008:
    Map252_Reg[5] = (Map252_Reg[5] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 5 ] = VROMPAGE(Map252_Reg[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD00C:
    Map252_Reg[5] = (Map252_Reg[5] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 5 ] = VROMPAGE(Map252_Reg[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xE000:
    Map252_Reg[6] = (Map252_Reg[6] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 6 ] = VROMPAGE(Map252_Reg[6] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE004:
    Map252_Reg[6] = (Map252_Reg[6] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 6 ] = VROMPAGE(Map252_Reg[6] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE008:
    Map252_Reg[7] = (Map252_Reg[7] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 7 ] = VROMPAGE(Map252_Reg[7] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE00C:
    Map252_Reg[7] = (Map252_Reg[7] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 7 ] = VROMPAGE(Map252_Reg[7] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xF000:
    Map252_IRQ_Latch = (Map252_IRQ_Latch & 0xF0) | (byData & 0x0F);
    Map252_IRQ_Occur = 0;
    break;
  case 0xF004:
    Map252_IRQ_Latch = (Map252_IRQ_Latch & 0x0F) | ((byData & 0x0F) << 4);
    Map252_IRQ_Occur = 0;
    break;
    
  case 0xF008:
    Map252_IRQ_Enable = byData & 0x03;
    if( Map252_IRQ_Enable & 0x02 ) {
      Map252_IRQ_Counter = Map252_IRQ_Latch;
      Map252_IRQ_Clock = 0;
    }
    Map252_IRQ_Occur = 0;
    break;
    
  case 0xF00C:
    Map252_IRQ_Enable = (Map252_IRQ_Enable & 0x01) * 3;
    Map252_IRQ_Occur = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 252 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map252_HSync()
{
  if( Map252_IRQ_Enable & 0x02 ) {
    if( Map252_IRQ_Counter == 0xFF ) {
      Map252_IRQ_Occur = 0xFF;
      Map252_IRQ_Counter = Map252_IRQ_Latch;
      Map252_IRQ_Enable = (Map252_IRQ_Enable & 0x01) * 3;
    } else {
      Map252_IRQ_Counter++;
    }
  }
  if( Map252_IRQ_Occur ) {
    IRQ_REQ;
  }
}

/*===================================================================*/
/*                                                                   */
/*                     Mapper 255 : 110-in-1                         */
/*                                                                   */
/*===================================================================*/

BYTE    Map255_Reg[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 255                                            */
/*-------------------------------------------------------------------*/
void Map255_Init()
{
  /* Initialize Mapper */
  MapperInit = Map255_Init;

  /* Write to Mapper */
  MapperWrite = Map255_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map255_Apu;

  /* Read from APU */
  MapperReadApu = Map255_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Registers */
  InfoNES_Mirroring( 1 );

  Map255_Reg[0] = 0;
  Map255_Reg[1] = 0;
  Map255_Reg[2] = 0;
  Map255_Reg[3] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map255_Write( WORD wAddr, BYTE byData )
{
  BYTE	byPrg = (wAddr & 0x0F80)>>7;
  int	nChr = (wAddr & 0x003F);
  int	nBank = (wAddr & 0x4000)>>14;

  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }

  if( wAddr & 0x1000 ) {
    if( wAddr & 0x0040 ) {
      ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
  }
  
  PPUBANK[ 0 ] = VROMPAGE((0x200*nBank+nChr*8+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE((0x200*nBank+nChr*8+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE((0x200*nBank+nChr*8+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE((0x200*nBank+nChr*8+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE((0x200*nBank+nChr*8+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE((0x200*nBank+nChr*8+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE((0x200*nBank+nChr*8+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE((0x200*nBank+nChr*8+7) % (NesHeader.byVRomSize<<3));
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map255_Apu( WORD wAddr, BYTE byData )
{
  if( wAddr >= 0x5800 ) {
    Map255_Reg[wAddr&0x0003] = byData & 0x0F;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Read from APU Function                                */
/*-------------------------------------------------------------------*/
BYTE Map255_ReadApu( WORD wAddr )
{
  if( wAddr >= 0x5800 ) {
    return	Map255_Reg[wAddr&0x0003] & 0x0F;
  } else {
    return	wAddr>>8;
  }
}

/* End of InfoNES_Mapper.cpp */
