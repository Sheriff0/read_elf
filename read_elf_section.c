#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>
#include "le.elf.h"
#include <stdlib.h>


extern char *nl;

extern elf32_generic_value *
decode_elf_value (const char fmt, int endianness, ...);

extern char *values_shtypes[];

extern char *values_shflags[];

__attribute__((always_inline))
static inline int
read_elf_sh_name (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;

	Elf32_Off sh_name;

	static elf32_shdr_mem sh_names =
	{
		"Section name" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_name) ;

	sh_name = tmp->i ;

	buff[l_count++] = sh_names.name ;
	buff[l_count++] = strtable+sh_name ;

	free (tmp) ;
	return l_count;
}

static inline int
read_elf_sh_flags (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;

	Elf32_Word sh_flags;

	int l_count = 0;

	static elf32_shdr_mem shflags =
	{
		"|Section flags" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	tmp = decode_elf_value('i', ei_data, &shdr->sh_flags);

	sh_flags = tmp->i;
	
	free (tmp) ;

	buff[l_count++] = shflags.name;
	 elf32_node_t *flag = &shf_write;
	 if (sh_flags > 0) {
		 for (Elf32_Word n = flag->field, p = 0; p < n; flag = flag->next, p = n,
				 n = flag->field) {
			 if ((sh_flags & n) == n) {
				 buff[l_count++] = flag->name;
			 }

		 }
	 } /*else {

	buff [l_count++] = NULL;	
	 } */

	return l_count;
}

static inline int
read_elf_sh_addr (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;

	Elf32_Addr sh_addr;

	static char *val;

	val = malloc (VAL_MAX);

	static elf32_shdr_mem shaddr =
	{
		"|Virtual Address" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_addr) ;

	sh_addr = tmp->i ;

	buff[l_count++] = shaddr.name ;
	sprintf(val, "%1$#x (%1$i)\n", sh_addr);

        buff[l_count++] = NULL;

	buff[l_count++] = val ;

	free (tmp) ;

	return l_count;
}


static inline int
read_elf_sh_offset (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;

	Elf32_Off sh_offset;

	static char *val;

	val = malloc (VAL_MAX);

	static elf32_shdr_mem shoffset =
	{
		"|File Offset" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_offset) ;

	sh_offset = tmp->i ;

	buff[l_count++] = shoffset.name ;
	sprintf(val, "byte %1$#x (%1$i)\n", sh_offset);

        buff[l_count++] = NULL;

	buff[l_count++] = val ;

	free (tmp) ;

	return l_count;
}


static inline int
read_elf_sh_size (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;

	Elf32_Word sh_size;

	static char *val;

	val = malloc (VAL_MAX);

	static elf32_shdr_mem shsize =
	{
		"|Size" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	int l_count = 0;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_size) ;

	sh_size = tmp->i ;

	buff[l_count++] = shsize.name ;
	sprintf(val, "%1$#x bytes (%1$i)\n", sh_size);

        buff[l_count++] = NULL;

	buff[l_count++] = val ;

	free (tmp) ;

	return l_count;
}

__attribute__((always_inline))
static inline int
read_elf_sh_type (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	elf32_generic_value *tmp;


	Elf32_Word sh_type;
	int l_count = 0;

	static elf32_shdr_mem sh_types =
	{
		"|Section type" KEY_VALUE_DELIM,
		NULL,
		NULL,
		values_shtypes,
	};

	


	tmp = decode_elf_value('i', ei_data, &shdr->sh_type);

	sh_type = tmp->i;
	
	free (tmp) ;

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
read_elf_shdr (Elf32_Shdr *shdr, struct elf32_hdr *e_hdr, char *strtable, 
		void *fimg, char **buff)
{
	unsigned int l_count = 0;
	unsigned char ei_data = e_hdr->e_ident[EI_DATA];

	l_count += read_elf_sh_name (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_type (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_flags (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_addr (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_offset (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_size (shdr, strtable, ei_data, buff+l_count);
	buff[l_count++] = "\n\n";

	return l_count;
}

int
read_elf_shtab (struct elf32_hdr *e_hdr, void *fimg, char **buff)
{

	int l_count = 0;

	Elf32_Word shnum, shstrndx;

	const int ei_data = e_hdr->e_ident[EI_DATA]; 

	elf32_generic_value *tmp;

	Elf32_Shdr *shtab ;

	Elf32_Off sh_offset;

	if (fimg_annot.shtab == 0)
		GET_SHTAB (fimg_annot.shtab);

	shtab = fimg_annot.shtab;


	if (fimg_annot.shnum == 0)
		GET_SHNUM (fimg_annot.shnum);

	shnum = fimg_annot.shnum;

	if (fimg_annot.shstrndx == 0)
		GET_SHSTRNDX (fimg_annot.shstrndx, shtab);

	shstrndx = fimg_annot.shstrndx;

	decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
			&shtab[shstrndx].sh_offset) ;

	sh_offset = tmp->i ;

	char *strtable = fimg + sh_offset;

	for (int idx = 0; idx < shnum; idx++) {
		l_count += read_elf_shdr (&shtab[idx], e_hdr, strtable, fimg, buff+l_count);

	}


	return l_count;
}
