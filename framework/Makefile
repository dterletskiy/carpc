include ../Makefile.variables


###########################################
#                                         #
#         Framework configuration         #
#                                         #
###########################################
# The Directories, Source, Includes, Objects, Binary
SOURCE_DIR       := $(PROJECT_DIR)
OBJECT_DIR       := $(PRODUCT_DIR)/obj
LIBRARY_DIR      := $(PRODUCT_DIR)/lib
GENERATED_DIR    := $(PRODUCT_DIR)/gen
TARGET_DIR       := $(PRODUCT_DIR)/exe
PERF_DIR         := $(PRODUCT_DIR)/perf

INCLUDE          += -I$(SOURCE_DIR) -I$(SOURCE_DIR)/trace
# CFLAGS           += -fPIC

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCE_FILES     := $(shell find $(SOURCE_DIR) -type f -name *.$(SRC_EXT))
OBJECT_FILES     := $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(OBJ_EXT)))
GENERATED_FILES  := $(patsubst $(SOURCE_DIR)/%,$(GENERATED_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(SRC_EXT)))



# Generate
$(GENERATED_DIR)/%.$(SRC_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call generate,$(CXX),$(CFLAGS),$(INCLUDE),$@,$<)

# Compile
$(OBJECT_DIR)/%.$(OBJ_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call compile,$(CXX),$(CFLAGS),$(INCLUDE),$@,$<)



config:
	$(call create_directories,$(LIBRARY_DIR) $(OBJECT_DIR) $(GENERATED_DIR))

clean:
	$(call delete_directories,$(PRODUCT_DIR))

generate: config $(GENERATED_FILES)

build: config $(OBJECT_FILES)
	ar rcs $(LIBRARY_DIR)/lib$(PROJECT_NAME).a $(OBJECT_FILES)
# 	variant 1
# gcc -g -shared -Wl,-soname,libhello.so.0 -o libhello.so.0.0 libhello.o -lc
# 	variant 2
# 	g++ -g -shared -Wl,-soname,lib$(PROJECT_NAME).so.0 -o $(LIBRARY_DIR)/lib$(PROJECT_NAME).so.0.0 $(OBJECT_FILES) $(LDFLAGS)
# 	ldconfig -v -n $(LIBRARY_DIR)
# 	ln -sf $(LIBRARY_DIR)/lib$(PROJECT_NAME).so.0 $(LIBRARY_DIR)/lib$(PROJECT_NAME).so

clean_build: clean build

archive:
	@$(TAR) -cvf $(SOURCE_DIR)/../$(PROJECT_NAME)_$(DATE)_$(TIME).tar $(SOURCE_DIR)

infi:
	$(call print_info,$(PROJECT_NAME),$(PROJECT_DIR),$(PRODUCT_DIR),$(SOURCE_FILES),$(GENERATED_FILES),$(OBJECT_FILES))



# Non-File Targets
.PHONY: all config build clean clean_build info archive
