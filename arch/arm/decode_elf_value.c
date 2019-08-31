#include <elf.h>
#include "../../le.elf.h"
#include <stdlib.h>

union elf32_generic_value *
decode_elf_value (const char fmt, int endianness, int *vp)
{
	static union elf32_generic_value *vvp;
	vvp = malloc (sizeof (union elf32_generic_value));

	switch (endianness) {
		case ELFDATA2LSB:
			{
				if (fmt == 'b') {
					vvp->b = (char)*vp ;
				} else if (fmt == 's') {

					vvp->s = (Elf32_Half) *vp;

				} else if (fmt == 'i') {

					vvp->i = (Elf32_Word) *vp ;
				} else if (fmt == 'l') {

					vvp->l = (unsigned long long) *vp ;
				} else {
					vvp->l = -1 ;
				}
				return vvp;
			}

		case ELFDATA2MSB:
			{
				asm volatile (	".arch armv6\n"
						"vvp .req %0 \n"
						"fmt .req %1\n"
						"vp .req %2\n"
						"setend be ;"
						"cmp fmt, #0x62 ; @ 'b'\n"
						"ldrbeq r0, [vp] ;"
						"mov r1, #0;"
						"beq exit ;"
						"cmp fmt, #0x73 ;@ s\n"
						"ldrheq r0, [vp];"
						"mov r1, #0;"
						"beq exit ;"
						"cmp fmt, #0x69 ;@ i\n"
						"ldreq r0, [vp];"
						"mov r1, #0;"
						"beq exit ;"
						"cmp fmt, #0x6c ;@ l\n"
						"ldrdeq r0, [vp];"
						"exit:\n\t"
						"setend le;"
						"mvnne r0, #0;"
						"movne r1, r0;"
						"strd r0, [vvp];"
						: "+r" (vvp)
						: "r" (fmt), "r" (vp)
						       : "r0", "r1");
				return vvp;
			}
		default: case ELFDATANONE: 
			{
				vvp->l = -1;
				return vvp;
			}

	}
}
