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
#include <getopt.h>
#include "le.elf.h"

extern int read_elf_symtabs(struct elf32_hdr *e_hdr, void *fimg, char **buff);

extern int read_elf_header(struct elf32_hdr *e_hdr, void *fimg, char **buff);
extern int read_elf_shtab(struct elf32_hdr *e_hdr, void *fimg, char **buff);
char *nl = "\n";

__attribute__((section(".bss")))
struct fimg_a fimg_annot = { };

const struct option ops_long[] = {
	{"file-header", no_argument, NULL, 'h'},
	{"section-header", no_argument, NULL, 'S'},
	{"symbols", no_argument, NULL, 's'},
	{"all", no_argument, NULL, 'a'},
	{0, 0, 0, 0},
};

char *ops_short = "+sSha";
int main(int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "read_elf [options] <objfile>\n");
		return 1;
	}
	struct stat *statp = malloc(sizeof(struct stat));
	int st = stat(argv[argc - 1], statp);

	if (st == -1) {
		perror("stat");
	}
	struct elf32_hdr *e_hdr;
	int fd = open(argv[argc - 1], O_RDWR);

	if (fd < 0) {
		perror("open");
		return 1;
	}
	void *fimg = malloc(statp->st_size);

	if (fimg == NULL) {
		perror("malloc");
		return 1;
	}
	int rd = read(fd, fimg, statp->st_size);

	if (rd == -1) {
		perror("read");
		return 1;
	}
	fimg_annot.e_hdr = e_hdr = fimg;
	char *buff[10000];
	unsigned int g_count = 0u;

	for (int c, longindex = 0;
	     (c =
	      getopt_long(argc, argv, ops_short, ops_long,
			  &longindex)) != -1;) {
		if (c == 'h' || c == 'a')
			g_count += read_elf_header(e_hdr, fimg, buff + g_count);
		if (c == 'S' || c == 'a')
			g_count += read_elf_shtab(e_hdr, fimg, buff + g_count);
		if (c == 's' || c == 'a')
			g_count +=
			    read_elf_symtabs(e_hdr, fimg, buff + g_count);
	}
	for (unsigned int str = 0u, s; str < g_count; str++) {
		if (buff[str] == NULL) {
			printf("%s", buff[++str]);
			s = fflush(stdout);
			if (s == EOF) {
				perror("fflush");
			} else {
				free(buff[str]);
			}
		} else {
			printf("%s", buff[str]);
		}
	}
	free(fimg);
	close(fd);
	return 0;
}
