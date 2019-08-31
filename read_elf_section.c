#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>
#include "le.elf.h"


extern union elf32_generic_value *
decode_elf_value (const char fmt, int endianness, int *vp);

int
read_elf_sh_name (Elf32_Shdr *shdr, char *strtable, char ei_data, char *buff)
{
	union elf32_generic_value *tmp;
	Elf32_Off sh_name;

	tmp = decode_elf_value ('i', ei_data, &shdr->sh_name) ;

	sh_name = tmp->i ;

	int count = sprintf (buff, "%s", strtable+sh_name) ;

	free (tmp) ;
	return count; 
}

int
read_elf_shdr (Elf32_Shdr *shdr, struct elf32_hdr *e_hdr, char *strtable,
		char *buff) 
{
	unsigned int count = 0;
	unsigned char ei_data = e_hdr->e_ident[EI_DATA];
	
	count += read_elf_sh_name (shdr, strtable, ei_data, buff+count);
	
	buff[count++] = '\n';

	return count;
}

char *
read_elf_shtable (struct elf32_hdr *e_hdr, char *fimage, char *buff)
{

	int count = 0;
	char *off;

	Elf32_Half shnum, shstrndx;

	union elf32_generic_value *tmp;

	Elf32_Shdr *shtable ;
	
	Elf32_Off sh_offset;

	tmp = decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shoff) ;
	off = tmp->i ;

	off += (int) fimage ;

	shtable = off;

	free (tmp);

	tmp = decode_elf_value ('s', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shnum);
	shnum = tmp->s ;

	free (tmp);

	 tmp = decode_elf_value ('s', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shstrndx) ;

	shstrndx = tmp->s ;

	free (tmp);

	tmp = (shstrndx == 0)? decode_elf_value ('i',
			e_hdr->e_ident[EI_DATA], &shtable[0].sh_link) :
		decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
				&shtable[shstrndx].sh_offset) ; 

	sh_offset = tmp->i ;

	char *strtable = fimage + sh_offset;

	for (int idx = 0; idx < shnum; idx++) {
		count += read_elf_shdr (&shtable[idx], e_hdr, strtable, buff+count);

	}
	
	buff[count] = 0;

	return buff;
}
