cmake_minimum_required(VERSION 3.16)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG        "v3.1.0"
)
FetchContent_MakeAvailable(Catch2)

# set the project name
project(nolibc1-units VERSION 1 LANGUAGES C ASM CXX)

set(CMAKE_C_FLAGS "-pedantic \
                    -Wall -Werror \
                    -s -Os -no-pie \
                    -nostdlib \
                    -ffreestanding \
                    -fno-stack-protector \
                    -fdata-sections \
                    -ffunction-sections \
                    -fno-unwind-tables \
                    -fno-asynchronous-unwind-tables \
                    -Wl,-n \
                    -Wl,--gc-sections \
                    -Wl,--build-id=none"
)

# add the executable
add_executable(nolibc1-units src/_start.S src/main.c src/syscall.S src/write.c)

add_executable(tests tests/test.spec.cpp tests/helpers/mock_syscall.c src/write.c)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_custom_target(run
    COMMAND nolibc1-units
    DEPENDS nolibc1-units
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests TEST_SUFFIX ".spec.cpp")