USE_RASPIKE_ART=1

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Shellスクリプトで、moduleディレクトリ中のソースコード名をオブジェクトファイル名に変換している
SRCS := $(shell find ${mkfile_path}modules -name '*.cpp')
SRCS += $(shell find ${mkfile_path}../../common/library/libcpp-spike/src -name '*.cpp')
OBJS := $(notdir $(SRCS:.cpp=.o))
APPL_CXXOBJS += $(OBJS)

SRCLANG := c++

APPL_LIBS += -lm

APPL_DIRS += \
	$(mkfile_path)modules\
	$(mkfile_path)modules/API\
	$(mkfile_path)modules/calculators\
	$(mkfile_path)modules/common\
	$(mkfile_path)modules/motions\
	$(mkfile_path)modules/CameraCapture\
	$(mkfile_path)../../common/library/libcpp-spike/src

INCLUDES += \
	-I$(mkfile_path)modules\
	-I$(mkfile_path)modules/API\
	-I$(mkfile_path)modules/calculators\
	-I$(mkfile_path)modules/common\
	-I$(mkfile_path)modules/motions\
	-I$(mkfile_path)modules/CameraCapture \
	-I$(mkfile_path)../../common/library/libcpp-spike/include \
	-I/usr/include/opencv4

APPL_LIBS += $(shell pkg-config --libs opencv4)

CXXFLAGS += -std=c++11 -pthread
LDFLAGS += -pthread
