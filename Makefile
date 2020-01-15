include ./Makefile.variables




all: 
	reset
	@echo "Common target is not defined."
	@echo "Please, specify make target."

config:
	reset



FRAMEWORK_MAKE_PARAMS     := PROJECT_NAME=$(PROJECT_FRAMEWORK_NAME) PROJECT_DIR=$(PROJECT_FRAMEWORK_DIR) PRODUCT_DIR=$(PRODUCT_FRAMEWORK_DIR)

$(PROJECT_FRAMEWORK_NAME)_$(TARGET_BUILD):
	$(MAKE) -C $(PROJECT_FRAMEWORK_DIR) $(TARGET_BUILD) $(FRAMEWORK_MAKE_PARAMS)

$(PROJECT_FRAMEWORK_NAME)_$(TARGET_GENERATE):
	$(MAKE) -C $(PROJECT_FRAMEWORK_DIR) $(TARGET_GENERATE) $(FRAMEWORK_MAKE_PARAMS)

$(PROJECT_FRAMEWORK_NAME)_$(TARGET_CLEAN):
	$(MAKE) -C $(PROJECT_FRAMEWORK_DIR) $(TARGET_CLEAN) $(FRAMEWORK_MAKE_PARAMS)

$(PROJECT_FRAMEWORK_NAME)_$(TARGET_INFO):
	$(MAKE) -C $(PROJECT_FRAMEWORK_DIR) $(TARGET_INFO) $(FRAMEWORK_MAKE_PARAMS)

$(PROJECT_FRAMEWORK_NAME)_$(TARGET_ARCHIVE):
	$(MAKE) -C $(PROJECT_FRAMEWORK_DIR) $(TARGET_ARCHIVE) $(FRAMEWORK_MAKE_PARAMS)



APPLICATION_MAKE_PARAMS     := PROJECT_NAME=$(PROJECT_APPLICATION_NAME) PROJECT_DIR=$(PROJECT_APPLICATION_DIR) PRODUCT_DIR=$(PRODUCT_APPLICATION_DIR)

$(PROJECT_APPLICATION_NAME)_$(TARGET_BUILD):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_BUILD) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_GENERATE):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_GENERATE) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_CLEAN):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_CLEAN) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_INFO):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_INFO) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_ARCHIVE):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_ARCHIVE) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_EXECUTE):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_EXECUTE) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APPLICATION_NAME)_$(TARGET_DEBUG):
	$(MAKE) -C $(PROJECT_APPLICATION_DIR) $(TARGET_DEBUG) $(APPLICATION_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_DISASSM):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_DISASSM) $(APP0_MAKE_PARAMS)



SERVICEBROCKER_MAKE_PARAMS   := PROJECT_NAME=$(PROJECT_SERVICEBROCKER_NAME) PROJECT_DIR=$(PROJECT_SERVICEBROCKER_DIR) PRODUCT_DIR=$(PRODUCT_SERVICEBROCKER_DIR)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_BUILD):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_BUILD) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_GENERATE):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_GENERATE) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_CLEAN):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_CLEAN) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_INFO):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_INFO) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_ARCHIVE):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_ARCHIVE) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_EXECUTE):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_EXECUTE) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_DEBUG):
	$(MAKE) -C $(PROJECT_SERVICEBROCKER_DIR) $(TARGET_DEBUG) $(SERVICEBROCKER_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_DISASSM):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_DISASSM) $(APP0_MAKE_PARAMS)



HOOKS_MAKE_PARAMS   := PROJECT_NAME=$(PROJECT_HOOKS_NAME) PROJECT_DIR=$(PROJECT_HOOKS_DIR) PRODUCT_DIR=$(PRODUCT_HOOKS_DIR)

$(PROJECT_HOOKS_NAME)_$(TARGET_BUILD):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_BUILD) $(HOOKS_MAKE_PARAMS)

$(PROJECT_HOOKS_NAME)_$(TARGET_GENERATE):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_GENERATE) $(HOOKS_MAKE_PARAMS)

$(PROJECT_HOOKS_NAME)_$(TARGET_CLEAN):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_CLEAN) $(HOOKS_MAKE_PARAMS)

$(PROJECT_HOOKS_NAME)_$(TARGET_INFO):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_INFO) $(HOOKS_MAKE_PARAMS)

$(PROJECT_HOOKS_NAME)_$(TARGET_ARCHIVE):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_ARCHIVE) $(HOOKS_MAKE_PARAMS)

$(PROJECT_HOOKS_NAME)_$(TARGET_EXECUTE):
	$(MAKE) -C $(PROJECT_HOOKS_DIR) $(TARGET_EXECUTE) $(HOOKS_MAKE_PARAMS)



APP0_MAKE_PARAMS   := PROJECT_NAME=$(PROJECT_APP0_NAME) PROJECT_DIR=$(PROJECT_APP0_DIR) PRODUCT_DIR=$(PRODUCT_APP0_DIR)

$(PROJECT_APP0_NAME)_$(TARGET_BUILD):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_BUILD) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_GENERATE):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_GENERATE) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_CLEAN):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_CLEAN) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_INFO):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_INFO) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_ARCHIVE):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_ARCHIVE) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_EXECUTE):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_EXECUTE) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_DEBUG):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_DEBUG) $(APP0_MAKE_PARAMS)

$(PROJECT_APP0_NAME)_$(TARGET_DISASSM):
	$(MAKE) -C $(PROJECT_APP0_DIR) $(TARGET_DISASSM) $(APP0_MAKE_PARAMS)





$(TARGET_BUILD): config \
						$(PROJECT_HOOKS_NAME)_$(TARGET_BUILD) \
						$(PROJECT_FRAMEWORK_NAME)_$(TARGET_BUILD) \
						$(PROJECT_APPLICATION_NAME)_$(TARGET_BUILD) \
						$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_BUILD) \
						$(PROJECT_APP0_NAME)_$(TARGET_BUILD) \

$(TARGET_GENERATE): config \
						$(PROJECT_HOOKS_NAME)_$(TARGET_GENERATE) \
						$(PROJECT_FRAMEWORK_NAME)_$(TARGET_GENERATE) \
						$(PROJECT_APPLICATION_NAME)_$(TARGET_GENERATE) \
						$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_GENERATE) \
						$(PROJECT_APP0_NAME)_$(TARGET_GENERATE) \

$(TARGET_CLEAN): config \
						$(PROJECT_HOOKS_NAME)_$(TARGET_CLEAN) \
						$(PROJECT_FRAMEWORK_NAME)_$(TARGET_CLEAN) \
						$(PROJECT_APPLICATION_NAME)_$(TARGET_CLEAN) \
						$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_CLEAN) \
						$(PROJECT_APP0_NAME)_$(TARGET_CLEAN) \

$(TARGET_INFO): config \
						$(PROJECT_HOOKS_NAME)_$(TARGET_INFO) \
						$(PROJECT_FRAMEWORK_NAME)_$(TARGET_INFO) \
						$(PROJECT_APPLICATION_NAME)_$(TARGET_INFO) \
						$(PROJECT_SERVICEBROCKER_NAME)_$(TARGET_INFO) \
						$(PROJECT_APP0_NAME)_$(TARGET_INFO) \

$(TARGET_ARCHIVE): config
	@$(TAR) -cvf $(ROOT_DIR)/../$(PROJECT_NAME)_$(DATE)_$(TIME).tar $(ROOT_DIR)

$(TARGET_EXECUTE):
	reset
	@echo "'$(TARGET_EXECUTE)' target is not defined."
	@echo "Please, specify make target."

$(TARGET_DEBUG):
	reset
	@echo "'$(TARGET_DEBUG)' target is not defined."
	@echo "Please, specify make target."



# Non-File Targets
.PHONY: all
