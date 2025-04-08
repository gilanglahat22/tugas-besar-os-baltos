# Made by batOS Operating System

# Makefile
all: diskimage bootloader stdlib kernel shell cd ls cp mv mkdir cat program_tes

# Recipes
diskimage:
	@dd if=/dev/zero of=out/system.img bs=512 count=2880
	
bootloader:
	@nasm src/asm/bootloader.asm -o out/bootloader
	@dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc
	
kernel:
	@bcc -ansi -c -o out/kernel.o src/c/kernel.c
	@nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	@nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	@nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	@ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/lib_interrupt.o out/math.o out/string.o
	@dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

shell:
	@bcc -ansi -c -o out/shell.o src/c/shell.c
	@ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

cd:
	@bcc -ansi -c -o out/cd.o src/c/cd.c
	@ld86 -o out/cd -d out/cd.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

ls:
	@bcc -ansi -c -o out/ls.o src/c/ls.c
	@ld86 -o out/ls -d out/ls.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

mv:
	@bcc -ansi -c -o out/mv.o src/c/mv.c
	@ld86 -o out/mv -d out/mv.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

cp:
	@bcc -ansi -c -o out/cp.o src/c/cp.c
	@ld86 -o out/cp -d out/cp.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

cat:
	@bcc -ansi -c -o out/cat.o src/c/cat.c
	@ld86 -o out/cat -d out/cat.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

mkdir:
	@bcc -ansi -c -o out/mkdir.o src/c/mkdir.c
	@ld86 -o out/mkdir -d out/mkdir.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

program_tes:
	@bcc -ansi -c -o out/program_tes.o src/c/program_tes.c
	@ld86 -o out/program_tes -d out/program_tes.o out/lib_interrupt.o out/string.o out/program.o out/textio.o out/lib_utils.o out/utils.o out/fileio.o out/math.o

stdlib:
	@bcc -ansi -c -o out/math.o src/c/math.c
	@bcc -ansi -c -o out/textio.o src/c/textio.c
	@bcc -ansi -c -o out/fileio.o src/c/fileio.c
	@bcc -ansi -c -o out/utils.o src/c/utils.c
	@bcc -ansi -c -o out/string.o src/c/string.c
	@bcc -ansi -c -o out/program.o src/c/program.c

build-test:
	@gcc test/tc_gen.c test/tc_lib -o out/tc_gen

test-shell:
	@cd out; ./tc_gen E; cd ..;

run:
	@bochs -f src/config/if2230.config

test: build-test test-shell run

build-run: all run
