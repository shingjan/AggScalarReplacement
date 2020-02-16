#!/bin/bash

all_test_files="
./test0_main
./test1_single_struct
./test2_multiple_struct
./test3_nested_struct
./test4_nested_struct
"

for test_file in $all_test_files
do
	clang-8 -O3 -emit-llvm $test_file.c -c -o $test_file.bc
	opt -load $LLVM_HOME/build/lib/LLVMTest.so -test < $test_file.bc > /dev/null
	rm $test_file.bc
done



