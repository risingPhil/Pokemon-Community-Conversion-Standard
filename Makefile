# Get the directory of this Makefile
export MKFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
# -------- Directory layout --------
BUILDDIR := $(MKFILE_DIR)/build
OBJDIR   := $(BUILDDIR)
LIBDIR   := $(MKFILE_DIR)
DATA	 := $(MKFILE_DIR)/data

#---------------------------------------------------------------------------------
# canned command sequence for binary data
# Taken from https://github.com/devkitPro/devkitarm-rules/blob/master/base_tools
#---------------------------------------------------------------------------------
define bin2o
	$(eval _tmpasm := $(shell mktemp))
	$(SILENTCMD)bin2s -a 4 -H `(echo $(<F) | tr . _)`.h $< > $(_tmpasm)
	$(SILENTCMD)$(CC) -x assembler-with-cpp $(CPPFLAGS) $(ASFLAGS) -c $(_tmpasm) -o $(<F).o
	@rm $(_tmpasm)
endef

ifneq (build,$(notdir $(CURDIR)))
# -------- Cleanup --------
# -------- Top-level targets --------
.PHONY: all lib clean dirs symlinks
all: dirs generate_tables lib

lib:
	@$(MAKE) -C build -f $(MKFILE_DIR)/Makefile

# Ensure the build directories exist
dirs:
	@mkdir -p $(OBJDIR) $(LIBDIR)

generate_tables:
	mkdir -p data
	mkdir -p to_compress
	@env -i "PATH=$(PATH)" $(MAKE) -C tools/table-generator
	@echo
	@echo "----------------------------------------------------------------"
	@echo
	@tools/table-generator/table-generator to_compress
ifeq ($(CXX),arm-none-eabi-g++)
	@echo "Compressing bin files for GBA!"
	@echo -n "["
	@find to_compress -name "*.bin" -print0 | xargs -0 -n1 ./compress_lz10.sh
	@echo "]"
	@echo "Compressing finished!"
else
	rmdir data
	mv to_compress data
endif
	@echo
	@echo "----------------------------------------------------------------"
	@echo

clean:
	$(MAKE) -C tools/table-generator clean
	rm -rf $(BUILDDIR) $(MKFILE_DIR)/data $(MKFILE_DIR)/to_compress $(LIBDIR)/*.a $(LIBDIR)/*.so*

else
# -------- Toolchain defaults (can be overridden by parent) --------
CC       ?= gcc
CXX      ?= g++
AR       ?= ar
CFLAGS   ?= -O2 -fPIC
CXXFLAGS ?= $(CFLAGS)
LDFLAGS  ?=
SOFLAGS  ?= -shared

# -------- Library name + versioning --------
TARGET         := pccs
MAJOR_VERSION  := 1
MINOR_VERSION  := 0
FULL_VERSION   := $(MAJOR_VERSION).$(MINOR_VERSION)

STATIC_LIB     := lib$(TARGET).a
SHARED_REAL    := lib$(TARGET).so.$(FULL_VERSION)
SHARED_MAJOR   := lib$(TARGET).so.$(MAJOR_VERSION)
SHARED_UNVER   := lib$(TARGET).so

# -------- Automatic source scanning --------
C_SOURCES   := $(wildcard $(MKFILE_DIR)/source/*.c)
CPP_SOURCES := $(wildcard $(MKFILE_DIR)/source/*.cpp)
SOURCES     := $(C_SOURCES) $(CPP_SOURCES)
INCLUDES    := $(MKFILE_DIR)/include $(INCLUDE) -I$(OBJDIR)
BINFILES	:= $(wildcard $(DATA)/*.bin)

BINOFILES := $(addprefix $(OBJDIR)/,$(notdir $(BINFILES:.bin=.bin.o)))

# create object paths
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJS += $(addprefix $(OBJDIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
OBJS += $(BINOFILES)
#OBJS += $(patsubst $(DATA)/%.bin,$(OBJDIR)/%.bin.o,$(BINFILES))

VPATH += $(MKFILE_DIR)/source $(DATA)

$(info CURDIR = $(CURDIR))
$(info OBJDIR = $(OBJDIR))
$(info C_SOURCES = $(C_SOURCES))
$(info CPP_SOURCES = $(CPP_SOURCES))
$(info BINFILES = $(BINFILES))
$(info OBJS = $(OBJS))

# -------- Top-level targets --------
.PHONY: all symlinks
all: $(LIBDIR)/$(STATIC_LIB) $(LIBDIR)/$(SHARED_REAL) symlinks


# -------- Static library --------
$(LIBDIR)/$(STATIC_LIB): $(BINOFILES) $(OBJS)
	$(AR) rcs $@ $(OBJS)


# -------- Shared library (real file with full version) --------
$(LIBDIR)/$(SHARED_REAL): $(BINOFILES) $(OBJS)
	$(CXX) $(SOFLAGS) $(LDFLAGS) \
	       -Wl,-soname,$(SHARED_MAJOR) \
	       -o $@ $(OBJS)


# -------- Soname symlinks --------
symlinks: $(LIBDIR)/$(SHARED_REAL)
	ln -sf $(SHARED_REAL) $(LIBDIR)/$(SHARED_MAJOR)
	ln -sf $(SHARED_MAJOR) $(LIBDIR)/$(SHARED_UNVER)


# -------- Object file rules --------
$(OBJDIR)/%.bin.o $(OBJDIR)/%.bin.h: %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDES) -c -o $@ $<

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -c -o $@ $<

endif