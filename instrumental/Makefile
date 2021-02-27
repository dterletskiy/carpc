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

INCLUDE          += -I$(SOURCE_DIR) -I$(PROJECT_FRAMEWORK_DIR)
LDFLAGS          += -L$(PRODUCT_FRAMEWORK_DIR)/lib -lframework

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCE_FILES         := $(shell find $(SOURCE_DIR) -type f -name *.$(SRC_EXT))
OBJECT_FILES         := $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(OBJ_EXT)))
OBJECT_PIC_FILES     := $(patsubst $(SOURCE_DIR)/%,$(OBJECT_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(OBJ_PIC_EXT)))
GENERATED_FILES      := $(patsubst $(SOURCE_DIR)/%,$(GENERATED_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(SRC_EXT)))
GENERATED_PIC_FILES  := $(patsubst $(SOURCE_DIR)/%,$(GENERATED_DIR)/%,$(SOURCE_FILES:.$(SRC_EXT)=.$(SRC_PIC_EXT)))



# Generate
$(GENERATED_DIR)/%.$(SRC_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call generate,$(CXX),$(CFLAGS),$(INCLUDE),$@,$<)

# Compile
$(OBJECT_DIR)/%.$(OBJ_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call compile,$(CXX),$(CFLAGS),$(INCLUDE),$@,$<)

# Link
$(PROJECT_NAME): $(OBJECT_FILES)
	ar rcs $(LIBRARY_DIR)/lib$(PROJECT_NAME).a $(OBJECT_FILES)

# Generate PIC
$(GENERATED_DIR)/%.$(SRC_PIC_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call generate,$(CXX),$(CFLAGS) -fPIC,$(INCLUDE),$@,$<)

# Compile PIC
$(OBJECT_DIR)/%.$(OBJ_PIC_EXT): $(SOURCE_DIR)/%.$(SRC_EXT)
	$(call compile,$(CXX),$(CFLAGS) -fPIC,$(INCLUDE),$@,$<)

#Link PIC
$(PROJECT_NAME)_PIC: $(OBJECT_PIC_FILES)
	$(call link,$(CXX),-g -shared,$(LIBRARY_DIR)/lib$(PROJECT_NAME).so,$^)



config:
	$(call create_directories,$(LIBRARY_DIR) $(OBJECT_DIR) $(GENERATED_DIR) $(TARGET_DIR) $(GENERATED_DIR))

clean:
	$(call delete_directories,$(PRODUCT_DIR))

generate: config $(GENERATED_FILES)

build: config $(PROJECT_NAME) $(PROJECT_NAME)_PIC

clean_build: clean build

archive:
	@$(TAR) -cvf $(SOURCE_DIR)/../$(PROJECT_NAME)_$(DATE)_$(TIME).tar $(SOURCE_DIR)

info:
	$(call print_info,$(PROJECT_NAME),$(PROJECT_DIR),$(PRODUCT_DIR),$(SOURCE_FILES),$(GENERATED_FILES),$(OBJECT_FILES),$(OBJECT_PIC_FILES))



# Non-File Targets
.PHONY: all config build clean clean_build info archive
