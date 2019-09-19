#include "le.elf.h"

elf32_node_t sht_null;
elf32_node_t sht_progbit;
elf32_node_t sht_symtab;
elf32_node_t sht_strtab;
elf32_node_t sht_rela;
elf32_node_t sht_hash;
elf32_node_t sht_dynamic;
elf32_node_t sht_note;
elf32_node_t sht_nobits;
elf32_node_t sht_rel;
elf32_node_t sht_shlib;
elf32_node_t sht_dynsym;
elf32_node_t sht_num;
elf32_node_t sht_init_array;
elf32_node_t sht_fini_array;
elf32_node_t sht_preinit_array;
elf32_node_t sht_group;
elf32_node_t sht_symtab_shndx;
elf32_node_t sht_relr;
extern elf32_node_t *sht_proc;

elf32_node_t sht_null = {
	.name = "(SHT_NULL)",
	.field = SHT_NULL,
	.next = &sht_progbit
};

elf32_node_t sht_progbit = {
	.name = "(SHT_PROGBITS)",
	.field = SHT_PROGBITS,
	.next = &sht_symtab
};

elf32_node_t sht_symtab = {
	.name = "(SHT_SYMTAB)",
	.field = SHT_SYMTAB,
	.next = &sht_strtab
};

elf32_node_t sht_strtab = {
	.name = "(SHT_STRTAB)",
	.field = SHT_STRTAB,
	.next = &sht_rela
};

elf32_node_t sht_rela = {
	.name = "(SHT_RELA)",
	.field = SHT_RELA,
	.next = &sht_hash
};

elf32_node_t sht_hash = {
	.name = "(SHT_HASH)",
	.field = SHT_HASH,
	.next = &sht_dynamic
};

elf32_node_t sht_dynamic = {
	.name = "(SHT_DYNAMIC)",
	.field = SHT_DYNAMIC,
	.next = &sht_note
};

elf32_node_t sht_note = {
	.name = "(SHT_NOTE)",
	.field = SHT_NOTE,
	.next = &sht_nobits
};

elf32_node_t sht_nobits = {
	.name = "(SHT_NOBITS)",
	.field = SHT_NOBITS,
	.next = &sht_rel
};

elf32_node_t sht_rel = {
	.name = "(SHT_REL)",
	.field = SHT_REL,
	.next = &sht_shlib
};

elf32_node_t sht_shlib = {
	.name = "(SHT_SHLIB)",
	.field = SHT_SHLIB,
	.next = &sht_dynsym
};

elf32_node_t sht_dynsym = {
	.name = "(SHT_DYNSYM)",
	.field = SHT_DYNSYM,
	.next = &sht_init_array
};

elf32_node_t sht_init_array = {
	.name = "(SHT_INIT_ARRAY)",
	.field = SHT_INIT_ARRAY,
	.next = &sht_fini_array
};

elf32_node_t sht_fini_array = {
	.name = "(SHT_FINI_ARRAY)",
	.field = SHT_FINI_ARRAY,
	.next = &sht_preinit_array
};

elf32_node_t sht_preinit_array = {
	.name = "(SHT_PREINIT_ARRAY)",
	.field = SHT_PREINIT_ARRAY,
	.next = &sht_group
};

elf32_node_t sht_group = {
	.name = "(SHT_GROUP)",
	.field = SHT_GROUP,
	.next = &sht_symtab_shndx
};

elf32_node_t sht_symtab_shndx = {
	.name = "(SHT_SYMTAB_SHNDX)",
	.field = SHT_SYMTAB_SHNDX,
	.next = &sht_relr
};

elf32_node_t sht_relr = {
	.name = "(SHT_RELR)",
	.field = SHT_RELR,
	.next = &sht_num
};

elf32_node_t sht_num = {
	.name = "(SHT_NUM)",
	.field = SHT_NUM
};

__attribute__((constructor))
void sht_init(void)
{
	sht_num.next = sht_proc;

}

/**
 * sh_flags
 */

elf32_node_t shf_write = {
	.field = 0x1,
	.name = "(SHF_WRITE)",
	.next = &shf_alloc
};

elf32_node_t shf_alloc = {
	.field = 0x2,
	.name = "(SHF_ALLOC)",
	.next = &shf_execinstr
};

elf32_node_t shf_execinstr = {
	.field = 0x4,
	.name = "(SHF_EXECINSTR)",
	.next = &shf_merge
};

elf32_node_t shf_merge = {
	.field = 0x10,
	.name = "(SHF_MERGE)",
	.next = &shf_strings
};

elf32_node_t shf_strings = {
	.field = 0x20,
	.name = "(SHF_STRINGS)",
	.next = &shf_info_link
};

elf32_node_t shf_info_link = {
	.field = 0x40,
	.name = "(SHF_INFO_LINK)",
	.next = &shf_link_order
};

elf32_node_t shf_link_order = {
	.field = 0x80,
	.name = "(SHF_LINK_ORDER)",
	.next = &shf_os_nonconforming
};

elf32_node_t shf_os_nonconforming = {
	.field = 0x100,
	.name = "(SHF_OS_NONCONFORMING)",
	.next = &shf_group
};

elf32_node_t shf_group = {
	.field = 0x200,
	.name = "(SHF_GROUP)",
	.next = &shf_tls
};

elf32_node_t shf_tls = {
	.field = 0x400,
	.name = "(SHF_TLS)",
	.next = &shf_compressed
};

elf32_node_t shf_compressed = {
	.field = 0x800,
	.name = "(SHF_COMPRESSED)",
	.next = &shf_write	//list ends here for now
};

elf32_node_t shf_maskos = {
	.field = 0x0ff00000,
	.name = "(SHF_MASKOS)",
	.next = &shf_maskproc
};

elf32_node_t shf_maskproc = {
	.field = 0xf0000000,
	.name = "(SHF_MASKPROC)",
	.next = &shf_write
};
