/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swenn <swenn@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:50:04 by swenn             #+#    #+#             */
/*   Updated: 2025/12/11 23:48:45 by swenn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "ROPchain.h"


int
main(int argc, char **argv){
	if (argc != 2)
		return (1);
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (1);
	struct stat st = {0};
	if (fstat(fd, &st) == -1)
		return (1);

	void *file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (!file)
		return (1);
	Elf64_Ehdr	*header = file;
	Elf64_Shdr	*shdrs = file + header->e_shoff;
	Elf64_Shdr	shstrtab = shdrs[header->e_shstrndx];
	char		*strtab = file + shstrtab.sh_offset;



	for (int i = 0; i < header->e_shnum; i++){
		Elf64_Shdr	shdr = shdrs[i];
		// printf("[DEBUG]: %s\n", strtab + shdr.sh_name);


		if (!strcmp(strtab + shdr.sh_name, ".text")){
			//.text found
			char *text = malloc(sizeof(char) * shdr.sh_size);
			memcpy(text, file + shdr.sh_offset, shdr.sh_size);
			// write(1, text, shdr.sh_size);
			
			ROPchainGenerator(text, shdr.sh_size);
		}
		
	}
}
