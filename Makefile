.PHONY: all
all:
	gcc -Wall -o bin/huffman files/*.c
.PHONY: clean encode decode
encode:
	./bin/huffman -c bin/huffman.hc Put_here_text_for_encoding.txt
decode:
	./bin/huffman -d Here_decoded_file.txt bin/huffman.hc
clean:
	rm bin/huffman && rm *.hc
-include $(DEPS)

