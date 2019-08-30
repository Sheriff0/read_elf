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

extern char *
read_elf_header (struct elf32_hdr *e_hdr, char *buff);

extern char *
read_elf_shtable (struct elf32_hdr *, char *, char *);

int 
main (int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "read_elf [options] <objfile>\n");
		return 1;
	}

	struct stat *statp = malloc(sizeof(struct stat));

	int st = stat (argv[1], statp);

	if (st == -1) {
		perror ("stat");
	}

	struct elf32_hdr *e_hdr;


	int fd	= open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}


	 char *fimage = malloc(statp->st_size);

	if (fimage == NULL) {
		perror("malloc");
		return 1;
	}

	int rd = read(fd, fimage, statp->st_size);

	if (rd == -1) {
		perror("read");
		return 1;
	}

	e_hdr = fimage;

	char buff[2000];

	printf("%s", read_elf_header(e_hdr, buff));

	printf ("%s", read_elf_shtable(e_hdr, fimage, buff));

	free(fimage);

	free(fimage);

	close(fd);

	return 0;
}
