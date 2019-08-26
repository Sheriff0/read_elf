source := read_header.c ei_osabi.c arch/arm/decode_elf_value.c 

objs := read_header.o ei_osabi.o decode_elf_value.o

headers := le.elf.h

FLAGS := 

read_elf: $(objs) $(headers)
	cc -g -o read_elf $(objs)

$(objs): $(source)
	cc -c -g $(FLAGS) $(source)

.PHONY: ass big little

ass: $(source) $(headers)
	cc -S $(FLAGS) $(source)

big: $(source) $(headers)
	cc -Wa, -EB -o big -c $<

little: $(source) $(headers)
	cc -Winline -Wa, -EL -c -o little $(source)
