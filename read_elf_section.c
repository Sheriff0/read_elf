#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>
#include "le.elf.h"


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
	if (*buff[l_count-1] == 0)
		buff[l_count-1] = NULL;

	free (tmp) ;
	return l_count;
}

__attribute__((always_inline))
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
	 } else {

	buff [l_count++] = NULL;	
	 }

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
		void *fimage, char **buff)
{
	unsigned int l_count = 0;
	unsigned char ei_data = e_hdr->e_ident[EI_DATA];

	l_count += read_elf_sh_name (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_type (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_sh_flags (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n\n";

	return l_count;
}

int
read_elf_shtable (struct elf32_hdr *e_hdr, void *fimage, char **buff)
{

	int l_count = 0;

	Elf32_Word shnum, shstrndx;

	const int ei_data = e_hdr->e_ident[EI_DATA]; 

	elf32_generic_value *tmp;

	Elf32_Shdr *shtable ;

	Elf32_Off sh_offset;

	tmp = decode_elf_value ('i', ei_data,
			&e_hdr->e_shoff) ;


	shtable = fimage + tmp->i;

	free (tmp);

	tmp = decode_elf_value ('s', ei_data,
			&e_hdr->e_shnum);
	shnum = (tmp->s)?
		tmp->s :
		(
		  tmp = decode_elf_value ('i', ei_data, &shtable[0].sh_size),
		  tmp->i
		);

	free (tmp);

	 tmp = decode_elf_value ('s', ei_data, &e_hdr->e_shstrndx) ;

	shstrndx = tmp->s ;

	free (tmp);

	if (shstrndx == SHN_XINDEX) {

		tmp = decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
				&shtable[0].sh_link);
		shstrndx = tmp->i;
		free (tmp);
	}
		

		decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
				&shtable[shstrndx].sh_offset) ;

	sh_offset = tmp->i ;

	char *strtable = fimage + sh_offset;

	for (int idx = 0; idx < shnum; idx++) {
		l_count += read_elf_shdr (&shtable[idx], e_hdr, strtable, fimage, buff+l_count);

	}

	buff[l_count] = NULL;

	return l_count;
}
