echo "COMPILING main.c"
gcc -c ./src/main.c -o ./build/main.o
echo "COMPILING flags.c"
gcc -c ./src/flags.c -o ./build/flags.o
echo "COMPILING file_reader.c"
gcc -c ./src/file_reader.c -o ./build/file_reader.o

echo "LINKING"
gcc ./build/main.o ./build/flags.o ./build/file_reader.o -o disassembler
