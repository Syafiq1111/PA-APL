#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

#define MaxTiket 40

struct Tiket {
    string Pembeli;
    string hari;
    string durasi;
    double harga;
    bool kausKaki;
    double totalHarga;
    string KodeTiket;
    string status;
};

struct User {
    string username;
    string password;
    string role; 
};

User daftarUser[40] = { {"Nadia Rahmah", "018", "admin"} };
int jumlahUser = 1;
Tiket daftarTiket[MaxTiket];
int jumlahTiket = 0;

double hargaTiket[2][3] = {
    {20000, 35000, 45000},  // Harga untuk Weekday
    {25000, 40000, 50000}   // Harga untuk Weekend
};

void registrasi(User *user);
bool login(User *user);
void menuTiketAdmin();
void menuTiketUser(const User& userLogin);
void menuSorting();
void beliTiketAdmin();
void beliTiketUser(const User& userLogin);
void tampilkanTiket();
void ubahTiket();
void hapusTiket();


void bubbleSortHariDesc(Tiket arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j].hari < arr[j+1].hari) {
                // tukar elemen
                Tiket temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void selectionSortHargaAsc(Tiket arr[], int panjang) {
    for (int i = 0; i < panjang - 1; i++) {
        int min = i;
        for (int j = i + 1; j < panjang; j++) {
            if (arr[j].totalHarga < arr[min].totalHarga) {
                min = j;
            }
        }
        // tukar elemen
        Tiket temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}

void insertionSortDurasiAsc(Tiket arr[], int panjang) {
    for (int i = 1; i < panjang; i++) {
        Tiket key = arr[i];  
        int j = i - 1;

        // tukar elemen
        while (j >= 0 && arr[j].durasi > key.durasi) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;  
    }
}

void menuUtama(){
    User userLogin;
    User user;
    while (true){
        try{
            cout << "========================" << endl;
            cout << "|      MENU UTAMA      |" << endl;
            cout << "========================" << endl;
            cout << "1. Registrasi\n";
            cout << "2. Login\n";
            cout << "3. Keluar\n";
            cout << "Pilih Menu (1/2/3): ";
            int pilih;
            cin >> pilih; 
            if (cin.fail()) {
                throw invalid_argument("Input harus berupa angka!");
                }    
            if (pilih == 1){
                registrasi(&user); 
            }
            else if(pilih == 2){
                if (login(&user)){
                    if (user.role == "admin") {
                        menuTiketAdmin();
                    } 
                    else {
                        menuTiketUser(user);
                    }
                }
            }
            else if (pilih == 3) { 
                cout << "Terima Kasih!" << endl;
                break;
            }
            else {
                cout << "Pilihan tidak valid! Silakan pilih lagi." << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}

void registrasi(User *user) { 
    cout << "=== Registrasi ===" << endl;
    cin.ignore();
    while (true) {
        cout << "Masukkan Username: ";
        getline(cin, daftarUser[jumlahUser].username);
        if (daftarUser[jumlahUser].username.length() < 3) {
            cout << "Username minimal 3 huruf!" << endl;
        }
        else {
            break;
        }
    }
    cout << "Masukkan Password: ";
    getline(cin, daftarUser[jumlahUser].password);
    while (daftarUser[jumlahUser].password.empty() || daftarUser[jumlahUser].password.find_first_not_of(' ') == string::npos) {
        cout << "Password tidak boleh kosong atau hanya spasi!" << endl;
        cout << "Masukkan Password: ";
        getline(cin, daftarUser[jumlahUser].password);
    }
    daftarUser[jumlahUser].role = "user";
    *user = daftarUser[jumlahUser];

    cout << "Registrasi berhasil! Silakan lanjut login." << endl; 
    jumlahUser++;   
}

bool login(User *user) {
    string inputUsername, inputPassword;
    int login = 0;
    while (login < 3) {
        cout << "=== Login ===" << endl;
        cout << "Masukkan Username: ";
        cin >> ws;
        getline(cin, inputUsername);
        cout << "Masukkan Password: ";
        cin >> ws;
        getline(cin, inputPassword);
        while (inputPassword.empty() || inputPassword.find_first_not_of(' ') == string::npos) {
            cout << "Password tidak boleh kosong atau hanya spasi! Masukkan ulang: ";
            getline(cin, inputPassword);
        }
        for (int i = 0; i < jumlahUser; i++) {
            if (inputUsername == daftarUser[i].username && inputPassword == daftarUser[i].password) {
                *user = daftarUser[i]; 
                cout << "Login berhasil! Selamat datang " << user->username << "." << endl; 
                return true;
            }
        } 
        login++;
        if (login < 3) {
            cout << "Username atau password salah! Coba lagi" << endl;
        }
    }
    cout << "Anda sudah mencoba 3 Kali. Program berhenti." << endl;
    exit(0);
}

void menuTiketAdmin() {
    while (true) {
        try{
            cout << "=========================" << endl;
            cout << "|    MENU TIKET ADMIN   |" << endl;
            cout << "=========================" << endl;
            cout << "1. Beli Tiket" << endl;
            cout << "2. Tampilkan Tiket" << endl;
            cout << "3. Ubah data Tiket" << endl;
            cout << "4. Hapus Tiket" << endl;
            cout << "5. Menu Sorting tiket" << endl;
            cout << "6. Keluar ke menu utama" << endl;
            cout << "Pilih menu (1/2/3/4/5): ";
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                throw invalid_argument("Input harus berupa angka!");
                }
            switch (pilihan) {
                case 1:
                    beliTiketAdmin();
                    break;
                case 2:
                    tampilkanTiket();
                    break;
                case 3:
                    ubahTiket();
                    break;
                case 4:
                    hapusTiket();
                    break;
                case 5:
                    menuSorting();
                    break;
                case 6:
                    return;
                default:
                    cout << "Pilihan tidak valid" << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}

void menuTiketUser(const User& userLogin) {
    while (true) {
        try{
            cout << "========================" << endl;
            cout << "|    MENU TIKET USER   |" << endl;
            cout << "========================" << endl;
            cout << "1. Beli Tiket" << endl;
            cout << "2. Tampilkan Tiket" << endl;
            cout << "3. Keluar ke menu utama" << endl;
            cout << "Pilih menu (1/2/3): ";
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                throw invalid_argument("Input harus berupa angka!");
                }
            switch (pilihan) {
                case 1:
                    cout << "=======================================================" << endl;
                    cout << "|          DAFTAR HARGA TIKET MINI PLAYGROUND         |" << endl;
                    cout << "=======================================================" << endl;
                    cout << "| Hari       | Paket Durasi       | Harga Tiket       |" << endl;
                    cout << "-------------------------------------------------------" << endl;
                    cout << "| Weekday    | 1 Jam              | Rp20.000          |" << endl;
                    cout << "|            | 2 Jam              | Rp35.000          |" << endl;
                    cout << "|            | Sepuasnya          | Rp45.000          |" << endl;
                    cout << "-------------------------------------------------------" << endl;
                    cout << "| Weekend    | 1 Jam              | Rp25.000          |" << endl;
                    cout << "|            | 2 Jam              | Rp40.000          |" << endl;
                    cout << "|            | Sepuasnya          | Rp50.000          |" << endl;
                    cout << "-------------------------------------------------------" << endl;
                    cout << "| Kaus Kaki (Wajib, jika tidak bawa) | Rp10.000       |" << endl;
                    cout << "=======================================================" << endl;
                    cout << "Catatan:\n";
                    cout << "- Harga tiket berbeda antara weekday dan weekend.\n";
                    cout << "- Pengunjung wajib menggunakan kaus kaki saat bermain.\n" << endl;
                    beliTiketUser(userLogin);
                    break;
                case 2:
                    tampilkanTiket();
                    break;
                case 3:
                    return;
                default:
                    cout << "Pilihan tidak valid" << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}


void menuSorting() {
    while (true) {
        cout << "\n=========================" << endl;
        cout << "|      MENU SORTING     |" << endl;
        cout << "=========================" << endl;
        cout << "1. Sorting berdasarkan Hari (Descending) - Bubble Sort" << endl;
        cout << "2. Sorting Harga (Ascending) - Selection Sort" << endl;
        cout << "3. Sorting berdasarkan Durasi (Ascending) - Insertion Sort" << endl;
        cout << "4. Keluar ke Menu Tiket" << endl;
        cout << "Pilih metode sorting (1-4): ";
        
        int pilihan;
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                bubbleSortHariDesc(daftarTiket, jumlahTiket);
                cout << "\nHasil Sorting Berdasarkan Hari (Weekend -> Weekday)\n";
                tampilkanTiket();
                break;
            case 2:
                selectionSortHargaAsc(daftarTiket, jumlahTiket);
                cout << "\nHasil Sorting Berdasarkan Harga (Termurah -> Termahal)\n";
                tampilkanTiket();
                break;
            case 3:
                insertionSortDurasiAsc(daftarTiket, jumlahTiket);
                cout << "\nHasil Sorting Berdasarkan Durasi (Pendek -> Panjang)   \n";
                tampilkanTiket();
                break;
            case 4:
                return;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
}

void beliTiketAdmin() {
    string NamaPembeli, PasswordPembeli;
    cout << "=== MENU BELI TIKET ===" << endl;  
    if (jumlahTiket < MaxTiket) {
        Tiket tiketBaru;
        int inputHari, inputDurasi;
        char inputKausKaki;
        cin.ignore();
        cout << "Nama pembeli: ";
        getline(cin, NamaPembeli);
        tiketBaru.Pembeli = NamaPembeli;
        cout << "1. Weekday " << endl;
        cout << "2. Weekend " << endl;
        cout << "Masukkan Hari (1/2): ";
        cin >> inputHari;
        tiketBaru.hari = (inputHari == 1) ? "Weekday" : "Weekend";

        cout << "1. 1 Jam " << endl;
        cout << "2. 2 Jam " << endl;
        cout << "3. Sepuasnya " << endl;
        cout << "Masukkan Paket Durasi (1/2/3): ";
        cin >> inputDurasi;

        tiketBaru.durasi = (inputDurasi == 1) ? "1 Jam" : (inputDurasi == 2) ? "2 Jam" : "Sepuasnya";
        tiketBaru.harga = hargaTiket[inputHari - 1][inputDurasi - 1]; 
 
        cout << "Apakah Anda membawa kaus kaki? (Y/y untuk Ya, T/t untuk Tidak): ";
        cin >> inputKausKaki;
        tiketBaru.kausKaki = (inputKausKaki == 'Y' || inputKausKaki == 'y');
        tiketBaru.totalHarga = tiketBaru.harga + (tiketBaru.kausKaki ? 0 : 10000);
        tiketBaru.status = "Aktif";
        daftarTiket[jumlahTiket++] = tiketBaru;
        cout << "Tiket berhasil ditambahkan! Kode Tiket: " << endl;
    } else {
        cout << "Jumlah tiket sudah mencapai batas maksimum!" << endl;
    }
}

void beliTiketUser(const User& userLogin) {
    cout << "=== MENU BELI TIKET ===" << endl;
    if (jumlahTiket < MaxTiket) {
        Tiket tiketBaru;
        int inputHari, inputDurasi;
        char inputKausKaki;
        cout << "1. Weekday " << endl;
        cout << "2. Weekend " << endl;
        cout << "Masukkan Hari (1/2): ";
        cin >> inputHari;
        tiketBaru.hari = (inputHari == 1) ? "Weekday" : "Weekend";

        cout << "1. 1 Jam " << endl;
        cout << "2. 2 Jam " << endl;
        cout << "3. Sepuasnya " << endl;
        cout << "Masukkan Paket Durasi (1/2/3): ";
        cin >> inputDurasi;

        tiketBaru.durasi = (inputDurasi == 1) ? "1 Jam" : (inputDurasi == 2) ? "2 Jam" : "Sepuasnya";
        tiketBaru.harga = hargaTiket[inputHari - 1][inputDurasi - 1]; 

        cout << "Apakah Anda membawa kaus kaki? (Y/y untuk Ya, T/t untuk Tidak): ";
        cin >> inputKausKaki;
        tiketBaru.kausKaki = (inputKausKaki == 'Y' || inputKausKaki == 'y');
        tiketBaru.totalHarga = tiketBaru.harga + (tiketBaru.kausKaki ? 0 : 10000);
        tiketBaru.status = "Aktif";
        tiketBaru.Pembeli = userLogin.username;
        daftarTiket[jumlahTiket++] = tiketBaru;
        cout << "Tiket berhasil ditambahkan!" << endl;
    } else {
        cout << "Jumlah tiket sudah mencapai batas maksimum!" << endl;
    }
}

void tampilkanTiket() {
    cout << "=== MENU TAMPILKAN TIKET ===" << endl;    
    if (jumlahTiket == 0) {
        cout << "Tidak ada tiket tersedia" << endl;
    } else {
        cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < jumlahTiket; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << daftarTiket[i].Pembeli
                 << setw(15) << daftarTiket[i].hari
                 << setw(15) << daftarTiket[i].durasi
                 << setw(10) << daftarTiket[i].harga
                 << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                 << setw(15) << daftarTiket[i].totalHarga
                 << setw(15) << daftarTiket[i].status << endl;
        }
    }
}

int interpolationSearch(string NamaPembeli) {
    int low = 0;
    int high = jumlahTiket - 1;

    while (low <= high && NamaPembeli >= daftarTiket[low].Pembeli && NamaPembeli <= daftarTiket[high].Pembeli) {
        if (low == high) {
            if (daftarTiket[low].Pembeli == NamaPembeli) return low;
            return -1;
        }

        int pos = low + ((double)(high - low) /
                         (daftarTiket[high].Pembeli[0] - daftarTiket[low].Pembeli[0])) *
                        (NamaPembeli[0] - daftarTiket[low].Pembeli[0]);

        if (daftarTiket[pos].Pembeli == NamaPembeli)
            return pos;

        if (daftarTiket[pos].Pembeli < NamaPembeli)
            low = pos + 1;
        else
            high = pos - 1;
    }
    return -1;
}

void ubahTiket() {
    cout << "=== MENU UBAH TIKET ===" << endl;      
    string NamaPembeli;
    for (int i = 0; i < jumlahTiket - 1; i++) {
        for (int j = 0; j < jumlahTiket - i - 1; j++) {
            if (daftarTiket[j].Pembeli > daftarTiket[j + 1].Pembeli) {
                swap(daftarTiket[j], daftarTiket[j + 1]);
            }
        }
    }
    cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < jumlahTiket; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << daftarTiket[i].Pembeli
                 << setw(15) << daftarTiket[i].hari
                 << setw(15) << daftarTiket[i].durasi
                 << setw(10) << daftarTiket[i].harga
                 << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                 << setw(15) << daftarTiket[i].totalHarga
                 << setw(15) << daftarTiket[i].status << endl;
        }
    cout << "Masukkan Nama Pembeli: ";
    cin >> NamaPembeli;
    int index = interpolationSearch(NamaPembeli);
    if (index == -1) {
        cout << "Tiket dengan kode tersebut tidak ditemukan!" << endl;
        return;
    }
    Tiket &tiket = daftarTiket[index];
    cout << "\nData Tiket Ditemukan:" << endl;
    cout << "Nama: " << tiket.Pembeli << endl 
         << "Hari: " << tiket.hari << endl
         << "Durasi: " << tiket.durasi << endl
         << "Harga: " << tiket.harga << endl
         << "Kaus Kaki: " << (tiket.kausKaki ? "Ya" : "Tidak") << endl
         << "Total Harga: " << tiket.totalHarga << endl
         << "Status: " << tiket.status << endl;
    char konfirmasi;
    while (true){
        cout << "\nApakah Anda yakin ingin mengubah tiket ini? (Y/N): ";
        cin >> konfirmasi;
        if (konfirmasi == 'Y' || konfirmasi == 'y') {
            break;
        }
        else if (konfirmasi == 'N' || konfirmasi == 'n'){
            cout << "Perubahan dibatalkan." << endl;
            return;
        }
        else {
            cout << "Input Tidak Valid" << endl;
        }
    }
    int inputHari, inputDurasi;
    char inputKausKaki;
    cout << "1. 1 Jam 20.000" << endl;
    cout << "2. 2 Jam 35.000" << endl;
    cout << "3. Sepuasnya 45.000" << endl;
    try {
        cout << "Masukkan Durasi baru (1/2/3): ";
        cin >> inputDurasi;
        if (cin.fail()) {
            throw invalid_argument("Input harus berupa angka!");
        }
        if (inputDurasi == 1) {
            tiket.durasi = "1 Jam";
            tiket.harga = 20000;
        } else if (inputDurasi == 2) {
            tiket.durasi = "2 Jam";
            tiket.harga = 35000;
        } else {
            tiket.durasi = "Sepuasnya";
            tiket.harga = 45000;
        }
        cout << "Apakah Anda membawa kaus kaki? (Y untuk Ya, T untuk Tidak): ";
        cin >> inputKausKaki;
        tiket.kausKaki = (inputKausKaki == 'Y' || inputKausKaki == 'y');
        tiket.totalHarga = tiket.harga + ((inputHari == 1) ? 5000 : 0) + (tiket.kausKaki ? 0 : 10000);
        cin.ignore();
        while (true){
            cout << "Masukkan status baru tiket (Aktif/Nonaktif): ";
            getline(cin, tiket.status);
            if (tiket.status == "Aktif" || tiket.status == "Nonaktif" || tiket.status == "aktif" || tiket.status == "nonaktif"){
                if (cin.fail()) {
                    throw invalid_argument("Input harus berupa angka!");
                }
                if (tiket.status == "aktif") {
                    tiket.status = "Aktif";
                }
                else if (tiket.status == "nonaktif") {
                    tiket.status = "Nonaktif";
                }
                break;
            }
            else{
                cout << "Status Tidak Valid" << endl;
            }
        }
    }
    catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << endl;
    }
    cout << "Tiket berhasil diubah!" << endl;
}

void hapusTiket() {
    cout << "=== MENU HAPUS TIKET ===" << endl;  
    string NamaPembeli;
    for (int i = 0; i < jumlahTiket - 1; i++) {
        for (int j = 0; j < jumlahTiket - i - 1; j++) {
            if (daftarTiket[j].KodeTiket > daftarTiket[j + 1].KodeTiket) {
                swap(daftarTiket[j], daftarTiket[j + 1]);
            }
        }
    }
    cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < jumlahTiket; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << daftarTiket[i].Pembeli
                 << setw(15) << daftarTiket[i].hari
                 << setw(15) << daftarTiket[i].durasi
                 << setw(10) << daftarTiket[i].harga
                 << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                 << setw(15) << daftarTiket[i].totalHarga
                 << setw(15) << daftarTiket[i].status << endl;
        }
    cout << "Masukkan Nama Pembeli: ";
    cin >> NamaPembeli;
    int index = interpolationSearch(NamaPembeli);
    if (index == -1) {
        cout << "Tiket dengan kode tersebut tidak ditemukan!" << endl;
        return;
    }
    Tiket &tiket = daftarTiket[index];
    cout << "\nData Tiket Ditemukan:" << endl;
    cout << "Nama: " << tiket.Pembeli << endl 
         << "Hari: " << tiket.hari << endl
         << "Durasi: " << tiket.durasi << endl
         << "Harga: " << tiket.harga << endl
         << "Kaus Kaki: " << (tiket.kausKaki ? "Ya" : "Tidak") << endl
         << "Total Harga: " << tiket.totalHarga << endl
         << "Status: " << tiket.status << endl;
    char konfirmasi;
    while (true) {
        cout << "\nApakah Anda yakin ingin menghapus tiket ini? (Y/N): ";
        cin >> konfirmasi;
        if (konfirmasi == 'Y' || konfirmasi == 'y') {
            jumlahTiket--; // Mengurangi jumlah tiket
            cout << "Tiket berhasil dihapus!" << endl;
            return;
        }
        else if (konfirmasi == 'N' || konfirmasi == 'n'){
            cout << "Penghapusan dibatalkan." << endl;
            return;
        }
        else {
            cout << "Input Tidak Valid" << endl;
        }
    }
}

int main() {
    srand(time(0));
    cout << "===============================================================" << endl;
    cout << "| SELAMAT DATANG DI APLIKASI PEMBELIAN TIKET MINI PLAYGROUND  |" << endl;
    cout << "===============================================================" << endl;
    cout << " Silakan registrasi atau login untuk mulai menggunakan.\n";
    menuUtama();
}
