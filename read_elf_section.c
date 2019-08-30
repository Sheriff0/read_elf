#include <stdio.h>
#include <elf.h>
#include <readline/chardefs.h>

extern long long int
decode_elf_value (const char fmt, int endianness, int *value);

int
read_elf_sh_name (Elf32_Shdr *shdr, char *strtable, char ei_data, char *buff)
{
	Elf32_Off sh_name = decode_elf_value ('i', ei_data, &shdr->sh_name) ;

	int count = sprintf (buff, "%s", strtable+sh_name) ;

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

	Elf32_Shdr *shtable ;

	char *tmp = decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shoff) ;
	tmp += (int) fimage ;

	shtable = tmp;

	Elf32_Half shnum = decode_elf_value ('s', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shnum);
	Elf32_Half shstrndx = decode_elf_value ('s', e_hdr->e_ident[EI_DATA],
			&e_hdr->e_shstrndx) ;

	Elf32_Off sh_offset = (shstrndx == 0)? decode_elf_value ('i',
			e_hdr->e_ident[EI_DATA], &shtable[0].sh_link) :
		decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
				&shtable[shstrndx].sh_offset) ; 

	char *strtable = fimage + sh_offset;

	for (int idx = 0; idx < shnum; idx++) {
		count += read_elf_shdr (&shtable[idx], e_hdr, strtable, buff+count);

	}
	
	buff[count] = 0;

	return buff;
}
