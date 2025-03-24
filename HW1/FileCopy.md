## FileCopy Execution Trace

Below is the detailed execution trace of the `FileCopy` program using `strace`:

### Command Executed
```bash
zre@zre:~/code/Operating-System/HW1/output$ strace ./FileCopy ./FileCopy ../FileCopy
```

### Full Trace
For a detailed view, refer to the raw trace below:

```plaintext
execve("./FileCopy", ["./FileCopy", "./FileCopy", "../FileCopy"], 0x7ffe028388f0 /* 34 vars */) = 0
brk(NULL)                               = 0x574b7dfe1000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x78815571e000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=32527, ...}) = 0
mmap(NULL, 32527, PROT_READ, MAP_PRIVATE, 3, 0) = 0x788155716000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220\243\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
fstat(3, {st_mode=S_IFREG|0755, st_size=2125328, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2170256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x788155400000
mmap(0x788155428000, 1605632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x788155428000
mmap(0x7881555b0000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b0000) = 0x7881555b0000
mmap(0x7881555ff000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1fe000) = 0x7881555ff000
mmap(0x788155605000, 52624, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x788155605000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x788155713000
arch_prctl(ARCH_SET_FS, 0x788155713740) = 0
set_tid_address(0x788155713a10)         = 2239
set_robust_list(0x788155713a20, 24)     = 0
rseq(0x788155714060, 0x20, 0, 0x53053053) = 0
mprotect(0x7881555ff000, 16384, PROT_READ) = 0
mprotect(0x574b7dda2000, 4096, PROT_READ) = 0
mprotect(0x788155756000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x788155716000, 32527)           = 0
newfstatat(AT_FDCWD, "./FileCopy", {st_mode=S_IFREG|0775, st_size=49704, ...}, 0) = 0
getrandom("\x5a\x31\xd4\xd9\x4f\x7f\x8d\xb2", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x574b7dfe1000
brk(0x574b7e002000)                     = 0x574b7e002000
openat(AT_FDCWD, "./FileCopy", O_RDONLY) = 3
openat(AT_FDCWD, "../FileCopy", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 4
fstat(3, {st_mode=S_IFREG|0775, st_size=49704, ...}) = 0
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@\21\0\0\0\0\0\0"..., 4096) = 4096
fstat(4, {st_mode=S_IFREG|0664, st_size=0, ...}) = 0
read(3, "\363\17\36\372H\203\354\10H\213\5\331/\0\0H\205\300t\2\377\320H\203\304\10\303\0\0\0\0\0"..., 4096) = 4096
write(4, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@\21\0\0\0\0\0\0"..., 4096) = 4096
read(3, "\1\0\2\0\0\0\0\0Usage: %s <source> <dest"..., 4096) = 4096
write(4, "\363\17\36\372H\203\354\10H\213\5\331/\0\0H\205\300t\2\377\320H\203\304\10\303\0\0\0\0\0"..., 4096) = 4096
read(3, "\0\0\0\0\0\0\0\0\10@\0\0\0\0\0\0GCC: (Ubuntu 13."..., 4096) = 4096
write(4, "\1\0\2\0\0\0\0\0Usage: %s <source> <dest"..., 4096) = 4096
read(3, "_GLIBC_PREREQ(maj,min) ((__GLIBC"..., 4096) = 4096
write(4, "\0\0\0\0\0\0\0\0\10@\0\0\0\0\0\0GCC: (Ubuntu 13."..., 4096) = 4096
read(3, "F16\0__INT64_C(c) c ## L\0__restri"..., 4096) = 4096
write(4, "_GLIBC_PREREQ(maj,min) ((__GLIBC"..., 4096) = 4096
read(3, "(message) __glibc_macro_warning1"..., 4096) = 4096
write(4, "F16\0__INT64_C(c) c ## L\0__restri"..., 4096) = 4096
read(3, "_PTRDIFF_T__ 8\0__USECONDS_T_TYPE"..., 4096) = 4096
write(4, "(message) __glibc_macro_warning1"..., 4096) = 4096
read(3, ".2204460492503130808472633361816"..., 4096) = 4096
write(4, "_PTRDIFF_T__ 8\0__USECONDS_T_TYPE"..., 4096) = 4096
read(3, "BFLT16_MIN_10_EXP__ (-37)\0__HAVE"..., 4096) = 4096
write(4, ".2204460492503130808472633361816"..., 4096) = 4096
read(3, "\0|\17\0\0\5\0\317B\0\0\5\0\227)\0\0\5\0iV\0\0\5\0\0037\0\0\5\0\v"..., 4096) = 4096
write(4, "BFLT16_MIN_10_EXP__ (-37)\0__HAVE"..., 4096) = 4096
read(3, "\5\205\1\34\3\0\0\5\216\1aN\0\0\5\230\1P\4\0\0\5\231\1]-\0\0\5\232\1#"..., 4096) = 4096
write(4, "\0|\17\0\0\5\0\317B\0\0\5\0\227)\0\0\5\0iV\0\0\5\0\0037\0\0\5\0\v"..., 4096) = 4096
read(3, "k0\0\0\0\0\0\0E\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0"..., 4096) = 552
write(4, "\5\205\1\34\3\0\0\5\216\1aN\0\0\5\230\1P\4\0\0\5\231\1]-\0\0\5\232\1#"..., 4096) = 4096
read(3, "", 4096)                       = 0
close(3)                                = 0
write(4, "k0\0\0\0\0\0\0E\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0"..., 552) = 552
close(4)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```