source := read_elf.c \
	read_elf_header.c \
	read_elf_ei_osabi.c\
	read_elf_e_machine.c \
	arch/arm/decode_elf_value.c \
	read_elf_section.c \
	read_elf_section_data.c\
	read_elf_symtab.c\
	arch/arm/_elf_header.c\
	arch/arm/_elf_section_data.c\

objs := read_elf.o \
	read_elf_header.o \
	read_elf_ei_osabi.o \
	decode_elf_value.o \
	read_elf_e_machine.o \
	read_elf_section.o \
	read_elf_section_data.o\
	read_elf_symtab.o\
	_elf_header.o\
	_elf_section_data.o\

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
