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
read_elf_e_machine (struct elf32_hdr *e_hdr, char *buff);

char *
read_elf_header (struct elf32_hdr *e_hdr, char *fimage, char *buff);

extern int
read_elf_osabi (struct elf32_hdr *e_hdr, char *buff);


extern union elf32_generic_value *
decode_elf_value (const char fmt, int endianness, int *vp);

static inline int
read_elf_e_shstrndx (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;
	
	union elf32_generic_value *tmp;

	elf32_hdr_mem e_shstrndx =
	{
		.name = "Index of str table entry in sh table (e_shstrndx)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_shstrndx.name, e_shstrndx.pad_start);

	tmp = decode_elf_value('s', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shstrndx) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->s);

	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_shnum (struct elf32_hdr *e_hdr, char *fimage, char *buff)
{

	int count = 0;
	
	Elf32_Word shnum;

	Elf32_Shdr *shdr;

	Elf32_Off sh_offset;

	char ei_data = e_hdr->e_ident[EI_DATA];

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_shnum =
	{
		.name = "Number of sh table entries (e_shnum)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_shnum.name, e_shnum.pad_start);

	tmp = decode_elf_value('s', ei_data, &e_hdr->e_shnum) ;

	shnum = (tmp->s != SHN_UNDEF)?
		tmp->s : 
		( 
		 tmp = decode_elf_value ('i', ei_data, &e_hdr->e_shoff), 
		 sh_offset = tmp->i, 
		 free (tmp), 
		 shdr = fimage + sh_offset, 
		 tmp = decode_elf_value ('i', ei_data, &shdr->sh_size), 
		 tmp->s 
		);

	count += sprintf(buff+count, "0x%1$x (%1$i)", shnum);

	free (tmp);
	
	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_shentsize (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_shentsize =
	{
		.name = "Bytes per sh table entry (e_shentsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_shentsize.name, e_shentsize.pad_start);

	tmp = decode_elf_value('s', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shentsize) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->s);
	
	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_phnum (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_phnum =
	{
		.name = "Number of ph table entries (e_phnum)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_phnum.name, e_phnum.pad_start);

	tmp = decode_elf_value('s', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phnum) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->s);

	free (tmp);
	
	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_phentsize (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_phentsize =
	{
		.name = "Bytes per ph table entry (e_phentsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_phentsize.name, e_phentsize.pad_start);

	tmp = decode_elf_value('s', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phentsize) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->s);
	
	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_ehsize (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_ehsize =
	{
		.name = "Size of header (e_ehsize)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_ehsize.name, e_ehsize.pad_start);

	tmp = decode_elf_value('s', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_ehsize) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->s);

	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}


static inline int
read_elf_e_flags (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_flags =
	{
		.name = "Flags (e_flags)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_flags.name, e_flags.pad_start);

	tmp = decode_elf_value('i', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_flags) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->i);
	
	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_shoff (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_shoff =
	{
		.name = "Section header table file offset(e_shoff)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_shoff.name, e_shoff.pad_start);

	tmp = decode_elf_value('i', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_shoff) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->i);

	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_phoff (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_phoff =
	{
		.name = "Program header table file offset(e_phoff)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_phoff.name, e_phoff.pad_start);

	tmp = decode_elf_value('i', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_phoff) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->i);
	
	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_e_entry (struct elf32_hdr *e_hdr, char *buff)
{

	int count = 0;

	union elf32_generic_value *tmp;
	elf32_hdr_mem e_entry =
	{
		.name = "Entry point(e_entry)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL, 
	};

	count += sprintf(buff, "%s%s", e_entry.name, e_entry.pad_start);

	tmp = decode_elf_value('i', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_entry) ;

	count += sprintf(buff+count, "0x%1$x (%1$i)", tmp->i);
	
	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}


static inline int
read_elf_e_type (struct elf32_hdr *e_hdr, char *buff)
{

#define ET_OS 5
#define ET_PROC 6

	int count = 0;

	union elf32_generic_value *tmp;
	Elf32_Half et;
	
	char *values[] = 
	{
		[ET_NONE] = "No file type", 

		[ET_REL] = "Relocatable file", 

		[ET_EXEC] = "Executable file",

		[ET_DYN] = "Shared object file",

		[ET_CORE] = "Core file",

		[ET_OS] = "Operating system-specific(pending)",

		[ET_PROC] = "Processor-specific(pending)",

	};

	elf32_hdr_mem e_type = 
	{
		.name = "Object file type(e_type)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values 
	};


	count += sprintf(buff, "%s%s", e_type.name, e_type.pad_start);

	tmp = decode_elf_value('s', (int)
			e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_type) ;

	et = tmp->s ;

	if (et >= ET_NONE || et <= ET_CORE) {

		count += sprintf(buff+count, "%s",
				e_type.values[et]);

	} else if (et >= ET_LOPROC || et <= ET_HIPROC) {

		count += sprintf(buff+count, "%s",
				e_type.values[ET_PROC]);

	} else if (et >= ET_LOOS || et <= ET_HIOS) {

		count += sprintf(buff+count, "%s",
				e_type.values[ET_OS]);

	} else {

		count += sprintf(buff+count, "%s",
				e_type.values[ET_NONE]);

	}

	free (tmp);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_magic (struct elf32_hdr *e_hdr, char *buff)
{
	char *values[] = {NULL};

	int count = 0;

	struct e_ident_el e_magic = 
	{
		"Magic(ELFMAG1-ELFMAG3)",
			KEY_VALUE_DELIM,
			NULL,
			values

	};

//to look good break sprintf() call into:

	count += sprintf(buff, "%s%s", e_magic.name, e_magic.pad_start);

	//and:

	count += sprintf(buff+count, "%c%c%c", e_hdr->e_ident[EI_MAG1],
			e_hdr->e_ident[EI_MAG2], e_hdr->e_ident[EI_MAG3]);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_class (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;
	char *values[] = 
	{
		"Invalid Class", 
		"32-bits Objects", 
		"64-bits Objects"
	};

	struct e_ident_el ei_class = 
	{
		.name = "Class(EI_CLASS)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values 
	};


	count += sprintf(buff, "%s%s", ei_class.name, ei_class.pad_start);


	switch (e_hdr->e_ident[EI_CLASS]) {

		case ELFCLASS32:
			{
				count += sprintf(buff+count, "%s",
						ei_class.values[ELFCLASS32]);
				break;
			}
		case ELFCLASS64:
			{
				count += sprintf(buff+count, "%s",
						ei_class.values[ELFCLASS64]);
				break;
			}
		case ELFCLASSNONE: default:
			{
				count += sprintf(buff+count,
						"%s", ei_class.values[ELFCLASSNONE]); 
				break; 			      }

	}

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_data (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;
	char *values[] = 
	{
		"Invalid Data Encoding",
		"Least Significant Byte",
		"Most Significant Byte"
	};

	struct e_ident_el ei_data = 
	{
		.name = "Data Encoding(EI_DATA)",
		.pad_start = KEY_VALUE_DELIM "2's Complement ",
		.pad_end = NULL,

		.values = values
	};

	count += sprintf(buff, "%s%s", ei_data.name, ei_data.pad_start); 

	switch (e_hdr->e_ident[EI_DATA]) {

		case ELFDATA2LSB:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATA2LSB]); 
				break;

			}

		case ELFDATA2MSB:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATA2MSB]); 
				break;

			}

		case ELFDATANONE: default:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATANONE]); 
				break;

			}

	}

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_version (struct elf32_hdr *e_hdr, char *buff)
{
	unsigned long long count = 0;

	union elf32_generic_value *tmp;
	Elf32_Word ei;

	char *values[] = 
	{
		"Invalid",

		"Current"
	};

	struct e_ident_el ei_version = 
	{
		.name = "Version(EI_VERSION)",
		.pad_start = KEY_VALUE_DELIM ,
		.pad_end = " Version\n",

		.values = values
	};

	
	count += sprintf(buff, "%s%s", ei_version.name, ei_version.pad_start); 
	tmp = decode_elf_value('i', 
			(int) e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_version) ;

	ei = tmp->i;

	switch (e_hdr->e_ident[EI_VERSION] | ei) {

		case EV_CURRENT:
			{

				count += sprintf(buff+count, "%s", ei_version.values[EV_CURRENT]); 
				break;

			}


		case EV_NONE: default:
			{

				count += sprintf(buff+count, "%s", ei_version.values[EV_NONE]); 
				break;

			}

	}

	count += sprintf(buff+count, "%s", ei_version.pad_end);

	free (tmp);

	return count;
}



static inline int
read_elf_ident (struct elf32_hdr *e_hdr, char *buff)
{
	unsigned long long count = 0;

	count += read_elf_magic(e_hdr, buff+count);
	count += read_elf_class(e_hdr, buff+count);
	count += read_elf_data(e_hdr, buff+count);
	count += read_elf_version(e_hdr, buff+count);
	count += read_elf_osabi(e_hdr, buff+count);
	return count;
}

char *
read_elf_header (struct elf32_hdr *e_hdr, char *fimage, char *buff)
{

	unsigned long long count = 0llu;

	count += read_elf_ident(e_hdr, buff+count);

	count += read_elf_e_type(e_hdr, buff+count);

	count += read_elf_e_machine(e_hdr, buff+count);

	count += read_elf_e_entry (e_hdr, buff+count);

	count += read_elf_e_phoff (e_hdr, buff+count);

	count += read_elf_e_shoff (e_hdr, buff+count);

	count += read_elf_e_flags (e_hdr, buff+count);

	count += read_elf_e_ehsize (e_hdr, buff+count);

	count += read_elf_e_phentsize (e_hdr, buff+count);

	count += read_elf_e_phnum (e_hdr, buff+count);

	count += read_elf_e_shentsize (e_hdr, buff+count);

	count += read_elf_e_shnum (e_hdr, fimage, buff+count);

	count += read_elf_e_shstrndx (e_hdr, buff+count);

	buff[count] = 0;

	return buff;

}
