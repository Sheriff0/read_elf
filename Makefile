source := read_elf.c \
	read_elf_header.c \
	ei_osabi.c \
	read_elf_e_machine.c \
	arch/arm/decode_elf_value.c \
	read_elf_section.c \
	read_elf_section_data.c\
	read_elf_symtab.c\

objs := read_elf.o \
	read_elf_header.o \
	ei_osabi.o \
	decode_elf_value.o \
	read_elf_e_machine.o \
	read_elf_section.o \
	read_elf_section_data.o\
	read_elf_symtab.o\

headers := le.elf.h

FLAGS := 

read_elf: $(objs) $(headers)
	cc -g -o read_elf $(objs)

$(objs): $(source)
	cc -c -g $(FLAGS) $(source)

.PHONY: ass big little clean

ass: $(source) $(headers)
	cc -S $(FLAGS) $(source)

big: $(source) $(headers)
	cc -Wa, -EB -o big -c $<

little: $(source) $(headers)
	cc -Winline -Wa, -EL -c -o little $(source)

clean:
	rm -I *.o
