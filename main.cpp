#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>

#pragma disable(warning : 4995)

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

Lagu playlist__cari_lagu(Playlist *playlist, string judul) {
  for (int i = 0; i < playlist->jumlah; i++) {
    if (playlist->list[i].judul == judul) {
      return playlist->list[i];
    }
  }

  return NULL_LAGU;
}

int main() {
  Lagu lagu1 = {
      .judul = "lagu 1",
      .penyanyi = "penyanyi 1",
      .genre = "genre 1",
      .tahun = 1987,
  };

  FILE *lagu_store = fopen("./lagu_store.dat", "wb");
  if (lagu_store == NULL) {
    cout << "there is a problem with writing to a file" << endl;
    return 1;
  }

  fwrite(&lagu1, sizeof(Lagu), 1, lagu_store);
  fclose(lagu_store);

  Lagu lagu2;
  FILE *lagu_read = fopen("./lagu_store.dat", "rb");
  if (lagu_read == NULL) {
    cout << "there's a problem with reading to a file" << endl;
    return 1;
  }

  fread(&lagu2, sizeof(Lagu), 1, lagu_read);
  fclose(lagu_read);

  assert(!lagu__bernilai_null(lagu2));
  assert(strcmp(lagu2.judul, "lagu 1") == 0);
}
