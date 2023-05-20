.PHONY: all
all:
	mkdir -p bin decoded_file
	gcc -Wall -o bin/huffman files/*.c
.PHONY: clean
clean:
	rm -r bin decoded_file
