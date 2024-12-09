#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include <ctime>
#include <iomanip>
using namespace std;

//variabel global
int shippingCost = 0;

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

struct RiwayatPembayaran {
    string waktuPembayaran;
    double totalHarga;
    int totalBarang;
    BarangKeranjang detailBarang[100];
    int jumlahBarang;
    string metodePembayaran;
    int shippingCost;
};

RiwayatPembayaran riwayatPembayaran[100];
int jumlahRiwayatPembayaran = 0;

// Node untuk graph perhitungan jarak
struct Node {
    int id;
    vector<pair<int, int>> neighbors;  // pasangan (node_id, jarak)
};

// Fungsi untuk mencari jalur terpendek menggunakan Dijkstra
int findShortestPath(vector<Node>& graph, int source, int destination) {
    vector<int> distance(graph.size(), INT_MAX);  // Menyimpan jarak dari source ke setiap node
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  // Priority queue untuk memproses node berdasarkan jarak terpendek
    distance[source] = 0;  // Jarak dari source ke dirinya sendiri adalah 0
    pq.push({0, source});  // Memasukkan node source ke dalam priority queue
    
    while (!pq.empty()) {
        int curr_distance = pq.top().first;
        int curr_node = pq.top().second;  // Node yang sedang diproses
        pq.pop();
        
        // Jika node saat ini adalah tujuan, kembalikan jaraknya
        if (curr_node == destination)
            return curr_distance;
        
        // Jika jarak saat ini lebih besar daripada jarak yang sudah diketahui, lanjutkan ke node berikutnya
        if (curr_distance > distance[curr_node])
            continue;
        
        // Proses semua tetangga node saat ini
        for (auto neighbor : graph[curr_node].neighbors) {
            int neighbor_id = neighbor.first;  // ID tetangga
            int neighbor_distance = neighbor.second;  // Jarak ke tetangga
            
            // Jika ditemukan jalur yang lebih pendek ke tetangga, perbarui jarak dan masukkan ke priority queue
            if (distance[neighbor_id] > curr_distance + neighbor_distance) {
                distance[neighbor_id] = curr_distance + neighbor_distance;
                pq.push({distance[neighbor_id], neighbor_id});
            }
        }
    }
    return -1;  // Jika tidak ditemukan jalur terpendek, kembalikan -1
}

// Fungsi untuk menghitung biaya pengiriman
int calculateShippingCost(int distance) {
    if (distance < 0) return 0;  // Jarak tidak valid
    
    // Biaya pengiriman per kilometer
    const int costPerKm = 3000;
    
    // Menghitung total biaya berdasarkan jarak
    int totalCost = distance * costPerKm;
    
    return totalCost;
}

// Fungsi untuk mendapatkan waktu sekarang dalam format string
string getWaktuSekarang() {
    time_t now = time(0);
    char buffer[80];
    tm *ltm = localtime(&now);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ltm);
    return string(buffer);
}


void prosesPembayaran(BarangKeranjang keranjang[], int& jumlahKeranjang, int maxKeranjang, int shippingCost) {
    if (jumlahKeranjang == 0) {
        cout << "Keranjang belanja Anda kosong. Tidak ada yang bisa dibayar!" << endl;
        return;
    }
    else if (shippingCost == 0) {
        cout << "Masuk ke Pilihan 6 Terlebih Dahulu. Tentukan jarak kirim!" << endl;
        return;
    }

    double totalHarga = 0;
    for (int i = 0; i < jumlahKeranjang; ++i) {
        totalHarga += keranjang[i].produk.harga * keranjang[i].jumlah;
    }

    int totalSemua = totalHarga + shippingCost;
    cout << fixed << setprecision(0);
    cout << "Total harga belanja: Rp " << totalHarga << endl;
    cout << "Biaya Pengiriman: Rp " << shippingCost << endl;
    cout << "Total Semuanya: Rp " << totalSemua << endl;

    // Menampilkan pilihan metode pembayaran
    cout << "\nPilih Metode Pembayaran:" << endl;
    cout << "1. Transfer Bank" << endl;
    cout << "2. E-Wallet" << endl;
    cout << "3. Bayar di Tempat (COD)" << endl;
    cout << "Masukkan pilihan (1-3): ";
    int metodePembayaran;
    cin >> metodePembayaran;

    string metode;
    switch (metodePembayaran) {
        case 1:
            metode = "Transfer Bank";
            break;
        case 2:
            metode = "E-Wallet";
            break;
        case 3:
            metode = "Bayar di Tempat (COD)";
            break;
        default:
            cout << "Pilihan tidak valid. Pembayaran dibatalkan." << endl;
            return;
    }

    // Konfirmasi pembayaran
    cout << "\nAnda memilih metode pembayaran: " << metode << endl;
    cout << "Konfirmasi pembayaran? (y/n): ";
    char konfirmasi;
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // Simpan riwayat pembayaran
        RiwayatPembayaran pembayaran;
        pembayaran.waktuPembayaran = getWaktuSekarang();
        pembayaran.totalHarga = totalHarga;
        pembayaran.totalBarang = 0;
        pembayaran.jumlahBarang = jumlahKeranjang;
        pembayaran.shippingCost = shippingCost;
        pembayaran.metodePembayaran = metode;

        for (int i = 0; i < jumlahKeranjang; ++i) {
            pembayaran.detailBarang[i] = keranjang[i];
            pembayaran.totalBarang += keranjang[i].jumlah;
        }

        pembayaran.metodePembayaran = metode; // Simpan metode pembayaran
        riwayatPembayaran[jumlahRiwayatPembayaran++] = pembayaran;

        // Kosongkan keranjang
        jumlahKeranjang = 0;

        cout << "Pembayaran berhasil dilakukan menggunakan " << metode << "!" << endl;
    } else {
        cout << "Pembayaran dibatalkan." << endl;
    }
}


// Fungsi untuk melihat riwayat pembayaran
void lihatRiwayatPembayaran(const RiwayatPembayaran riwayatPembayaran[], int jumlahRiwayatPembayaran) {
    if (jumlahRiwayatPembayaran == 0) {
        cout << "Belum ada riwayat pembayaran." << endl;
        return;
    }

    cout << "==== Riwayat Pembayaran ====\n";
    for (int i = 0; i < jumlahRiwayatPembayaran; ++i) {
        cout << "\nPembayaran ke-" << i + 1 << endl;
        cout << "Total Harga Barang: Rp " << riwayatPembayaran[i].totalHarga << endl;
        cout << "Total Barang: " << riwayatPembayaran[i].totalBarang << endl;
        cout << "Detail Barang:\n";

        for (int j = 0; j < riwayatPembayaran[i].jumlahBarang; ++j) {
            cout << "- " << riwayatPembayaran[i].detailBarang[j].produk.nama
                << ", Jumlah: " << riwayatPembayaran[i].detailBarang[j].jumlah
                << ", Harga: Rp " << riwayatPembayaran[i].detailBarang[j].produk.harga << endl;
        }
    }
}

void lihatDetailRiwayatPembayaran(const RiwayatPembayaran riwayatPembayaran[], int jumlahRiwayatPembayaran) {
    unordered_map<int, string> pembayaranDetails;

    for (int i = 0; i < jumlahRiwayatPembayaran; ++i) {
        string detailPembayaran = "";

        // Menambahkan informasi riwayat pembayaran
        detailPembayaran += "Waktu: " + riwayatPembayaran[i].waktuPembayaran + "\n";

        // Menggunakan ostringstream untuk mengatur tampilan harga
        ostringstream hargaStream;
        if (riwayatPembayaran[i].totalHarga == static_cast<int>(riwayatPembayaran[i].totalHarga)) {
            hargaStream << fixed << setprecision(0) << riwayatPembayaran[i].totalHarga;
        } else {
            hargaStream << fixed << setprecision(2) << riwayatPembayaran[i].totalHarga;
        }
        string totalHarga = hargaStream.str();
        
        // Menambahkan detail harga dan biaya pengiriman
        detailPembayaran += "Total Harga Barang: Rp " + totalHarga + "\n";
        
        ostringstream shippingCostStream;
        shippingCostStream << fixed << setprecision(2) << riwayatPembayaran[i].shippingCost;
        string shippingCost = shippingCostStream.str();
        detailPembayaran += "Biaya Pengiriman: Rp " + shippingCost + "\n";

        // Menambahkan metode pembayaran dan total barang
        detailPembayaran += "Metode Pembayaran: " + riwayatPembayaran[i].metodePembayaran + "\n";
        detailPembayaran += "Total Barang: " + to_string(riwayatPembayaran[i].totalBarang) + "\n";
        
        // Menambahkan detail barang
        for (int j = 0; j < riwayatPembayaran[i].jumlahBarang; ++j) {
            ostringstream hargaBarangStream;
            hargaBarangStream << fixed << setprecision(2) << riwayatPembayaran[i].detailBarang[j].produk.harga;
            string hargaBarang = hargaBarangStream.str();
            detailPembayaran += "- " + riwayatPembayaran[i].detailBarang[j].produk.nama
                                + ", Harga: Rp " + hargaBarang + "\n";
        }

        pembayaranDetails[i] = detailPembayaran; // Menyimpan seluruh detail barang untuk pembayaran ke-i
    }

    if (pembayaranDetails.empty()) {
        cout << "Belum ada riwayat pembayaran." << endl;
        return;
    }

    cout << "==== Riwayat Pembayaran (Hash Map) ====\n";
    for (const auto& entry : pembayaranDetails) {
        cout << "\nPembayaran ke-" << entry.first + 1 << endl;
        cout << entry.second; // Menampilkan seluruh detail pembayaran
    }
}

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
    int totalBarang = 0;
    for (int i = 0; i < jumlahKeranjang; ++i) {
        double hargaTotal = keranjang[i].produk.harga * keranjang[i].jumlah;
        totalHarga += hargaTotal;
        totalBarang += keranjang[i].jumlah;
        cout << keranjang[i].produk.nama << ", Jumlah: " << keranjang[i].jumlah 
            << ", Harga : Rp " << keranjang[i].produk.harga 
            << ", Total harga : Rp " << hargaTotal << endl;
    }
    cout << "Total harga semua barang: Rp " << totalHarga << endl;
    cout << "Total jumlah barang: " << totalBarang << endl;
}

void cariProdukBerdasarkanID(const unordered_map<int, Produk>& produkMap, int id) {
    auto it = produkMap.find(id);  // Mencari produk berdasarkan ID
    if (it != produkMap.end()) {
        it->second.display();  // Jika ditemukan, tampilkan produk
    } else {
        cout << "Produk dengan ID " << id << " tidak ditemukan!" << endl;
    }
}

void hapusDariKeranjang(BarangKeranjang keranjang[], int& jumlahKeranjang, int id) {
    bool ditemukan = false;
    for (int i = 0; i < jumlahKeranjang; ++i) {
        if (keranjang[i].produk.id == id) {
            ditemukan = true;
            for (int j = i; j < jumlahKeranjang - 1; ++j) {
                keranjang[j] = keranjang[j + 1];
            }
            --jumlahKeranjang;
            cout << "Barang dengan ID " << id << " berhasil dihapus dari keranjang." << endl;
            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang dengan ID " << id << " tidak ditemukan dalam keranjang." << endl;
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

    // Contoh graph untuk perhitungan jarak
    vector<Node> graph(8);  // Terdapat 8 node dalam graf
    graph[0].neighbors = {{1, 5}, {3, 4}, {8, 7}};  // Node 0 terhubung dengan node 1 (jarak 5), node 3 (jarak 4), dan node 8 (jarak 7)
    graph[1].neighbors = {{0, 5}, {6, 4}};  // Node 1 terhubung dengan node 0 (jarak 5) dan node 6 (jarak 4)
    graph[2].neighbors = {{0, 7}};  // Node 2 terhubung dengan node 0 (jarak 7)
    graph[3].neighbors = {{0, 4}, {4, 4}};  // Node 3 terhubung dengan node 0 (jarak 4) dan node 4 (jarak 4)
    graph[4].neighbors = {{3, 4}, {5, 9}};  // Node 4 terhubung dengan node 3 (jarak 4) dan node 5 (jarak 9)



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
            system("cls");
            int id;
            cout << "Masukkan ID produk yang ingin dihapus dari keranjang: ";
            cin >> id;
            hapusDariKeranjang(keranjang, jumlahKeranjang, id);
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.ignore();
            cin.get();
        }

        else if (pilihan == 6) {
            system("cls");
            int source, destination;
            cout << "Masukkan node asal (0-4): ";
            cin >> source;
            cout << "Masukkan node tujuan (0-4): ";
            cin >> destination;

            // Validasi input
            if (source < 0 || source >= graph.size() || destination < 0 || destination >= graph.size()) {
                cout << "Node tidak valid!" << endl;
                cout << "\n[Tekan Enter untuk kembali ke menu utama]";
                cin.ignore();
                cin.get();
                continue;
            }

            // Cari jarak terpendek
            int shortestDistance = findShortestPath(graph, source, destination);
            
            // Hitung biaya pengiriman
            shippingCost = calculateShippingCost(shortestDistance);

            // Tampilkan informasi pengiriman
            if (shortestDistance != -1) {
                cout << "Rute Tercepat dari Node " << source << " ke Node " << destination << endl;
                cout << "Total Jarak: " << shortestDistance << " km" << endl;
                
                // Tampilkan total jumlah barang di keranjang
                int totalBarang = 0;
                for (int i = 0; i < jumlahKeranjang; ++i) {
                    totalBarang += keranjang[i].jumlah;
                }
                
                cout << "Total Jumlah Barang: " << totalBarang << endl;
                cout << "Biaya Pengiriman: Rp " << shippingCost << endl;
            } else {
                cout << "Tidak ada rute yang tersedia antara node " << source << " dan " << destination << endl;
            }

            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.ignore();
            cin.get();
        }

        else if (pilihan == 7) {
            system("cls");
            prosesPembayaran(keranjang, jumlahKeranjang, maxKeranjang, shippingCost);
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.ignore();
            cin.get();
        }

        else if (pilihan == 8) {
            system("cls");
            lihatRiwayatPembayaran(riwayatPembayaran, jumlahRiwayatPembayaran);
            cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.ignore();
            cin.get();
        }

        else if (pilihan == 9) {
            system("cls");
             lihatDetailRiwayatPembayaran(riwayatPembayaran, jumlahRiwayatPembayaran);
             cout << "\n[Tekan Enter untuk kembali ke menu utama]";
            cin.ignore();
            cin.get();
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
