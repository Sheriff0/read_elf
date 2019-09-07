#include <elf.h>
#define KEY_VALUE_DELIM ": "
#define VAL_MAX 100
#define SHT_OS 19
#define SHT_PROC 20
#define SHT_USER 21

struct e_ident_el 
{
	char *name;
	char *pad_start;
	char *pad_end;
	char **values;
};

typedef struct e_ident_el elf32_hdr_mem;

typedef struct e_ident_el elf32_shdr_mem;

typedef union 
{
	Elf32_Word i;
	Elf32_Half s;
	long long l; 
} elf32_generic_value;

typedef struct elf32_node 
{
  char *name;
  Elf32_Word field;
  struct elf32_node *next;
} elf32_node_t;


typedef struct elf32_node_r 
{
  char *name;
  Elf32_Word field;
  Elf32_Word l_len;
  elf32_node_t *next;
} elf32_node_root;

extern elf32_node_t shf_write;
extern elf32_node_t shf_alloc;
extern elf32_node_t shf_execinstr;
extern elf32_node_t shf_merge;
extern elf32_node_t shf_strings;
extern elf32_node_t shf_info_link;
extern elf32_node_t shf_link_order;
extern elf32_node_t shf_os_nonconforming;
extern elf32_node_t shf_group;
extern elf32_node_t shf_tls;
extern elf32_node_t shf_compressed;
extern elf32_node_t shf_maskos;
extern elf32_node_t shf_maskproc;
