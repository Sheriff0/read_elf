#include <elf.h>

long long int
decode_elf_value (const char fmt, int endianness, int *value)
{
	long long int v = 0x0ll;

	switch (endianness) {
		case ELFDATA2LSB:
			{
				v = *value;
				return v;
			}

		case ELFDATA2MSB:
			{
				asm volatile (	".arch armv6\n"
						"setend be ;"
						"cmp %2, #0x62 ; @ 'b'\n"
						"ldrbeq %1, [%0] ;"
						"beq exit ;"
						"cmp %2, #0x73 ;@ s\n"
						"ldrh %1, [%0];"
						"beq exit ;"
						"cmp %2, #0x69 ;@ i\n"
						"ldreq %1, [%0];"
						"beq exit ;"
						"cmp %2, #0x6c ;@ l\n"
						"ldrdeq %1, [%0];"
						"exit:\n\t"
						"setend le;"
						: "+r" (value), "=r" (v)
						: "r" (fmt)
						:  );
				return v;
			}
		default: case ELFDATANONE: 
			{
				return -1;
			}

	}
}
