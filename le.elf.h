#include <elf.h>
#define KEY_VALUE_DELIM ": "
#define VAL_MAX 100
#define SHT_OS 19
#define SHT_PROC 20
#define SHT_USER 21

#define GET_SHTAB(shtab)\
{\
	get_mb_elf_value ('i', e_hdr->e_ident[EI_DATA],\
			&tmp, &e_hdr->e_shoff) ;\
	shtab = fimg + tmp.i;\
}

#define GET_SHNUM(sym, shtab)\
{\
	get_mb_elf_value ('s', e_hdr->e_ident[EI_DATA],\
			&tmp, &e_hdr->e_shnum);\
	sym = (tmp.s)?\
	tmp.s :\
	(\
	 get_mb_elf_value ('i', e_hdr->e_ident[EI_DATA], &tmp, &shtab[0].sh_size),\
	 tmp.i\
	 );\
}
#define GET_SHSTRNDX(e_hdr, shstrndx, shtab, strtab)\
{\
	get_mb_elf_value ('s', e_hdr->e_ident[EI_DATA], &tmp, &e_hdr->e_shstrndx) ;\
	shstrndx = tmp.s ;\
	if (shstrndx == SHN_XINDEX) {\
		get_mb_elf_value ('i', e_hdr->e_ident[EI_DATA],\
				&tmp, &shtab[0].sh_link);\
		shstrndx = tmp.i;\
	}\
	get_mb_elf_value('i', e_hdr->e_ident[EI_DATA],\
			 &tmp, &shtab[shstrndx].sh_offset);\
	strtab = fimg + tmp.i;\
}

struct e_ident_el 
{
	char *name;
	char *pad_start;
	char *pad_end;
	char **values;
};
typedef struct e_ident_el elf32_hdr_mem;
typedef struct e_ident_el elf32_shdr_mem;

typedef struct e_ident_el elf32_sym_mem;

typedef union 
{
	Elf32_Word i;
	Elf32_Half s;
	long long l; 
} elf32_generic_value;
typedef struct elf32_node 
{
  char *name;
  Elf32_Word field;
  struct elf32_node *next;
} elf32_node_t;
typedef struct elf32_node_r 
{
  char *name;
  Elf32_Word field;
  Elf32_Word l_len;
  elf32_node_t *next;
} elf32_node_root;
struct fimg_a
{
	struct elf32_hdr *e_hdr;
	Elf32_Shdr *shtab;
	Elf32_Sym *symtab;
	Elf32_Word shstrndx, shnum, symtabnum, *symtab_shndx;
	char *symtabstrtab, *strtab;
};
extern elf32_node_t shf_write;
extern elf32_node_t shf_alloc;
extern elf32_node_t shf_execinstr;
extern elf32_node_t shf_merge;
extern elf32_node_t shf_strings;
extern elf32_node_t shf_info_link;
extern elf32_node_t shf_link_order;
extern elf32_node_t shf_os_nonconforming;
extern elf32_node_t shf_group;
extern elf32_node_t shf_tls;
extern elf32_node_t shf_compressed;
extern elf32_node_t shf_maskos;
extern elf32_node_t shf_maskproc;
extern struct fimg_a fimg_annot;
