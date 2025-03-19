#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
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

Lagu playlist__cari_lagu(Playlist *playlist, string judul) {
  for (int i = 0; i < playlist->jumlah; i++) {
    if (playlist->list[i].judul == judul) {
      return playlist->list[i];
    }
  }

  return NULL_LAGU;
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

  fseek(store, playlist->jumlah * sizeof(Lagu), SEEK_SET);
  fwrite(&playlist->jumlah, sizeof(int), 1, store);

  fclose(store);
}

int main() {
  Playlist playlist;

  /*playlist__tambah_lagu(&playlist, {*/
  /*                                     .judul = "lagu 1",*/
  /*                                 });*/
  /*playlist__tambah_lagu(&playlist, {*/
  /*                                     .judul = "lagu 2",*/
  /*                                 });*/
  /*playlist__tambah_lagu(&playlist, {*/
  /*                                     .judul = "lagu 3",*/
  /*                                 });*/
  /**/
  /*store__save_song_to_file("./lagu_store.dat", &playlist);*/

  store__load_song_from_file("./lagu_store.dat", &playlist);

  assert(strcmp(playlist.list[0].judul, "lagu 1") == 0);
  assert(strcmp(playlist.list[1].judul, "lagu 2") == 0);
  assert(strcmp(playlist.list[2].judul, "lagu 3") == 0);
  assert(playlist.jumlah == 3);

  store__delete_data("./lagu_store.dat", &playlist);
  store__load_song_from_file("./lagu_store.dat", &playlist);

  assert(playlist.jumlah != 3);
}
