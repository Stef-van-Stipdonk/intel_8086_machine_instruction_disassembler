gcc ./src/main.c 

echo "TEST EXPLANATION"
echo "Each test case runs the dissassembler against a different compiled .asm file."
echo "The dissassembled contents of the file are put into the test.asm, then compiled with nasm."
echo "Lastly the compiled test.asm file and the compiled .asm file are compared with 'diff', if no differences are found, the test passes."
echo ""

# TESTS
echo "CASE 1 - listing_0037_single_register_mov"
output=$(./a.out ./compiled_asm/listing_0037_single_register_mov > test.asm)
return_val=$?

nasm ./test.asm
diff ./compiled_asm/listing_0037_single_register_mov ./test
if [ $? != 0 ]
then	
		echo -e $ERROR "TEST FAILED"
else
		echo -e $SUCCESS "TEST PASSED"
fi

echo "CASE 2 - listing_0038_many_register_mov"
output=$(./a.out ./compiled_asm/listing_0038_many_register_mov > test.asm)
return_val=$?

nasm ./test.asm
diff ./compiled_asm/listing_0038_many_register_mov ./test
if [ $? != 0 ]
then	
		echo -e $ERROR "TEST FAILED"
else
		echo -e $SUCCESS "TEST PASSED"
fi

echo "CASE 3 - listing_0039_more_movs"
output=$(./a.out ./compiled_asm/listing_0039_more_movs > test.asm)
return_val=$?

nasm ./test.asm
diff ./compiled_asm/listing_0039_more_movs ./test
if [ $? != 0 ]
then	
		echo -e $ERROR "TEST FAILED"
else
		echo -e $SUCCESS "TEST PASSED"
fi

echo "CASE 4 - listing_0041_add_sub_cmp_jnz"
output=$(./a.out ./compiled_asm/listing_0041_add_sub_cmp_jnz > test.asm)
return_val=$?

nasm ./test.asm
diff ./compiled_asm/listing_0041_add_sub_cmp_jnz ./test
if [ $? != 0 ]
then	
		echo -e $ERROR "TEST FAILED"
else
		echo -e $SUCCESS "TEST PASSED"
fi
