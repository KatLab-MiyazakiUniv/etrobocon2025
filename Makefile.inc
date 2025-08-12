USE_RASPIKE_ART=1

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Source files for the 
SRCS := $(shell find ${mkfile_path}modules/spike_server -name '*.cpp')
SRCS += $(shell find ${mkfile_path}modules/common -name '*.cpp')
SRCS += $(shell find ${mkfile_path}../../common/library/libcpp-spike/src -name '*.cpp')

OBJS := $(notdir $(SRCS:.cpp=.o))
APPL_CXXOBJS += $(OBJS)

SRCLANG := c++

APPL_LIBS += -lm

# Application directories
APPL_DIRS += \
	$(mkfile_path)modules/spike_server \
	$(mkfile_path)modules/spike_server/handlers \
	$(mkfile_path)modules/common \
	$(mkfile_path)../../common/library/libcpp-spike/src

# Include paths
INCLUDES += \
	-I$(mkfile_path) \
	-I$(mkfile_path)modules/spike_server/handlers \
	-I$(mkfile_path)modules/common \
	-I$(mkfile_path)modules/spike_server \
	-I$(mkfile_path)../../common/library/libcpp-spike/include
