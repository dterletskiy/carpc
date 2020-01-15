# Compiler and Linker
CC          := g++
# Debugger
DEBUGGER 	:= lldb
# tar
TAR 			:= tar

TIME := $(shell date +"%H-%M-%S")
DATE := $(shell date +"%Y-%m-%d")

# The Target Binary Program
TARGET      := program

# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := source
INCDIR      := inc
BUILDDIR    := build
TARGETDIR   := exe
RESDIR      := resources
PERFDIR     := perf
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

# Flags, Libraries and Includes
CCONST		:= -DLINUX=0 -DWINDOWS=1 -DSTD=2 -DOS=LINUX
CCONFIG		:= -DCOLORED_TRACE
# CCONFIG		+= -DHOOK_MEMORY_ALLOCATOR
CFLAGS      := -fopenmp -g -std=c++17 $(CCONST) $(CCONFIG)
# CFLAGS      += -Wall -O3 -g
LIB         := -fopenmp -lrt -ldl
# LIB         += -lm -larmadillo
INC         := -I./$(SRCDIR) -I./$(SRCDIR)/trace -I/usr/local/include
INCDEP      := -I./$(SRCDIR) -I./$(SRCDIR)/trace

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))



# Defauilt Make
all: config resources $(TARGET)

# Clean build
build: all

# Clean build
clean_build: clean all

# Full Clean, Objects and Binaries
clean:
	@$(RM) -rf $(BUILDDIR)
	@$(RM) -rf $(TARGETDIR)
	@$(RM) -rf $(PERFDIR)

# execute
exec:
	reset
	$(TARGETDIR)/$(TARGET)

perf:
	reset
	@mkdir -p $(PERFDIR)
	sudo perf timechart record $(TARGETDIR)/$(TARGET)
	sudo mv -f perf.data $(PERFDIR)/$(TARGET).data
	sudo perf timechart -i $(PERFDIR)/$(TARGET).data -o $(PERFDIR)/$(TARGET).svg

disassm:
	objdump -S --disassemble $(TARGETDIR)/$(TARGET) > $(TARGETDIR)/$(TARGET).s

debug:
	$(DEBUGGER) $(TARGETDIR)/$(TARGET)

# Set configuration
config:
	reset

# Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

archive:
	$(TAR) -cvf ../RPC_$(DATE)_$(TIME).tar ../

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

# Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	$(CC) $(CFLAGS) $(INCDEP) -E $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(SRCEXT)
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# Non-File Targets
.PHONY: all build clean_build clean perf
