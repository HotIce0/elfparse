/**
 * elfparse
 * @function 1. read the executable elf file.
 *           2. parse the entry point,segments info,sections info.
 * @author HotIce0
 * @date 2018.9.2
 */ 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int errno;

int main(int argc, char **argv, char **envp)
{
    int i_fd, i;
    struct stat st = {0};
    uint8_t *p_mem;
    char *p_string_table, *p_interp;

    Elf64_Ehdr *p_ehdr;
    Elf64_Phdr *p_phdr;
    Elf64_Shdr *p_shdr;

    if (argc < 2) {
        printf("Usage: %s <executable>\n", argv[0]);
        exit(0);
    }

    //open specify file.
    if ((i_fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(-1);
    }

    //read file status.
    if (fstat(i_fd, &st) < 0) {
        perror("fstat");
        exit(-1);
    }

    // Map the executable into memeory.
    p_mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, i_fd, 0);
    if (p_mem == MAP_FAILED) {
        printf("st_size: %ld\n", st.st_size);
        perror("mmap");
        exit(-1);
    }

    /**
     * The initial ELF Header starts at offset 0
     * of our mapped memory.
     */
    p_ehdr = (Elf64_Ehdr *)p_mem;

    /**
     * The offset of The Program Headers and The Section Headers
     * is saved at ELF Header.
     */
    p_phdr = (Elf64_Phdr *)&p_mem[p_ehdr->e_phoff];
    p_shdr = (Elf64_Shdr *)&p_mem[p_ehdr->e_shoff];

    /**
     * Check to see if the ELF magic (The first 4 byte)
     * match up as 0x7f E L F
     */
    if (p_mem[0] != 0x7f || strncmp("ELF", &p_mem[1], 3)) {
        fprintf(stderr, "%s is not an ELF file\n", argv[1]);
        exit(-1);
    }

    /**
     * Only Parsing executbales with this code so
     * ET_EXEC marks an executable.
     */ 
    if (p_ehdr->e_type != ET_EXEC) {
        fprintf(stderr, "%s is not an executable\n", argv[1]);
        exit(-1);
    }

    printf("Program Entery Point: 0x%lx\n", p_ehdr->e_entry);

    
    if (p_ehdr->e_shstrndx == SHN_UNDEF) {
        fprintf(stderr, "%s has no section name string table\n", argv[1]);
        exit(-1);
    }
    
    /**
     * Find the string table for the section header
     * names with e_shstrndx which gives the index of
     * which section holds the string table.
     */ 
    printf("the string table section index : %d\n", p_ehdr->e_shstrndx);
    p_string_table = (char *)&p_mem[p_shdr[p_ehdr->e_shstrndx].sh_offset];

    /**
     * Print each section header name and address
     */ 
    printf("\nSection header list:\n\n");
    for (i = 0; i < p_ehdr->e_shnum; i++) {
        printf("[%d] %s 0x%lx\n", i, &p_string_table[p_shdr[i].sh_name], p_shdr[i].sh_addr);
    }

    /**
     * Print each program header name and address
     */ 
    printf("\nProgram header list:\n\n");
    for (i = 0; i < p_ehdr->e_phnum; i++) {
        switch(p_phdr[i].p_type) {
            case PT_NULL:
            // this element is unused.  Refer to elf document.
            break;
            
            case PT_LOAD:
                if (p_phdr[i].p_offset == 0)
                    printf("Text segment: 0x%lx\n", p_phdr[i].p_vaddr);
                else
                    printf("Data segment: 0x%lx\n", p_phdr[i].p_vaddr);
            break;

            case PT_DYNAMIC:
                printf("Dynamic segment: 0x%lx\n", p_phdr[i].p_vaddr);
            break;  

            case PT_INTERP:
                /**
                 * This element specifies the location and size of a 
                 * null-terminated path name to invoke as an interpreter.
                 * Refer to elf document.
                 */ 
                p_interp = strdup((char *)&p_mem[p_phdr[i].p_offset]);
                printf("Interpreter: %s\n", p_interp);
            break;

            case PT_NOTE:
                printf("Note segment: 0x%lx\n", p_phdr[i].p_vaddr);
            break;
            
            case PT_SHLIB:
            // The element is reserved.  Refer to elf document.
            break;

            case PT_PHDR:
                printf("Phdr segment: 0x%lx\n", p_phdr[i].p_vaddr);
            break;

            case PT_LOPROC:
            // The element is reserved.  Refer to elf document.
            break;

            case PT_HIPROC:
            // The element is reserved.  Refer to elf document.
            break;
        }
    }

    /**
     * Close the file
     */ 
    if (close(i_fd) < 0) {
        perror("close");
        exit(-1);
    }

    exit(0);
}
