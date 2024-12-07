#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>
using namespace std;

// Struktur data untuk produk
struct Produk {
    int id;
    string nama;
    string kategori;
    double harga;

    // Fungsi untuk menampilkan produk
    void display() const {
        cout << "("<< id << ") " << nama << " - Rp " << harga << endl;
    }
};

struct BarangKeranjang {
    Produk produk;
    int jumlah;
};

// Fungsi untuk mencetak produk dalam rentang indeks
void tampilkanProduk(const Produk produk[], int start, int end) {
    for (int i = start; i <= end; ++i) {
        produk[i].display();
    }
}

void tampilkanProdukTertinggi(const Produk produk[], int start, int end) {
    for (int i = end; i >= start; i--) {
        produk[i].display();
    }
}

// Fungsi untuk menyortir produk Bubble Sort
void sortProduk(Produk produk[], int start, int end) {
    for (int i = start; i <= end; ++i) { 
        for (int j = start; j < end - (i - start); ++j) {
            if (produk[j].harga > produk[j + 1].harga) { 
                Produk temp = produk[j];
                produk[j] = produk[j + 1];
                produk[j + 1] = temp;
            }
        }
    }
}

void tambahKeKeranjang(BarangKeranjang keranjang[], int& jumlahKeranjang, const Produk produk[], int id, int jumlah, int& maxKeranjang) {
    // Cari produk berdasarkan ID
    for (int i = 0; i < maxKeranjang; ++i) {
        if (keranjang[i].produk.id == id) {
            keranjang[i].jumlah += jumlah;  // Update jumlah barang
            cout << jumlah << " produk " << keranjang[i].produk.nama << " berhasil ditambahkan ke keranjang." << endl;
            return;
        }
    }
    
    // Jika produk belum ada di keranjang, tambahkan ke dalam keranjang
    if (jumlahKeranjang < maxKeranjang) {
        keranjang[jumlahKeranjang].produk = produk[id - 1];  // ID produk dimulai dari 1, array dimulai dari 0
        keranjang[jumlahKeranjang].jumlah = jumlah;
        ++jumlahKeranjang;
        cout << jumlah << " produk " << produk[id - 1].nama << " berhasil ditambahkan ke keranjang." << endl;
    } else {
        cout << "Keranjang penuh! Tidak bisa menambahkan lebih banyak barang." << endl;
    }
}

void lihatKeranjang(const BarangKeranjang keranjang[], int jumlahKeranjang) {
    if (jumlahKeranjang == 0) {
        cout << "Keranjang belanja Anda kosong!" << endl;
        return;
    }

    cout << "Daftar Barang di Keranjang Belanja:" << endl;
    double totalHarga = 0;
    for (int i = 0; i < jumlahKeranjang; ++i) {
        double hargaTotal = keranjang[i].produk.harga * keranjang[i].jumlah;
        totalHarga += hargaTotal;
        cout << keranjang[i].produk.nama << ", Jumlah: " << keranjang[i].jumlah 
             << ", Harga : Rp " << keranjang[i].produk.harga 
             << ", Total harga : Rp " << hargaTotal << endl;
    }
    cout << "Total harga semua barang: Rp " << totalHarga << endl;
}

void cariProdukBerdasarkanID(const unordered_map<int, Produk>& produkMap, int id) {
    auto it = produkMap.find(id);  // Mencari produk berdasarkan ID
    if (it != produkMap.end()) {
        it->second.display();  // Jika ditemukan, tampilkan produk
    } else {
        cout << "Produk dengan ID " << id << " tidak ditemukan!" << endl;
    }
}


int main() {
    // Array produk
    Produk produk[] = {
        {1, "Smartphone Samsung Galaxy S23", "Elektronik", 12000000},
        {2, "Laptop ASUS ROG Zephyrus G14", "Elektronik", 25000000},
        {3, "TV LED LG 43 Inch", "Elektronik", 6500000},
        {4, "Earbuds Apple AirPods Pro", "Elektronik", 3500000},
        {5, "Kamera DSLR Canon EOS 90D", "Elektronik", 17000000},
        {6, "Sepeda MTB Polygon", "Fitness", 5000000},
        {7, "Dumbbell 5kg", "Fitness", 200000},
        {8, "Matras Yoga", "Fitness", 250000},
        {9, "Mesin Elliptical", "Fitness", 7500000},
        {10, "Resistance Band", "Fitness", 100000},
        {11, "Serum Wajah Vitamin C", "Kecantikan", 150000},
        {12, "Masker Wajah Aloe Vera", "Kecantikan", 75000},
        {13, "Lipstik Matte L'Oréal", "Kecantikan", 120000},
        {14, "Parfum Chanel", "Kecantikan", 2500000},
        {15, "Sabun Cuci Muka Himalaya", "Kecantikan", 35000},
        {16, "Roti Tawar Serba Roti", "Konsumsi", 15000},
        {17, "Kopi Arabica 100g", "Konsumsi", 50000},
        {18, "Mie Instan", "Konsumsi", 5000},
        {19, "Susu UHT Indomilk 1 Liter", "Konsumsi", 18000},
        {20, "Teh Kotak Sosro 500ml", "Konsumsi", 7500}
    };

    // Ukuran array
    int size = sizeof(produk) / sizeof(produk[0]);

    // Array untuk menyimpan barang dalam keranjang
    int maxKeranjang = 100;  // Ukuran maksimal keranjang
    BarangKeranjang keranjang[maxKeranjang];
    int jumlahKeranjang = 0;  // Jumlah barang dalam keranjang

    // Hash map untuk kategori
    unordered_map<string, pair<int, int>> categoryMap;
    categoryMap["elektronik"] = {0, 4};
    categoryMap["fitness"] = {5, 9};
    categoryMap["kecantikan"] = {10, 14};
    categoryMap["konsumsi"] = {15, 19};

    unordered_map<int, Produk> produkMap;
    for (int i = 0; i < size; ++i) {
        produkMap[produk[i].id] = produk[i];  // Menambahkan produk ke dalam map
    }

    while (true) {
        int pilihan;
        system("cls");
        cout << "#@#@#@#@#@#@#@# Selamat Datang #@#@#@#@#@#@#@#\n";
        cout << "1. Sortir Produk berdasarkan Kategori dan Harga\n";
        cout << "2. Tambahkan Produk ke Keranjang Belanja\n";
        cout << "3. Cari Produk berdasarkan ID\n";
        cout << "4. Lihat Daftar Barang di Keranjang\n";
        cout << "5. Hapus Barang dari Keranjang\n";
        cout << "6. Biaya Pengiriman berdasarkan Jarak\n";
        cout << "7. Pembayaran\n";
        cout << "8. Riwayat Pembayaran\n";
        cout << "9. Detail Transaksi Riwayat Pembayaran\n";
        cout << "10. Keluar\n";
        cout << "Silahkan Pilih Opsi: ";

        cin >> pilihan;
        cin.ignore();  

        if (pilihan == 1) {
            bool kembaliKeMenuUtama = false;
            while (!kembaliKeMenuUtama){
                int opsi;
                system("cls");
                cout << "==== Sortir Produk ====\n";
                cout << "1. Berdasarkan Kategori\n";
                cout << "2. Berdasarkan Harga\n";
                cout << "3. Kembali ke Menu Utama\n";
                cout << "Silahkan Pilih Opsi: ";
                cin >> opsi;
                cin.ignore();

                if (opsi == 1) {
                    string kategori;
                    system("cls");  
                    cout << "Masukkan kategori (Elektronik, Fitness, Kecantikan, Konsumsi): ";
                    getline(cin, kategori);

                    transform(kategori.begin(), kategori.end(), kategori.begin(), ::tolower);

                    if (categoryMap.find(kategori) != categoryMap.end()) {
                        auto range = categoryMap[kategori];
                        tampilkanProduk(produk, range.first, range.second);
                    } else {
                        cout << "Kategori tidak ditemukan!" << endl;
                    }
                    cout << "\n[Tekan Enter untuk kembali]";
                    cin.get();
                } else if (opsi == 2) {
                    int urutan;
                    system("cls");  
                    cout << "1. Terendah ke Tertinggi (Ascending)\n";
                    cout << "2. Tertinggi ke Terendah (Descending)\n";
                    cout << "Pilih Urutan: ";
                    cin>>urutan;

                    sortProduk(produk, 0, size - 1);

                    if (urutan == 1){
                        cout << "\nProduk sudah disortir berdasarkan harga Terendah ke Tertinggi: \n";
                        tampilkanProduk(produk, 0, size - 1);
                        getchar();
                    }
                    else if (urutan == 2){
                        cout << "\nProduk sudah disortir berdasarkan harga Tertinggi ke Terendah: \n";
                        tampilkanProdukTertinggi(produk, 0, size - 1);
                        getchar();
                    }
                    else {
                        cout << "Pilihan tidak Terdaftar\n";
                    } 
                    cout << "\n[Tekan Enter untuk kembali]";
                    cin.get();
                } else if (opsi == 3) {
                    kembaliKeMenuUtama = true;
                }
            }
        } 
        
        else if (pilihan == 2) {
           int id, jumlah;
            cout << "Masukkan ID produk yang ingin ditambahkan ke keranjang: ";
            cin >> id;
            cout << "Masukkan jumlah produk: ";
            cin >> jumlah;
            tambahKeKeranjang(keranjang, jumlahKeranjang, produk, id, jumlah, maxKeranjang);
            cin.ignore();
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.get();
        } 
        
        else if (pilihan == 3) {
            system("cls");
            int id;
            cout << "Masukkan ID produk yang ingin dicari: ";
            cin >> id;
            cariProdukBerdasarkanID(produkMap, id);  // Pencarian produk berdasarkan ID menggunakan hash
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.get();
            getchar();
        }
        
        else if (pilihan == 4) {
            lihatKeranjang(keranjang, jumlahKeranjang);
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            getchar(); 
        } 
        
        else if (pilihan == 5) {
            // Fungsionalitas untuk menghapus barang dari keranjang
        }
        
        else if (pilihan == 10) {
            cout << "Terima kasih telah menggunakan aplikasi ini!\n";
            break;
        } 
        
        else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }

    return 0;
}
