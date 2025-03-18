#include <cassert>
#include <iostream>
#include <string>

#pragma disable(warning : 4995)

using namespace std;

typedef struct {
  string judul;
  string penyanyi;
  string genre;
  int tahun;
} Lagu;

// Playlist ditaruh setelah struct untuk menghilangkan warning yang muncul
// karena memberikan default initializer kepada jumlah
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
}

int main() {
  Playlist playlist;
  Lagu lagu = {.judul = "lagu 1",
               .penyanyi = "penyanyi 1",
               .genre = "bebop",
               .tahun = 197};

  playlist__tambah_lagu(&playlist, lagu);
  playlist__tambah_lagu(&playlist, {.judul = "lagu 2"});

  assert(playlist__cari_lagu(&playlist, "lagu 2").judul == "lagu 2");
}
