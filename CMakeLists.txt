# GoogleTest用のCMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(etrobocon2025)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# C++標準とカバレッジ用フラグ
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# デバッグビルド用のフラグ
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# カバレッジレポート作成のオプション
option(ENABLE_COVERAGE "Enable test coverage reporting" ON)
if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# GoogleTest のフェッチとビルド
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# プロジェクトのソースディレクトリ構造の定義 (適宜登録)
set(PROJECT_MODULE_DIR ${PROJECT_SOURCE_DIR}/modules)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# 必要なインクルードディレクトリの設定 (適宜登録)
set(INCLUDE_DIRS
  ${PROJECT_MODULE_DIR}
  ${PROJECT_MODULE_DIR}/CameraCapture
  ${PROJECT_MODULE_DIR}/calculators
  ${PROJECT_MODULE_DIR}/common
  ${PROJECT_MODULE_DIR}/motions
  ${PROJECT_MODULE_DIR}/image_processors
  ${PROJECT_MODULE_DIR}/API
  ${PROJECT_TEST_DIR}
  ${PROJECT_TEST_DIR}/dummy
  ${PROJECT_TEST_DIR}/helpers
  /usr/include/opencv4
)

# ソースファイルの登録 (適宜登録)
file(GLOB_RECURSE PROJECT_SOURCES
  "${PROJECT_MODULE_DIR}/*.cpp"
  "${PROJECT_MODULE_DIR}/CameraCapture/*.cpp"
  "${PROJECT_MODULE_DIR}/calculators/*.cpp"
  "${PROJECT_MODULE_DIR}/motions/*.cpp"
  "${PROJECT_MODULE_DIR}/image_processors/*.cpp"
  "${PROJECT_TEST_DIR}/helpers/*.cpp"
)


# プロジェクトのライブラリターゲットを作成
add_library(${PROJECT_NAME}_impl STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}_impl PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_impl ${OpenCV_LIBS})

# テスト設定
enable_testing()

# tests ディレクトリ内の *Test.cpp をすべてテストソースとして登録
file(GLOB TEST_SOURCES "${PROJECT_TEST_DIR}/*Test.cpp")

# テスト実行可能ファイルの作成
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}_test
PRIVATE
    ${PROJECT_TEST_DIR}/dummy  # spikeapi ダミー用
)

# ビルドディレクトリ内に etrobocon2025/datafiles をコピーするカスタムターゲット
add_custom_target(copy_datafiles ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/etrobocon2025/datafiles
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/datafiles
    ${CMAKE_BINARY_DIR}/etrobocon2025/datafiles
  COMMENT "Copying datafiles to build/etrobocon2025/datafiles"
)


target_link_libraries(${PROJECT_NAME}_test
  PRIVATE
    ${PROJECT_NAME}_impl
    gtest_main
    ${OpenCV_LIBS}
)

# テストの登録
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
