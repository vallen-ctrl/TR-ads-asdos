
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct barang {
	int nomorBarang;
	int HargaBarang;
	float diskon;
	char namaBarang[20];
	int jumlahBarang;
	time_t kadaluarsa;
}; // struct barang ini hanya memberikan strukturnya saja, bukan menyimpan datanya



struct nodeBarang {
    struct barang data;
	time_t waktuDibuat;
	time_t waktuDiubah;
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


/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 * @param key adalah variable untuk mencari match yang sesuai mode
 * @param mode adalah variable untuk menentukan fungsi cari dari key, 0 untuk 'namaBarang' 1 untuk 'nomorBarang'
 * @return pointer langsung ke nodeBarang jika ketemu
 */

struct nodeBarang *searchBarang(void *key, int mode){
	struct nodeBarang *temp = headBarang;
	while (temp != NULL)
	{
		if (mode == 0)
		{
			if (strcmp((char *)key, temp->data.namaBarang) == 0)
			{
				return temp;
			}
			
		}else if (mode == 1)
		{
			if (*(int *)key == temp->data.nomorBarang)
			{
				return temp;
			}
			
		}
		temp = temp->next;
	}
	return NULL;
	
} //bagian searcingnya

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
struct nodeBarang *searchBarang_name(char namaBarang[20]) {
	return searchBarang((void *)namaBarang, 0);
} //untuk nama

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
struct nodeBarang *searchBarang_nomor(int nomorBarang) {
   return searchBarang((void *)&nomorBarang, 1);
}//untuk nomor kalo ga ada null

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
int addBarang(struct barang *add_barang){
	struct nodeBarang *newNode = (struct nodeBarang*)malloc(sizeof(struct nodeBarang));
	strcpy(newNode->data.namaBarang, add_barang->namaBarang);
	newNode->data.diskon = add_barang->diskon;
	newNode->data.HargaBarang = add_barang->HargaBarang;
	newNode->data.kadaluarsa = add_barang->kadaluarsa;
	newNode->waktuDibuat = time(NULL);
	newNode->waktuDiubah = time(NULL);
	newNode->data.jumlahBarang = add_barang->jumlahBarang;

	if (headBarang == NULL)
	{
		headBarang = newNode;
		headBarang->next = NULL;
	}else{
		struct nodeBarang *temp = headBarang;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newNode;	
	}
	return 1;
} //retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini hanya boleh dipanggil di bagian manajemen barang saja
 */
int deleteBarang(int nomorbarang){
	struct nodeBarang *temp = searchBarang_nomor(nomorbarang);
	struct nodeBarang *PREV = NULL;
	while (temp != NULL)
	{
		if (temp->data.nomorBarang == nomorbarang)
		{
			if (PREV == NULL)
			{
				headBarang = temp->next;
			}else{
				PREV = temp->next;
			}
			free(temp);
			return 1;
		}
		PREV = temp;
		temp = temp->next;
	}
	return 0;
} //retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini hanya boleh dipanggil di bagian manajemen barang saja
 */
int editBarang(int nomorBarang ,struct barang setBarang){
	struct nodeBarang *mencariBarang = searchBarang_nomor(nomorBarang);
	if (mencariBarang == NULL)
	{
		return 0; // barang tidak ditemukan
	}

	mencariBarang->data.diskon = setBarang.diskon;
	mencariBarang->data.HargaBarang = setBarang.HargaBarang;
	mencariBarang->data.kadaluarsa = setBarang.kadaluarsa;
	mencariBarang->data.nomorBarang = setBarang.nomorBarang;
	strcpy(mencariBarang->data.namaBarang, setBarang.namaBarang);

	mencariBarang->waktuDiubah = time(NULL);
	return 1;	
} //retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini adalah fungsi untuk mengurangi jumlah barang yang ada di databases
 * @param nomorBarang adalah nomor identifikasi yang digunakan untuk mencari barang tersebut
 * @param diambil adalah jumlah barang yang akan diambil
 * @return jika barang diambil > jumlah barang di databases maka akan 0 dan ngasih output print barang tidak mencukupi
 */
int ambilBarang(int nomorBarang, int diambil){
	struct nodeBarang *temp = searchBarang_nomor(&nomorBarang);
	if (searchBarang == NULL)
	{
		return 0;
	}
	if ((temp->data.jumlahBarang-diambil) < 0)
	{
		printf("barang sudah habis / tidak mencukupi. mohon lakuakan penambahan barang");
		return 0;
	}
	temp->data.jumlahBarang = temp->data.jumlahBarang-diambil;
	temp->waktuDiubah = time(NULL); 
	
}

/**
 * @brief getBarang_nama adalah metode untuk mencari barang berdasarkan nama dari barang tersebut
 */
struct barang *getBarang_nama(char namaBarang[20]){
	struct nodeBarang *temp = searchBarang_name(namaBarang);
	if (temp == NULL)	
	{
		return NULL;
	}
	return &temp->data;
} //return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL

/**
 * @brief getBarang_nomor adalah metode untuk mencari barang berdasarkan nama dari nomor barang tersebut
 */
struct barang *getBarang_nomor(int nomorBarang){
    struct nodeBarang *temp = searchBarang_name(&nomorBarang);
	if (temp == NULL)	
	{
		return NULL;
	}
	return &temp->data;
} //return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL


//Bagian manajemen baranng
void tampilanManajemen(){
	
} //ada tampilan yaitu menambah barang, dan menghapus barang, serta melihat semua barang, dan exit


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
