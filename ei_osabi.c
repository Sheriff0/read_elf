
#ifndef ELFOSABI_LINUX

#include <elf.h>

#endif

#include "le.elf.h"
#include <stdio.h>

/* these are probably obsolete: */
#define ELFOSABI_ARM		97	/* ARM */
#define ELFOSABI_STANDALONE	255	/* standalone (embedded) application */


static char *osabis [] =
{

	[ELFOSABI_NONE] = "No extensions/unspecified",

		[ELFOSABI_HPUX] = "Hewlett-Packard/HP-UX",

		[ELFOSABI_NETBSD] = "NetBSD",

		[ELFOSABI_GNU | ELFOSABI_LINUX] = "GNU/Linux",

		[ELFOSABI_SOLARIS] = "Sun Solaris",

		[ELFOSABI_AIX] = "AIX",

		[ELFOSABI_IRIX] = "IRIX",

		[ELFOSABI_FREEBSD] = "FreeBSD",

		[ELFOSABI_TRU64] = "Compaq TRU64 UNIX",

		[ELFOSABI_MODESTO] = "Novell Modesto",

		[ELFOSABI_OPENBSD] = "Open BSD",

		[ELFOSABI_OPENVMS] = "Open VMS",

		[ELFOSABI_NSK] = "Hewlett-Packard Non-Stop Kernel",

		[ELFOSABI_AROS] = "Amiga Research OS",

		[ELFOSABI_FENIXOS] = "The FenixOS highly scalable multi-core OS",

		[ELFOSABI_CLOUDABI] = "Nuxi CloudABI",

		[ELFOSABI_OPENVOS] = "Stratus Technologies OpenVOS",

		[ELFOSABI_ARM_AEABI] = "ARM Embedded ABI",

		[ELFOSABI_ARM] = "ARM",

};

int
read_elf_osabi (struct elf32_hdr *e_hdr, char **buff)
{
	int l_count = 0;

	static struct e_ident_el ei_osabi =
	{
		.name = "OS/ABI(EI_OSABI)",
		.pad_start = KEY_VALUE_DELIM ,
		.pad_end = "NULL",

		.values = osabis,
	};

	(buff[l_count++] = ei_osabi.name, buff[l_count++] = ei_osabi.pad_start);

	switch (e_hdr->e_ident[EI_OSABI]) {

		case ELFOSABI_LINUX | ELFOSABI_GNU:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_LINUX];
				break;

			}

		case ELFOSABI_HPUX:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_HPUX];
				break;

			}

		case ELFOSABI_NETBSD:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_NETBSD];
				break;

			}

		case ELFOSABI_SOLARIS:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_SOLARIS];
				break;

			}

		case ELFOSABI_AIX:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_AIX];
				break;

			}

		case ELFOSABI_IRIX:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_IRIX];
				break;

			}

		case ELFOSABI_FREEBSD:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_FREEBSD];
				break;

			}

		case ELFOSABI_TRU64:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_TRU64];
				break;

			}

		case ELFOSABI_MODESTO:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_MODESTO];
				break;

			}

		case ELFOSABI_OPENBSD:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_OPENBSD];
				break;

			}

		case ELFOSABI_OPENVMS:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_OPENVMS];
				break;

			}

		case ELFOSABI_NSK:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_NSK];
				break;

			}

		case ELFOSABI_AROS:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_AROS];
				break;

			}

		case ELFOSABI_FENIXOS:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_FENIXOS];
				break;

			}

		case ELFOSABI_CLOUDABI:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_CLOUDABI];
				break;

			}

		case ELFOSABI_OPENVOS:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_OPENVOS];
				break;

			}


		case ELFOSABI_ARM_AEABI:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_ARM_AEABI];
				break;

			}

		case ELFOSABI_ARM:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_ARM];
				break;

			}


		case ELFOSABI_NONE | ELFOSABI_SYSV: default:
			{

				buff[l_count++] = ei_osabi.values[ELFOSABI_NONE];
				break;

			}

	}

	buff[l_count++] = "\n";

	return l_count;
}
