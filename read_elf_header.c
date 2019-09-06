#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/chardefs.h>

#include "le.elf.h"

int
read_elf_e_machine (struct elf32_hdr *e_hdr, char **buff);

int
read_elf_header (struct elf32_hdr *e_hdr, void *fimage, char **buff);

extern int
read_elf_osabi (struct elf32_hdr *e_hdr, char **buff);


extern union elf32_generic_value *
decode_elf_value (const char fmt, int endianness, ...);

static inline int
read_elf_e_shstrndx (struct elf32_hdr *e_hdr, void *fimage, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;

	static char val[VAL_MAX];

	static char *values[] =
	{
	  val,
	};

	Elf32_Shdr *shtable ;

	Elf32_Off sh_offset;

	const int ei_data = e_hdr->e_ident[EI_DATA];

	static elf32_hdr_mem e_shstrndx =
	{
		.name = "Index of str table entry in sh table (e_shstrndx)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_shstrndx.name, buff[l_count++] = e_shstrndx.pad_start);

	tmp = decode_elf_value('s',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shstrndx) ;

	Elf32_Word shstrndx = tmp->s;

	free (tmp);

	tmp = decode_elf_value ('i', ei_data,
			&e_hdr->e_shoff) ;


	shtable = fimage + tmp->i;

	free (tmp);
	if (shstrndx == SHN_XINDEX) {

		tmp = decode_elf_value ('i', e_hdr->e_ident[EI_DATA],
				&shtable[0].sh_link);
		shstrndx = tmp->i;
		free (tmp);
	}

	sprintf(e_shstrndx.values[0], "%1$#x (%1$i)\n", shstrndx);

	free (tmp);

	buff[l_count++] = e_shstrndx.values[0];
	return l_count;
}

static inline int
read_elf_e_shnum (struct elf32_hdr *e_hdr, void *fimage, char **buff)
{

	int l_count = 0;

	Elf32_Word shnum;

	Elf32_Shdr *shdr;

	Elf32_Off sh_offset;

	char ei_data = e_hdr->e_ident[EI_DATA];

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
	  val,
	};

	static elf32_hdr_mem e_shnum =
	{
		.name = "Number of sh table entries (e_shnum)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_shnum.name, buff[l_count++] = e_shnum.pad_start);

	tmp = decode_elf_value('s', ei_data, &e_hdr->e_shnum) ;

	shnum = (tmp->s != SHN_UNDEF)?
		tmp->s :
		(
		 tmp = decode_elf_value ('i', ei_data, &e_hdr->e_shoff),
		 sh_offset = tmp->i,
		 free (tmp),
		 shdr = fimage + sh_offset,
		 tmp = decode_elf_value ('i', ei_data, &shdr->sh_size),
		 tmp->i
		);


	sprintf(e_shnum.values[0], "%1$#x (%1$i)\n", shnum);

	free (tmp);

	buff[l_count++] = e_shnum.values[0];
	return l_count;
}

static inline int
read_elf_e_shentsize (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
	  val,
	};
	
	static elf32_hdr_mem e_shentsize =
	{
		.name = "Bytes per sh table entry (e_shentsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_shentsize.name, buff[l_count++] = e_shentsize.pad_start);

	tmp = decode_elf_value('s',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shentsize) ;


	sprintf(e_shentsize.values[0], "%1$#x (%1$i)\n", tmp->s);

	free (tmp);

	buff[l_count++] = e_shentsize.values[0];
	return l_count;
}

static inline int
read_elf_e_phnum (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
	  val,
	};

	static elf32_hdr_mem e_phnum =
	{
		.name = "Number of ph table entries (e_phnum)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_phnum.name, buff[l_count++] = e_phnum.pad_start);

	tmp = decode_elf_value('s',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phnum) ;


	sprintf(e_phnum.values[0], "%1$#x (%1$i)\n", tmp->s);

	free (tmp);

	buff[l_count++] = e_phnum.values[0];
	return l_count;
}

static inline int
read_elf_e_phentsize (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};

	static elf32_hdr_mem e_phentsize =
	{
		.name = "Bytes per ph table entry (e_phentsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_phentsize.name, buff[l_count++] = e_phentsize.pad_start);

	tmp = decode_elf_value('s',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phentsize) ;


	sprintf(e_phentsize.values[0], "%1$#x (%1$i)\n", tmp->s);

	free (tmp);

	buff[l_count++] = e_phentsize.values[0];
	return l_count;
}

static inline int
read_elf_e_ehsize (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};

	static elf32_hdr_mem e_ehsize =
	{
		.name = "Size of header (e_ehsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_ehsize.name, buff[l_count++] = e_ehsize.pad_start);

	tmp = decode_elf_value('s',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_ehsize) ;

	sprintf(e_ehsize.values[0], "%1$#x (%1$i)\n", tmp->s);

	free (tmp);

	buff[l_count++] = e_ehsize.values[0];
	return l_count;
}


static inline int
read_elf_e_flags (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};


	static elf32_hdr_mem e_flags =
	{
		.name = "Flags (e_flags)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_flags.name, buff[l_count++] = e_flags.pad_start);

	tmp = decode_elf_value('i',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_flags) ;

	sprintf(e_flags.values[0], "%1$#x (%1$i)\n", tmp->i);

	free (tmp);

	buff[l_count++] = e_flags.values[0];
	return l_count;
}

static inline int
read_elf_e_shoff (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};


	static elf32_hdr_mem e_shoff =
	{
		.name = "Section header table file offset(e_shoff)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_shoff.name, buff[l_count++] = e_shoff.pad_start);

	tmp = decode_elf_value('i',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shoff) ;

	sprintf(e_shoff.values[0], "%1$#x (%1$i)\n", tmp->i);

	free (tmp);

	buff[l_count++] = e_shoff.values[0];
	return l_count;
}

static inline int
read_elf_e_phoff (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};


	static elf32_hdr_mem e_phoff =
	{
		.name = "Program header table file offset(e_phoff)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_phoff.name, buff[l_count++] = e_phoff.pad_start);

	tmp = decode_elf_value('i',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phoff) ;

	sprintf(e_phoff.values[0], "%1$#x (%1$i)\n", tmp->i);

	free (tmp);

	buff[l_count++] = e_phoff.values[0];
	return l_count;
}

static inline int
read_elf_e_entry (struct elf32_hdr *e_hdr, char **buff)
{

	int l_count = 0;

	union elf32_generic_value *tmp;
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};


	static elf32_hdr_mem e_entry =
	{
		.name = "Entry point(e_entry)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values,
	};

	(buff[l_count++] = e_entry.name, buff[l_count++] = e_entry.pad_start);

	tmp = decode_elf_value('i',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_entry) ;

	sprintf(e_entry.values[0], "%1$#x (%1$i)\n", tmp->i);

	free (tmp);

	buff[l_count++] = e_entry.values[0];
	return l_count;
}


static inline int
read_elf_e_type (struct elf32_hdr *e_hdr, char **buff)
{

#define ET_OS 5
#define ET_PROC 6

	int l_count = 0;

	union elf32_generic_value *tmp;
	Elf32_Half et;

	static char *values[] =
	{
		[ET_NONE] = "No file type",

		[ET_REL] = "Relocatable file",

		[ET_EXEC] = "Executable file",

		[ET_DYN] = "Shared object file",

		[ET_CORE] = "Core file",

		[ET_OS] = "Operating system-specific(pending)",

		[ET_PROC] = "Processor-specific(pending)",

	};

	static elf32_hdr_mem e_type =
	{
		.name = "Object file type(e_type)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values
	};


	(buff[l_count++] = e_type.name, buff[l_count++] = e_type.pad_start);

	tmp = decode_elf_value('s', (int)
			e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_type) ;

	et = tmp->s ;

	if (et >= ET_NONE || et <= ET_CORE) {

		buff[l_count++] = e_type.values[et];

	} else if (et >= ET_LOPROC || et <= ET_HIPROC) {

		buff[l_count++] = e_type.values[ET_PROC];

	} else if (et >= ET_LOOS || et <= ET_HIOS) {

		buff[l_count++] = e_type.values[ET_OS];

	} else {

		buff[l_count++] = e_type.values[ET_NONE];

	}

	free (tmp);

	buff[l_count++] = "\n";
	return l_count;
}

static inline int
read_elf_magic (struct elf32_hdr *e_hdr, char **buff)
{
	static char val[VAL_MAX];

	static char *values[] =
	{
		val,
	};


	int l_count = 0;

	static struct e_ident_el ei_magic =
	{
		"Magic(ELFMAG1-ELFMAG3)",
			KEY_VALUE_DELIM,
			NULL,
			values

	};

//to look good break sprintf() call into:

	(buff[l_count++] = ei_magic.name, buff[l_count++] = ei_magic.pad_start);

	//and:

	sprintf(values[0], "%c%c%c\n", e_hdr->e_ident[EI_MAG1],
			e_hdr->e_ident[EI_MAG2], e_hdr->e_ident[EI_MAG3]);

	buff[l_count++] = values[0];
	return l_count;
}

static inline int
read_elf_class (struct elf32_hdr *e_hdr, char **buff)
{
	int l_count = 0;
	static char *values[] =
	{
		"Invalid Class",
		"32-bits Objects",
		"64-bits Objects"
	};

	static struct e_ident_el ei_class =
	{
		.name = "Class(EI_CLASS)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values
	};


	(buff[l_count++] = ei_class.name, buff[l_count++] = ei_class.pad_start);


	switch (e_hdr->e_ident[EI_CLASS]) {

		case ELFCLASS32:
			{
				buff[l_count++] = ei_class.values[ELFCLASS32];
				break;
			}
		case ELFCLASS64:
			{
				buff[l_count++] = ei_class.values[ELFCLASS64];
				break;
			}
		case ELFCLASSNONE: default:
			{
				buff[l_count++] = ei_class.values[ELFCLASSNONE];
				break; 			      }

	}

	buff[l_count++] = "\n";
	return l_count;
}

static inline int
read_elf_data (struct elf32_hdr *e_hdr, char **buff)
{
	int l_count = 0;
	static char *values[] =
	{
		"Invalid Data Encoding",
		"Least Significant Byte",
		"Most Significant Byte"
	};

	static struct e_ident_el ei_data =
	{
		.name = "Data Encoding(EI_DATA)",
		.pad_start = KEY_VALUE_DELIM "2's Complement ",
		.pad_end = NULL,

		.values = values
	};

	(buff[l_count++] = ei_data.name, buff[l_count++] = ei_data.pad_start);

	switch (e_hdr->e_ident[EI_DATA]) {

		case ELFDATA2LSB:
			{

				buff[l_count++] = ei_data.values[ELFDATA2LSB];
				break;

			}

		case ELFDATA2MSB:
			{

				buff[l_count++] = ei_data.values[ELFDATA2MSB];
				break;

			}

		case ELFDATANONE: default:
			{

				buff[l_count++] = ei_data.values[ELFDATANONE];
				break;

			}

	}

	buff[l_count++] = "\n";
	return l_count;
}

static inline int
read_elf_version (struct elf32_hdr *e_hdr, char **buff)
{
	unsigned long long l_count = 0;

	union elf32_generic_value *tmp;
	Elf32_Word ei;

	static char *values[] =
	{
		"Invalid",

		"Current"
	};

	static struct e_ident_el ei_version =
	{
		.name = "Version(EI_VERSION)",
		.pad_start = KEY_VALUE_DELIM ,
		.pad_end = " Version\n",

		.values = values
	};


	(buff[l_count++] = ei_version.name, buff[l_count++] = ei_version.pad_start);
	tmp = decode_elf_value('i',
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_version) ;

	ei = tmp->i;

	switch (e_hdr->e_ident[EI_VERSION] | ei) {

		case EV_CURRENT:
			{

				buff[l_count++] = ei_version.values[EV_CURRENT];
				break;

			}


		case EV_NONE: default:
			{

				buff[l_count++] = ei_version.values[EV_NONE];
				break;

			}

	}

	buff[l_count++] = ei_version.pad_end;

	free (tmp);

	return l_count;
}



static inline int
read_elf_ident (struct elf32_hdr *e_hdr, char **buff)
{
	unsigned long long l_count = 0;

	l_count += read_elf_magic(e_hdr, buff+l_count);
	l_count += read_elf_class(e_hdr, buff+l_count);
	l_count += read_elf_data(e_hdr, buff+l_count);
	l_count += read_elf_version(e_hdr, buff+l_count);
	l_count += read_elf_osabi(e_hdr, buff+l_count);
	return l_count;
}

int
read_elf_header (struct elf32_hdr *e_hdr, void *fimage, char **buff)
{

	unsigned long long l_count = 0llu;

	l_count += read_elf_ident(e_hdr, buff+l_count);

	l_count += read_elf_e_type(e_hdr, buff+l_count);

	l_count += read_elf_e_machine(e_hdr, buff+l_count);

	l_count += read_elf_e_entry (e_hdr, buff+l_count);

	l_count += read_elf_e_phoff (e_hdr, buff+l_count);

	l_count += read_elf_e_shoff (e_hdr, buff+l_count);

	l_count += read_elf_e_flags (e_hdr, buff+l_count);

	l_count += read_elf_e_ehsize (e_hdr, buff+l_count);

	l_count += read_elf_e_phentsize (e_hdr, buff+l_count);

	l_count += read_elf_e_phnum (e_hdr, buff+l_count);

	l_count += read_elf_e_shentsize (e_hdr, buff+l_count);

	l_count += read_elf_e_shnum (e_hdr, fimage, buff+l_count);

	l_count += read_elf_e_shstrndx (e_hdr, fimage, buff+l_count);

	buff[l_count] = NULL;

	return l_count;

}
