#include <iostream>
#include "AirportSystem.h"

using namespace std;

void tampilkanMenu() {
    cout << "\n==============================================" << endl;
    cout << "       SISTEM AIR TRAFFIC CONTROL (ATC)       " << endl;
    cout << "==============================================" << endl;
    cout << "1. Tambah Jenis Pesawat ke Katalog " << endl;
    cout << "2. Lihat Buku Katalog Tipe Pesawat " << endl;
    cout << "3. Input Pesawat Masuk Sekaligus " << endl;
    cout << "4. Lihat Kondisi Antrean & Landasan " << endl;
    cout << "5. Alokasikan Pendaratan " << endl;
    cout << "6. Selesaikan Pendaratan & Simpan ke Arsip " << endl;
    cout << "7. Lihat Laporan Harian Terurut " << endl;
    cout << "8. Cari Riwayat Penerbangan " << endl;
    cout << "0. Keluar" << endl;
    cout << "Pilih menu [0-8]: ";
}

int main() {
    PriorityQueue airQueue;
    BSTNode* archiveRoot = nullptr;

    // Inisialisasi 2 Landasan Pacu (MLL Parent)
    Runway* r2 = new Runway{"Runway 02 (Panjang)", 3500, nullptr, nullptr};
    Runway* r1 = new Runway{"Runway 01 (Pendek)",  2200, nullptr, r2};

    int pilihan;
    do {
        tampilkanMenu();
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (pilihan) {
            case 1: {
                string tipe;
                int minRunway;
                cout << "\n-- Tambah Spesifikasi Pesawat Baru --" << endl;
                cout << "Masukkan Tipe Pesawat (misal: B737 / CRJ1000): ";
                cin >> tipe;
                cout << "Kebutuhan Panjang Landasan (meter): ";
                cin >> minRunway;
                if (addAircraftSpec(tipe, minRunway)) {
                    cout << ">> Tipe pesawat berhasil ditambahkan ke Katalog!" << endl;
                }
                break;
            }

            case 2:
                displayCatalog();
                break;

            case 3: {
                int jumlah;
                cout << "\n===============================================" << endl;
                cout << "     INPUT PESAWAT      " << endl;
                cout << "===============================================" << endl;
                cout << "Berapa banyak pesawat yang ingin dimasukkan?: ";
                cin >> jumlah;

                if (jumlah <= 0) {
                    cout << "[INFO] Jumlah tidak valid atau dibatalkan." << endl;
                    break;
                }

                // Tampilkan katalog sekali di awal sebagai referensi pilihan tipe
                displayCatalog();

                for (int i = 1; i <= jumlah; i++) {
                    string fn, tipe;
                    int prioritas;

                    cout << "\n-----------------------------------------------" << endl;
                    cout << "Data Pesawat ke-" << i << " dari " << jumlah << ":" << endl;
                    cout << "-----------------------------------------------" << endl;
                    cout << "Nomor Penerbangan (misal: GA-101) : ";
                    cin >> fn;

                    cout << "Tipe Pesawat (sesuai katalog di atas) : ";
                    cin >> tipe;

                    // Validasi: Jika tipe salah ketik, minta input ulang langsung
                    int req = getRequiredRunway(tipe);
                    while (req == -1) {
                        cout << "[!] Tipe '" << tipe << "' tidak ada di katalog. Masukkan tipe yang valid: ";
                        cin >> tipe;
                        req = getRequiredRunway(tipe);
                    }

                    cout << "Tingkat Prioritas:" << endl;
                    cout << "  1. Normal" << endl;
                    cout << "  2. Pan-Pan (Darurat Medis)" << endl;
                    cout << "  3. Mayday (Bahan Bakar Kritis / Mesin Rusak)" << endl;
                    cout << "Pilih Tingkat [1-3]: ";
                    cin >> prioritas;

                    if (prioritas < 1 || prioritas > 3) {
                        cout << "[!] Pilihan di luar rentang, otomatis diset ke 1 (Normal)." << endl;
                        prioritas = 1;
                    }

                    airQueue.enqueue(fn, tipe, prioritas);
                    cout << ">> " << fn << " (" << tipe << ") berhasil dimasukkan ke holding pattern!" << endl;
                }

                // Langsung tampilkan antrean udara setelah semua pesawat selesai diinput
                cout << "\n===============================================" << endl;
                cout << "    HASIL PENGURUTAN ANTREAN OTOMATIS (ATC)    " << endl;
                cout << "===============================================" << endl;
                airQueue.display();
                break;
            }

            case 4:
                airQueue.display();
                displayRunways(r1);
                break;

            case 5:
                allocateRunway(r1, airQueue);
                break;

            case 6:
                cout << "\n-- Menyelesaikan Pendaratan --" << endl;
                finishLanding(r1, archiveRoot);
                break;

            case 7:
                cout << "\n=== BUKU ARSIP HARIAN PENERBANGAN (BST IN-ORDER) ===" << endl;
                if (!archiveRoot) {
                    cout << "[Arsip Masih Kosong]" << endl;
                } else {
                    printArsip(archiveRoot);
                }
                break;

            case 8: {
                string cariFn;
                cout << "\nMasukkan Nomor Penerbangan yang ingin dicari (misal: GA-402): ";
                cin >> cariFn;
                BSTNode* hasil = searchBST(archiveRoot, cariFn);
                if (hasil) {
                    cout << "\n>> DATA DITEMUKAN DI ARSIP <<" << endl;
                    cout << "No Penerbangan : " << hasil->flightNum << endl;
                    cout << "Tipe Pesawat   : " << hasil->type << endl;
                    cout << "Catatan Status : " << getPriorityLabel(hasil->priority) << endl;
                } else {
                    cout << "\n[!] Data dengan nomor '" << cariFn << "' tidak ditemukan di arsip." << endl;
                }
                break;
            }

            case 0:
                cout << "\nKeluar dari sistem ATC. Sampai jumpa!" << endl;
                break;

            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }

    } while (pilihan != 0);

    return 0;
}