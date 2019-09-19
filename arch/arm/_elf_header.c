#include <bits/elf_arm.h>
#include "../../le.elf.h"
#include <stdio.h>

#define EF_ARM_GCCMASK 0x00400fff
#define EF_ARM_ABI_FLOAT_HARD 0x00000400
#define EF_ARM_ABI_FLOAT_SOFT 0x00000200

extern elf32_generic_value *get_mb_elf_value(const char fmt, int endianness,
					     elf32_generic_value * buff, ...);

static elf32_node_w_mask ef_arm_eabimask;
static elf32_node_w_mask ef_arm_be8;
static elf32_node_w_mask ef_arm_gccmask;
static elf32_node_w_mask ef_arm_abi_float_hard;
static elf32_node_w_mask ef_arm_bs_call;
static elf32_node_w_mask ef_arm_abi_float_soft;

static elf32_node_w_mask ef_arm_eabimask = {
	.name = "(Arm Abi Version 5)",
	.field = EF_ARM_EABI_VER5,
	.mask = EF_ARM_EABIMASK,
	.next = &ef_arm_be8
};

static elf32_node_w_mask ef_arm_be8 = {
	.name = "(BE-8 code)",
	.field = EF_ARM_BE8,
	.mask = EF_ARM_BE8,
	.next = &ef_arm_gccmask
};

static elf32_node_w_mask ef_arm_gccmask = {
	.name = "(built by gcc-arm-xxx)",
	.field = EF_ARM_GCCMASK,
	.mask = EF_ARM_GCCMASK,
	.next = &ef_arm_abi_float_hard
};

static elf32_node_w_mask ef_arm_abi_float_hard = {
	.name = "(Hardware fp call std)",
	.field = EF_ARM_ABI_FLOAT_HARD,
	.mask = EF_ARM_ABI_FLOAT_HARD,
	.next = &ef_arm_abi_float_soft
};

static elf32_node_w_mask ef_arm_abi_float_soft = {
	.name = "(Software fp std call)",
	.field = EF_ARM_ABI_FLOAT_SOFT,
	.mask = EF_ARM_ABI_FLOAT_SOFT,
	.next = &ef_arm_bs_call
};

static elf32_node_w_mask ef_arm_bs_call = {
	.name = "(Base std call)",
	.field = 0,
	.mask = EF_ARM_ABI_FLOAT_SOFT | EF_ARM_ABI_FLOAT_HARD,
	.next = &ef_arm_eabimask
};

int read_elf_e_flags(struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	elf32_generic_value tmp = {.l = 0l };
	static char val[VAL_MAX];

	static char *values[] = {
		val,
	};

	static elf32_hdr_mem e_flags = {
		.name = "Flags (e_flags)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_flags.name, buff[l_count++] = e_flags.pad_start);

	get_mb_elf_value('i', e_hdr->e_ident[EI_DATA], &tmp, &e_hdr->e_flags);

	sprintf(e_flags.values[0], "%1$#x: ", tmp.i);

	buff[l_count++] = e_flags.values[0];
	elf32_node_w_mask *node = &ef_arm_eabimask;

	char *p = node->name;

	Elf32_Word eflags = tmp.i;

	if ((node->mask & eflags) == node->field)
		buff[l_count++] = node->name;

	for (node = node->next; node->name != p; node = node->next) {

		if ((node->mask & eflags) == node->field)
			buff[l_count++] = node->name;
	}

	buff[l_count++] = "\n";

	return l_count;
}
