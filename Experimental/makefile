#Config
#COMPILE_FLAGS = -g #DEBUG
COMPILE_FLAGS = -O3 #RELEASE
LINK_FLAGS = -lwsock32
TARGET_NAME=FruitDetectionService.exe
#HIDE_CONSOLE= -mwindows #uncomment command to hide console 
LANG_VERSION= -std=c++23
CC=g++ -m64
SHELL=cmd



#automatic path finding
BLANK :=
SLASH = \$(BLANK)
SRC_DIR=src/
TARGET_DIR=target/
TARGET_DIR_WIN=target$(SLASH)
OBJ_DIR=$(TARGET_DIR)ObjectFiles/
OBJ_DIR_WIN=$(TARGET_DIR_WIN)ObjectFiles$(SLASH)
TARGET_EXE=$(TARGET_DIR)$(TARGET_NAME)
TARGET_EXE_WIN=$(TARGET_DIR_WIN)$(TARGET_NAME)
#INCLUDE_DIR=dependencies/include/


#automatic src and lib files detection
SRC_FILES := $(wildcard $(SRC_DIR)*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))


#scripts
all: build
rebuild: clean build

build:objectfolder $(OBJ_FILES) $(LIB_OBJ_FILES) $(COMP_OBJ_FILES)
	$(CC) $(LANG_VERSION) -o $(TARGET_EXE) $(OBJ_FILES) $(LIB_OBJ_FILES) $(LINK_FLAGS) $(HIDE_CONSOLE)


objectfolder:
	if not exist $(OBJ_DIR_WIN) mkdir $(OBJ_DIR_WIN)
	
clean:
	if exist $(TARGET_EXE_WIN) del $(TARGET_EXE_WIN)
	if exist $(OBJ_DIR_WIN) rmdir /s /q $(OBJ_DIR_WIN)
	cls
	
run:build
	python frontend.py

$(OBJ_FILES): $(SRC_FILES)
	$(CC) -c $(LANG_VERSION) -o $@  $(subst .o,.cpp,$(subst $(OBJ_DIR),$(SRC_DIR),$@)) $(COMPILE_FLAGS)
