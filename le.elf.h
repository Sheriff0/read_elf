#define KEY_VALUE_DELIM ": "

struct e_ident_el 
{
	char *name;
	char *pad_start;
	char *pad_end;
	char **values;
};

typedef struct e_ident_el elf32_hdr_mem;

