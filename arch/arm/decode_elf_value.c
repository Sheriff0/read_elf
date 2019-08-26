#include <elf.h>

int
decode_elf_value (int endianness, int *value)
{
	int v = 0x0;

	switch (endianness) {
		case ELFDATA2LSB:
			{
				return *value;
			}

		case ELFDATA2MSB:
			{
				asm volatile (	".arch armv6\n"
						"setend be;"
						"ldrh r1, [%0];"
						"mov %1, r1;"
						"setend le;"
						: "+r" (value), "=r" (v)
						: "r" (value)
						: "r1" );
				return v;
			}
		default: case ELFDATANONE: 
			{
				return -1;
			}

	}
}
