# elfparse
1.read the executable elf file. 2.parse the entry point,segments info,sections info.

just support 64bit linux system

demo:
```
zg@ubuntu:~/Documents/elfparse$ ./elfparse test
Program Entery Point: 0x400a30
the string table section index : 32

Section header list:

[0]  0x0
[1] .note.ABI-tag 0x400190
[2] .note.gnu.build-id 0x4001b0
[3] .rela.plt 0x4001d8
[4] .init 0x400400
[5] .plt 0x400418
[6] .text 0x4004d0
[7] __libc_freeres_fn 0x48fb30
[8] __libc_thread_freeres_fn 0x491060
[9] .fini 0x4920f0
[10] .rodata 0x492100
[11] .stapsdt.base 0x4ab36c
[12] .eh_frame 0x4ab370
[13] .gcc_except_table 0x4b5880
[14] .tdata 0x6b6120
[15] .tbss 0x6b6140
[16] .init_array 0x6b6140
[17] .fini_array 0x6b6150
[18] .data.rel.ro 0x6b6160
[19] .got 0x6b8ef8
[20] .got.plt 0x6b9000
[21] .data 0x6b90e0
[22] __libc_subfreeres 0x6babd0
[23] __libc_IO_vtables 0x6bac20
[24] __libc_atexit 0x6bb2c8
[25] __libc_thread_subfreeres 0x6bb2d0
[26] .bss 0x6bb2e0
[27] __libc_freeres_ptrs 0x6bc9d8
[28] .comment 0x0
[29] .note.stapsdt 0x0
[30] .symtab 0x0
[31] .strtab 0x0
[32] .shstrtab 0x0

Program header list:

Text segment: 0x400000
Data segment: 0x6b6120
Note segment: 0x400190
```
