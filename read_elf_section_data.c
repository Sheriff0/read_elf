#include "le.elf.h"	

char *values_shtypes[] =
{

	[SHT_NULL] = "Inactive(SHT_NULL)",
	[SHT_PROGBITS] = "Program bits(SHT_PROGBITS)",
	[SHT_SYMTAB] = "Symbol table(SHT_SYMTAB)",
	[SHT_STRTAB] = "String table(SHT_STRTAB)",
	[SHT_RELA] = "Reloc+addend(SHT_RELA)",
	[SHT_HASH] = "Hash(SHT_HASH)",
	[SHT_DYNAMIC] = "Dynamic(SHT_DYNAMIC)",
	[SHT_NOTE] = "Note(SHT_NOTE)",
	[SHT_NOBITS] = "BSS-like(SHT_NOBITS)",
	[SHT_REL] = "Reloc-addend(SHT_REL)",
	[SHT_SHLIB] = "Unspecified(SHT_SHLIB)",
	[SHT_DYNSYM] = "Dynamic symbols(SHT_DYNSYM)",
	[SHT_INIT_ARRAY] = "Initializers(SHT_INIT_ARRAY)",
	[SHT_FINI_ARRAY] = "Terminators(SHT_FINI_ARRAY)",
	[SHT_PREINIT_ARRAY] = "Pre-initializers(SHT_PREINIT_ARRAY)",
	[SHT_GROUP] = "Section group(SHT_GROUP)",
	[SHT_SYMTAB_SHNDX] = "(SHT_SYMTAB_SHNDX)",
	[SHT_OS] = "OS-specific(SHT_OS)",
	[SHT_PROC] = "Processor-specific(SHT_PROC)",
	[SHT_USER] = "Program-specific(SHT_USER)",

};



elf32_node_t shf_write =
{
	.field = 0x1,
	.name = "(SHF_WRITE)",
	.next = &shf_alloc
};


elf32_node_t shf_alloc =
{
	.field = 0x2,
	.name = "(SHF_ALLOC)",
	.next = &shf_execinstr
};


elf32_node_t shf_execinstr =
{
	.field = 0x4,
	.name = "(SHF_EXECINSTR)",
	.next = &shf_merge
};


elf32_node_t shf_merge =
{
	.field = 0x10,
	.name = "(SHF_MERGE)",
	.next = &shf_strings
};


elf32_node_t shf_strings =
{
	.field = 0x20,
	.name = "(SHF_STRINGS)",
	.next = &shf_info_link
};


elf32_node_t shf_info_link =
{
	.field = 0x40,
	.name = "(SHF_INFO_LINK)",
	.next = &shf_link_order
};


elf32_node_t shf_link_order =
{
	.field = 0x80,
	.name = "(SHF_LINK_ORDER)",
	.next = &shf_os_nonconforming
};


elf32_node_t shf_os_nonconforming =
{
	.field = 0x100,
	.name = "(SHF_OS_NONCONFORMING)",
	.next = &shf_group
};


elf32_node_t shf_group =
{
	.field = 0x200,
	.name = "(SHF_GROUP)",
	.next = &shf_tls
};


elf32_node_t shf_tls =
{
	.field = 0x400,
	.name = "(SHF_TLS)",
	.next = &shf_compressed
};


elf32_node_t shf_compressed =
{
	.field = 0x800,
	.name = "(SHF_COMPRESSED)",
	.next = &shf_write //list ends here for now
};


elf32_node_t shf_maskos =
{
	.field = 0x0ff00000,
	.name = "(SHF_MASKOS)",
	.next = &shf_maskproc
};


elf32_node_t shf_maskproc =
{
	.field = 0xf0000000,
	.name = "(SHF_MASKPROC)",
	.next = &shf_write
};


