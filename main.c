
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct barang {
	int nomoBarang;
	int HargaBarang;
	float diskon;
	char namaBarang[20];
	time_t kadaluarsa; 
}; // struct barang ini hanya memberikan strukturnya saja, bukan menyimpan datanya



struct nodeBarang {
    struct barang data;
    struct nodeBarang *next;
}; //bagian ini akan menjadi sebuah node dari barang tersebut

struct nodeBarang *headBarang = NULL;

struct kasir {
	char username [20];
	char password [8];
}; // struct ini hanya struktur dari usernya saja bukan menyimpan data user yang login

struct kasir semua_Kasir[2] = {
	{"admin", "1234"},
	{"dodo", "12345"}
};

struct kasir kasir_sekarang; //ini baru menyimpan data user yang sudah login

struct barangdibeli{
	int nomorNota;
	struct barang barang;
	int jumlah; 
float diskon;
	int subTotall;
};

struct nota{
	int nomorNota;
	char nama_kasir [20];
	time_t waktuTanggalTransaksi;
	struct barangdibeli semuabarangp[100];
};

struct nota semuanotaTersimpan[100];
int totalNotaTersimpan = 0; 

//dumy data adder

//================

//Bagian manajemen baranng
void tampilanManajemen(){
	
} //ada tampilan yaitu menambah barang, dan menghapus barang, serta melihat semua barang, dan exit

struct nodeBarang *searchBarang(void *key, int mode){
	 
} //bagian searcingnya

struct nodeBarang *searchBarang_name(char namaBarang[20]) {
  
} //untuk nama

struct nodeBarang *searchBarang_nomor(int nomor) {
   
}//untuk nomor kalo ga ada null

int addBarang(struct barang *add_barang){
	
} //retrunnya 1 adalah sukses dan 0 adalah faild.

int setBarang(){
	
} //retrunnya 1 adalah sukses dan 0 adalah faild.

int deleteBarang(int nomorbarang){
	
} //retrunnya 1 adalah sukses dan 0 adalah faild.

struct barang *getBarang_nama(char namaBarang[20]){
	
} //return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL

struct barang *getBarang_nomor(int nomorBarang){
    
} //return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL


//Bagian kasir
int simpanNota(struct nota notaSelesai) {
   
} //bagian ini harus ada agar bagian kasir ngikutin. dan simpannya kedalam struct nota semuanotaTersimpan yang sudah ada 



// Bagian riwayat Transaksi

// tinggal iklutin instruksi yang di atas ya


//Bagian login
int  login(char username[20], char password [8]){
	
} // pada bagian ini berfungsi melakukan proses login, jadi fungsi loginnya itu di sini kalau berhasil 1 kalau gagal 0

void mainmenu(){
	
} //di sini adalah semua main menu yang tersedia. total ada 3 (manajemen barang, pos, riwayat transaksi)
