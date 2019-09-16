#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>
#include "le.elf.h"
#include <stdlib.h>

extern char *nl;

extern elf32_generic_value *get_mb_elf_value(const char fmt, int endianness,
					     elf32_generic_value * buff, ...);

extern char *values_shtypes[];

extern char *values_shflags[];

int
read_elf_sh_name(Elf32_Shdr * shdr, void *fimg, char ei_data, char **buff,
		 _Bool in_line)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Off sh_name;

	static elf32_shdr_mem sh_names = {
		"Section name" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	if (fimg_annot.shstrndx == 0)
		GET_SHSTRNDX(fimg_annot.e_hdr, fimg_annot.shstrndx,
			     fimg_annot.shtab, fimg_annot.strtab);

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_name);

	sh_name = tmp.i;

	if (!in_line)
		buff[l_count++] = sh_names.name;
	buff[l_count++] = fimg_annot.strtab + sh_name;

	return l_count;
}

static inline int
read_elf_sh_flags(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_flags;

	int l_count = 0;

	static elf32_shdr_mem shflags = {
		"|Section flags" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_flags);

	sh_flags = tmp.i;

	buff[l_count++] = shflags.name;
	elf32_node_t *flag = &shf_write;

	if (sh_flags > 0) {
		for (Elf32_Word n = flag->field, p = 0; p < n;
		     flag = flag->next, p = n, n = flag->field) {
			if ((sh_flags & n) == n) {
				buff[l_count++] = flag->name;
			}

		}
	}			/*else {

				   buff [l_count++] = NULL;      
				   } */

	return l_count;
}

static inline int read_elf_sh_addr(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Addr sh_addr;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shaddr = {
		"|Virtual Address" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_addr);

	sh_addr = tmp.i;

	buff[l_count++] = shaddr.name;
	sprintf(val, "%1$#x (%1$i)\n", sh_addr);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int
read_elf_sh_offset(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Off sh_offset;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shoffset = {
		"|File Offset" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_offset);

	sh_offset = tmp.i;

	buff[l_count++] = shoffset.name;
	sprintf(val, "byte %1$#x (%1$i)\n", sh_offset);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int read_elf_sh_size(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_size;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shsize = {
		"|Size" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_size);

	sh_size = tmp.i;

	buff[l_count++] = shsize.name;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_size);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int read_elf_sh_link(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_link;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shlink = {
		"|Link" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_link);

	sh_link = tmp.i;

	buff[l_count++] = shlink.name;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_link);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int read_elf_sh_info(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_info;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shinfo = {
		"|Info" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_info);

	sh_info = tmp.i;

	buff[l_count++] = shinfo.name;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_info);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int
read_elf_sh_addralign(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_addralign;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shaddralign = {
		"|Addr align" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_addralign);

	sh_addralign = tmp.i;

	buff[l_count++] = shaddralign.name;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_addralign);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

static inline int
read_elf_sh_entsize(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_entsize;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_shdr_mem shentsize = {
		"|Entry size" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_entsize);

	sh_entsize = tmp.i;

	buff[l_count++] = shentsize.name;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_entsize);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;
}

__attribute__((always_inline))
static inline int read_elf_sh_type(Elf32_Shdr * shdr, char ei_data, char **buff)
{
	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word sh_type;
	int l_count = 0;

	static elf32_shdr_mem sh_types = {
		"|Section type" KEY_VALUE_DELIM,
		NULL,
		NULL,
		values_shtypes,
	};

	get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_type);

	sh_type = tmp.i;

	buff[l_count++] = sh_types.name;

	if (sh_type >= SHT_NULL && sh_type <= SHT_SYMTAB_SHNDX) {

		buff[l_count++] = sh_types.values[sh_type];

	} else if (sh_type >= SHT_LOOS && sh_type <= SHT_HIOS) {

		buff[l_count++] = sh_types.values[SHT_OS];

	} else if (sh_type >= SHT_LOPROC && sh_type <= SHT_HIPROC) {

		buff[l_count++] = sh_types.values[SHT_PROC];

	} else if (sh_type <= SHT_LOUSER && sh_type <= SHT_HIUSER) {

		buff[l_count++] = sh_types.values[SHT_USER];

	} else {

		buff[l_count++] = sh_types.values[SHT_NULL];

	}

	return l_count;
}

int
read_elf_shdr(Elf32_Shdr * shdr, struct elf32_hdr *e_hdr,
	      void *fimg, char **buff)
{
	unsigned int l_count = 0;
	unsigned char ei_data = e_hdr->e_ident[EI_DATA];

	l_count += read_elf_sh_name(shdr, fimg, ei_data, buff + l_count, 0);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_type(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_flags(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_addr(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_offset(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_size(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_link(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_info(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_addralign(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_entsize(shdr, ei_data, buff + l_count);

	buff[l_count++] = "\n\n";

	return l_count;
}

int read_elf_shtab(struct elf32_hdr *e_hdr, void *fimg, char **buff)
{

	int l_count = 0;

	Elf32_Word shnum, shstrndx;

	const char ei_data = e_hdr->e_ident[EI_DATA];

	elf32_generic_value tmp = {.l = 0l };

	char *strtable;

	Elf32_Shdr *shtab;

	Elf32_Off sh_offset;

	if (fimg_annot.shtab == 0)
		GET_SHTAB(fimg_annot.shtab);

	shtab = fimg_annot.shtab;

	if (fimg_annot.shnum == 0)
		GET_SHNUM(fimg_annot.shnum, shtab);

	shnum = fimg_annot.shnum;

	if (fimg_annot.shstrndx == 0)
		GET_SHSTRNDX(e_hdr, fimg_annot.shstrndx, shtab,
			     fimg_annot.strtab);

	for (int idx = 0; idx < shnum; idx++) {
		l_count +=
		    read_elf_shdr(&shtab[idx], e_hdr, fimg, buff + l_count);

	}

	return l_count;
}
