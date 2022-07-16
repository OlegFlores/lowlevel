TARGET = hello
CC = gcc
CFLAGS = -pedantic \
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
		-Wl,--build-id=none
OBJECTS = main.c write.c _start.S syscall.S

all: dist/hello

.PHONY strip: dist/hello
	strip -R .comment ./dist/hello

dist/hello:
	$(CC) $(CFLAGS) ${OBJECTS} -o ./dist/hello

clean:
	rm -rf ./dist/hello
run: dist/hello
	./dist/hello