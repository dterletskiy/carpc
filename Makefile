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
SRC_DIR      := source
INC_DIR      := inc
RES_DIR      := resources
PRODUCT_DIR  := product
OBJ_DIR      := $(PRODUCT_DIR)/obj
GEN_DIR      := $(PRODUCT_DIR)/gen
TARGET_DIR   := $(PRODUCT_DIR)/exe
PERF_DIR     := $(PRODUCT_DIR)/perf
# File extentions
SRC_EXT      := cpp
DEP_EXT      := d
OBJ_EXT      := o

# Flags, Libraries and Includes
CCONST      := -DLINUX=0 -DWINDOWS=1 -DSTD=2 -DOS=LINUX
CCONFIG     := -DCOLORED_TRACE
# CCONFIG     += -DHOOK_MEMORY_ALLOCATOR
CFLAGS      := -fopenmp -g -std=c++17 $(CCONST) $(CCONFIG)
# CFLAGS      += -Wall -O3 -g
LDFLAGS     := -fopenmp -lrt -ldl
# LDFLAGS     += -lm -larmadillo
INC         := -I./$(SRC_DIR) -I./$(SRC_DIR)/framework -I./$(SRC_DIR)/framework/trace -I/usr/local/include
INCDEP      := -I./$(SRC_DIR) -I./$(SRC_DIR)/framework -I./$(SRC_DIR)/framework/trace

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS     := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:.$(SRC_EXT)=.$(OBJ_EXT)))



# Defauilt Make
all: config resources $(TARGET)

# Clean build
build: all

# Clean build
clean_build: clean all

# Full Clean, Objects and Binaries
clean:
	@rm -rf $(PRODUCT_DIR)

# Make the Directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(GEN_DIR)
	@mkdir -p $(TARGET_DIR)
	@mkdir -p $(PERF_DIR)

# execute
exec:
	reset
	$(TARGET_DIR)/$(TARGET)

perf:
	reset
	sudo perf timechart record $(TARGET_DIR)/$(TARGET)
	sudo mv -f perf.data $(PERF_DIR)/$(TARGET).data
	sudo perf timechart -i $(PERF_DIR)/$(TARGET).data -o $(PERF_DIR)/$(TARGET).svg

disassm:
	objdump -S --disassemble $(TARGET_DIR)/$(TARGET) > $(TARGET_DIR)/$(TARGET).s

debug:
	$(DEBUGGER) $(TARGET_DIR)/$(TARGET)

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
	@cp $(RES_DIR)/* $(TARGET_DIR)/

archive:
	@$(TAR) -cvf ../RPC_$(DATE)_$(TIME).tar ../RPC

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJ_EXT)=.$(DEP_EXT))

#Link
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET_DIR)/$(TARGET) $^ $(LDFLAGS)

# Compile
$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<
	$(CXX) $(CFLAGS) $(INCDEP) -MM $(SRC_DIR)/$*.$(SRC_EXT) > $(OBJ_DIR)/$*.$(DEP_EXT)
	@cp -f $(OBJ_DIR)/$*.$(DEP_EXT) $(OBJ_DIR)/$*.$(DEP_EXT).tmp
	@sed -e 's|.*:|$(OBJ_DIR)/$*.$(OBJ_EXT):|' < $(OBJ_DIR)/$*.$(DEP_EXT).tmp > $(OBJ_DIR)/$*.$(DEP_EXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJ_DIR)/$*.$(DEP_EXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJ_DIR)/$*.$(DEP_EXT)
	@rm -f $(OBJ_DIR)/$*.$(DEP_EXT).tmp

# Generate
$(GEN_DIR)/%.$(SRC_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INCDEP) -E $(SRC_DIR)/$*.$(SRC_EXT) > $(GEN_DIR)/$*.$(SRC_EXT)

# Non-File Targets
.PHONY: all build clean_build clean perf
