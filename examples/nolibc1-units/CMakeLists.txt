cmake_minimum_required(VERSION 3.16)

# set the project name
project(nolibc1-units VERSION 1 LANGUAGES C ASM)

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