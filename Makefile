source := read_header.c le.elf.h

FLAGS := -finline-functions 

read_elf: $(source)
	cc -g $(FLAGS) -o read_elf read_header.c

.PHONY: ass big little

ass: $(source)
	cc -S $(FLAGS) read_header.c

big: $(source)
	cc -Wa, -EB -c -o big read_header.c

little: $(source)
	cc -Wa, -EL -c -o little read_header.c
