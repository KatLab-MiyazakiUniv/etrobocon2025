USE_RASPIKE_ART=1

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Shellスクリプトで、moduleディレクトリ中のソースコード名をオブジェクトファイル名に変換している

MODULE_DIRS := \
	$(mkfile_path)modules/ \
	$(mkfile_path)common \
	$(mkfile_path)camera_server/modules


SRCS := $(foreach dir,$(MODULE_DIRS),$(shell find $(dir) -name '*.cpp'))
SRCS += $(shell find ${mkfile_path}../../common/library/libcpp-spike/src -name '*.cpp')
OBJS := $(notdir $(SRCS:.cpp=.o))
APPL_CXXOBJS += $(OBJS)

SRCLANG := c++

APPL_LIBS += -lm
APPL_LIBS += $(shell pkg-config --libs opencv4)

APPL_DIRS += \
	$(mkfile_path)modules\
	$(mkfile_path)modules/API\
	$(mkfile_path)modules/calculators\
	$(mkfile_path)modules/motions\
	$(mkfile_path)modules/client\
	$(mkfile_path)common\
	$(mkfile_path)camera_server/modules \
	$(mkfile_path)camera_server/modules/actions \
	$(mkfile_path)camera_server/modules/camera \
	$(mkfile_path)camera_server/modules/image_processors \
	$(mkfile_path)camera_server/modules/server \
	$(mkfile_path)camera_server/modules/utils \
	$(mkfile_path)../../common/library/libcpp-spike/src

INCLUDES += \
	-I$(mkfile_path)modules\
	-I$(mkfile_path)modules/API\
	-I$(mkfile_path)modules/calculators\
	-I$(mkfile_path)modules/motions\
	-I$(mkfile_path)modules/client \
	-I$(mkfile_path)common\
	-I$(mkfile_path)camera_server/modules \
	-I$(mkfile_path)camera_server/modules/actions \
	-I$(mkfile_path)camera_server/modules/camera \
	-I$(mkfile_path)camera_server/modules/image_processors \
	-I$(mkfile_path)camera_server/modules/server \
	-I$(mkfile_path)camera_server/modules/utils \
	-I$(mkfile_path)..\/..\/common\/library\/libcpp-spike\/include \

	
INCLUDES += $(shell pkg-config --cflags opencv4)

CXXFLAGS += -std=c++17 -pthread
LDFLAGS += -pthread