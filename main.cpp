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

int main() {
  Playlist playlist;
  Lagu lagu = {.judul = "lagu 1",
               .penyanyi = "penyanyi 1",
               .genre = "bebop",
               .tahun = 197};

  playlist__tambah_lagu(&playlist, lagu);

  assert(playlist.jumlah > 0);
  assert(playlist.list[0].judul == "lagu 1");
}
