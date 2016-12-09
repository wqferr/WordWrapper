# Output file name
OUT := out


CC := gcc
SRC_PTRN := "*.c"

# Compile flags
C_FLAGS = -L$(LIB_DIR) -I$(INC_DIR)

# Tree customization
# Default tree structure:
#
# - Project directory
#   \_ Makefile - This file
#   \_ src/ - Source files (subdirectories included)
#   |  \_ .obj/ - Object files
#   |  \_ .dep/ - Dependency files
#   \_ include/ - Header files
#   \_ test/ - Test case files
#   \_ lib/ - Libraries
#   \_ build/ - Output directory

SRC_DIR := ./src
OBJ_DIR := $(SRC_DIR)/.obj
DEP_DIR := $(SRC_DIR)/.dep
INC_DIR := ./include
TST_DIR := ./test
LIB_DIR := ./lib
BLD_DIR := ./build


# File where the output of the last execution is saved
STDOUT_LOG := out.log

# Diff command
diff = vimdiff -MR $(1) $(2) 2> /dev/null

# Functions that return the input and output files associated with a test case
test_ifile = $(1:%=%.in)
test_ofile = $(1:%=%.out)


# END OF CUSTOM STUFF
# DON'T MESS WITH ANYTHING DOWN HERE


.PHONY: all run test clean stuff rebuild again tree destroy-tree-yes-i-am-sure\
		valgrind gdb g

# Find all source files
SOURCES := $(shell find $(SRC_DIR) -name $(SRC_PTRN) 2> /dev/null)


OBJECTS := $(SOURCES:%.c=%.o)
DEPS := $(SOURCES:%.c=%.d)

# Add $(OBJ_DIR)/ as a prefix to each object file bare name
OBJECTS := $(addprefix $(OBJ_DIR)/,$(notdir $(OBJECTS)))
DEPS := $(addprefix $(DEP_DIR)/,$(notdir $(DEPS)))

# Search path for make
# Allows use of pattern rules in
# directories discovered in runtime
SRC_SUBDIR := $(shell find $(SRC_DIR) -type d 2> /dev/null)
VPATH += $(SRC_SUBDIR)


debug_mode = 
ifneq (,$(findstring g,$(MAKECMDGOALS)))
debug_mode = yep
endif
ifneq (,$(findstring valgrind,$(MAKECMDGOALS)))
debug_mode = yep
endif
ifneq (,$(findstring gdb,$(MAKECMDGOALS)))
debug_mode = yep
endif
ifdef debug_mode
C_FLAGS += -g
endif

# You can define a file to be read as stdin by setting the command line
# variable `case`. The input and output files used are the ones returned by
# `test_ifile` and `test_ofile` respectively, and are looked for in the
# $(TST_DIR) subdirectory.
ifdef case
I_FILE := $(TST_DIR)/$(call test_ifile,$(case))
O_FILE := $(TST_DIR)/$(call test_ofile,$(case))


ifndef I_FILE
	$(error `test_ifile` function did not return a file name)
endif

ifndef O_FILE
	$(error `test_ofile` function did not return a file name)
endif

endif



all: $(BLD_DIR)/$(OUT)

g: clean all

run:
ifdef case
	@$(BLD_DIR)/$(OUT) < $(I_FILE)
else
	$(BLD_DIR)/$(OUT)
endif
	@printf "====================\n"

valgrind:
ifdef case
	@valgrind --leak-check=full $(BLD_DIR)/$(OUT) < $(I_FILE)
else
	@valgrind --leak-check=full $(BLD_DIR)/$(OUT)
endif
	@printf "====================\n"

gdb:
	@gdb (BLD_DIR)/$(OUT)
	@printf "====================\n"

test:
ifdef case
	@$(BLD_DIR)/$(OUT) < $(I_FILE) | tee $(STDOUT_LOG)
	@$(call diff,$(STDOUT_LOG),$(O_FILE))
else
	@$(error No test case given)
endif
	@printf "====================\n"


clean:
	-@rm -f $(OBJ_DIR)/*.o
	-@rm -f $(DEP_DIR)/*.d
	-@rm -f --preserve-root $(BLD_DIR)/*

stuff: all run

rebuild: clean all

again: rebuild run


$(BLD_DIR)/$(OUT): $(OBJECTS)
	@printf "Linking object files... "
	@$(CC) -o $(BLD_DIR)/$(OUT) $^
	@printf "\n"
	@printf "====================\n"
	@printf " COMPILING COMPLETE \n"
	@printf "====================\n\n"

$(OBJ_DIR)/%.o: %.c
	@printf "Building -%s-... " $(notdir $(basename $<))
	@$(CC) $(C_FLAGS) $(CFLAGS) -c -o $@ $<
	@printf "Done.\n"

$(DEP_DIR)/%.d: %.c
	@$(CC) $(C_FLAGS) -MM -MT'$(OBJ_DIR)/$(notdir $(@:%.d=%.o))' $< > $@

include $(DEPS)

.gitignore:
	@echo "$(OBJ_DIR:./%=/%)/*" >> .gitignore
	@echo "$(DEP_DIR:./%=/%)/*" >> .gitignore
	@echo "$(LIB_DIR:./%=/%)/*" >> .gitignore
	@echo "$(BLD_DIR:./%=/%)/*" >> .gitignore
	@echo "!**/.gitkeep" >> .gitignore

tree: .gitignore
	@printf "Creating project tree...\n"
	-@mkdir -p $(SRC_DIR)
	-@touch $(SRC_DIR)/.gitkeep
	@printf "%s directory created.\n" "Source"
	-@mkdir -p $(INC_DIR)
	-@touch $(INC_DIR)/.gitkeep
	@printf "%s directory created.\n" "Include"
	-@mkdir -p $(DEP_DIR)
	-@touch $(DEP_DIR)/.gitkeep
	@printf "%s directory created.\n" "Dependency"
	-@mkdir -p $(OBJ_DIR)
	-@touch $(OBJ_DIR)/.gitkeep
	@printf "%s directory created.\n" "Object"
	-@mkdir -p $(TST_DIR)
	-@touch $(TST_DIR)/.gitkeep
	@printf "%s directory created.\n" "Test case"
	-@mkdir -p $(LIB_DIR)
	-@touch $(LIB_DIR)/.gitkeep
	@printf "%s directory created.\n" "Library"
	-@mkdir -p $(BLD_DIR)
	-@touch $(BLD_DIR)/.gitkeep
	@printf "%s directory created.\n" "Build"
	@printf "Project tree complete.\n"
	@printf "======================\n\n"

destroy-tree-yes-i-am-sure:
	-@rm --preserve-root -rf $(SRC_DIR) $(INC_DIR) $(DEP_DIR) $(OBJ_DIR)
	-@rm --preserve-root -rf $(TST_DIR) $(LIB_DIR) $(BLD_DIR)
	-@rm -f $(STDOUT_LOG) .gitignore
	@printf "Too late to change your mind.\n"
	@printf "Goodbye project!\n"
