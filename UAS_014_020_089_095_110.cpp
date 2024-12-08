#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <climits>
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
    
    // Contoh skema biaya pengiriman berdasarkan jarak
    if (distance <= 5) return 10000;     // Jarak dekat
    if (distance <= 10) return 15000;    // Jarak menengah
    if (distance <= 20) return 25000;    // Jarak jauh
    return 50000;                        // Jarak sangat jauh
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
    vector<Node> graph(5);  // Misalnya 5 node
    graph[0].neighbors = {{1, 5}, {2, 10}};  // Node 0 terhubung dengan node 1 (jarak 5) dan node 2 (jarak 10)
    graph[1].neighbors = {{0, 5}, {3, 7}};   // Node 1 terhubung dengan node 0 dan node 3
    graph[2].neighbors = {{0, 10}, {3, 15}, {4, 12}};  // Node 2 terhubung dengan node 0, 3, dan 4
    graph[3].neighbors = {{1, 7}, {2, 15}, {4, 5}};    // Node 3 terhubung dengan node 1, 2, dan 4
    graph[4].neighbors = {{2, 12}, {3, 5}};  // Node 4 terhubung dengan node 2 dan 3

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
            // ... [previous code remains the same]
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
            int shippingCost = calculateShippingCost(shortestDistance);

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


 //else if (pilihan == 7) {
    //selanjutnya letakin disini 

        
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
