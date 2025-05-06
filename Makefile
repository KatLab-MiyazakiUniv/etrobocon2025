MAKEFILE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# 使い方
help:
	@echo ビルドする
	@echo " $$ make build"
	@echo 走行を開始する\(実機限定\)
	@echo " $$ make start"
	@echo 指定ファイルをフォーマットする
	@echo " $$ make format FILES=<ディレクトリ名>/<ファイル名>.cpp"
	@echo すべての変更ファイルをフォーマットする
	@echo " $$ make format"
	@echo フォーマットチェックをする
	@echo " $$ make format-check"
	@echo テストをビルドする
	@echo " $$ make test-build"
	@echo テストを実行する
	@echo " $$ make test-exec"
	@echo テスト用の'build'ディレクトリを削除する
	@echo " $$ make clean"
	@echo 環境が変わっている場合のみ build ディレクトリを削除する
	@echo " $$ make smart-clean"
	@echo 必要があればbuildディレクトリを削除し, テストをビルドして実行する
	@echo " $$ make test"

## 実行関連 ##
build:
	cd $(MAKEFILE_PATH)../ && make img=etrobocon2025

# 実機の場合、走行を開始する
start:
	cd $(MAKEFILE_PATH)../ && make start

## テスト関連 ##
# テストのビルドディレクトリが存在しない場合は作成する
test-build:
	@mkdir -p $(MAKEFILE_PATH)bin/build
	cd $(MAKEFILE_PATH)bin/build && cmake ../.. && make

# テストを実行する
test-exec: 
	@if [ ! -f $(MAKEFILE_PATH)bin/build/etrobocon2025_test ]; then \
		echo "テスト実行ファイルが見つかりません。まずビルドを実行してください。"; \
		echo " $$ make test-build"; \
		exit 1; \
	fi
	cd $(MAKEFILE_PATH)bin/build && ./etrobocon2025_test

# テストをビルドして実行する
test: smart-clean test-build test-exec

# build ディレクトリを完全に削除する
clean:
	@if [ -d $(MAKEFILE_PATH)bin/build ]; then \
		rm -rf $(MAKEFILE_PATH)bin/build; \
		echo "'build/' ディレクトリを削除しました。"; \
	else \
		echo "'build/' ディレクトリは既に存在しません。"; \
	fi

# 実行環境が変更されている場合にのみ 'build' を削除する
smart-clean:
	@if [ -d $(MAKEFILE_PATH)bin/build ]; then \
		if [ -f "$(MAKEFILE_PATH)bin/build/Makefile" ]; then \
			CMAKE_SOURCE_DIR=`grep -E "^CMAKE_SOURCE_DIR[[:space:]]*=" $(MAKEFILE_PATH)bin/build/Makefile | cut -d= -f2 | xargs`; \
			CMAKE_SOURCE_DIR_REAL=`readlink -f "$$CMAKE_SOURCE_DIR"`; \
			CURRENT_DIR_REAL=`readlink -f "$$(pwd)"`; \
			echo "[DEBUG] CMAKE_SOURCE_DIR: '$$CMAKE_SOURCE_DIR_REAL'"; \
			echo "[DEBUG] CURRENT_DIR    : '$$CURRENT_DIR_REAL'"; \
			if [ "$$CMAKE_SOURCE_DIR_REAL" != "$$CURRENT_DIR_REAL" ]; then \
				echo "[LOG] 実行環境の変更が検出されたため 'build' を削除します。"; \
				rm -rf $(MAKEFILE_PATH)bin/build; \
			else \
				echo "[LOG] 実行環境は変更されていません。"; \
			fi; \
		else \
			echo "[LOG] 'build/Makefile' が存在しません。"; \
			echo "[LOG] 'buildディレクトリを完全削除します。"; \
			make clean; \
		fi; \
	else \
		echo "'build' ディレクトリは既に存在しません。"; \
	fi

## 開発関連 ##
# ファイルにclang-formatを適用する
format:
# 指定ファイルがある場合、そのファイルにclang-formatを適用する
ifdef FILES
	clang-format -i -style=file $(FILES)
# ない場合、変更されたファイルのうち、cpp、hファイルにclang-formatを適用する
else
	git diff origin/main --name-only | awk '/\.cpp$$|\.h$$/ {print $$1}' | xargs clang-format -i -style=file
endif

format-check:
	find ./test ./modules -type f -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror *.h *.cpp