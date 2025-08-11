USE_RASPIKE_ART=1

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Source files for the server
SRCS := \
    $(mkfile_path)SpikeServer.cpp \
    $(mkfile_path)modules/utils/Socket.cpp \
    $(mkfile_path)modules/common/StringOperator.cpp \
    $(shell find $(mkfile_path)../../common/library/libcpp-spike/src -name '*.cpp')

OBJS := $(notdir $(SRCS:.cpp=.o))
APPL_CXXOBJS += $(OBJS)

SRCLANG := c++

APPL_LIBS += -lm

# Application directories
APPL_DIRS += \
	$(mkfile_path) \
	$(mkfile_path)modules/utils \
	$(mkfile_path)modules/common \
	$(mkfile_path)../../common/library/libcpp-spike/src

# Include paths
INCLUDES += \
	-I$(mkfile_path) \
	-I$(mkfile_path)../../common/library/libcpp-spike/include

CXXFLAGS += -std=c++11 -pthread
LDFLAGS += -pthread

