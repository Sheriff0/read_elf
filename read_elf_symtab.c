#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "le.elf.h"

extern int
read_elf_sh_name(Elf32_Shdr * shdr, void *fimg, char ei_data, char **buff,
		 _Bool in_line);

extern elf32_generic_value *get_mb_elf_value(const char fmt, int endianness,
					     elf32_generic_value * buff, ...);

static inline int
read_elf_st_name(Elf32_Sym * sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word st_name;

	static elf32_sym_mem stname = {
		"Symbol name" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	get_mb_elf_value('i', ei_data, &tmp, &sym->st_name);

	st_name = tmp.i;

	buff[l_count++] = stname.name;

	buff[l_count++] = strtab + st_name;

	return l_count;

}

static inline int
read_elf_st_value(Elf32_Sym * sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	elf32_generic_value tmp = {.l = 0l };

	Elf32_Addr st_value;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_sym_mem stvalue = {
		"|value" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	get_mb_elf_value('i', ei_data, &tmp, &sym->st_value);

	st_value = tmp.i;

	buff[l_count++] = stvalue.name;

	sprintf(val, "%1$#x (%1$i)\n", st_value);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;

}

static inline int
read_elf_st_size(Elf32_Sym * sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	elf32_generic_value tmp = {.l = 0l };

	Elf32_Word st_size;

	static char *val;

	val = malloc(VAL_MAX);

	static elf32_sym_mem stsize = {
		"|size" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	get_mb_elf_value('i', ei_data, &tmp, &sym->st_size);

	st_size = tmp.i;

	buff[l_count++] = stsize.name;

	sprintf(val, "%1$#x (%1$i)\n", st_size);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	return l_count;

}

__attribute__((always_inline))
static inline char *read_elf_st_bind(Elf32_Sym * sym)
{

	unsigned char st_bind;

	static char *values[] = {
		[STB_LOCAL] = "Local",

		[STB_GLOBAL] = "Global",

		[STB_WEAK] = "Weak",

		[STB_LOOS] = "OS-low",

		[STB_HIOS] = "OS-High",

		[STB_LOPROC] = "Processor-Low",
		[STB_HIPROC] = "Processor-high",
	};

	st_bind = ELF32_ST_BIND(sym->st_info);

	if (st_bind >= STB_LOCAL && st_bind < STB_LOOS)
		return values[st_bind];
	else
		return values[STB_LOCAL];

}

__attribute__((always_inline))
static inline char *read_elf_st_type(Elf32_Sym * sym)
{

	unsigned char st_type;

	static char *values[] = {
		[STT_NOTYPE] = "No type",

		[STT_OBJECT] = "Object",

		[STT_FUNC] = "Function",

		[STT_SECTION] = "Section",

		[STT_FILE] = "File",

		[STT_TLS] = "Thread local",

		[STT_LOPROC] = "Processor-low",
		[STT_HIPROC] = "Processor-high",
		[STT_LOOS] = "OS-low",

		[STT_HIOS] = "OS-high",
	};

	st_type = ELF32_ST_TYPE(sym->st_info);

	if (st_type >= STT_NOTYPE && st_type < STT_LOOS)
		return values[st_type];
	else
		return values[STT_NOTYPE];

}

static inline int
read_elf_st_info(Elf32_Sym * sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	static elf32_sym_mem stinfo = {
		"|bind/type" KEY_VALUE_DELIM,
		" / ",
		NULL,
		NULL,
	};

	buff[l_count++] = stinfo.name;

	buff[l_count++] = read_elf_st_bind(sym);

	buff[l_count++] = stinfo.pad_start;
	buff[l_count++] = read_elf_st_type(sym);

	return l_count;

}

static inline int
read_elf_st_other(Elf32_Sym * sym, char *strtab, char ei_data, char **buff)
{
	int l_count = 0;

	unsigned char st_other;

	static char *values[] = {
		[STV_DEFAULT] = "Default",
		[STV_INTERNAL] = "Internal",
		[STV_HIDDEN] = "Hidden",
		[STV_PROTECTED] = "Protected",
	};

	static elf32_sym_mem stother = {
		"|Visibility" KEY_VALUE_DELIM,
		NULL,
		NULL,
		values,
	};

	st_other = sym->st_other & 3;

	buff[l_count++] = stother.name;

	buff[l_count++] = stother.values[st_other];

	return l_count;

}

static inline int
read_elf_st_shndx(Elf32_Sym * sym, char *strtab, void *fimg, char ei_data,
		  int idx, Elf32_Word symtab_shndx[], char **buff)
{
	int l_count = 0;

	Elf32_Word st_shndx;

	elf32_generic_value tmp = {.l = 0l };

	static elf32_sym_mem stndx = {
		"|Section" KEY_VALUE_DELIM,
		NULL,
		NULL,
		NULL,
	};

	get_mb_elf_value('s', ei_data, &tmp, &sym->st_shndx);

	st_shndx = (tmp.s == SHN_XINDEX) ?
	    (get_mb_elf_value('i', ei_data, &tmp, &symtab_shndx[idx]),
	     tmp.i) : tmp.s;

	buff[l_count++] = stndx.name;

	l_count +=
	    read_elf_sh_name(&fimg_annot.shtab[st_shndx], fimg,
			     ei_data, buff + l_count, 1);

	return l_count;

}

static inline int
read_elf_sym(Elf32_Sym * sym, char *strtab, void *fimg, char ei_data,
	     Elf32_Word symtab_shndx[], int idx, char **buff)
{
	int l_count = 0;

	char *val;

	val = malloc(VAL_MAX);

	sprintf(val, "(%d)\n", idx);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	l_count += read_elf_st_name(sym, strtab, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_st_value(sym, strtab, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_st_size(sym, strtab, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_st_info(sym, strtab, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count += read_elf_st_other(sym, strtab, ei_data, buff + l_count);

	buff[l_count++] = "\n\t";

	l_count +=
	    read_elf_st_shndx(sym, strtab, fimg, ei_data, idx, symtab_shndx,
			      buff + l_count);

	buff[l_count++] = "\n\n";

	return l_count;
}

int read_elf_symtabs(struct elf32_hdr *e_hdr, void *fimg, char **buff)
{
	Elf32_Sym *symtab;

	Elf32_Word st_num;

	char *val;

	int l_count = 0;

	elf32_generic_value tmp = {.l = 0l };

	const char ei_data = e_hdr->e_ident[EI_DATA];

	if (fimg_annot.symtab == NULL) {
		if (fimg_annot.shtab == NULL) {
			GET_SHTAB(fimg_annot.shtab);
			GET_SHNUM(fimg_annot.shnum, fimg_annot.shtab);

		}

		int ndx = 0, t = 0, t0 = 0, t1 = 0;

		Elf32_Shdr *shdr;

		do {

			shdr = &fimg_annot.shtab[ndx];
			get_mb_elf_value('i', ei_data, &tmp, &shdr->sh_type);
			t = tmp.i;

			if (t == SHT_SYMTAB) {
				t0 = ndx;
				if (t1)
					break;
			} else if (t == SHT_SYMTAB_SHNDX) {
				t1 = ndx;

				if (t0)
					break;
			}

		}
		while (++ndx < fimg_annot.shnum);

		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t0].sh_offset);

		fimg_annot.symtab = fimg + tmp.i;

		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t0].sh_size);
		t = tmp.i;

		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t0].sh_entsize);

		fimg_annot.symtabnum = t / tmp.i;
		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t0].sh_link);
		t = tmp.i;

		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t].sh_offset);

		fimg_annot.symtabstrtab = fimg + tmp.i;

		get_mb_elf_value('i', ei_data, &tmp,
				 &fimg_annot.shtab[t1].sh_offset);

		fimg_annot.symtab_shndx = fimg + tmp.i;

	}

	symtab = fimg_annot.symtab;

	st_num = fimg_annot.symtabnum;

	val = malloc(VAL_MAX);

	sprintf(val, "%1$#x (%1$d) entries found\n", st_num);

	buff[l_count++] = NULL;

	buff[l_count++] = val;

	for (int idx = 0; idx < st_num; idx++) {
		l_count +=
		    read_elf_sym(&symtab[idx], fimg_annot.symtabstrtab, fimg,
				 ei_data, fimg_annot.symtab_shndx, idx,
				 buff + l_count);

	}

	return l_count;
}
