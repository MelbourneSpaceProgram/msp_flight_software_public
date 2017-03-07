#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;C:/ti/ccsv7/ccs_base;C:/Users/Anthony/OwnCloud/UMSP/SVN/operating-systems/software/MSP/.config
override XDCROOT = C:/ti/tirex-content/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;C:/ti/tirex-content/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;C:/ti/ccsv7/ccs_base;C:/Users/Anthony/OwnCloud/UMSP/SVN/operating-systems/software/MSP/.config;C:/ti/tirex-content/xdctools_3_32_00_06_core/packages;..
HOSTOS = Windows
endif
