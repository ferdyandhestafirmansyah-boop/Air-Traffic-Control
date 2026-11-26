# ✈️ Sistem Simulasi Air Traffic Control (ATC) Bandara

Proyek tugas besar struktur data yang mensimulasikan manajemen lalu lintas udara di bandara secara terpadu, mulai dari validasi pesawat, antrean holding pattern di udara dengan prioritas urutan penanganan darurat, alokasi landasan pacu secara *real-time*, hingga pengarsipan riwayat pendaratan harian.

---

## 👥 Anggota Kelompok
1. Mahardhika Putra Azlian/109082500025/S1IF-13-01
2. Janica Prima Ginting/109082500064/S1IF-13-02
3. Ferdyan Dhesta Firmansyah/109082500109/S1IF-13-03
4. Zhafif Yusuf Al Amin/109082500137/S1IF-13-04

---

## 🛠️ Struktur Data yang Digunakan

| No | Struktur Data | Komponen Sistem | Fungsi Utama |
| :---: | :--- | :--- | :--- |
| **1** | **Array** | Buku Katalog Pesawat | Menyimpan data spesifikasi tipe pesawat (misal: ATR72, A320, B777) dan kebutuhan panjang landasan minimum sebagai filter keselamatan fisik. Pengguna dapat menambahkan tipe pesawat baru secara dinamis. |
| **2** | **Priority Queue** | Holding Pattern Udara | Mengatur antrean pesawat di udara berdasarkan derajat kegawatan darurat (*Multi-level Emergency*). Pesawat darurat otomatis memotong antrean, sedangkan pesawat dengan level yang sama mematuhi aturan FIFO. |
| **3** | **Multi-Linked List (MLL)** | Peta Landasan Pacu | Memantau kondisi nyata di darat secara *real-time*. **Parent** merepresentasikan Landasan (*Runway*), dan **Child** merepresentasikan pesawat yang sedang aktif *Landing/Taxi*. Mencegah tabrakan dan memaksimalkan utilisasi landasan. |
| **4** | **Binary Search Tree (BST)** | Buku Arsip Harian | Menyimpan riwayat penerbangan yang telah selesai mendarat. Memungkinkan pencarian data penerbangan spesifik dalam kompleksitas $O(\log n)$ serta pencetakan laporan harian terurut abjad (*In-Order Traversal*). |

---

## 🚨 Tingkatan Prioritas di Ruang Udara
Antrean ruang udara (*Priority Queue*) dibagi menjadi 3 level kegentingan:
* **Level 3 - `MAYDAY` (Prioritas Tertinggi):** Kerusakan mesin, bahan bakar kritis/menipis (*Fuel Starvation*). Otomatis memotong ke urutan nomor 1 antrean.
* **Level 2 - `PAN-PAN` (Prioritas Menengah):** Darurat medis penumpang, gangguan teknis sekunder.
* **Level 1 - `NORMAL` (Prioritas Standar):** Penerbangan terjadwal reguler.

---

## 📁 Struktur File Proyek

```text
├── AirportSystem.h    # Deklarasi struct, konstanta, dan prototipe fungsi
├── AirportSystem.cpp  # Implementasi logika struktur data (Array, Queue, MLL, BST)
├── main.cpp           # Menu interaktif CLI dan skenario simulasi
├── .gitignore         # Mencegah file executable (.exe) ikut ter-upload
└── README.md          # Dokumentasi proyek
```

---

## 🚀 Cara Kompilasi dan Menjalankan

### Di Windows (Command Prompt / PowerShell / MinGW):
```bash
# 1. Kompilasi file
g++ main.cpp AirportSystem.cpp -o airport_sim.exe

# 2. Jalankan program
.\airport_sim.exe
```

---

## 🎮 Fitur Menu Program
1. **Tambah Spesifikasi Pesawat ke Katalog (Array):** Menambah jenis pesawat dan panjang landasan yang dibutuhkan.
2. **Lihat Katalog Pesawat (Array):** Menampilkan semua tipe pesawat yang terdaftar.
3. **Input Pesawat Sekaligus (Batch Input Queue):** Memasukkan banyak pesawat ke ruang udara sekaligus tanpa harus mengulang menu.
4. **Cek Kondisi Udara & Landasan (Queue & MLL):** Memantau posisi antrean dan status pemakaian landasan pacu.
5. **Alokasikan Pendaratan (Queue -> MLL):** Mendaratkan pesawat prioritas tertinggi ke landasan kosong yang sesuai.
6. **Selesaikan Pendaratan (MLL -> BST):** Pesawat keluar landasan dan otomatis diarsipkan ke BST.
7. **Laporan Harian (BST In-Order):** Menampilkan seluruh riwayat pendaratan terurut abjad nomor penerbangan.
8. **Pencarian Riwayat (BST Search):** Mencari status penerbangan tertentu dalam $O(\log n)$.