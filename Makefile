CC := gcc
CFLAGS := -std=c99 -g -Wpedantic -Wall -Wextra -Werror -O3

src := $(wildcard *.c)
obj := $(src:%.c=bin/%.o)
bin := fetch


ifeq ($(OS),Windows_NT)
    LDLIBS := -lpdh
endif


build: $(obj)
	gcc $(CFLAGS) $(obj) -o $(bin) $(LDLIBS)

bin/%.o: %.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LDLIBS)


clean:
	rm bin/*.o
	rm $(bin)

