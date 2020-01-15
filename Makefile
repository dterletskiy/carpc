# AR           Программа работы с архивами; по умолчанию, `ar'. 
# AS           Ассемблер; по умолчанию, `as'. 
# CC           Компилятор Си; по умолчанию, `cc'. 
# CXX          Компилятор C++; по умолчанию, `g++'. 
# CO           Программа для извлечения файлов из RCS; по умолчанию, `co'. 
# CPP          Препроцессор языка Си, выдающий результат на стандартный вывод; по умолчанию, `$(CC) -E'. 
# FC           Препроцессор и компилятор для Фортрана и Ратфора; по умолчанию, `f77'. 
# GET          Программа для извлечения файлов из SCCS; по умолчанию, `get'. 
# LEX          Программа для преобразования Lex-грамматики в текст на языках Си или Ратфор; по умолчанию - `lex'. 
# PC           Компилятор Паскаля; по умолчанию, `pc'. 
# YACC         Программа для преобразования Yacc-грамматики в текст на Си; по умолчанию - `yacc'. 
# YACCR        Программа для преобразования Yacc-грамматики в текст на языке Ратфор; по умолчанию - `yacc -r'. 
# MAKEINFO     Программа для преобразования исходного файла формата Texinfo в файл формата Info; по умолчанию, `makeinfo'. 
# TEX          Программа для преобразования исходных файлов на TeX в файлы формата DVI; по умолчанию - `tex'. 
# TEXI2DVI     Программа, преобразующая исходные файлы в формате Texinfo, в DVI-файлы программы TeX; по умолчанию - `texi2dvi'. 
# WEAVE        Программа, преобразующая текст из формата Web в формат TeX; по умолчанию - `weave'. 
# CWEAVE       Программа, преобразующая текст на Си-Web в формат TeX; по умолчанию - `cweave'. 
# TANGLE       Программа, преобразующая текст на Web в Паскаль; по умолчанию - `tangle'. 
# CTANGLE      Программа, преобразующая текст на Си-Web в текст на Си; по умолчанию - `ctangle'. 
# RM           Команда удаления файла; по умолчанию, `rm -f'. 






# Debugger
DEBUGGER    := lldb
# tar
TAR         := tar

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
CCONST      := -DLINUX=0 -DWINDOWS=1 -DSTD=2 -DOS=LINUX
CCONFIG     := -DCOLORED_TRACE
# CCONFIG     += -DHOOK_MEMORY_ALLOCATOR
CFLAGS      := -fopenmp -g -std=c++17 $(CCONST) $(CCONFIG)
# CFLAGS      += -Wall -O3 -g
LDFLAGS     := -fopenmp -lrt -ldl
# LDFLAGS     += -lm -larmadillo
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
	@rm -rf $(BUILDDIR)
	@rm -rf $(TARGETDIR)
	@rm -rf $(PERFDIR)

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

info:
	reset
	@echo --------------- Source files ---------------
	@echo $(SOURCES)
	@echo --------------- Object files ---------------
	@echo $(OBJECTS)

# Copy Resources from Resources Directory to Target Directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

archive:
	@$(TAR) -cvf ../RPC_$(DATE)_$(TIME).tar ../RPC

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGETDIR)/$(TARGET) $^ $(LDFLAGS)

# Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<
	$(CXX) $(CFLAGS) $(INCDEP) -E $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(SRCEXT)
	@$(CXX) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# Non-File Targets
.PHONY: all build clean_build clean perf
