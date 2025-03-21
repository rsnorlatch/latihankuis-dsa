#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <string>

#pragma disable(warning : 4995)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

using namespace std;

typedef struct {
  char judul[100];
  char penyanyi[100];
  char genre[100];
  int tahun;
} Lagu;

#define NULL_LAGU {"", "", "", 0}

bool lagu__bernilai_null(Lagu lagu) {
  return strcmp(lagu.judul, "") && strcmp(lagu.penyanyi, "") &&
         strcmp(lagu.genre, "") && lagu.tahun == 0;
}

typedef struct Playlist {
  Lagu list[100];
  int jumlah = 0;
} Playlist;

void playlist__tambah_lagu(Playlist *playlist, Lagu lagu) {
  playlist->list[playlist->jumlah] = lagu;
  playlist->jumlah++;
}

void playlist__tampilkan_lagu(Playlist *playlist) {
  for (int i = 0; i < playlist->jumlah; i++) {
    cout << "> " << playlist->list[i].judul << endl;
  }
}

Lagu *playlist__cari_lagu(Playlist *playlist, string judul) {
  for (int i = 0; i < playlist->jumlah; i++) {
    if (playlist->list[i].judul == judul) {
      return &playlist->list[i];
    }
  }

  return nullptr;
}

// menggunakkan algoritma shellsort
void playlist__sort_lagu(Playlist *playlist) {
  for (int gap = playlist->jumlah / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < playlist->jumlah; i++) {
      Lagu temp = playlist->list[i];

      int j;
      for (j = i; j >= gap && playlist->list[j - gap].judul > temp.judul;
           j -= gap) {
        playlist->list[j] = playlist->list[j - gap];
      }

      playlist->list[j] = temp;
    }
  }
}

void store__save_song_to_file(const char *store_path, Playlist *playlist) {
  int size = sizeof(playlist->list) / sizeof(playlist->list[0]);

  FILE *store = fopen(store_path, "wb");

  if (store == NULL) {
    perror("there's a problem with writing to a file");
    return;
  }

  fwrite(playlist->list, sizeof(Lagu), size, store);
  fwrite(&playlist->jumlah, sizeof(int), 1, store);

  fclose(store);
}

void store__load_song_from_file(const char *store_path, Playlist *playlist) {
  int size = sizeof(playlist->list) / sizeof(playlist->list[0]);

  FILE *store = fopen(store_path, "rb");

  if (store == NULL) {
    perror("there's a problem with writing to a file");
    return;
  }

  fread(playlist->list, sizeof(Lagu), size, store);
  fread(&playlist->jumlah, sizeof(int), 1, store);

  fclose(store);
}

void store__delete_data(const char *store_path, Playlist *playlist) {
  FILE *store = fopen(store_path, "wb");
  if (store == NULL) {
    perror("there's a problem with deleting file data");
    return;
  }

  playlist->jumlah = 0;

  // geser ke nilai jumlah
  fseek(store, playlist->jumlah * sizeof(Lagu), SEEK_SET);
  fwrite(&playlist->jumlah, sizeof(int), 1, store);

  fclose(store);
}

void page__main(Playlist *playlist);

void page__tambah_lagu(Playlist *playlist) {
  int jumlah_input;
  Lagu lagu_input;
  string error;

  do {
    system("clear");
    cout << error;
    cout << "Masukkan jumlah lagu yang ingin dimasukkan: ";
    cin >> jumlah_input;

    if (jumlah_input == 0) {
      error = "\n tolong masukkan jumlah lagu > 1!";
      continue;
    }

    error = "";

    cout << endl;

    for (int i = 0; i < jumlah_input; i++) {
      cout << "----" << endl;
      cout << "Judul: ";
      cin.ignore();
      cin.getline(lagu_input.judul, 100);
      cout << "Penyanyi: ";
      cin.getline(lagu_input.penyanyi, 100);
      cout << "Genre: ";
      cin.getline(lagu_input.genre, 100);
      cout << "Tahun: ";
      cin >> lagu_input.tahun;
      cout << endl;

      playlist__tambah_lagu(playlist, lagu_input);
    }
  } while (jumlah_input == 0 && lagu__bernilai_null(lagu_input));

  store__save_song_to_file("./lagu_store.dat", playlist);

  cout << "berhasil ditulis ke file!" << endl;
  cout << "tekan tombol sembarang untuk melanjutkan ke halaman utama" << endl;

  getchar();
  page__main(playlist);
}

void page__edit_lagu(Playlist *playlist) {
  string query_input;
  Lagu lagu_input;
  string error;

  do {
    system("clear");
    cout << error;
    cout << "Masukkan judul lagu yang ingin diedit: ";
    cin.ignore();
    getline(cin, query_input);

    if (query_input == "") {
      error = "\nTolong masukkan query!!\n";
      continue;
    }

    error = "";

    Lagu *hasil_pencarian = playlist__cari_lagu(playlist, query_input);
    if (!hasil_pencarian) {
      error = "\nLagu dengan judul " + query_input + " tidak ditemukkan!\n";
      continue;
    }

    cout << "Lagu ditemukkan!" << endl;
    cout << "edit lagu: " << endl;

    cout << "Judul: " << hasil_pencarian->judul << " -> ";
    cin.getline(lagu_input.judul, 100);

    cout << "Penyanyi: " << hasil_pencarian->penyanyi << " -> ";
    cin.getline(lagu_input.penyanyi, 100);

    cout << "Genre: " << hasil_pencarian->genre << " -> ";
    cin.getline(lagu_input.genre, 100);

    cout << "Tahun: " << hasil_pencarian->tahun << " -> ";
    cin >> lagu_input.tahun;

    strcpy(hasil_pencarian->judul, lagu_input.judul);
    strcpy(hasil_pencarian->penyanyi, lagu_input.penyanyi);
    strcpy(hasil_pencarian->genre, lagu_input.genre);
    hasil_pencarian->tahun = lagu_input.tahun;
  } while (query_input == "" && lagu__bernilai_null(lagu_input));

  store__save_song_to_file("./lagu_store.dat", playlist);

  cout << "berhasil ditulis ke file!" << endl;
  cout << "Tekan tombol sembarang untuk kembali ke halaman awal!" << endl;

  getchar();
  page__main(playlist);
}

void page__tampilkan_lagu(Playlist *playlist) {
  system("clear");

  for (int i = 0; i < playlist->jumlah; i++) {
    cout << "---" << endl;
    cout << "Judul: " << playlist->list[i].judul << endl;
    cout << "Penyanyi: " << playlist->list[i].penyanyi << endl;
    cout << "Genre: " << playlist->list[i].genre << endl;
    cout << "Tahun: " << playlist->list[i].tahun << endl;
    cout << endl;
  }

  cout << "Tekan tombol sembarang untuk kembali ke halaman awal!" << endl;
  getchar();
  page__main(playlist);
}

void page__cari_lagu(Playlist *playlist) {
  string query_input;
  string error;

  do {
    system("clear");

    cout << error;
    cout << "Masukkan judul lagu yang ingin anda cari: ";
    cin.ignore();
    getline(cin, query_input);

    Lagu *hasil_pencarian = playlist__cari_lagu(playlist, query_input);
    if (!hasil_pencarian) {
      error = "\nLagu dengan judul " + query_input + " tidak ditemukkan!\n";
      continue;
    }

    cout << "Lagu ditemukkan!" << endl;

    cout << "Judul: " << hasil_pencarian->judul << endl;
    cout << "Penyanyi: " << hasil_pencarian->penyanyi << endl;
    cout << "Genre: " << hasil_pencarian->genre << endl;
    cout << "Tahun: " << hasil_pencarian->tahun << endl;
  } while (query_input == "");

  cout << "Tekan tombol sembarang untuk kembali ke halaman awal!" << endl;
  getchar();
  page__main(playlist);
}

void page__main(Playlist *playlist) {
  int pilihan_input;
  string error;

  do {
    system("clear");
    cout << "Playlist Lagu" << endl;
    cout << setw(13) << setfill('=') << "" << endl;
    cout << "1. Tambah Lagu" << endl;
    cout << "2. Edit Lagu" << endl;
    cout << "3. Tampilkan Lagu" << endl;
    cout << "4. Cari Lagu" << endl;
    cout << "5. Keluar" << endl;

    cout << error;

    cout << "Pilih Menu: ";
    cin >> pilihan_input;

    if (pilihan_input > 5 || pilihan_input < 0) {
      error = "\nPilihan menu tidak valid!\n";
      pilihan_input = 0;
    } else if (pilihan_input == 0) {
      error = "\nMasukkan pilihan menu!\n";
      pilihan_input = 0;
    }
  } while (pilihan_input == 0);

  error = "";

  switch (pilihan_input) {
  case 1:
    page__tambah_lagu(playlist);
    break;
  case 2:
    page__edit_lagu(playlist);
    break;
  case 3:
    page__tampilkan_lagu(playlist);
    break;
  case 4:
    page__cari_lagu(playlist);
    break;
  case 5:
    cout << "Terima kasih telah menggunakkan layanan kami! :)" << endl;
    exit(0);
    break;
  default:
    // unreachable
    break;
  }
}

int main() {
  Playlist playlist;

  store__load_song_from_file("./lagu_store.dat", &playlist);

  page__main(&playlist);
}
