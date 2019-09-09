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

extern int
read_elf_header (struct elf32_hdr *e_hdr, void *fimage, char **buff);

extern int
read_elf_shtable (struct elf32_hdr *e_hdr, void *fimage, char **buff);

char *nl = "\n";

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


	 void *fimage = malloc(statp->st_size);

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

	char *buff[2000];

	unsigned int g_count = 0u;


	g_count += read_elf_header(e_hdr, fimage, buff+g_count);

	g_count += read_elf_shtable(e_hdr, fimage, buff+g_count);

	for (unsigned int str = 0u, s; str < g_count; str++) {

		if (buff[str] == NULL) {
			printf ("%s", buff[++str]);
			s = fflush (stdout);

			if (s == EOF) {
				perror ("fflush");
			} else {
				free (buff[str]);
						}

						} else {

		printf ("%s", buff[str]);
		}

	}

	free(fimage);

	close(fd);

	return 0;
}
