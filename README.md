# Tugas Besar IF2230 - System Operasi

# baltOS - Milestone 02
KELOMPOK BaltOS -  K02
## Anggota :
Nama                     | NIM
-------------------------| --------
Firizky Ardiansyah       | 13520095
Muhammad Gilang Ramadhan | 13520137
Muhammad Fahmi Irfan     | 13520152

## Shell script yang tersedia pada file makefile yang bisa dijalankan di terminal:
1. all => untuk compile semua program
2. diskimage => untuk compile diskimage
3. bootloader => untuk compile bootloader
4. kernel => untuk compile kernel
5. shell, cd, ls, cp, mv, mkdir, cat => compile semua program global (program utilitas)
6. run => Untuk melakukan run program saja
7. build-run => Untuk melakukan compile dan melakukan run program
8. build-test => Untuk melakukan compile program untuk memasukkan file ke system.img
9. test-shell => Memasukkan program utilitas ke dalam system.img
10. test => Memasukkan program utilitas dan melakukan run

## Cara menjalankan OS
Jalankan 
```
make build-run
```
Ketik c untuk melanjutkan, kemudian tutup window yang terbuka. (tujuannya untuk inisialisasi map pada filesystem). Setelah program berhenti, jalankan perintah berikut
```
make test
```
Ketik c, untuk melanjutkan.



## Petunjuk Penggunaan Utility

### mkdir
Membuat satu file
```
mkdir <input-nama-folder-baru>
```
Membuat banyak file
```
mkdir <input-nama-folder-baru-1> <input-nama-folder-baru-2> <input-nama-folder-baru-3> ...
```
### cd

Masuk ke suatu folder yang terdapat pada directori sekarang
```
cd <input-path-ke-folder-tujuan>
```
Pergi ke folder parent
```
cd ..
```
Pergi ke folder root
```
cd /
```
### cat
```
cat <input-path-ke-file-yang-ingin-dibaca>
```
### ls
ls current directory
```
ls
```
ls directory relatif terhadap current directory
```
ls <input-path-directory> 
```
### cp
copy ke file
```
cp <input-nama-file-sumber> <input-nama-file-tujuan>
```
copy ke folder
```
cp <input-nama-file-sumber> <input-nama-folder-tujuan>
```
### mv 
pindah konten file ke file/rename file
```
mv <input-nama-file-sumber> <input-nama-file-tujuan>
```
rename folder
```
mv <intpur-nama-folder> <input-nama-folder>
```
### Menjalankan executable
```
./<nama-file-executable>
```

### Menjalankan lebih dari satu program (multiprogramming)
```
<cmd-1> <arg-1> <arg-2> ... <arg-n> ; <cmd-2> <arg-1> <arg-2> ... <arg-n> ; ... ; <cmd-n> <arg-1> <arg-2> ... <arg-n>
```
Maksimal program yang bisa dijalankan sekaligus adalan 5.
# tugas-besar-os-baltos
