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
# CFLAGS           += -fPIC
LDFLAGS          += -L$(PRODUCT_FRAMEWORK_DIR)/lib -lframework

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
	$(call create_directories,$(LIBRARY_DIR) $(OBJECT_DIR) $(GENERATED_DIR) $(TARGET_DIR))

$(TARGET_CLEAN):
	$(call delete_directories,$(PRODUCT_DIR))

$(TARGET_GENERATE): config $(GENERATED_FILES)

$(TARGET_BUILD): config $(OBJECT_FILES)
# 	g++ -shared -o $(LIBRARY_DIR)/lib$(PROJECT_NAME).so $(OBJECT_FILES) $(LDFLAGS)
	ar rcs $(LIBRARY_DIR)/lib$(PROJECT_NAME).a $(OBJECT_FILES)

clean_build: $(TARGET_CLEAN) $(TARGET_BUILD)

$(TARGET_INFO):
	@echo "###########################################################"
	@echo "#"
	@echo "# information:  '$(PROJECT_NAME)'"
	@echo "#"
	@echo "###########################################################"
	@echo PWD: $(PWD)
	@echo PROJECT_NAME: $(PROJECT_NAME)
	@echo PROJECT_DIR: $(PROJECT_DIR)
	@echo PRODUCT_DIR: $(PRODUCT_DIR)
	@echo INCLUDE: $(INCLUDE)
	@echo --------------- Source files ---------------
	@echo $(SOURCE_FILES)
	@echo -------------- Generated files -------------
	@echo $(GENERATED_FILES)
	@echo --------------- Object files ---------------
	@echo $(OBJECT_FILES)
	@echo --------------------------------------------

$(TARGET_ARCHIVE):
	@$(TAR) -cvf $(SOURCE_DIR)/../$(PROJECT_NAME)_$(DATE)_$(TIME).tar $(SOURCE_DIR)



# Non-File Targets
.PHONY: all config build clean clean_build info archive
