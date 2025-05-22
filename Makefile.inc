USE_RASPIKE_ART=1

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

# Shellスクリプトで、moduleディレクトリ中のソースコード名をオブジェクトファイル名に変換している
SRCS := $(shell find ${mkfile_path}modules -name '*.cpp')
OBJS := $(notdir $(SRCS:.cpp=.o))
APPL_CXXOBJS += $(OBJS)

SRCLANG := c++

APPL_LIBS += -lm

APPL_DIRS += \
	$(mkfile_path)modules\
	$(mkfile_path)modules/API\
	$(mkfile_path)modules/calculators

INCLUDES += \
	-I$(mkfile_path)modules\
	-I$(mkfile_path)modules/API\
	-I$(mkfile_path)modules/calculators
