#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

const int MAX_TASK = 100;

// Struct utama untuk menyimpan data task
struct Task {
    int id;
    string nama;
    int prioritas;      // 1=Low, 2=Medium, 3=High
    bool selesai;       // true jika task selesai
    float deadlineHari; // deadline dalam hitungan hari
};

// Struct stack dinamis sederhana (menggunakan top index)
struct StackUndo {
    Task data[MAX_TASK];
    int top;
};

// Inisialisasi stack undo
void initStack(StackUndo &s) {
    s.top = -1;
}

// Cek stack kosong
bool isStackKosong(const StackUndo &s) {
    return s.top == -1;
}

// Cek stack penuh
bool isStackPenuh(const StackUndo &s) {
    return s.top == MAX_TASK - 1;
}

// Push task ke stack undo
void pushStack(StackUndo &s, const Task &item) {
    if (!isStackPenuh(s)) {
        s.top++;
        s.data[s.top] = item;
    }
}

// Pop task dari stack undo
bool popStack(StackUndo &s, Task &item) {
    if (isStackKosong(s)) {
        return false;
    }
    item = s.data[s.top];
    s.top--;
    return true;
}

// Membersihkan input jika user salah memasukkan data
void bersihkanInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Membuat ID task baru secara otomatis
int generateIdBaru(const Task daftar[], int jumlahTask) {
    int maxId = 0;
    for (int i = 0; i < jumlahTask; i++) {
        if (daftar[i].id > maxId) {
            maxId = daftar[i].id;
        }
    }
    return maxId + 1;
}

// Mencari index task berdasarkan ID
int cariIndexById(const Task daftar[], int jumlahTask, int id) {
    for (int i = 0; i < jumlahTask; i++) {
        if (daftar[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Mengubah angka prioritas menjadi teks
string teksPrioritas(int p) {
    if (p == 3) return "High";
    if (p == 2) return "Medium";
    return "Low";
}

// Menampilkan garis tabel
void garisTabel() {
    cout << "+-----+------------------------------+----------+-----------+----------+\n";
}

// Menampilkan header tabel task
void headerTabel() {
    garisTabel();
    cout << "| ID  | Nama Task                    | Prioritas| Deadline  | Status   |\n";
    garisTabel();
}

// Menampilkan 1 data task dalam format tabel
void tampilkanSatuTask(const Task &t) {
    cout << "| " << setw(3) << t.id << " "
         << "| " << setw(28) << left << t.nama << right << " "
         << "| " << setw(8) << teksPrioritas(t.prioritas) << " "
         << "| " << setw(8) << fixed << setprecision(1) << t.deadlineHari << " "
         << "| " << setw(8) << (t.selesai ? "Selesai" : "Belum") << " |\n";
}

// Fungsi menambah task baru ke array
void tambahTask(Task daftar[], int &jumlahTask) {
    if (jumlahTask >= MAX_TASK) {
        cout << "\n[!] Data task sudah penuh (maks 100).\n";
        return;
    }

    Task baru;
    baru.id = generateIdBaru(daftar, jumlahTask);
    baru.selesai = false;

    cout << "\n=== Tambah Task ===\n";
    cout << "Nama task: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, baru.nama);

    while (baru.nama.empty()) {
        cout << "[!] Nama task tidak boleh kosong. Masukkan lagi: ";
        getline(cin, baru.nama);
    }

    cout << "Prioritas (1=Low, 2=Medium, 3=High): ";
    while (!(cin >> baru.prioritas) || baru.prioritas < 1 || baru.prioritas > 3) {
        cout << "[!] Input salah. Masukkan 1, 2, atau 3: ";
        bersihkanInput();
    }

    cout << "Deadline (dalam hari, contoh 2.5): ";
    while (!(cin >> baru.deadlineHari) || baru.deadlineHari < 0) {
        cout << "[!] Input salah. Masukkan angka >= 0: ";
        bersihkanInput();
    }

    daftar[jumlahTask] = baru;
    jumlahTask++;

    cout << "[+] Task berhasil ditambahkan dengan ID " << baru.id << ".\n";
}

// Fungsi menampilkan semua task
void tampilkanTask(const Task daftar[], int jumlahTask) {
    cout << "\n=== Daftar Task ===\n";
    if (jumlahTask == 0) {
        cout << "Belum ada task.\n";
        return;
    }

    headerTabel();
    for (int i = 0; i < jumlahTask; i++) {
        tampilkanSatuTask(daftar[i]);
    }
    garisTabel();
}

// Fungsi menandai task menjadi selesai
void tandaiSelesai(Task daftar[], int jumlahTask) {
    if (jumlahTask == 0) {
        cout << "\n[!] Belum ada task.\n";
        return;
    }

    int id;
    cout << "\nMasukkan ID task yang ingin ditandai selesai: ";
    while (!(cin >> id)) {
        cout << "[!] ID harus angka. Masukkan lagi: ";
        bersihkanInput();
    }

    int idx = cariIndexById(daftar, jumlahTask, id);
    if (idx == -1) {
        cout << "[!] Task dengan ID " << id << " tidak ditemukan.\n";
        return;
    }

    if (daftar[idx].selesai) {
        cout << "[i] Task ini sudah selesai sebelumnya.\n";
    } else {
        daftar[idx].selesai = true;
        cout << "[+] Task ID " << id << " berhasil ditandai selesai.\n";
    }
}

// Fungsi menghapus task dari array dan simpan ke stack undo
void hapusTask(Task daftar[], int &jumlahTask, StackUndo &undoStack) {
    if (jumlahTask == 0) {
        cout << "\n[!] Belum ada task untuk dihapus.\n";
        return;
    }

    int id;
    cout << "\nMasukkan ID task yang ingin dihapus: ";
    while (!(cin >> id)) {
        cout << "[!] ID harus angka. Masukkan lagi: ";
        bersihkanInput();
    }

    int idx = cariIndexById(daftar, jumlahTask, id);
    if (idx == -1) {
        cout << "[!] Task dengan ID " << id << " tidak ditemukan.\n";
        return;
    }

    // Simpan ke stack undo sebelum dihapus
    pushStack(undoStack, daftar[idx]);

    // Geser data array ke kiri
    for (int i = idx; i < jumlahTask - 1; i++) {
        daftar[i] = daftar[i + 1];
    }
    jumlahTask--;

    cout << "[-] Task ID " << id << " berhasil dihapus.\n";
}

// Fungsi mencari task berdasarkan kata kunci nama
void cariTask(const Task daftar[], int jumlahTask) {
    if (jumlahTask == 0) {
        cout << "\n[!] Belum ada task untuk dicari.\n";
        return;
    }

    string kunci;
    cout << "\nMasukkan kata kunci: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, kunci);

    if (kunci.empty()) {
        cout << "[!] Kata kunci tidak boleh kosong.\n";
        return;
    }

    bool ditemukan = false;
    cout << "\n=== Hasil Pencarian ===\n";
    headerTabel();
    for (int i = 0; i < jumlahTask; i++) {
        if (daftar[i].nama.find(kunci) != string::npos) {
            tampilkanSatuTask(daftar[i]);
            ditemukan = true;
        }
    }
    garisTabel();

    if (!ditemukan) {
        cout << "[i] Tidak ada task yang cocok dengan kata kunci \"" << kunci << "\".\n";
    }
}

// Fungsi rekomendasi aktivitas sehat berdasarkan waktu (simulasi input user)
void rekomendasiAktivitas() {
    int jam;
    cout << "\nMasukkan jam saat ini (0-23): ";
    while (!(cin >> jam) || jam < 0 || jam > 23) {
        cout << "[!] Input tidak valid. Masukkan jam 0-23: ";
        bersihkanInput();
    }

    cout << "\n=== Rekomendasi Aktivitas Harian Sehat ===\n";
    if (jam >= 5 && jam <= 10) {
        cout << "- Pagi: olahraga ringan 15-30 menit.\n";
        cout << "- Jangan lupa sarapan sehat.\n";
        cout << "- Buat rencana belajar hari ini.\n";
    } else if (jam >= 11 && jam <= 15) {
        cout << "- Siang: fokus belajar/tugas sekolah.\n";
        cout << "- Minum air yang cukup.\n";
        cout << "- Istirahat singkat setiap 1 jam.\n";
    } else if (jam >= 16 && jam <= 18) {
        cout << "- Sore: jalan santai atau peregangan.\n";
        cout << "- Rapikan meja belajar.\n";
        cout << "- Lanjutkan tugas yang belum selesai.\n";
    } else if (jam >= 19 && jam <= 22) {
        cout << "- Malam: review pelajaran hari ini.\n";
        cout << "- Kurangi screen time sebelum tidur.\n";
        cout << "- Tidur cukup agar badan tetap fit.\n";
    } else {
        cout << "- Waktu istirahat/tidur.\n";
        cout << "- Jaga pola tidur agar besok lebih produktif.\n";
    }
}

// Fungsi menukar dua data task (dipakai untuk sorting)
void tukarTask(Task &a, Task &b) {
    Task temp = a;
    a = b;
    b = temp;
}

// Fungsi sorting task berdasarkan prioritas atau deadline
void sortingTask(Task daftar[], int jumlahTask) {
    if (jumlahTask <= 1) {
        cout << "\n[i] Data task belum cukup untuk sorting.\n";
        return;
    }

    int mode;
    cout << "\n=== Sorting Task ===\n";
    cout << "1. Berdasarkan prioritas (High ke Low)\n";
    cout << "2. Berdasarkan deadline (kecil ke besar)\n";
    cout << "Pilih mode sorting: ";
    while (!(cin >> mode) || (mode != 1 && mode != 2)) {
        cout << "[!] Pilihan hanya 1 atau 2: ";
        bersihkanInput();
    }

    // Bubble sort agar mudah dipahami siswa kelas X
    for (int i = 0; i < jumlahTask - 1; i++) {
        for (int j = 0; j < jumlahTask - i - 1; j++) {
            bool tukar = false;

            if (mode == 1) {
                if (daftar[j].prioritas < daftar[j + 1].prioritas) {
                    tukar = true;
                }
            } else {
                if (daftar[j].deadlineHari > daftar[j + 1].deadlineHari) {
                    tukar = true;
                }
            }

            if (tukar) {
                tukarTask(daftar[j], daftar[j + 1]);
            }
        }
    }

    cout << "[+] Sorting task berhasil.\n";
}

// Fungsi statistik jumlah task selesai dan belum selesai
void statistikTask(const Task daftar[], int jumlahTask) {
    int selesai = 0;
    int belum = 0;

    for (int i = 0; i < jumlahTask; i++) {
        if (daftar[i].selesai) {
            selesai++;
        } else {
            belum++;
        }
    }

    cout << "\n=== Statistik Task ===\n";
    cout << "Total task   : " << jumlahTask << '\n';
    cout << "Task selesai : " << selesai << '\n';
    cout << "Belum selesai: " << belum << '\n';
}

// Fungsi undo task yang terakhir dihapus
void undoHapusTask(Task daftar[], int &jumlahTask, StackUndo &undoStack) {
    if (isStackKosong(undoStack)) {
        cout << "\n[i] Riwayat hapus kosong. Tidak bisa undo.\n";
        return;
    }

    if (jumlahTask >= MAX_TASK) {
        cout << "\n[!] Array penuh. Undo gagal.\n";
        return;
    }

    Task item;
    if (popStack(undoStack, item)) {
        daftar[jumlahTask] = item;
        jumlahTask++;
        cout << "[+] Undo berhasil. Task ID " << item.id << " dikembalikan.\n";
    }
}

// Fungsi menu utama
void menu() {
    cout << "\n=============================================\n";
    cout << " ADVANCED TO-DO LIST + AKTIVITAS SEHAT (CLI)\n";
    cout << "=============================================\n";
    cout << "1. Tambah Task\n";
    cout << "2. Tampilkan Semua Task\n";
    cout << "3. Tandai Task Selesai\n";
    cout << "4. Hapus Task\n";
    cout << "5. Cari Task\n";
    cout << "6. Rekomendasi Aktivitas Harian Sehat\n";
    cout << "7. Sorting Task\n";
    cout << "8. Statistik Task\n";
    cout << "9. Undo Hapus Task (Stack)\n";
    cout << "0. Keluar\n";
    cout << "Pilih menu: ";
}

int main() {
    Task daftarTask[MAX_TASK];
    int jumlahTask = 0;
    int pilihan;
    StackUndo undoStack;

    initStack(undoStack);

    // Perulangan utama program (do-while) untuk CLI interaktif
    do {
        menu();
        while (!(cin >> pilihan)) {
            cout << "[!] Input menu harus angka. Masukkan lagi: ";
            bersihkanInput();
        }

        switch (pilihan) {
            case 1:
                tambahTask(daftarTask, jumlahTask);
                break;
            case 2:
                tampilkanTask(daftarTask, jumlahTask);
                break;
            case 3:
                tandaiSelesai(daftarTask, jumlahTask);
                break;
            case 4:
                hapusTask(daftarTask, jumlahTask, undoStack);
                break;
            case 5:
                cariTask(daftarTask, jumlahTask);
                break;
            case 6:
                rekomendasiAktivitas();
                break;
            case 7:
                sortingTask(daftarTask, jumlahTask);
                break;
            case 8:
                statistikTask(daftarTask, jumlahTask);
                break;
            case 9:
                undoHapusTask(daftarTask, jumlahTask, undoStack);
                break;
            case 0:
                cout << "\nTerima kasih, program selesai.\n";
                break;
            default:
                cout << "\n[!] Menu tidak tersedia.\n";
        }
    } while (pilihan != 0);

    return 0;
}
