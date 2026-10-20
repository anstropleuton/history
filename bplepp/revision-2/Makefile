# Makefile for bplepp project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -DNDEBUG
LDFLAGS = -Wl,-rpath=libs

# Build mode specific flags
ifeq ($(BUILD_MODE), DEBUG)
	CXXFLAGS += -g
else
	CXXFLAGS += -s -O3
endif

# Directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
BIN_RES_DIR = $(BIN_DIR)/res
BIN_LIBS_DIR = $(BIN_DIR)/libs
RES_DIR = res

# Executable
EXEC = $(BIN_DIR)/bplepp

# Folders and libraries
INC_DIR = include
INC_DIRS = -I $(INC_DIR) -I deps/PhysX/include -I deps/raylib/include -I deps/rlImGui/include
SRC_DIR = src
LIB_DIRS = -L lib -L deps/PhysX/lib -L deps/raylib/lib -L deps/rlImGui/lib

# List of libraries
LIBS = -lPhysXCharacterKinematic_static_64 \
       -lPhysXCommon_static_64 \
       -lPhysXCooking_static_64 \
       -lPhysXExtensions_static_64 \
       -lPhysXFoundation_static_64 \
       -lPhysXPvdSDK_static_64 \
       -lPhysX_static_64 \
       -lPhysXVehicle2_static_64 \
       -lPhysXVehicle_static_64 \
       -lSnippetRender_static_64 \
       -lSnippetUtils_static_64 \
       -lraylib \
       -lrlImGui \
       # -lPVDRuntime_64 \
       -lPhysXGpu_64 \
	   (1. Not using PVD, 2. Do not have cuda installed at the moment)

# Include files
INCS = $(wildcard $(INC_DIR)/*.hpp)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Targets
.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS) | $(BIN_DIR) $(BIN_RES_DIR) # $(BIN_LIBS_DIR) # Not using any shared libraries at the moment
	$(CXX) $(LDFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)
	cp -r $(RES_DIR)/* $(BIN_RES_DIR)/
    # cp -r deps/PhysX/lib/*.so $(BIN_LIBS_DIR)/
    # cp -r deps/raylib/lib/*.so $(BIN_LIBS_DIR)/
    # cp -r deps/rlImGui/lib/*.so $(BIN_LIBS_DIR)/

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS) | $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_RES_DIR):
	mkdir -p $(BIN_RES_DIR)

$(BIN_LIBS_DIR):
	mkdir -p $(BIN_LIBS_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)
