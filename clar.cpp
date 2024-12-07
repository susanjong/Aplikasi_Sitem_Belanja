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
        cout << id << ". " << nama << " - Rp " << harga << endl;
    }
};

// Fungsi untuk mencetak produk dalam rentang indeks
void tampilkanProduk(const Produk produk[], int start, int end) {
    for (int i = start; i <= end; ++i) {
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

    // Hash map untuk kategori
    unordered_map<string, pair<int, int>> categoryMap;
    categoryMap["elektronik"] = {0, 4};
    categoryMap["fitness"] = {5, 9};
    categoryMap["kecantikan"] = {10, 14};
    categoryMap["konsumsi"] = {15, 19};

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
                    system("cls");  
                    sortProduk(produk, 0, size - 1);
                    cout << "Produk sudah disortir berdasarkan harga:\n";
                    tampilkanProduk(produk, 0, size - 1);
                    cout << "\n[Tekan Enter untuk kembali]";
                    cin.get();
                } else if (opsi == 3) {
                    kembaliKeMenuUtama = true;
                }
            }
        } 
        
        else if (pilihan == 2) {
            // Fungsionalitas untuk menambahkan produk ke keranjang
        } 
        
        else if (pilihan == 3) {
            // Fungsionalitas untuk mencari produk berdasarkan ID
        }
        
        else if (pilihan == 4) {
            // Fungsionalitas untuk melihat daftar barang di keranjang
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
