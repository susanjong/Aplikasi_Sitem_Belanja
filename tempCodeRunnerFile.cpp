#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// Struktur data untuk menyimpan informasi titik
struct Node {
    int id;
    vector<pair<int, int>> neighbors; // (neighbor_id, distance)
};

// Struktur data untuk menyimpan informasi produk
struct Product {
    string name;
    int quantity;
    int price;
};

// Fungsi untuk mencari jarak terpendek menggunakan Dijkstra's algorithm
int findShortestPath(vector<Node>& graph, int source, int destination) {
    vector<int> distance(graph.size(), INT_MAX);
    priority_queue<pair<int, int>> pq; // (distance, node_id)

    distance[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        int curr_node = pq.top().second;
        int curr_distance = -pq.top().first;
        pq.pop();

        if (curr_node == destination)
            return curr_distance;

        if (curr_distance > distance[curr_node])
            continue;

        for (auto neighbor : graph[curr_node].neighbors) {
            int neighbor_id = neighbor.first;
            int neighbor_distance = neighbor.second;

            if (distance[neighbor_id] > curr_distance + neighbor_distance) {
                distance[neighbor_id] = curr_distance + neighbor_distance;
                pq.push(make_pair(-distance[neighbor_id], neighbor_id));
            }
        }
    }

    return -1; // Tidak ditemukan jalur terpendek
}

// Fungsi untuk menghitung biaya pengiriman
int calculateShippingCost(int distance, int baseRate = 3000) {
    return distance * baseRate;
}

// Fungsi untuk menghitung diskon berdasarkan total belanja
double calculateDiscount(int totalPrice) {
    double discount = 0.0;
    if (totalPrice >= 50000000) {
        discount = 0.15; // 15% diskon untuk pembelian di atas Rp. 50 juta
    } else if (totalPrice >= 25000000) {
        discount = 0.10; // 10% diskon untuk pembelian di atas Rp. 25 juta
    } else if (totalPrice >= 10000000) {
        discount = 0.05; // 5% diskon untuk pembelian di atas Rp. 10 juta
    }
    return discount;
}

// Fungsi untuk menampilkan daftar produk
void displayProductList(const vector<Product>& products) {
    cout << "\n===== DAFTAR PRODUK =====\n";
    for (size_t i = 0; i < products.size(); ++i) {
        cout << i+1 << ". " << products[i].name << " - Rp. " 
             << products[i].price << endl;
    }
    cout << "0. Selesai memilih produk\n";
}

int main() {
    // Inisialisasi graph berdasarkan informasi pada gambar
    vector<Node> graph = {
        {0, {{1, 7}, {2, 2}, {5, 4}}},
        {1, {{0, 7}, {2, 3}, {3, 4}, {4, 6}}},
        {2, {{0, 2}, {1, 3}, {3, 1}, {4, 6}}},
        {3, {{1, 4}, {2, 1}, {6, 11}}},
        {4, {{1, 6}, {2, 6}, {5, 9}, {6, 4}, {7, 4}}},
        {5, {{0, 4}, {4, 9}, {6, 5}, {7, 3}}},
        {6, {{3, 11}, {4, 4}, {5, 5}, {7, 9}}},
        {7, {{4, 4}, {5, 3}, {6, 9}}}
    };

    // Inisialisasi daftar produk
    vector<Product> availableProducts = {
        {"Smartphone Samsung Galaxy S23", 1, 12000000},
        {"Laptop ASUS ROG Zephyrus G14", 1, 25000000},
        {"TV LED LG 43 Inch", 1, 6500000},
        {"Earbuds Apple AirPods Pro", 1, 3500000},
        {"Kamera DSLR Canon EOS 90D", 1, 17000000},
        {"Sepeda MTB Polygon", 1, 5000000},
        {"Dumbbell 5kg", 1, 200000},
        {"Matras Yoga", 1, 250000},
        {"Mesin Elliptical", 1, 7500000},
        {"Resistance Band", 1, 100000},
        {"Serum Wajah Vitamin C", 1, 150000},
        {"Masker Wajah Aloe Vera", 1, 75000},
        {"Lipstik Matte L'Oreal", 1, 120000},
        {"Parfum Chanel", 1, 2500000},
        {"Sabun Cuci Muka Himalaya", 1, 35000},
        {"Roti Tawar Serba Roti", 1, 15000},
        {"Kopi Arabica 100g", 1, 50000},
        {"Mie Instan", 1, 5000},
        {"Susu UHT Indomilk 1 Liter", 1, 18000},
        {"Teh Kotak Sosro 500ml", 1, 7500}
    };

    // Keranjang belanja
    vector<Product> cart;

    // Proses pemilihan produk
    int choice;
    int quantity;
    do {
        // Tampilkan daftar produk
        displayProductList(availableProducts);

        // Minta input pilihan produk
        cout << "\nPilih nomor produk (0 untuk selesai): ";
        cin >> choice;

        // Validasi input
        if (choice > 0 && choice <= static_cast<int>(availableProducts.size())) {
            // Minta jumlah produk
            cout << "Masukkan jumlah " << availableProducts[choice-1].name << ": ";
            cin >> quantity;

            // Buat salinan produk dengan jumlah yang diinginkan
            Product selectedProduct = availableProducts[choice-1];
            selectedProduct.quantity = quantity;

            // Tambahkan ke keranjang
            cart.push_back(selectedProduct);

            cout << quantity << " " << selectedProduct.name << " ditambahkan ke keranjang.\n";
        } else if (choice != 0) {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (choice != 0);

    // Jika keranjang kosong, keluar dari program
    if (cart.empty()) {
        cout << "Keranjang belanja kosong. Terima kasih!\n";
        return 0;
    }

    // Pilih titik tujuan pengiriman
    int source = 0; // Titik awal (lokasi toko online)
    int destination;

    cout << "\nMasukkan titik tujuan pengiriman (0-7): ";
    cin >> destination;

    // Mencari jarak terpendek
    int shortestDistance = findShortestPath(graph, source, destination);
    
    if (shortestDistance != -1) {
        // Menampilkan informasi jarak dan biaya pengiriman
        cout << "\nJarak terpendek antara titik " << source << " dan " << destination 
             << " adalah: " << shortestDistance << " kilometer." << endl;
        
        int shippingCost = calculateShippingCost(shortestDistance);
        cout << "Biaya pengiriman: Rp. " << shippingCost << endl;

        // Menampilkan daftar barang di keranjang
        cout << "\nDaftar Barang di Keranjang:" << endl;
        int totalPrice = 0;
        for (const auto& product : cart) {
            int productTotal = product.quantity * product.price;
            cout << "- " << product.name << " (Jumlah: " << product.quantity 
                 << ", Harga: Rp. " << product.price << " per item, Subtotal: Rp. " 
                 << productTotal << ")" << endl;
            totalPrice += productTotal;
        }

        // Menghitung diskon
        double discount = calculateDiscount(totalPrice);
        int discountAmount = totalPrice * discount;
        int finalPrice = totalPrice + shippingCost - discountAmount;

        // Menampilkan ringkasan harga
        cout << "\nTotal Harga Produk: Rp. " << totalPrice << endl;
        cout << "Diskon (" << (discount * 100) << "%): Rp. " << discountAmount << endl;
        cout << "Biaya Pengiriman: Rp. " << shippingCost << endl;
        cout << "Total Harga Akhir: Rp. " << finalPrice << endl;
    } else {
        cout << "Tidak ditemukan jalur terpendek." << endl;
    }

    return 0;
}