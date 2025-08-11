# プロジェクト設定
PROJECT_NAME := yolo_detection_system
SRC_DIR := modules
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/$(PROJECT_NAME)

MAKEFILE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# ツールチェーン
CXX := g++
CXXSTD := -std=c++17
WARN := -Wall -Wextra -Wpedantic
OPT := -O2

# 依存関係
# OpenCV は pkg-config から取得
OPENCV_CFLAGS := $(shell pkg-config --cflags opencv4 2>/dev/null)
OPENCV_LIBS   := $(shell pkg-config --libs opencv4 2>/dev/null)

# ONNX Runtime
ONNXRUNTIME_DIR := $(MAKEFILE_PATH)../third_party/onnxruntime
ONNX_INC := $(ONNXRUNTIME_DIR)/include
ONNX_LIB := $(ONNXRUNTIME_DIR)/lib

# nlohmann/json はヘッダオンリー。標準以外にある場合は JSON_INC を指定
JSON_INC ?=

INC := -I$(SRC_DIR) -I$(ONNX_INC) $(JSON_INC) $(OPENCV_CFLAGS)
LIB := -L$(ONNX_LIB) -lonnxruntime $(OPENCV_LIBS) -pthread -Wl,-rpath,$(ONNX_LIB)

SRCS := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/BackgroundDirectionDetector.cpp \
	$(SRC_DIR)/MiniFigDirectionDetector.cpp

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXSTD) $(WARN) $(OPT) -o $@ $^ $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXSTD) $(WARN) $(OPT) -c $< -o $@ $(INC)

help:
	@echo ビルドする
	@echo " $$ make build"
	@echo 背景向き判定を行う
	@echo " $$ make background-detection"
	@echo ミニフィグ向き判定を行う
	@echo " $$ make fig-detection"
	@echo buildディレクトリを削除する
	@echo " $$ make clean"
	@echo 一時/エディタ生成ファイルのみ削除
	@echo " $$ make tidy"
	@echo 指定ファイルをフォーマットする
	@echo " $$ make format FILES=<ディレクトリ名>/<ファイル名>.cpp"
	@echo すべての変更ファイルをフォーマットする
	@echo " $$ make format"
	@echo フォーマットチェックをする
	@echo " $$ make format-check"

background-detection:
	@mkdir -p result
	./$(TARGET) background

fig-detection:
	@mkdir -p result
	./$(TARGET) fig

build: clean $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 一時/エディタ生成ファイルのみ削除
tidy:
	find . -name "*~" -o -name "*.swp" -o -name "*.DS_Store" -delete

# ファイルにclang-formatを適用する
format:
# 指定ファイルがある場合、そのファイルにclang-formatを適用する
ifdef FILES
	clang-format -i -style=file $(FILES)
	@ echo "フォーマットを適用しました: $(FILES)"
# ない場合、変更されたファイルのうち、cpp、hファイルにclang-formatを適用する
else
# 変更されたファイルと未追跡のファイル (新規追加など) の両方を検出
	@{ \
        CANDIDATE_FILES=$$( (git diff origin/main --name-only; git ls-files --others --exclude-standard) | grep -E '\.cpp$$|\.h$$' || true ); \
        ACTUALLY_FORMATTED_FILES=""; \
        FORMATTED_COUNT=0; \
        if [ -n "$$CANDIDATE_FILES" ]; then \
            for FILE_PATH in $$CANDIDATE_FILES; do \
                REPLACEMENTS_XML=$$(clang-format -style=file --output-replacements-xml "$$FILE_PATH" 2>/dev/null); \
                if echo "$$REPLACEMENTS_XML" | grep -q "<replacement "; then \
                    clang-format -i -style=file "$$FILE_PATH"; \
                    if [ -z "$$ACTUALLY_FORMATTED_FILES" ]; then \
                        ACTUALLY_FORMATTED_FILES="$$FILE_PATH"; \
                    else \
                        ACTUALLY_FORMATTED_FILES="$${ACTUALLY_FORMATTED_FILES}\n$$FILE_PATH"; \
                    fi; \
                    FORMATTED_COUNT=$$(($$FORMATTED_COUNT + 1)); \
                fi; \
            done; \
            if [ $$FORMATTED_COUNT -gt 0 ]; then \
                echo "以下のファイルにフォーマットを適用しました ($$FORMATTED_COUNT 件):"; \
                printf "%b\n" "$$ACTUALLY_FORMATTED_FILES"; \
            else \
                echo "検査したファイルは全てフォーマット済みでした。適用された変更はありません。"; \
            fi; \
        else \
            echo "フォーマットをチェックする対象の .cpp または .h ファイルがありません。"; \
        fi \
    }
endif

format-check:
	find ./modules -type f -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror *.h *.cpp