
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define TERMINALWIDTH 100
#define TERMINALHEIGHT 50

// boxes
#define light_HL "─"
#define light_VLL "│"
#define light_DR "┌"
#define light_UR "└"
#define light_DL "┐"
#define light_UL "┘"
#define light_VR "├"
#define light_VL "┤"
#define light_DH "┬"
#define light_UH "┴"
#define light_UDRL "┼"

// colors

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

struct barang
{
	int nomorBarang;
	int HargaBarang;
	float diskon;
	char namaBarang[20];
	int jumlahBarang;
	time_t kadaluarsa;
}; // struct barang ini hanya memberikan strukturnya saja, bukan menyimpan datanya

struct nodeBarang
{
	struct barang data;
	time_t waktuDibuat;
	time_t waktuDiubah;
	struct nodeBarang *next;
}; // bagian ini akan menjadi sebuah node dari barang tersebut

struct nodeBarang *headBarang = NULL;

struct kasir
{
	char username[20];
	char password[8];
}; // struct ini hanya struktur dari usernya saja bukan menyimpan data user yang login

struct kasir semua_Kasir[2] = {
	{"admin", "1234"},
	{"dodo", "12345"}};

struct kasir kasir_sekarang; // ini baru menyimpan data user yang sudah login

struct barangdibeli
{
	int nomorNota;
	struct barang barang;
	int jumlah;
	float diskon;
	int subTotall;
};

struct nota
{
	int nomorNota;
	char nama_kasir[20];
	time_t waktuTanggalTransaksi;
	struct barangdibeli semuabarangp[100];
};

struct nota semuanotaTersimpan[100];
int totalNotaTersimpan = 0;

// dumy data adder

//================

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; // Horizontal coordinate
	coord.Y = y; // Vertical coordinate
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setTextColor(int COLOR)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR);
}
void ResetColor()
{
	setTextColor(WHITE);
}
/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 * @param key adalah variable untuk mencari match yang sesuai mode
 * @param mode adalah variable untuk menentukan fungsi cari dari key, 0 untuk 'namaBarang' 1 untuk 'nomorBarang'
 * @return pointer langsung ke nodeBarang jika ketemu
 */

struct nodeBarang *searchBarang(void *key, int mode)
{
	struct nodeBarang *temp = headBarang;
	while (temp != NULL)
	{
		if (mode == 0)
		{
			if (strcmp((char *)key, temp->data.namaBarang) == 0)
			{
				return temp;
			}
		}
		else if (mode == 1)
		{
			if (*(int *)key == temp->data.nomorBarang)
			{
				return temp;
			}
		}
		temp = temp->next;
	}
	return NULL;

} // bagian searcingnya

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
struct nodeBarang *searchBarang_name(char namaBarang[20])
{
	return searchBarang((void *)namaBarang, 0);
} // untuk nama

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
struct nodeBarang *searchBarang_nomor(int nomorBarang)
{
	return searchBarang((void *)&nomorBarang, 1);
} // untuk nomor kalo ga ada null

time_t buatTanggal(int day, int month, int year)
{
	struct tm t = {0};
	t.tm_mday = day;
	t.tm_mon = month - 1;	 // struct tm bulan mulai dari 0
	t.tm_year = year - 1900; // struct tm tahun dihitung dari 1900
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	return mktime(&t);
}

/**
 * @brief fungsi ini adalah fungsi khusus yang hanya boleh digunakan oleh bagian manajemen saja
 */
int addBarang(struct barang add_barang)
{
	struct nodeBarang *newNode = (struct nodeBarang *)malloc(sizeof(struct nodeBarang));
	newNode->data = add_barang;

	newNode->waktuDibuat = time(NULL);
	newNode->waktuDiubah = time(NULL);

	newNode->next = NULL;

	if (headBarang == NULL)
	{
		headBarang = newNode;
	}
	else
	{
		struct nodeBarang *temp = headBarang;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newNode;
	}
	return 1;
} // retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini hanya boleh dipanggil di bagian manajemen barang saja
 */
int deleteBarang(int nomorbarang)
{
	struct nodeBarang *temp = searchBarang_nomor(nomorbarang);
	struct nodeBarang *PREV = NULL;
	while (temp != NULL)
	{
		if (temp->data.nomorBarang == nomorbarang)
		{
			if (PREV == NULL)
			{
				headBarang = temp->next;
			}
			else
			{
				PREV = temp->next;
			}
			free(temp);
			return 1;
		}
		PREV = temp;
		temp = temp->next;
	}
	return 0;
} // retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini hanya boleh dipanggil di bagian manajemen barang saja
 */
int editBarang(int nomorBarang, struct barang setBarang)
{
	struct nodeBarang *mencariBarang = searchBarang_nomor(nomorBarang);
	if (mencariBarang == NULL)
	{
		return 0; // barang tidak ditemukan
	}

	if (setBarang.diskon > 1)
	{
		return 0;
	}
	

	mencariBarang->data = setBarang;

	mencariBarang->waktuDiubah = time(NULL);
	return 1;
} // retrunnya 1 adalah sukses dan 0 adalah faild.

/**
 * @brief fungsi ini adalah fungsi untuk mengurangi jumlah barang yang ada di databases
 * @param nomorBarang adalah nomor identifikasi yang digunakan untuk mencari barang tersebut
 * @param diambil adalah jumlah barang yang akan diambil
 * @return jika barang diambil > jumlah barang di databases maka akan 0 dan ngasih output print barang tidak mencukupi
 */
int ambilBarang(int nomorBarang, int diambil)
{
	struct nodeBarang *temp = searchBarang_nomor(nomorBarang);
	if (searchBarang == NULL)
	{
		return 0;
	}
	if ((temp->data.jumlahBarang - diambil) < 0)
	{
		printf("barang sudah habis / tidak mencukupi. mohon lakuakan penambahan barang");
		return 0;
	}
	temp->data.jumlahBarang = temp->data.jumlahBarang - diambil;
	temp->waktuDiubah = time(NULL);
}

/**
 * @brief getBarang_nama adalah metode untuk mencari barang berdasarkan nama dari barang tersebut
 */
struct barang *getBarang_nama(char namaBarang[20])
{
	struct nodeBarang *temp = searchBarang_name(namaBarang);
	if (temp == NULL)
	{
		return NULL;
	}
	return &temp->data;
} // return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL

/**
 * @brief getBarang_nomor adalah metode untuk mencari barang berdasarkan nama dari nomor barang tersebut
 */
struct barang *getBarang_nomor(int nomorBarang)
{
	struct nodeBarang *temp = searchBarang_nomor(nomorBarang);
	if (temp == NULL)
	{
		return NULL;
	}
	return &temp->data;
} // return struct barang yang kalian cari. misal ga ada maka akan menjadi NULL

void createBox(int width, int height, int posX, int posY)
{
	gotoxy(posX, posY);
	for (int row = 0; row <= height; row++)
	{
		for (int col = 0; col <= width; col++)
		{
			if (row == 0 && col == 0)
			{
				printf(light_DR);
			}
			else if (row == 0 && col == width)
			{
				printf(light_DL);
			}
			else if ((row > 0 && row < height && (col == 0 || col == width)))
			{
				printf(light_VLL);
			}
			else if (row == height && col == 0)
			{
				printf(light_UR);
			}
			else if (row == height && col == width)
			{
				printf(light_UL);
			}
			else if (row == 0 || row == height)
			{
				printf(light_HL);
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void createTableHeader(int rowWidth[], char *headerName[], int lenght, int xpos, int ypos)
{
	int temp = 0;
	gotoxy(xpos, ypos);
	for (int i = 0; i < lenght; i++)
	{
		for (int x = 0; x < rowWidth[i] + 1; x++)
		{
			gotoxy(temp, ypos);
			if (temp == 0)
			{
				printf(light_DR);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				printf(light_DL);
			}
			else if (x == rowWidth[i])
			{
				printf(light_DH);
			}
			else
			{
				printf(light_HL);
			}

			if (x == 0)
			{
				int xpos = temp == 0 ? 0 : temp - 1;
				gotoxy(xpos, ypos + 1);
				printf("%s%s", light_VLL, headerName[i]);
				gotoxy(xpos, ypos);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				gotoxy(temp, ypos + 1);
				printf(light_VLL);
			}

			gotoxy(temp, ypos + 2);
			if (temp == 0)
			{
				printf(light_VR);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				printf(light_VL);
			}
			else if (x == rowWidth[i])
			{
				printf(light_UDRL);
			}
			else
			{
				printf(light_HL);
			}

			temp += 1;
		}
	}
}

void createbodyTabel(int num, int rowWidth[], struct barang data, int lenght, int xpos, int ypos, int selection)
{
	int temp = 0;

	for (int i = 0; i < lenght; i++)
	{
		int startTemp = temp;
		for (int x = 0; x < rowWidth[i] + 1; x++)
		{
			if (temp == 0)
			{
				gotoxy(temp, ypos);
				printf(light_VLL);
				gotoxy(temp, ypos + 1);
				printf(light_VR);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				gotoxy(temp, ypos);
				printf(light_VLL);
				gotoxy(temp, ypos + 1);
				printf(light_VL);
			}
			else if (x == rowWidth[i])
			{
				gotoxy(temp, ypos);
				printf(light_VLL);
				gotoxy(temp, ypos + 1);
				printf(light_UDRL);
			}
			else
			{
				gotoxy(temp, ypos + 1);
				printf(light_HL);
			}

			if (x == 0)
			{
				int xpos = temp == 0 ? 0 : temp - 1;
				gotoxy(xpos, ypos + 1);
				gotoxy(xpos, ypos);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				gotoxy(temp, ypos + 1);
				printf(light_VLL);
			}

			temp += 1;
		}
	}

	if (selection == num)
	{
		setTextColor(GREEN);
	}

	gotoxy(xpos + 1, ypos);
	printf("%d", num);

	int dataTemprow = 0;
	dataTemprow += rowWidth[0] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%d", data.nomorBarang);

	dataTemprow += rowWidth[1] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%s", data.namaBarang);

	dataTemprow += rowWidth[2] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%d", data.HargaBarang);

	dataTemprow += rowWidth[3] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%.2f", data.diskon);

	dataTemprow += rowWidth[4] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%d", data.jumlahBarang);

	struct tm *t = localtime(&data.kadaluarsa);
	dataTemprow += rowWidth[5] + 1;
	gotoxy(dataTemprow, ypos);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", t);
	printf("%s", buffer);
	ResetColor();
}

void createfooterTabel(int rowWidth[], int lenght, int xpos, int ypos)
{
	int temp = 0;

	for (int i = 0; i < lenght; i++)
	{
		for (int x = 0; x < rowWidth[i] + 1; x++)
		{
			if (temp == 0)
			{
				gotoxy(temp, ypos);
				printf(light_UR);
			}
			else if ((i == lenght - 1) && (x == rowWidth[lenght - 1]))
			{
				gotoxy(temp, ypos);
				printf(light_UL);
			}
			else if (x == rowWidth[i])
			{
				gotoxy(temp, ypos);
				printf(light_UH);
			}
			else
			{
				gotoxy(temp, ypos);
				printf(light_HL);
			}

			temp += 1;
		}
	}
}

void editData(int selected)
{
	ResetColor();
	system("cls");
	struct nodeBarang *temp = headBarang;
	int hop = 1;
	while (temp != NULL)
	{
		if (hop == selected)
		{
			break;
		}
		hop += 1;
		temp = temp->next;
	}

	struct tm *t = localtime(&temp->data.kadaluarsa);
	char buffer[80];

	int width = 95;
	int height = 13;
	int posX = 0, posY = 0;
	createBox(width, height, posX, posY);

	gotoxy((95/2) - (17/2), posY+1);
	setTextColor(BLUE);
	printf("EDIT BARANG ANDA");
	ResetColor();

	int marginTop = 3;
	int marginLeft = 2;

	struct tm *diubah = localtime(&temp->waktuDiubah);
	
	gotoxy(posX+marginLeft, marginTop+posY);
	char bf[80];
	strftime(bf, sizeof(bf), "%d-%m-%Y", diubah);
	setTextColor(CYAN);
	printf("Terakhir diubah -> %s", bf);
	ResetColor();
	
	gotoxy(posX + marginLeft, marginTop + 2 + posY);
	printf("Nomor \t\t(%d)\n", temp->data.nomorBarang);
	gotoxy(posX + marginLeft, marginTop + 3 + posY);
	printf("Harga \t\t(%d)\n", temp->data.HargaBarang);
	gotoxy(posX + marginLeft, marginTop + 4 + posY);
	printf("Diskon \t\t(%.2f)\n", temp->data.diskon);
	gotoxy(posX + marginLeft, marginTop + 5 + posY);
	printf("Nama \t\t\t(%s)\n", temp->data.namaBarang);
	gotoxy(posX + marginLeft, marginTop + 6 + posY);
	printf("Jumlah \t\t(%d)\n", temp->data.jumlahBarang);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", t);
	gotoxy(posX + marginLeft, marginTop + 7 + posY);
	printf("Kadaluarsa \t\t(%s)\n", buffer);

	for (int i = 2; i <= 7; i++)
	{
		gotoxy(posX + 40 + marginLeft, marginTop + i + posY);
		printf(":");
	}

	struct barang newBarang = {
		.nomorBarang = temp->data.nomorBarang,
		.HargaBarang = 0,
		.diskon = 0,
		.namaBarang = "",
		.jumlahBarang = 0,
		.kadaluarsa = temp->data.kadaluarsa};
	gotoxy(posX + 60 + marginLeft, marginTop + 2 + posY);
	setTextColor(DARKGRAY);
	printf("-1 untuk tidak mengubah");
	ResetColor();
	gotoxy(posX + 42 + marginLeft, marginTop + 2 + posY);
	scanf("%d", &newBarang.nomorBarang);

	gotoxy(posX + 60 + marginLeft, marginTop + 3 + posY);
	setTextColor(DARKGRAY);
	printf("-1 untuk tidak mengubah");
	ResetColor();
	gotoxy(posX + 42 + marginLeft, marginTop + 3 + posY);
	scanf("%d", &newBarang.HargaBarang);

	gotoxy(posX + 60 + marginLeft, marginTop + 4 + posY);
	setTextColor(DARKGRAY);
	printf("-1 untuk tidak mengubah");
	ResetColor();
	gotoxy(posX + 42 + marginLeft, marginTop + 4 + posY);
	scanf("%f", &newBarang.diskon);

	gotoxy(posX + 60 + marginLeft, marginTop + 5 + posY);
	setTextColor(DARKGRAY);
	printf("- untuk tidak mengubah");
	ResetColor();
	gotoxy(posX + 42 + marginLeft, marginTop + 5 + posY);
	getchar();
	fgets(newBarang.namaBarang, sizeof(newBarang.namaBarang), stdin);
	newBarang.namaBarang[strcspn(newBarang.namaBarang, "\n")] = '\0';

	gotoxy(posX + 60 + marginLeft, marginTop + 6 + posY);
	setTextColor(DARKGRAY);
	printf("-1 untuk tidak mengubah");
	ResetColor();
	gotoxy(posX + 42 + marginLeft, marginTop + 6 + posY);
	scanf("%d", &newBarang.jumlahBarang);

	gotoxy(posX + 60 + marginLeft, marginTop + 7 + posY);
	setTextColor(DARKGRAY);
	printf("0 | 0 | 0 untuk tidak mengubah");
	ResetColor();
	int hari, bulan, tahun;
	gotoxy(posX + 42 + marginLeft, marginTop + 7 + posY);
	scanf("%d", &hari);
	gotoxy(posX + 45 + marginLeft, marginTop + 7 + posY);
	printf("| ");
	scanf("%d", &bulan);
	gotoxy(posX + 49 + marginLeft, marginTop + 7 + posY);
	printf("| ");
	scanf("%d", &tahun);
	

	newBarang.kadaluarsa = buatTanggal(hari, bulan, tahun);

	newBarang.nomorBarang = newBarang.nomorBarang < 0 ? temp->data.nomorBarang : newBarang.nomorBarang;
	newBarang.HargaBarang = newBarang.HargaBarang < 0 ? temp->data.HargaBarang : newBarang.HargaBarang;
	newBarang.diskon = newBarang.diskon < 0 ? temp->data.diskon : newBarang.diskon;
	if (strcmp(newBarang.namaBarang, "-") == 0)
	{
		strcpy(newBarang.namaBarang, temp->data.namaBarang);
	}
	newBarang.jumlahBarang = newBarang.jumlahBarang < 0 ? temp->data.jumlahBarang : newBarang.jumlahBarang;
	newBarang.kadaluarsa = (hari == 0 && bulan == 0 && tahun == 0) ? temp->data.kadaluarsa:buatTanggal(hari, bulan, tahun);

	editBarang(temp->data.nomorBarang, newBarang);

	gotoxy(posX + marginLeft, marginTop + 9 + posY);
	setTextColor(GREEN);
	printf("Tekan enter untuk kembali");
	ResetColor();
	getchar();
	getchar();
}

void RenderTabel(int rowWidth[], char *HeaderName[], int posX, int posY)
{
	int selection = 1;
	int itemsPerPage = 10;

	int totalItems = 0;
	struct nodeBarang *curr = headBarang;
	while (curr != NULL)
	{
		totalItems++;
		curr = curr->next;
	}

	do
	{
		system("cls");

		gotoxy(0, 1);
		setTextColor(YELLOW);
		printf("Selamat datang di menu manajemen");
		ResetColor();
		// Tentukan batas atas dan bawah item yang harus tampil di halaman saat ini
		int currentPage = (selection - 1) / itemsPerPage;
		int startItem = (currentPage * itemsPerPage) + 1;
		int endItem = startItem + itemsPerPage - 1;

		// Gambar Header
		createTableHeader(rowWidth, HeaderName, 7, posX, posY);

		struct nodeBarang *head = headBarang;
		int numbering = 1;
		int yRow = 0;

		// Gambar Body
		while (head != NULL)
		{
			if (numbering >= startItem && numbering <= endItem)
			{
				createbodyTabel(numbering, rowWidth, head->data, 7, posX, posY + 3 + yRow, selection);
				yRow += 2;
			}

			numbering++;
			head = head->next;
		}

		createfooterTabel(rowWidth, 7, posX, posY + 2 + yRow);

		
		gotoxy(posX, posY + 3 + yRow);
		printf("Item Terpilih: %d / %d  (Halaman %d)", selection, totalItems, currentPage + 1);
		gotoxy(posX, posY+4+ yRow);
		setTextColor(DARKGRAY);
		printf("Press Enter to edit..");
		gotoxy(posX, posY+5+ yRow);
		setTextColor(RED);
		printf("Press q to exit");
		ResetColor();

		char ch = getch();

		if (ch == 'w' || ch == 72)
		{
			if (selection > 1)
			{
				selection -= 1;
			}
		}

		else if (ch == 's' || ch == 80)
		{
			if (selection < totalItems)
			{
				selection += 1;
			}
		}

		else if (ch == 'q' || ch == 113)
		{
			break;
		}
		else if (ch == '\r' || ch == 13)
		{
			editData(selection);
		}

	} while (1);
}

void lihatSemuadata()
{
	addBarang((struct barang){.nomorBarang = 1, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 2, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 3, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 4, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 5, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 6, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 7, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 8, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 9, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 10, .HargaBarang = 15000, .diskon = 0.1f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 11, .HargaBarang = 5000, .diskon = 0.0f, .namaBarang = "Aqua Botol 600ml", .jumlahBarang = 100, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 12, .HargaBarang = 4000, .diskon = 0.0f, .namaBarang = "Teh Pucuk Harum", .jumlahBarang = 80, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 13, .HargaBarang = 12000, .diskon = 0.0f, .namaBarang = "Chitato Sapi", .jumlahBarang = 40, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 14, .HargaBarang = 18000, .diskon = 0.1f, .namaBarang = "Silverqueen Almond", .jumlahBarang = 25, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 15, .HargaBarang = 7000, .diskon = 0.0f, .namaBarang = "Coca Cola Kaleng", .jumlahBarang = 60, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 16, .HargaBarang = 22000, .diskon = 0.0f, .namaBarang = "Pringles Original", .jumlahBarang = 30, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 17, .HargaBarang = 8500, .diskon = 0.0f, .namaBarang = "Oreo Vanilla Pack", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 18, .HargaBarang = 15000, .diskon = 0.0f, .namaBarang = "Sari Roti Tawar", .jumlahBarang = 15, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 19, .HargaBarang = 6500, .diskon = 0.05f, .namaBarang = "Ultra Milk", .jumlahBarang = 70, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 20, .HargaBarang = 3500, .diskon = 0.0f, .namaBarang = "Mie Sedaap Goreng", .jumlahBarang = 120, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 21, .HargaBarang = 11000, .diskon = 0.0f, .namaBarang = "ABC Kecap", .jumlahBarang = 35, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 22, .HargaBarang = 28000, .diskon = 0.1f, .namaBarang = "Bimoli ", .jumlahBarang = 20, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 23, .HargaBarang = 16000, .diskon = 0.0f, .namaBarang = "Gulaku ", .jumlahBarang = 40, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 24, .HargaBarang = 25000, .diskon = 0.15f, .namaBarang = "Kopi Kapal Api 380g", .jumlahBarang = 30, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 25, .HargaBarang = 24000, .diskon = 0.0f, .namaBarang = "Lifebuoy Cair Refill", .jumlahBarang = 25, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 26, .HargaBarang = 12500, .diskon = 0.0f, .namaBarang = "Pepsodent White 190g", .jumlahBarang = 45, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 27, .HargaBarang = 30000, .diskon = 0.1f, .namaBarang = "Rinso Liquid 750ml", .jumlahBarang = 20, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 28, .HargaBarang = 9500, .diskon = 0.0f, .namaBarang = "Sunlight Jeruk Nipis", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 29, .HargaBarang = 14000, .diskon = 0.0f, .namaBarang = "Frisian Flag ", .jumlahBarang = 35, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 30, .HargaBarang = 6000, .diskon = 0.0f, .namaBarang = "Pocari Sweat 350ml", .jumlahBarang = 60, .kadaluarsa = time(NULL)});
	// create table
	//  |no.|Code|Nama barang|Harga|Diskon|Jumlah|Kadaluarsa
	int rowWidth[] = {4, 6, 25, 12, 7, 7, 12};
	char *headerName[] = {"No.", "Code", "Nama barang", "Harga (Rp.)", "Diskon", "Jumlah", "Kadaluarsa"};
	
	RenderTabel(rowWidth, headerName, 0, 4);
	int marginTop = 1;

	// print header
	return;
}

// Bagian manajemen baranng
void tampilanManajemen()
{

	lihatSemuadata();

	return;
	int pilihan = -1;
	do
	{
		system("cls");
		printf("1. Lihat semua Barang\n");
		printf("2. Tambah / hapus barang\n");
		printf("0. Keluar Data\n");
		printf("Pilih menu: ");
		scanf("%d", &pilihan);

		switch (pilihan)
		{
		case 1:
			break;
		case 2:
			// editData();
			break;
		case 0:
			return;
		default:
			system("cls");
			printf("Pilihan tidak ditemukan [Klik apapun untuk melanjutkan]");
			getchar();
			getchar();

			break;
		}
	} while (1);

} // ada tampilan yaitu menambah barang, dan menghapus barang, serta melihat semua barang, dan exit

// Bagian kasir
int simpanNota(struct nota notaSelesai)
{

} // bagian ini harus ada agar bagian kasir ngikutin. dan simpannya kedalam struct nota semuanotaTersimpan yang sudah ada

// Bagian riwayat Transaksi

// tinggal iklutin instruksi yang di atas ya

// Bagian login
int login(char username[20], char password[8])
{

} // pada bagian ini berfungsi melakukan proses login, jadi fungsi loginnya itu di sini kalau berhasil 1 kalau gagal 0

int main()
{
	system("cls");
	// DO NOT REMOVE THIS LINE
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	tampilanManajemen();

	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	return 0;
} // di sini adalah semua main menu yang tersedia. total ada 3 (manajemen barang, pos, riwayat transaksi)
