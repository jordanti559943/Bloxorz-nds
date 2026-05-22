TARGET      := bloxorz
BUILD       := build
SOURCES     := .
DATA        := .
INCLUDES    := .

ARCH        := -mthumb -mthumb-interwork

CFLAGS      := -g -Wall -O2 $(ARCH)
CXXFLAGS    := $(CFLAGS)

LDFLAGS     := -specs=ds_arm9.specs $(ARCH)

LIBS        := -lnds9

LIBDIRS     := $(LIBNDS)

export TARGET BUILD SOURCES DATA INCLUDES ARCH
export CFLAGS CXXFLAGS LDFLAGS LIBS LIBDIRS

include $(DEVKITPRO)/libnds/default_arm9_rules
