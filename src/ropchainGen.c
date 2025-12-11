#include "ROPchain.h"
#include <capstone/capstone.h>

#define CODE "\x55\x48\x8b\x05\xb8\x13\x00\x00"

void
ROPchainGenerator(char *section, uint64_t section_size){
	csh		handle;
	cs_insn	*insn;
	size_t	count;

	if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK){
		printf("CAPSTONE Failed\n");
		return ;
	}
	count = cs_disasm(handle, CODE, sizeof(CODE)-1, 0x1000, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
					insn[j].op_str);
		}
		cs_free(insn, count);
	}else{
		printf("ERROR: Failed to disassemble given code!\n");
	}
	cs_close(&handle);
	for (uint64_t i = 0; i < section_size; i++){
		if (section[i] == 0x3c){
			// printf("ret found at %lu\n", i);
		}
	}	
}
