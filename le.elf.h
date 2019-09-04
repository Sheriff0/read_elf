#include <elf.h>
#define KEY_VALUE_DELIM ": "
#define VAL_MAX 100

struct e_ident_el 
{
	char *name;
	char *pad_start;
	char *pad_end;
	char **values;
};

typedef struct e_ident_el elf32_hdr_mem;

union elf32_generic_value
{
	Elf32_Word i;
	Elf32_Half s;
	unsigned char b;
	unsigned long long l; 
};
