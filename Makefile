TARGET		:= bloxorz
BUILD		:= build
SOURCES		:= .
DATA		:= .
INCLUDES	:= .

ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	:=	-g -Wall -O2 \
			$(ARCH)

CXXFLAGS	:= $(CFLAGS)

LDFLAGS	=	-g $(ARCH) -specs=ds_arm9.specs

LIBS	:= -lnds9

LIBDIRS	:= $(LIBNDS)

export TARGET BUILD SOURCES DATA INCLUDES LIBDIRS

export ARCH CFLAGS CXXFLAGS LDFLAGS

include $(DEVKITPRO)/libnds/default_arm9_rules
include $(DEVKITPRO)/devkitARM/ds_rules
