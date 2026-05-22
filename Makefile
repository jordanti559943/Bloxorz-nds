#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO")
endif

include $(DEVKITARM)/ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:= bloxorz
BUILD		:= build
SOURCES		:= source
INCLUDES	:= include
DATA		:= data

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:= -mthumb -mthumb-interwork

CFLAGS	:= -g -Wall -O2\
			-march=armv5te -mtune=arm9tdmi -fomit-frame-pointer\
			-ffast-math $(ARCH)

CFLAGS	+= $(INCLUDE) -DARM9

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:= -g $(ARCH)
LDFLAGS	:= -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lnds9

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(LIBNDS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:= $(CURDIR)/$(TARGET)
export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
				$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR	:= $(CURDIR)/$(BUILD)

COMBINED_FLAGS := $(CFLAGS) $(CXXFLAGS)

EXPORT_CFLAGS	:= $(CFLAGS) $(INCLUDE) -DARM9 -std=gnu99
EXPORT_CXXFLAGS	:= $(CXXFLAGS) $(INCLUDE) -DARM9
EXPORT_ASFLAGS	:= $(ASFLAGS) $(INCLUDE) -DARM9
EXPORT_LDFLAGS	:= $(LDFLAGS)
EXPORT_LIBDIRS	:= $(LIBDIRS)
EXPORT_LIBS	:= $(LIBS)

FILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.*)))
FILES += $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

EXPORT_SOURCES := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

.PHONY: $(BUILD) clean all

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).nds

#---------------------------------------------------------------------------------
else

DEPENDS	:= $(ASOBJS:.o=.d) $(COBJS:.o=.d) $(CPPOBJS:.o=.d) $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).nds	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use
#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -c $(EXPORT_CXXFLAGS) -o $@ $<

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -c $(EXPORT_CFLAGS) -o $@ $<

%.o: %.s
	@echo $(notdir $<)
	@$(AS) -c $(EXPORT_ASFLAGS) -o $@ $<

#---------------------------------------------------------------------------------
# This rule creates assembly source files using cpp
#---------------------------------------------------------------------------------
%.s : %.cpp
	@echo $(notdir $<)
	@$(CPP) -MMD -MP -MT $(@:.s=.o) $(EXPORT_CXXFLAGS) -x c++ -E -o $@ $<

%.s : %.c
	@echo $(notdir $<)
	@$(CPP) -MMD -MP -MT $(@:.s=.o) $(EXPORT_CFLAGS) -x c -E -o $@ $<

#---------------------------------------------------------------------------------
# Include the dependency files, if they exist
#---------------------------------------------------------------------------------
-include $(DEPENDS)

endif
