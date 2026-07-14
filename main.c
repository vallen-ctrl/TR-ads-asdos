
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

void clearLine(int lenght)
{
	for (int i = 0; i < lenght; i++)
	{
		printf(" ");
	}
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
	struct nodeBarang *temp = headBarang;
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
				PREV->next = temp->next;
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
	for (int row = 0; row <= height; row++)
	{
		for (int col = 0; col <= width; col++)
		{
			if (row == 0 && col == 0)
			{
				gotoxy(posX + col, posY + row);
				printf(light_DR);
			}
			else if (row == 0 && col == width)
			{
				gotoxy(posX + col, posY + row);
				printf(light_DL);
			}
			else if ((row > 0 && row < height && (col == 0 || col == width)))
			{
				gotoxy(posX + col, posY + row);
				printf(light_VLL);
			}
			else if (row == height && col == 0)
			{
				gotoxy(posX + col, posY + row);
				printf(light_UR);
			}
			else if (row == height && col == width)
			{
				gotoxy(posX + col, posY + row);
				printf(light_UL);
			}
			else if (row == 0 || row == height)
			{
				gotoxy(posX + col, posY + row);
				printf(light_HL);
			}
			else
			{
				gotoxy(posX + col, posY + row);
				printf(" ");
			}
		}
		printf("\n");
	}
}

void inputTextWithMaxlenght(int maxLenght, char *buffer)
{
	int i = 0;
	char ch;
	while ((ch = getch()) != '\r')
	{
		if (ch == '\b')
		{ // Backspace
			if (i > 0)
			{
				i--;
				printf("\b \b");
			}
		}
		else if (i < maxLenght - 1)
		{
			buffer[i++] = ch;
			putchar(ch);
		}
	}
	buffer[i] = '\0';
	printf("\n");
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

void createbodyTabel(int num, int rowWidth[], struct nodeBarang *node, int lenght, int xpos, int ypos, int selection)
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
	printf("%d", node->data.nomorBarang);

	dataTemprow += rowWidth[1] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%s", node->data.namaBarang);

	dataTemprow += rowWidth[2] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%d", node->data.HargaBarang);

	dataTemprow += rowWidth[3] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%.2f", node->data.diskon);

	dataTemprow += rowWidth[4] + 1;
	gotoxy(dataTemprow, ypos);
	printf("%d", node->data.jumlahBarang);

	struct tm *t = localtime(&node->data.kadaluarsa);
	dataTemprow += rowWidth[5] + 1;
	gotoxy(dataTemprow, ypos);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", t);
	printf("%s", buffer);

	t = localtime(&node->waktuDibuat);
	dataTemprow += rowWidth[6] + 1;
	gotoxy(dataTemprow, ypos);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y", t);
	printf("%s", buffer);

	t = localtime(&node->waktuDiubah);
	dataTemprow += rowWidth[7] + 1;
	gotoxy(dataTemprow, ypos);
	strftime(buffer, sizeof(buffer), "%d-%m-%Y | %H:%M", t);
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

	if (temp == NULL)
	{
		return;
	}

	int width = 95;
	int height = 18;
	int posX = 0, posY = 0;
	createBox(width, height, posX, posY);

	gotoxy((95 / 2) - (17 / 2), posY + 1);
	setTextColor(BLUE);
	printf("EDIT BARANG ANDA");
	ResetColor();

	int marginTop = 3;
	int marginLeft = 2;

	struct tm *diubah = localtime(&temp->waktuDiubah);
	char bf[80];
	strftime(bf, sizeof(bf), "%d-%m-%Y", diubah);
	gotoxy(posX + marginLeft, marginTop + posY);
	setTextColor(CYAN);
	printf("Terakhir diubah -> %s", bf);
	ResetColor();

	struct barang newBarang = temp->data;
	struct tm *t_kadaluarsa = localtime(&temp->data.kadaluarsa);
	int hari = t_kadaluarsa->tm_mday;
	int bulan = t_kadaluarsa->tm_mon + 1;
	int tahun = t_kadaluarsa->tm_year + 1900;

	int Editselected = 0;
	int maxSelected = 8; // 0-5 fields, 6: Simpan, 7: Batal

	while (1)
	{
		// Redraw labels and current values
		gotoxy(posX + marginLeft, marginTop + 2 + posY);
		printf("Nomor \t");
		printf("(%d)", newBarang.nomorBarang);
		clearLine(24);

		gotoxy(posX + marginLeft, marginTop + 3 + posY);
		printf("Harga \t");
		printf("(%d)", newBarang.HargaBarang);
		clearLine(24);

		gotoxy(posX + marginLeft, marginTop + 4 + posY);
		printf("Diskon \t");
		printf("(%.2f)", newBarang.diskon);
		clearLine(24);

		gotoxy(posX + marginLeft, marginTop + 5 + posY);
		printf("Nama \t\t");
		printf("(%s)", newBarang.namaBarang);
		clearLine(24 - strlen(newBarang.namaBarang));

		gotoxy(posX + marginLeft, marginTop + 6 + posY);
		printf("Jumlah \t");
		printf("(%d)", newBarang.jumlahBarang);
		clearLine(24);

		gotoxy(posX + marginLeft, marginTop + 7 + posY);
		printf("Kadaluarsa \t");
		printf("(%02d-%02d-%04d)         ", hari, bulan, tahun);

		for (int i = 2; i <= 7; i++)
		{
			gotoxy(posX + 40 + marginLeft, marginTop + i + posY);
			printf(":");
		}

		// Action buttons
		gotoxy(posX + marginLeft, marginTop + 9 + posY);
		printf("                                            "); // clear line
		gotoxy(posX + marginLeft, marginTop + 9 + posY);
		if (Editselected == 6)
			setTextColor(GREEN);
		printf("[ SIMPAN BARANG ]");
		ResetColor();

		printf("   ");

		if (Editselected == 7)
			setTextColor(RED);
		printf("[ BATAL ]");
		ResetColor();

		// Help text
		gotoxy(posX + marginLeft, marginTop + 11 + posY);
		setTextColor(GREEN);
		printf("Gunakan W/S untuk memilih, Enter untuk mengedit/memilih tindakan.");
		gotoxy(posX + marginLeft, marginTop + 12 + posY);
		setTextColor(RED);
		printf("Tekan Q untuk batal.");
		ResetColor();

		// Draw selection arrow
		for (int i = 0; i < 6; i++)
		{
			gotoxy(width - 5, marginTop + 2 + posY + i);
			printf("  ");
		}
		if (Editselected < 6)
		{
			gotoxy(width - 5, marginTop + 2 + posY + Editselected);
			setTextColor(YELLOW);
			printf("<-");
			ResetColor();
		}

		char ch = getch();
		if (ch == 'w' || ch == 72)
		{
			if (Editselected > 0)
			{
				Editselected--;
			}
		}
		else if (ch == 's' || ch == 80)
		{
			if (Editselected < maxSelected - 1)
			{
				Editselected++;
			}
		}
		else if (ch == 'a' || ch == 75) // Left arrow on buttons
		{
			if (Editselected == 7)
			{
				Editselected = 6;
			}
		}
		else if (ch == 'd' || ch == 77) // Right arrow on buttons
		{
			if (Editselected == 6)
			{
				Editselected = 7;
			}
		}
		else if (ch == 'q' || ch == 113)
		{
			break;
		}
		else if (ch == '\r' || ch == 13)
		{
			if (Editselected == 6)
			{
				// Save action
				if (newBarang.nomorBarang <= 0 || newBarang.HargaBarang < 0 || strlen(newBarang.namaBarang) == 0 || newBarang.jumlahBarang < 0 || hari == 0 || bulan == 0 || tahun == 0)
				{
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					setTextColor(RED);
					printf("Gagal: Semua field harus diisi dengan benar!");
					ResetColor();
					getch();
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					printf("                                              ");
				}
				else if (newBarang.nomorBarang != temp->data.nomorBarang && searchBarang_nomor(newBarang.nomorBarang) != NULL)
				{
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					setTextColor(RED);
					printf("Gagal: Kode barang sudah digunakan barang lain!");
					ResetColor();
					getch();
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					printf("                                                 ");
				}
				else
				{
					newBarang.kadaluarsa = buatTanggal(hari, bulan, tahun);
					int res = editBarang(temp->data.nomorBarang, newBarang);
					if (res == 1)
					{
						gotoxy(posX + marginLeft, marginTop + 13 + posY);
						setTextColor(GREEN);
						printf("Sukses mengubah data barang!");
						ResetColor();
						getch();
						break;
					}
					else
					{
						gotoxy(posX + marginLeft, marginTop + 13 + posY);
						setTextColor(RED);
						printf("Gagal menyimpan perubahan!");
						ResetColor();
						getch();
						break;
					}
				}
			}
			else if (Editselected == 7)
			{
				break;
			}
			else
			{
				// Edit field
				gotoxy(posX + marginLeft, marginTop + 13 + posY);
				setTextColor(DARKGRAY);
				printf("Masukkan nilai baru...");
				ResetColor();

				switch (Editselected)
				{
				case 0:
					gotoxy(posX + 42 + marginLeft, marginTop + 2 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 2 + posY);
					scanf("%d", &newBarang.nomorBarang);
					break;
				case 1:
					gotoxy(posX + 42 + marginLeft, marginTop + 3 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 3 + posY);
					scanf("%d", &newBarang.HargaBarang);
					break;
				case 2:
					gotoxy(posX + 42 + marginLeft, marginTop + 4 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 4 + posY);
					scanf("%f", &newBarang.diskon);
					break;
				case 3:
					gotoxy(posX + 42 + marginLeft, marginTop + 5 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 5 + posY);
					inputTextWithMaxlenght(20, newBarang.namaBarang);
					break;
				case 4:
					gotoxy(posX + 42 + marginLeft, marginTop + 6 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 6 + posY);
					scanf("%d", &newBarang.jumlahBarang);
					break;
				case 5:
					gotoxy(posX + 42 + marginLeft, marginTop + 7 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 7 + posY);
					scanf("%d", &hari);
					gotoxy(posX + 45 + marginLeft, marginTop + 7 + posY);
					printf("| ");
					scanf("%d", &bulan);
					gotoxy(posX + 49 + marginLeft, marginTop + 7 + posY);
					printf("| ");
					scanf("%d", &tahun);
					break;
				}

				// Clear prompt
				gotoxy(posX + marginLeft, marginTop + 13 + posY);
				printf("                          ");
			}
		}
	}
}

void tambahBarangUI()
{
	ResetColor();
	system("cls");

	int width = 95;
	int height = 18;
	int posX = 0, posY = 0;
	createBox(width, height, posX, posY);

	gotoxy((95 / 2) - (18 / 2), posY + 1);
	setTextColor(BLUE);
	printf("TAMBAH BARANG BARU");
	ResetColor();

	int marginTop = 3;
	int marginLeft = 2;

	gotoxy(posX + marginLeft, marginTop + posY);
	setTextColor(CYAN);
	printf("Silakan isi data barang di bawah ini");
	ResetColor();

	struct barang newBarang = {
		.nomorBarang = 0,
		.HargaBarang = 0,
		.diskon = 0.0f,
		.namaBarang = "",
		.jumlahBarang = 0,
		.kadaluarsa = 0};

	int hari = 0, bulan = 0, tahun = 0;
	int Editselected = 0;
	int maxSelected = 8;

	while (1)
	{
		// Redraw labels and current values
		gotoxy(posX + marginLeft, marginTop + 2 + posY);
		printf("Nomor \t");
		if (newBarang.nomorBarang != 0)
			printf("(%d)                    ", newBarang.nomorBarang);
		else
			printf("(Belum diisi)        ");

		gotoxy(posX + marginLeft, marginTop + 3 + posY);
		printf("Harga \t");
		if (newBarang.HargaBarang != 0)
			printf("(%d)                 ", newBarang.HargaBarang);
		else
			printf("(Belum diisi)        ");

		gotoxy(posX + marginLeft, marginTop + 4 + posY);
		printf("Diskon \t");
		if (newBarang.diskon != 0.0f)
			printf("(%.2f)                ", newBarang.diskon);
		else
			printf("(Belum diisi)        ");

		gotoxy(posX + marginLeft, marginTop + 5 + posY);
		printf("Nama \t\t");
		if (strlen(newBarang.namaBarang) > 0)
			printf("(%s)             ", newBarang.namaBarang);
		else
			printf("(Belum diisi)        ");

		gotoxy(posX + marginLeft, marginTop + 6 + posY);
		printf("Jumlah \t");
		if (newBarang.jumlahBarang != 0)
			printf("(%d)                 ", newBarang.jumlahBarang);
		else
			printf("(Belum diisi)        ");

		gotoxy(posX + marginLeft, marginTop + 7 + posY);
		printf("Kadaluarsa \t");
		if (hari != 0 && bulan != 0 && tahun != 0)
			printf("(%02d-%02d-%04d)         ", hari, bulan, tahun);
		else
			printf("(Belum diisi)        ");

		for (int i = 2; i <= 7; i++)
		{
			gotoxy(posX + 40 + marginLeft, marginTop + i + posY);
			printf(":");
		}

		// Action buttons
		gotoxy(posX + marginLeft, marginTop + 9 + posY);
		printf("                                            ");
		gotoxy(posX + marginLeft, marginTop + 9 + posY);
		if (Editselected == 6)
			setTextColor(GREEN);
		printf("[ SIMPAN BARANG ]");
		ResetColor();

		printf("   ");

		if (Editselected == 7)
			setTextColor(RED);
		printf("[ BATAL ]");
		ResetColor();

		gotoxy(posX + marginLeft, marginTop + 11 + posY);
		setTextColor(GREEN);
		printf("Gunakan W/S untuk memilih, Enter untuk mengedit/memilih tindakan.");
		gotoxy(posX + marginLeft, marginTop + 12 + posY);
		setTextColor(RED);
		printf("Tekan Q untuk batal.");
		ResetColor();

		// Draw selection arrow
		for (int i = 0; i < 6; i++)
		{
			gotoxy(width - 5, marginTop + 2 + posY + i);
			printf("  ");
		}
		if (Editselected < 6)
		{
			gotoxy(width - 5, marginTop + 2 + posY + Editselected);
			setTextColor(YELLOW);
			printf("<-");
			ResetColor();
		}

		char ch = getch();
		if (ch == 'w' || ch == 72)
		{
			if (Editselected > 0)
			{
				Editselected--;
			}
		}
		else if (ch == 's' || ch == 80)
		{
			if (Editselected < maxSelected - 1)
			{
				Editselected++;
			}
		}
		else if (ch == 'a' || ch == 75)
		{
			if (Editselected == 7)
			{
				Editselected = 6;
			}
		}
		else if (ch == 'd' || ch == 77)
		{
			if (Editselected == 6)
			{
				Editselected = 7;
			}
		}
		else if (ch == 'q' || ch == 113)
		{
			break;
		}
		else if (ch == '\r' || ch == 13)
		{
			if (Editselected == 6)
			{
				if (newBarang.nomorBarang == 0 || newBarang.HargaBarang == 0 || strlen(newBarang.namaBarang) == 0 || newBarang.jumlahBarang == 0 || hari == 0 || bulan == 0 || tahun == 0)
				{
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					setTextColor(RED);
					printf("Gagal: Semua field harus diisi!");
					ResetColor();
					getch();
					gotoxy(posX + marginLeft, marginTop + 13 + posY);
					printf("                               ");
				}
				else
				{
					if (searchBarang_nomor(newBarang.nomorBarang) != NULL)
					{
						gotoxy(posX + marginLeft, marginTop + 13 + posY);
						setTextColor(RED);
						printf("Gagal: Kode barang sudah ada!");
						ResetColor();
						getch();
						gotoxy(posX + marginLeft, marginTop + 13 + posY);
						printf("                             ");
					}
					else
					{
						newBarang.kadaluarsa = buatTanggal(hari, bulan, tahun);
						int res = addBarang(newBarang);
						if (res == 1)
						{
							gotoxy(posX + marginLeft, marginTop + 13 + posY);
							setTextColor(GREEN);
							printf("Sukses menambahkan barang baru!");
							ResetColor();
							getch();
							break;
						}
						else
						{
							gotoxy(posX + marginLeft, marginTop + 13 + posY);
							setTextColor(RED);
							printf("Gagal menambahkan barang!");
							ResetColor();
							getch();
							break;
						}
					}
				}
			}
			else if (Editselected == 7)
			{
				break;
			}
			else
			{
				gotoxy(posX + marginLeft, marginTop + 13 + posY);
				setTextColor(DARKGRAY);
				printf("Masukkan nilai baru...");
				ResetColor();

				switch (Editselected)
				{
				case 0:
					gotoxy(posX + 42 + marginLeft, marginTop + 2 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 2 + posY);
					scanf("%d", &newBarang.nomorBarang);
					break;
				case 1:
					gotoxy(posX + 42 + marginLeft, marginTop + 3 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 3 + posY);
					scanf("%d", &newBarang.HargaBarang);
					break;
				case 2:
					gotoxy(posX + 42 + marginLeft, marginTop + 4 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 4 + posY);
					scanf("%f", &newBarang.diskon);
					break;
				case 3:
					gotoxy(posX + 42 + marginLeft, marginTop + 5 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 5 + posY);
					fgets(newBarang.namaBarang, sizeof(newBarang.namaBarang), stdin);
					newBarang.namaBarang[strcspn(newBarang.namaBarang, "\n")] = '\0';
					break;
				case 4:
					gotoxy(posX + 42 + marginLeft, marginTop + 6 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 6 + posY);
					scanf("%d", &newBarang.jumlahBarang);
					break;
				case 5:
					gotoxy(posX + 42 + marginLeft, marginTop + 7 + posY);
					clearLine(41);
					gotoxy(posX + 42 + marginLeft, marginTop + 7 + posY);
					scanf("%d", &hari);
					gotoxy(posX + 45 + marginLeft, marginTop + 7 + posY);
					printf("| ");
					scanf("%d", &bulan);
					gotoxy(posX + 49 + marginLeft, marginTop + 7 + posY);
					printf("| ");
					scanf("%d", &tahun);
					break;
				}

				gotoxy(posX + marginLeft, marginTop + 13 + posY);
				printf("                          ");
			}
		}
	}
}

void RenderTabel(int rowWidth[], char *HeaderName[], int posX, int posY)
{
	int selection = 1;
	int itemsPerPage = 10;

	do
	{
		int totalItems = 0;
		struct nodeBarang *curr = headBarang;
		while (curr != NULL)
		{
			totalItems++;
			curr = curr->next;
		}
		system("cls");

		gotoxy(0, 1);
		setTextColor(YELLOW);
		printf("Selamat datang di menu manajemen");
		ResetColor();

		int currentPage = (selection - 1) / itemsPerPage;
		int startItem = (currentPage * itemsPerPage) + 1;
		int endItem = startItem + itemsPerPage - 1;

		createTableHeader(rowWidth, HeaderName, 9, posX, posY);

		struct nodeBarang *head = headBarang;
		int numbering = 1;
		int yRow = 0;

		while (head != NULL)
		{
			if (numbering >= startItem && numbering <= endItem)
			{
				createbodyTabel(numbering, rowWidth, head, 9, posX, posY + 3 + yRow, selection);
				yRow += 2;
			}

			numbering++;
			head = head->next;
		}

		createfooterTabel(rowWidth, 9, posX, posY + 2 + yRow);

		gotoxy(posX, posY + 3 + yRow);
		printf("Item Terpilih: %d / %d  (Halaman %d)", selection, totalItems, currentPage + 1);
		gotoxy(posX, posY + 4 + yRow);
		setTextColor(DARKGRAY);
		printf("Press e to edit..");
		gotoxy(posX, posY + 5 + yRow);
		setTextColor(RED);
		printf("Press q to exit");

		gotoxy(posX + 25, posY + 4 + yRow);
		setTextColor(DARKGRAY);
		printf("| Press d to delete..");
		gotoxy(posX + 25, posY + 5 + yRow);
		setTextColor(DARKGRAY);
		printf("| Press a to add..");
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
		else if (ch == 'e' || ch == 101)
		{
			editData(selection);
		}
		else if (ch == 'd' || ch == 100)
		{
			int boxPosX = posX + 40, boxPosY = posY + 6;
			createBox(40, 10, boxPosX, boxPosY);
			gotoxy(boxPosX + (boxPosX / 2) - (28 / 2), boxPosY + 1);
			printf("Yakin menghapus barang ini?");
			gotoxy(boxPosX + 1, boxPosY + 3);
			printf("Proses penghapusan sangat berbahaya");
			gotoxy(boxPosX + 1, boxPosY + 4);
			printf("karena tidak bisa dikembalikan");

			int pilihan = 1;
			gotoxy(boxPosX + (boxPosX / 2) - (13 / 2), boxPosY + 7);
			printf("Ya");
			printf(" | ");
			setTextColor(GREEN);
			printf("Tidak");
			ResetColor();
			while (1)
			{
				char yorn = getch();
				if (yorn == 'a' || yorn == 75)
				{
					gotoxy(boxPosX + (boxPosX / 2) - (13 / 2), boxPosY + 7);
					setTextColor(RED);
					printf("Ya");
					ResetColor();
					printf(" | ");
					printf("Tidak");
					pilihan = 0;
				}
				else if (yorn == 'd' || yorn == 77)
				{
					gotoxy(boxPosX + (boxPosX / 2) - (13 / 2), boxPosY + 7);
					printf("Ya");
					printf(" | ");
					setTextColor(GREEN);
					printf("Tidak");
					ResetColor();
					pilihan = 1;
					/* code */
				}
				else if (yorn == '\r' || yorn == 13)
				{
					if (pilihan == 0)
					{
						struct nodeBarang *hapusBarang = headBarang;
						int hop = 1;
						while (hapusBarang != NULL)
						{
							if (hop == selection)
							{
								break;
							}
							hop += 1;
							hapusBarang = hapusBarang->next;
						}
						deleteBarang(hapusBarang->data.nomorBarang);
						break;
					}
					else if (pilihan == 1)
					{
						break;
					}
				}
			}
		}
		else if (ch == 'a' || ch == 75)
		{
			tambahBarangUI();
		}

	} while (1);
}

void lihatSemuadata()
{
	// create table
	//  |no.|Code|Nama barang|Harga|Diskon|Jumlah|Kadaluarsa
	int rowWidth[] = {4, 10, 25, 12, 7, 7, 12, 12, 20};
	char *headerName[] = {"No.", "Code", "Nama barang", "Harga (Rp.)", "Diskon", "Jumlah", "Kadaluarsa", "Dibuat pada", "Diubah pada"};

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
	if (totalNotaTersimpan >= 100)
	{
		return 0;
	}
	semuanotaTersimpan[totalNotaTersimpan] = notaSelesai;
	totalNotaTersimpan++;
	return 1;
}

void tampilKeranjang(struct barangdibeli keranjang[], int jumlahItem)
{
	int width = 98;
	int height = 22;
	int posX = 0, posY = 1;

	system("cls");
	createBox(width, height, posX, posY);
	gotoxy((width / 2) - 8, posY + 1);
	setTextColor(BLUE);
	printf("MENU KASIR");
	ResetColor();

	gotoxy(posX + 3, posY + 3);
	setTextColor(YELLOW);
	printf("Keranjang:");
	ResetColor();

	gotoxy(posX + 3, posY + 5);
	printf("%-25s| %-10s| %-8s| %-8s", "Nama", "Harga", "Diskon", "Qty");
	gotoxy(posX + 3, posY + 6);
	printf("--------------------------------------------------------------------------------");

	for (int i = 0; i < jumlahItem; i++)
	{
		int y = posY + 7 + i;
		if (y >= posY + height - 4)
		{
			gotoxy(posX + 3, posY + height - 4);
			setTextColor(RED);
			printf("(Terlalu banyak item untuk tampil)");
			ResetColor();
			break;
		}
		gotoxy(posX + 3, y);
		printf("%-25s| Rp%-7d| %-7.2f| %-8d",
				keranjang[i].barang.namaBarang,
				keranjang[i].barang.HargaBarang,
				keranjang[i].diskon,
				keranjang[i].jumlah);
	}
}

void tampilanKasir()
{
	int jumlahItem = 0;
	struct barangdibeli keranjang[100];
	int totalBayar = 0;

	if (strlen(kasir_sekarang.username) == 0)
	{
		strcpy(kasir_sekarang.username, "kasir");
	}

	while (1)
	{
		tampilKeranjang(keranjang, jumlahItem);

		int posX = 0, posY = 1;
		int inputY = posY + 20;
		gotoxy(posX + 3, inputY);
		setTextColor(GREEN);
		printf("Input nomorBarang (int) | 0 untuk checkout : ");
		ResetColor();

		int nomorBarang;
		if (scanf("%d", &nomorBarang) != 1)
		{
			getch();
			continue;
		}

		if (nomorBarang == 0)
		{
			break;
		}

		struct barang *b = getBarang_nomor(nomorBarang);
		if (b == NULL)
		{
			gotoxy(posX + 3, inputY + 1);
			setTextColor(RED);
			printf("Barang tidak ditemukan. Tekan apa saja untuk melanjutkan...");
			ResetColor();
			getch();
			continue;
		}

		gotoxy(posX + 3, inputY + 1);
		printf("Input quantity             : ");
		gotoxy(posX + 32, inputY + 1);

		int qty;
		if (scanf("%d", &qty) != 1)
		{
			getch();
			continue;
		}

		if (qty <= 0)
		{
			setTextColor(RED);
			printf("Quantity tidak valid. Tekan apa saja untuk melanjutkan...");
			ResetColor();
			getch();
			continue;
		}

		if (b->jumlahBarang < qty)
		{
			setTextColor(RED);
			printf("Stok kurang. Stok tersedia %d. Tekan apa saja untuk melanjutkan...", b->jumlahBarang);
			ResetColor();
			getch();
			continue;
		}

		if (jumlahItem >= 100)
		{
			setTextColor(RED);
			printf("Keranjang penuh. Tekan apa saja untuk melanjutkan...");
			ResetColor();
			getch();
			break;
		}

		keranjang[jumlahItem].nomorNota = totalNotaTersimpan + 1;
		keranjang[jumlahItem].barang = *b;
		keranjang[jumlahItem].jumlah = qty;
		keranjang[jumlahItem].diskon = b->diskon;

		float hargaLine = (float)b->HargaBarang * qty * (1.0f - b->diskon);
		keranjang[jumlahItem].subTotall = (int)(hargaLine + 0.5f);
		totalBayar += keranjang[jumlahItem].subTotall;
		jumlahItem++;
	}

	if (jumlahItem == 0)
	{
		gotoxy(0, 45);
		printf("Tidak ada item untuk checkout.\n");
		getch();
		return;
	}

	struct nota notaSelesai;
	notaSelesai.nomorNota = totalNotaTersimpan + 1;
	strcpy(notaSelesai.nama_kasir, kasir_sekarang.username);
	notaSelesai.waktuTanggalTransaksi = time(NULL);

	for (int i = 0; i < jumlahItem; i++)
	{
		keranjang[i].nomorNota = notaSelesai.nomorNota;
		notaSelesai.semuabarangp[i] = keranjang[i];
	}

	for (int i = 0; i < jumlahItem; i++)
	{
		ambilBarang(keranjang[i].barang.nomorBarang, keranjang[i].jumlah);
	}

	simpanNota(notaSelesai);

	system("cls");
	setTextColor(YELLOW);
	printf("CHECKOUT BERHASIL!\n");
	ResetColor();
	printf("nama barang yang dibeli:\n");
	for (int i = 0; i < jumlahItem; i++)
	{
		printf("%d. %s | Qty: %d | Subtotal: Rp%d\n",
			   i + 1,
			   keranjang[i].barang.namaBarang,
			   keranjang[i].jumlah,
			   keranjang[i].subTotall);
	}
	printf("Total bayar: Rp%d\n", totalBayar);
	printf("Nomor nota : %d\n", notaSelesai.nomorNota);
	printf("Tekan apa saja untuk kembali...");
	getch();
}

// Bagian riwayat Transaksi
void tampilRiwayatTransaksi()
{
    system("cls");
	


    int width = 100;
    int height = 30;

    createBox(width, height, 0, 0);

    gotoxy((width / 2) - 10, 1);
    setTextColor(BLUE);
    printf("RIWAYAT TRANSAKSI");
    ResetColor();

    if (totalNotaTersimpan == 0)
    {
        gotoxy(3, 4);
        setTextColor(RED);
        printf("Belum ada transaksi.");
        ResetColor();

        gotoxy(3, 6);
        printf("Tekan apa saja untuk kembali...");
        getch();
        return;
    }

    int y = 3;

    for (int i = 0; i < totalNotaTersimpan; i++)
    {
        struct nota *n = &semuanotaTersimpan[i];

        struct tm *tanggal = localtime(&n->waktuTanggalTransaksi);

        char waktu[50];
        strftime(waktu, sizeof(waktu), "%d-%m-%Y %H:%M", tanggal);

        gotoxy(2, y++);
        setTextColor(YELLOW);
        printf("==============================================================");
        ResetColor();

        gotoxy(2, y++);
        printf("Nomor Nota : %d", n->nomorNota);

        gotoxy(35, y - 1);
        printf("Kasir : %s", n->nama_kasir);

        gotoxy(2, y++);
        printf("Tanggal : %s", waktu);

        gotoxy(2, y++);
        printf("--------------------------------------------------------------");

        gotoxy(2, y++);
        printf("%-4s %-25s %-6s %-10s %-10s",
               "No",
               "Barang",
               "Qty",
               "Harga",
               "Subtotal");

        int total = 0;

        for (int j = 0; j < 100; j++)
        {
            if (n->semuabarangp[j].jumlah <= 0)
                break;

            gotoxy(2, y++);

            printf("%-4d %-25s %-6d %-10d %-10d",
                   j + 1,
                   n->semuabarangp[j].barang.namaBarang,
                   n->semuabarangp[j].jumlah,
                   n->semuabarangp[j].barang.HargaBarang,
                   n->semuabarangp[j].subTotall);

            total += n->semuabarangp[j].subTotall;

            if (y >= 27)
            {
                gotoxy(2, 28);
                printf("Tekan apa saja untuk halaman berikutnya...");
                getch();

                system("cls");
                createBox(width, height, 0, 0);

                gotoxy((width / 2) - 10, 1);
                setTextColor(BLUE);
                printf("RIWAYAT TRANSAKSI");
                ResetColor();

                y = 3;
            }
        }

        gotoxy(2, y++);
        setTextColor(GREEN);
        printf("TOTAL BELANJA : Rp%d", total);
        ResetColor();

        y++;

        if (y >= 26 && i != totalNotaTersimpan - 1)
        {
            gotoxy(2, 28);
            printf("Tekan apa saja untuk transaksi berikutnya...");
            getch();

            system("cls");
            createBox(width, height, 0, 0);

            gotoxy((width / 2) - 10, 1);
            setTextColor(BLUE);
            printf("RIWAYAT TRANSAKSI");
            ResetColor();

            y = 3;
        }
    }

    gotoxy(2, 28);
    printf("Tekan apa saja untuk kembali...");
    getch();
}

void ahkiran()
{

	system("cls");
	tampilanKasir();

	tampilRiwayatTransaksi();
	
	tampilanKasir();
	}

void adddumyData()
{
	addBarang((struct barang){.nomorBarang = 1, .HargaBarang = 3500, .diskon = 0.0f, .namaBarang = "Indomie Goreng", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 2, .HargaBarang = 3200, .diskon = 0.0f, .namaBarang = "Indomie Kuah Ayam", .jumlahBarang = 45, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 3, .HargaBarang = 4500, .diskon = 0.0f, .namaBarang = "Beras Rojolele 1kg", .jumlahBarang = 40, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 4, .HargaBarang = 2500, .diskon = 0.0f, .namaBarang = "Telur Ayam Butir", .jumlahBarang = 200, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 5, .HargaBarang = 9000, .diskon = 0.05f, .namaBarang = "Kecap Manis ABC", .jumlahBarang = 30, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 6, .HargaBarang = 13500, .diskon = 0.0f, .namaBarang = "Susu Kental Manis", .jumlahBarang = 35, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 7, .HargaBarang = 21000, .diskon = 0.1f, .namaBarang = "Deterjen Rinso 800g", .jumlahBarang = 25, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 8, .HargaBarang = 17500, .diskon = 0.0f, .namaBarang = "Sabun Lifebuoy 4pcs", .jumlahBarang = 30, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 9, .HargaBarang = 8000, .diskon = 0.0f, .namaBarang = "Pasta Gigi Pepsodent", .jumlahBarang = 40, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 10, .HargaBarang = 6500, .diskon = 0.0f, .namaBarang = "Sunlight 400ml", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
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
	addBarang((struct barang){.nomorBarang = 22, .HargaBarang = 28000, .diskon = 0.1f, .namaBarang = "Bimoli", .jumlahBarang = 20, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 23, .HargaBarang = 16000, .diskon = 0.0f, .namaBarang = "Gulaku", .jumlahBarang = 40, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 24, .HargaBarang = 25000, .diskon = 0.15f, .namaBarang = "Kopi Kapal Api 380g", .jumlahBarang = 30, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 25, .HargaBarang = 24000, .diskon = 0.0f, .namaBarang = "Lifebuoy Cair Refill", .jumlahBarang = 25, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 26, .HargaBarang = 12500, .diskon = 0.0f, .namaBarang = "Pepsodent White 190g", .jumlahBarang = 45, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 27, .HargaBarang = 30000, .diskon = 0.1f, .namaBarang = "Rinso Liquid 750ml", .jumlahBarang = 20, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 28, .HargaBarang = 9500, .diskon = 0.0f, .namaBarang = "Sunlight Jeruk Nipis", .jumlahBarang = 50, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 29, .HargaBarang = 14000, .diskon = 0.0f, .namaBarang = "Frisian Flag", .jumlahBarang = 35, .kadaluarsa = time(NULL)});
	addBarang((struct barang){.nomorBarang = 30, .HargaBarang = 6000, .diskon = 0.0f, .namaBarang = "Pocari Sweat 350ml", .jumlahBarang = 60, .kadaluarsa = time(NULL)});
}

// Bagian login
int login(char username[20], char password[8])
{
	system("cls");

	int width = 40, height = 12;
	int posX = (TERMINALWIDTH / 2) - (width / 2);
	int posY = 8;

	// Gambar box utama login
	createBox(width, height, posX, posY);

	// Judul
	setTextColor(CYAN);
	gotoxy(posX + (width / 2) - 5, posY + 1);
	printf("=[ LOGIN ]=");
	ResetColor();

	// Label Username
	gotoxy(posX + 3, posY + 3);
	setTextColor(LIGHTGRAY);
	printf("Username : ");
	ResetColor();

	// Input username
	gotoxy(posX + 13, posY + 3);
	setTextColor(WHITE);
	inputTextWithMaxlenght(20, username);
	ResetColor();

	// Label Password
	gotoxy(posX + 3, posY + 5);
	setTextColor(LIGHTGRAY);
	printf("Password : ");
	ResetColor();

	// Input password (disembunyikan dengan *)
	gotoxy(posX + 13, posY + 5);
	setTextColor(WHITE);
	int passLen = 0;
	char ch;
	while ((ch = getch()) != '\r')
	{
		if (ch == '\b')
		{
			if (passLen > 0)
			{
				passLen--;
				printf("\b \b");
			}
		}
		else if (passLen < 7)
		{
			password[passLen++] = ch;
			putchar('*');
		}
	}
	password[passLen] = '\0';
	ResetColor();

	// Cek kredensial
	for (int i = 0; i < 2; i++)
	{
		if (strcmp(semua_Kasir[i].username, username) == 0 &&
			strcmp(semua_Kasir[i].password, password) == 0)
		{
			kasir_sekarang = semua_Kasir[i];

			// Pesan sukses
			gotoxy(posX + 3, posY + 9);
			setTextColor(GREEN);
			printf("Login berhasil! Selamat datang, %s", kasir_sekarang.username);
			ResetColor();
			gotoxy(posX + 3, posY + 11);
			setTextColor(DARKGRAY);
			printf("Tekan sembarang tombol...");
			ResetColor();
			getch();
			return 1;
		}
	}

	// Pesan gagal
	gotoxy(posX + 3, posY + 9);
	setTextColor(RED);
	printf("Username atau password salah!");
	ResetColor();
	gotoxy(posX + 3, posY + 11);
	setTextColor(DARKGRAY);
	printf("Tekan sembarang tombol...");
	ResetColor();
	getch();
	return 0;
}

void mainmenu()
{
	int pilihan;
	int sudahLogout = 0;

	while (!sudahLogout)
	{
		system("cls");
		int width = 60, height = 14;
		int posX = (TERMINALWIDTH / 2) - (width / 2);
		int posY = 5;

		createBox(width, height, posX, posY);

		gotoxy(posX + (width / 2) - 5, posY + 1);
		setTextColor(BLUE);
		printf("MENU UTAMA");
		ResetColor();

		gotoxy(posX + 3, posY + 3);
		printf("Login sebagai : %s", kasir_sekarang.username);

		gotoxy(posX + 3, posY + 5);
		printf("1. Manajemen Barang");
		gotoxy(posX + 3, posY + 6);
		printf("2. Kasir / Transaksi");
		gotoxy(posX + 3, posY + 7);
		printf("3. Riwayat Transaksi");
		gotoxy(posX + 3, posY + 8);
		setTextColor(YELLOW);
		printf("9. Logout");
		ResetColor();
		gotoxy(posX + 3, posY + 9);
		setTextColor(RED);
		printf("0. Keluar Program");
		ResetColor();

		gotoxy(posX + 3, posY + 11);
		printf("Pilih menu: ");

		if (scanf("%d", &pilihan) != 1)
		{
			while (getchar() != '\n');
			pilihan = -1;
		}

		switch (pilihan)
		{
		case 1:
			tampilanManajemen();
			break;
		case 2:
			tampilanKasir();
			break;
		case 3:
			tampilRiwayatTransaksi();
			break;
		case 9:
			sudahLogout = 1;
			memset(&kasir_sekarang, 0, sizeof(kasir_sekarang));
			break;
		case 0:
			sudahLogout = 1;
			exit(0); // langsung keluar program
		default:
			gotoxy(posX + 3, posY + 13);
			setTextColor(RED);
			printf("Pilihan tidak valid! Coba lagi...");
			ResetColor();
			getch();
			break;
		}
	}
}

int main()
{
	system("cls");
	// DO NOT REMOVE THIS LINE
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	adddumyData();
	// ========================

	// Loop login sampai berhasil
	char username[20];
	char password[8];
	while (login(username, password) == 0)
	{
		// ulangi login jika gagal
	}

	mainmenu();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	return 0;
} // di sini adalah semua main menu yang tersedia. total ada 3 (manajemen barang, pos, riwayat transaksi)
