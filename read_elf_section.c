#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>
#include "le.elf.h"


extern union elf32_generic_value *
decode_elf_value (const char fmt, int endianness, ...);

static inline int
read_elf_sh_name (Elf32_Shdr *shdr, char *strtable, char ei_data, char **buff)
{
	union elf32_generic_value *tmp;
	Elf32_Off sh_name;

	int l_count = 0;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_name) ;

	sh_name = tmp->i ;

	buff[l_count++] = strtable+sh_name ;

	free (tmp) ;
	return l_count;
}

int
read_elf_shdr (Elf32_Shdr *shdr, struct elf32_hdr *e_hdr, char *strtable, 
		void *fimage, char **buff)
{
	unsigned int l_count = 0;
	unsigned char ei_data = e_hdr->e_ident[EI_DATA];

	l_count += read_elf_sh_name (shdr, strtable, ei_data, buff+l_count);

	buff[l_count++] = "\n";

	return l_count;
}

int
read_elf_shtable (struct elf32_hdr *e_hdr, void *fimage, char **buff)
{

	int l_count = 0;

	Elf32_Word shnum, shstrndx;

	const int ei_data = e_hdr->e_ident[EI_DATA]; 

	union elf32_generic_value *tmp;

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
