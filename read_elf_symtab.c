#include <elf.h>
#include <stdio.h>
#include "le.elf.h"


extern elf32_generic_value *
get_mb_elf_value (const char fmt, int endianness, elf32_generic_value *buff, ...);

static inline int
read_elf_st_name (Elf32_Sym *sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	elf32_generic_value tmp = { .l = 0l };

	Elf32_Word st_name;

	static elf32_sym_mem stname =
	{
		"Symbol name" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};


	get_mb_elf_value ('i', ei_data, &tmp, &sym->st_name);

	st_name = tmp.i ;

	buff[l_count++] = stname.name;

	buff[l_count++] = strtab + sym->st_name;

	return l_count;

}


static inline int
read_elf_sym (Elf32_Sym *sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	l_count += read_elf_st_name (sym, strtab, ei_data, buff+l_count);

	buff[l_count++] = "\n\n";

	return l_count;
}

int
read_elf_symtabs (struct elf32_hdr *e_hdr, void *fimg, char **buff)
{
	Elf32_Sym *symtab;

	Elf32_Word st_num;

	int l_count = 0;

	elf32_generic_value tmp = { .l = 0l };

	const char ei_data = e_hdr->e_ident[EI_DATA];

	if (fimg_annot.symtab == NULL) {
		if (fimg_annot.shtab == NULL) {
			GET_SHTAB (fimg_annot.shtab);
			GET_SHNUM (fimg_annot.shnum, fimg_annot.shtab);

		}

		int ndx = 0, t;

		Elf32_Shdr *shdr;

		do {

			shdr = &fimg_annot.shtab[ndx];
				get_mb_elf_value ('i', ei_data, &tmp, &shdr->sh_type);
				t = tmp.i;

			if (t == SHT_SYMTAB) {
				break;
			}


		} while (++ndx < fimg_annot.shnum);

					get_mb_elf_value ('i', ei_data, &tmp, &shdr->sh_offset);

			fimg_annot.symtab = fimg + tmp.i;

			get_mb_elf_value ('i', ei_data, &tmp, &shdr->sh_size); 
			t = tmp.i;

			get_mb_elf_value ('i', ei_data, &tmp, &shdr->sh_entsize);

			fimg_annot.symtabnum = 	t / tmp.i;
			get_mb_elf_value ('i', ei_data, &tmp, &shdr->sh_link);	
			t = tmp.i;

			get_mb_elf_value ('i', ei_data, &tmp,
					&fimg_annot.shtab[t].sh_offset);

			fimg_annot.symtabstrtab = fimg + tmp.i;

	}

	symtab = fimg_annot.symtab;

	st_num = fimg_annot.symtabnum;

	for (int idx = 0; idx < st_num; idx++) {
		l_count += read_elf_sym (&symtab[idx], fimg_annot.symtabstrtab, ei_data, buff+l_count);

	}

	return l_count;
}
