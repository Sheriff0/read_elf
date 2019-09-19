#include "../../le.elf.h"

#define SHT_ARM_EXIDX 0x70000001
#define SHT_ARM_PREEMPTMAP 0x70000002
#define SHT_ARM_ATTRIBUTES 0x70000003
#define SHT_ARM_DEBUGOVERLAY 0x70000004
#define SHT_ARM_OVERLAYSECTION 0x70000005

elf32_node_t sht_arm_exidx;
elf32_node_t sht_arm_preemptmap;
elf32_node_t sht_arm_attributes;
elf32_node_t sht_arm_debugoverlay;
elf32_node_t sht_arm_overlaysection;

elf32_node_t sht_nill = { 0, 0, 0 };


elf32_node_t sht_arm_exidx = {
	.name = "(SHT_ARM_EXIDX)",
	.field = SHT_ARM_EXIDX,
	.next = &sht_arm_preemptmap
};

elf32_node_t sht_arm_preemptmap = {
	.name = "(SHT_ARM_PREEMPTMAP)",
	.field = SHT_ARM_PREEMPTMAP,
	.next = &sht_arm_attributes
};

elf32_node_t sht_arm_attributes = {
	.name = "(SHT_ARM_ATTRIBUTES)",
	.field = SHT_ARM_ATTRIBUTES,
	.next = &sht_arm_debugoverlay
};

elf32_node_t sht_arm_debugoverlay = {
	.name = "(SHT_ARM_DEBUGOVERLAY)",
	.field = SHT_ARM_DEBUGOVERLAY,
	.next = &sht_arm_overlaysection
};

elf32_node_t sht_arm_overlaysection = {
	.name = "(SHT_ARM_OVERLAYSECTION)",
	.field = SHT_ARM_OVERLAYSECTION,
	.next = &sht_nill
};

elf32_node_t *sht_proc = &sht_arm_exidx;
