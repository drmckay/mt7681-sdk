#*******************************************************************************
#* MODULE NAME:     flags.mak
#* PROJECT CODE:
#* DESCRIPTION:
#* DESIGNER:        Charles Su
#* DATE:            Aug 2011
#*
#* SOURCE CONTROL:
#*
#* LICENSE:
#*     This source code is copyright (c) 2011 Ralink Tech. Inc.
#*     All rights reserved.
#*
#* REVISION     HISTORY:
#*   V1.0.0     Aug 2011    - Initial Version V1.0
#*
#*
#* SOURCE:
#* ISSUES:
#*    First Implementation.
#* NOTES TO USERS:
#*
#*******************************************************************************



#*******************************************************************************
# BUILD FLAGS
#*******************************************************************************
BFLAGS	 =
#			Uncomment this flag if try to build ROM code
BFLAGS	+=	-DROM_EXISTED=1
#			Support single antenna auto-adaption.
BFLAGS	+=	-DSINGLE_ANTENNA=1
#			Including $P0 and $P1 in CPU context
BFLAGS	+=	-DCTX_SUP_P0P1=0
#			Including $FP in CPU context
BFLAGS	+=	-DCTX_SUP_FP=1
#			Including $GP in CPU context
BFLAGS	+=	-DCTX_SUP_GP=1
#			Including $d0 and $d1 in CPU context
BFLAGS	+=	-DCTX_SUP_D0D1=0
#			Reset Bluetooth clock during booting
BFLAGS	+=	-DRST_CLK=1
#			Enable ILM
BFLAGS	+=	-DILM_ENA=0
#			Enable DLM
BFLAGS	+=	-DDLM_ENA=0
BFLAGS	+=	-DBUILD_LE
#			Reduced Register Setting
BFLAGS	+=	-DREDUCED_REG=1

#*******************************************************************************
# COMPILER FLAGS
#*******************************************************************************
CFLAGS	 =
#			Enable most warning messages
CFLAGS  +=	-g
CFLAGS	+=	-Wall
CFLAGS	+=	-fno-builtin
CFLAGS	+=	-ffunction-sections
CFLAGS	+=	-fno-strict-aliasing

#*******************************************************************************
# TARGET FLAGS
#*******************************************************************************
TFLAGS	 =
#	Generate 16/32-bit mixed instructions
TFLAGS	+=	-m16bit
#	Not generate 16/32-bit mixed instructions
#TFLAGS	+=	-mno-16bit
# Specify which ABI type to generate code for: 1, 2, and 2fp. The default is 2.
TFLAGS	+=	-mabi=2
# Specify which baseline type to generate code for: V1, V2. The default is V2.
TFLAGS	+=	-mbaseline=V2                 
# Specify which CPU to generate code for: n1213_43u1h, n1213_30t2g, n12, n10, n9. The default is n10.
TFLAGS	+=	-mcpu=n9
#	Generate integer div instructions using register $d0/$d1
#TFLAGS	+=	-mdiv
#	Not generate integer div instructions using register $d0/$d1
TFLAGS	+=	-mno-div
#	Use little-endian byte order
TFLAGS	+=	-mel
# Specify a max count of moving multiple words from 4 to 8. The default is 4.
TFLAGS	+=	-mmw-count=8
#	Not use 16 registers to generate code
#TFLAGS	+=	-mno-reduce-regs
#	Use 16 registers to generate code
#TFLAGS +=	-mreduce-regs
# Not generate multiply with accumulation instructions using register $d0/$d1	
TFLAGS +=	-mno-ext-mac
#	Not generate instructions relative to dx registers
TFLAGS +=	-mno-dx-regs



#*******************************************************************************
# ASM FLAGS
#*******************************************************************************
ASFLAGS	 =



#*******************************************************************************
# OPTIMIZATION FLAGS
#*******************************************************************************
OFLAGS	 =
#			Optimize for space rather than speed
OFLAGS	+=	-Os
#OFLAGS	+=	-O3
#			Align the start of functions
OFLAGS	+=	-falign-functions
#			Align labels which are only reached by jumping
OFLAGS	+=	-falign-jumps
#			Align all labels
OFLAGS	+=	-falign-labels
#			Align the start of loops
OFLAGS	+=	-falign-loops
#			Save registers around function calls
OFLAGS	+=	-fcaller-saves
#			Place data items into their own section
OFLAGS	+=	-fdata-sections
#			Integrate simple functions into their callers
OFLAGS	+=	-finline-functions
#			Integrate functions called once into their callers
#OFLAGS	+=	-finline-functions-called-once
#			Integrate simple functions into their callers when code size is known to not growth
OFLAGS	+=	-finline-small-functions
#			Use jump tables for sufficiently large switch statements
#OFLAGS	+=	-fjump-tables
#			When possible do not generate stack frames
OFLAGS	+=	-fomit-frame-pointer
#			Do the full register move optimization pass
OFLAGS	+=	-foptimize-register-move
#			Optimize sibling and tail recursive calls
OFLAGS	+=	-foptimize-sibling-calls
#			Enable dominator optimizations
OFLAGS	+=	-ftree-dominator-opts
#			Save registers around function calls
OFLAGS	+=	-fcaller-saves
#			Enables a register move optimization
OFLAGS	+=	-fregmove
#			Perform whole program optimizations
#OFLAGS	+=	-fwhole-program
#			Perform loop unrolling for all loops
OFLAGS	+=	-funroll-all-loops
OFLAGS	+=	-funroll-loops


#*******************************************************************************
# MAKE LIB FLAGS
#*******************************************************************************
ARFLAGS	 =
#ARFLAGS += r



#*******************************************************************************
# LD FLAGS
#*******************************************************************************
LFLAGS	 =
#			Link little-endian objects
LFLAGS	+=	-EL
#			Ignored
LFLAGS	+=	-g
#			Do not link against shared libraries
LFLAGS	+=	-Bstatic
#			Bind global references locally
LFLAGS	+=	-Bsymbolic
#			Bind global function references locally
LFLAGS	+=	-Bsymbolic-functions
#			Check section addresses for overlaps (default)
LFLAGS	+=	--check-sections
#			Remove unused sections (on some targets)
LFLAGS	+=	--gc-sections
#			Don't remove unused sections (default)
#LFLAGS	+=	--no-gc-sections



#*******************************************************************************
# DEBUG FLAGS
#*******************************************************************************
DBGFLAGS	 =
DBGFLAGS	+=	-DDBG_ENA=1
DBGFLAGS  +=  -DDBG_TRAP=0
DBGFLAGS	+=	-DDBG_ASSERT=0
DBGFLAGS	+=	-DSIM_RTL=0
DBGFLAGS	+=	-DSIM_ADS=0
DBGFLAGS	+=	-DDBG_FPGA=1
DBGFLAGS	+=	-DDBG_MICTOR=0


#*******************************************************************************
# FUNCTION FLAGS
#*******************************************************************************

# BSP SETTING
FUNCFLAGS	=	-D__MT7681
FUNCFLAGS	+=	-DUART_SUPPORT=1

# WIFI/BT FUNCTIONS 
FUNCFLAGS	+=	-DBT_SUPPORT=0
FUNCFLAGS	+=	-DWIFI_SUPPORT=1
FUNCFLAGS	+=	-DUSB_SUPPORT=0
FUNCFLAGS	+=	-DSCRIPT_EXE_SUPPORT=0
# WIN FEATURES FLAGS
FUNCFLAGS	+=	-DRADAR_DETEC=0
FUNCFLAGS	+=	-DRESP_WOWPKT=0
FUNCFLAGS	+=	-DWOW_SUPPORT=0
FUNCFLAGS	+=	-DNLO_SUPPORT=0
FUNCFLAGS	+=	-DBITMAP_PATT=0
FUNCFLAGS	+=	-DARP_OFFLOAD=0
FUNCFLAGS	+=	-DCOL_SUPPORT=0
FUNCFLAGS	+=	-DWINDOWS8=0
FUNCFLAGS	+=	-DADV_PWR_SAVING=0
FUNCFLAGS	+=	-DDOT11W_PMF_SUPPORT=0
FUNCFLAGS	+=	-DEP_CFM=1
FUNCFLAGS	+=	-DATCMD_SUPPORT=1
FUNCFLAGS	+=	-DATCMD_TCPIP_SUPPORT=0
FUNCFLAGS	+=	-DATCMD_UART_SUPPORT=0
FUNCFLAGS	+=	-DCRYPTWEP_SUPPORT=1
FUNCFLAGS	+=	-DCFG_SUPPORT_TCPIP=1
FUNCFLAGS	+=	-DCFG_SUPPORT_4WAY_HS=1
FUNCFLAGS	+=	-DDBG_MSG=0
FUNCFLAGS	+=	-DDBG_MSG_HIGH=1
#END-OF-FILE#
