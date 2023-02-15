#include "func.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Incorrect input! You may input './huffman -c file.hc file.txt' or './huffman -d file1.txt file.hc\n");
        return -1;
    }
    if (argv[1] && argv[2] && argv[3]){
        if(strcmp(argv[1], "-c") == 0){
            printf("Encoding in process...\n");
            compression(argv[2], argv[3]);
            printf("Encoding is done, now you can check up encoded file [%s], great work!\n", argv[2]);
        }
        else if(strcmp(argv[1], "-d") == 0){
            printf("Decompression in process...\n");
            decompression(argv[2], argv[3]);
            printf("Decompression is done, now you can check up decoded file [%s], have a nice day!\n", argv[2]);
        } else{
            printf("Incorrect input! You may input './huffman -c file.hc file.txt' or './huffman -d file1.txt file.hc\n");
            return -1;
        }
    }
    return 0;
}