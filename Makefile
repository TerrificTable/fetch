CC := gcc
CFLAGS := -std=c99 -g -Wpedantic -Wall -Wextra -Werror -O3

src := $(wildcard *.c)
obj := $(src:%.c=bin/%.o)
bin := fetch


ifeq ($(OS),Windows_NT)
    LDLIBS := -lpdh -lws2_32
endif


build: $(obj)
	gcc $(CFLAGS) $(obj) -o $(bin) $(LDLIBS)

bin/%.o: %.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LDLIBS)
.PHONY: build, install


# TODO: Windows and MacOS (maybe)
install: build
	@if [ $$(uname -s) = "Linux" ]; then \
    	cp -f ./$(bin) /usr/local/bin/$(bin); \
  	fi

test:
	@echo "No Tests"


clean:
	rm -r bin
	rm $(bin)

