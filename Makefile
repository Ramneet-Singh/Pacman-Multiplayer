TARGET_EXEC_CLIENT ?= game
TARGET_EXEC_SERVER ?= server

BUILD_DIR_CLIENT ?= ./build-game
BUILD_DIR_SERVER ?= ./build-server
SRC_DIRS ?= ./src
INCLUDE_DIRS ?= ./include ~/software/asio/asio-1.18.1/include

CXX ?= g++
SRCS_CLIENT := $(shell find $(SRC_DIRS) -name "*.cpp" ! -name "server.cpp" )
OBJS_CLIENT := $(SRCS_CLIENT:$(SRC_DIRS)/%.cpp=$(BUILD_DIR_CLIENT)/%.o)
DEPS_CLIENT := $(OBJS_CLIENT:.o=.d)

SRCS_SERVER ?= ./src/Networking/server.cpp ./src/Networking/Networking.cpp 
OBJS_SERVER := $(SRCS_SERVER:$(SRC_DIRS)/%.cpp=$(BUILD_DIR_SERVER)/%.o)
DEPS_SERVER := $(OBJS_SERVER:.o=.d)

INC_FLAGS := $(addprefix -I ,$(INCLUDE_DIRS))

CPPFLAGS ?= $(INC_FLAGS) --std=c++17 -MMD -MP -pthread -w -g 
LDLIBS_CLIENT ?= -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
LDLIBS_SERVER ?= -lpthread

all: $(TARGET_EXEC_CLIENT) $(TARGET_EXEC_SERVER)


$(TARGET_EXEC_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(OBJS_CLIENT) -o $@ $(LDLIBS_CLIENT)

$(BUILD_DIR_CLIENT)/%.o: $(SRC_DIRS)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(TARGET_EXEC_SERVER): $(OBJS_SERVER)
	$(CXX) $(OBJS_SERVER) -o $@ $(LDLIBS_SERVER)

$(BUILD_DIR_SERVER)/%.o: $(SRC_DIRS)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm -rf $(BUILD_DIR_CLIENT) $(BUILD_DIR_SERVER)
	rm $(TARGET_EXEC_CLIENT) $(TARGET_EXEC_SERVER)

MKDIR_P ?= mkdir -p

