#include <elf.h>
#include "../../le.elf.h"
#include <stdlib.h>

__attribute__((naked, always_inline))
elf32_generic_value *get_mb_elf_value(const char fmt, int endianness,
				      elf32_generic_value * buff, ...)
{
	asm volatile (".arch armv6\n"
		      "stmdb sp!, {r4-r6, fp, lr}\n"
		      "mov r4, r1\n"
		      "mov r5, r0\n"
		      "mov r6, r3\n"
		      "mov r0, r2\n"
		      "cmp r4, #2\n"
		      "bne 1f\n"
		      "setend be\n"
		      "1:\n"
		      "cmp r5, #115 @s\n"
		      "ldrheq r2, [r6]\n"
		      "moveq r3, #0\n"
		      "beq exit\n"
		      "cmp r5, #105 @i\n"
		      "ldreq r2, [r6]\n"
		      "moveq r3, #0\n"
		      "beq  exit\n"
		      "cmp r5, #108 @l\n"
		      "ldrdeq r2, [r6]\n"
		      "beq  exit\n"
		      "mvn r2, #0\n"
		      "mvn r3, #0\n"
		      "exit:\n"
		      "setend le\n"
		      "strd r2, [r0]\n"
		      "ldmia sp!, {r4-r6, fp, lr}\n" 
		      "bx lr\n");
}
