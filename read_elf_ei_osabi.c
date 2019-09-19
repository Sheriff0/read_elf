#ifndef ELFOSABI_LINUX
#include <elf.h>
#endif
#include "le.elf.h"
#include <stdio.h>
/* these are probably obsolete: */
#define ELFOSABI_ARM 97		/* ARM */
#define ELFOSABI_STANDALONE	255	/* standalone (embedded) application */
extern char *nl;

static elf32_node_t elfosabi_none;

static elf32_node_t elfosabi_hpux;

static elf32_node_t elfosabi_netbsd;

static elf32_node_t elfosabi_gnu_linux;

static elf32_node_t elfosabi_solaris;

static elf32_node_t elfosabi_aix;

static elf32_node_t elfosabi_irix;

static elf32_node_t elfosabi_freebsd;

static elf32_node_t elfosabi_tru64;

static elf32_node_t elfosabi_modesto;

static elf32_node_t elfosabi_openbsd;

static elf32_node_t elfosabi_openvms;

static elf32_node_t elfosabi_nsk;

static elf32_node_t elfosabi_aros;

static elf32_node_t elfosabi_fenixos;

static elf32_node_t elfosabi_cloudabi;

static elf32_node_t elfosabi_openvos;

static elf32_node_t elfosabi_arm_aeabi;

static elf32_node_t elfosabi_arm;

static elf32_node_t elfosabi_none = {
	.name = "System V",
	.field = ELFOSABI_NONE | ELFOSABI_SYSV,
	.next = &elfosabi_hpux
};

static elf32_node_t elfosabi_hpux = {
	.name = "Hewlett-Packard/HP-UX",
	.field = ELFOSABI_HPUX,
	.next = &elfosabi_netbsd
};

static elf32_node_t elfosabi_netbsd = {
	.name = "NetBSD",
	.field = ELFOSABI_NETBSD,
	.next = &elfosabi_gnu_linux
};

static elf32_node_t elfosabi_gnu_linux = {
	.name = "GNU/Linux",
	.field = ELFOSABI_GNU | ELFOSABI_LINUX,
	.next = &elfosabi_solaris
};

static elf32_node_t elfosabi_solaris = {
	.name = "Sun Solaris",
	.field = ELFOSABI_SOLARIS,
	.next = &elfosabi_aix
};

static elf32_node_t elfosabi_aix = {
	.name = "AIX",
	.field = ELFOSABI_AIX,
	.next = &elfosabi_irix
};

static elf32_node_t elfosabi_irix = {
	.name = "IRIX",
	.field = ELFOSABI_IRIX,
	.next = &elfosabi_freebsd
};

static elf32_node_t elfosabi_freebsd = {
	.name = "FreeBSD",
	.field = ELFOSABI_FREEBSD,
	.next = &elfosabi_tru64
};

static elf32_node_t elfosabi_tru64 = {
	.name = "Compaq TRU64 UNIX",
	.field = ELFOSABI_TRU64,
	.next = &elfosabi_modesto
};

static elf32_node_t elfosabi_modesto = {
	.name = "Novell Modesto",
	.field = ELFOSABI_MODESTO,
	.next = &elfosabi_openbsd
};

static elf32_node_t elfosabi_openbsd = {
	.name = "Open BSD",
	.field = ELFOSABI_OPENBSD,
	.next = &elfosabi_openvms
};

static elf32_node_t elfosabi_openvms = {
	.name = "Open VMS",
	.field = ELFOSABI_OPENVMS,
	.next = &elfosabi_nsk
};

static elf32_node_t elfosabi_nsk = {
	.name = "Hewlett-Packard Non-Stop Kernel",
	.field = ELFOSABI_NSK,
	.next = &elfosabi_aros
};

static elf32_node_t elfosabi_aros = {
	.name = "Amiga Research OS",
	.field = ELFOSABI_AROS,
	.next = &elfosabi_fenixos
};

static elf32_node_t elfosabi_fenixos = {
	.name = "The FenixOS highly scalable multi-core OS",
	.field = ELFOSABI_FENIXOS,
	.next = &elfosabi_cloudabi
};

static elf32_node_t elfosabi_cloudabi = {
	.name = "Nuxi CloudABI",
	.field = ELFOSABI_CLOUDABI,
	.next = &elfosabi_openvos
};

static elf32_node_t elfosabi_openvos = {
	.name = "Stratus Technologies OpenVOS",
	.field = ELFOSABI_OPENVOS,
	.next = &elfosabi_arm_aeabi
};

static elf32_node_t elfosabi_arm_aeabi = {
	.name = "ARM Embedded ABI",
	.field = ELFOSABI_ARM_AEABI,
	.next = &elfosabi_arm
};

static elf32_node_t elfosabi_arm = {
	.name = "ARM",
	.field = ELFOSABI_ARM,
	.next = &elfosabi_none
};

int read_elf_osabi(struct elf32_hdr *e_hdr, char **buff)
{
	int l_count = 0;

	static struct e_ident_el ei_osabi = {
		.name = "OS/ABI(EI_OSABI)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = NULL,
	};
	(buff[l_count++] = ei_osabi.name, buff[l_count++] = ei_osabi.pad_start);
	elf32_node_t *node = &elfosabi_none;

	char osabi = e_hdr->e_ident[EI_OSABI];

	if (osabi == node->field) {
		buff[l_count++] = node->name;
		goto wrap;
	}
	node = node->next;

	for (int f; (f = node->field) != 0; node = node->next) {
		if (f == osabi) {
			buff[l_count++] = node->name;
			goto wrap;
		}
	}

 wrap:
	buff[l_count++] = nl;
	return l_count;
}
