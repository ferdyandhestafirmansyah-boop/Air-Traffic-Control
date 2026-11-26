#include "AirportSystem.h"
#include <iostream>

using namespace std;

// 1. Inisialisasi Data Array Katalog (Dengan beberapa data awal)
AircraftSpec catalog[MAX_CATALOG] = {
    {"ATR72", 1300},
    {"A320",  2100},
    {"B777",  3000}
};
int catalogCount = 3; // Jumlah terisi saat ini

bool addAircraftSpec(const string& type, int minRunway) {
    if (catalogCount >= MAX_CATALOG) {
        cout << "[ERROR] Kapasitas Array Katalog Penuh!" << endl;
        return false;
    }
    // Cek apakah tipe sudah ada
    for (int i = 0; i < catalogCount; i++) {
        if (catalog[i].type == type) {
            cout << "[INFO] Tipe pesawat " << type << " sudah ada di katalog." << endl;
            return false;
        }
    }
    catalog[catalogCount] = {type, minRunway};
    catalogCount++;
    return true;
}

void displayCatalog() {
    cout << "\n=== BUKU KATALOG SPESIFIKASI PESAWAT (ARRAY) ===" << endl;
    for (int i = 0; i < catalogCount; i++) {
        cout << (i + 1) << ". Tipe: " << catalog[i].type 
             << " | Butuh Landasan: " << catalog[i].minRunway << " meter" << endl;
    }
}

int getRequiredRunway(const string& type) {
    for (int i = 0; i < catalogCount; i++) {
        if (catalog[i].type == type) {
            return catalog[i].minRunway;
        }
    }
    return -1;
}

string getPriorityLabel(int priority) {
    switch (priority) {
        case PRIORITY_MAYDAY_KRITIS:
            return "[MAYDAY - KRITIS / FUEL HABIS]";
        case PRIORITY_PAN_PAN_MEDIS:
            return "[PAN-PAN - DARURAT MEDIS]";
        case PRIORITY_NORMAL:
        default:
            return "[NORMAL]";
    }
}

// 2. Implementasi Priority Queue
PriorityQueue::PriorityQueue() {
    head = nullptr;
}

bool PriorityQueue::isEmpty() {
    return head == nullptr;
}

void PriorityQueue::enqueue(string fn, string type, int priority) {
    QueueNode* newNode = new QueueNode{fn, type, priority, nullptr};

    if (head == nullptr || priority > head->priority) {
        newNode->next = head;
        head = newNode;
    } else {
        QueueNode* curr = head;
        while (curr->next != nullptr && curr->next->priority >= priority) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }
}

QueueNode* PriorityQueue::dequeue() {
    if (head == nullptr) return nullptr;
    QueueNode* temp = head;
    head = head->next;
    return temp;
}

void PriorityQueue::display() {
    cout << "\n--- ANTREAN DI UDARA (PRIORITY QUEUE) ---" << endl;
    if (!head) { 
        cout << "[Holding Pattern Kosong / Tidak Ada Pesawat]" << endl; 
        return; 
    }
    QueueNode* curr = head;
    int pos = 1;
    while (curr) {
        cout << pos++ << ". [" << curr->flightNum << " | " << curr->type << "] " 
             << "Level " << curr->priority << " " << getPriorityLabel(curr->priority) << endl;
        curr = curr->next;
    }
}

// 3. Implementasi MLL
void displayRunways(Runway* headRunway) {
    cout << "\n--- PETA LANDASAN PACU (MULTI-LINKED LIST) ---" << endl;
    Runway* curr = headRunway;
    while (curr) {
        cout << "* " << curr->name << " (" << curr->length << "m) -> Status: ";
        if (curr->activeFlight == nullptr) {
            cout << "[TERSEDIA / KOSONG]" << endl;
        } else {
            cout << "[DIPAKAI] " << curr->activeFlight->flightNum << " (" 
                 << curr->activeFlight->type << ") " 
                 << getPriorityLabel(curr->activeFlight->priority) << endl;
        }
        curr = curr->next;
    }
}

void allocateRunway(Runway* headRunway, PriorityQueue& pq) {
    if (pq.isEmpty()) {
        cout << "[INFO] Tidak ada pesawat dalam antrean di udara." << endl;
        return;
    }

    // Cari runway yang kosong dan panjangnya cukup
    Runway* curr = headRunway;
    while (curr != nullptr) {
        if (curr->activeFlight == nullptr) {
            // Intip pesawat terdepan di Queue
            QueueNode* plane = pq.head;
            int req = getRequiredRunway(plane->type);

            if (curr->length >= req) {
                // Keluarkan pesawat dari queue
                plane = pq.dequeue();
                curr->activeFlight = new ChildFlight{plane->flightNum, plane->type, plane->priority};
                cout << "\n>> SUKSES: Pesawat " << plane->flightNum << " " 
                     << getPriorityLabel(plane->priority) << " dialokasikan ke " << curr->name << endl;
                delete plane;
                return;
            }
        }
        curr = curr->next;
    }
    cout << "\n[GAGAL] Tidak ada landasan kosong yang cukup panjang untuk pesawat terdepan saat ini!" << endl;
}

void finishLanding(Runway* headRunway, BSTNode*& archiveRoot) {
    bool adaPesawat = false;
    Runway* curr = headRunway;
    while (curr != nullptr) {
        if (curr->activeFlight != nullptr) {
            adaPesawat = true;
            ChildFlight* cf = curr->activeFlight;
            archiveRoot = insertBST(archiveRoot, cf->flightNum, cf->type, cf->priority);
            cout << ">> Pesawat " << cf->flightNum << " selesai taxi dari " 
                 << curr->name << " -> Berhasil diarsipkan ke BST." << endl;
            delete cf;
            curr->activeFlight = nullptr;
        }
        curr = curr->next;
    }
    if (!adaPesawat) {
        cout << "[INFO] Semua landasan sedang kosong, tidak ada pesawat yang sedang mendarat." << endl;
    }
}

// 4. Implementasi BST
BSTNode* insertBST(BSTNode* root, string fn, string type, int priority) {
    if (root == nullptr) {
        return new BSTNode{fn, type, priority, nullptr, nullptr};
    }
    if (fn < root->flightNum) root->left = insertBST(root->left, fn, type, priority);
    else if (fn > root->flightNum) root->right = insertBST(root->right, fn, type, priority);
    return root;
}

void printArsip(BSTNode* root) {
    if (root == nullptr) return;
    printArsip(root->left);
    cout << "  - No: " << root->flightNum << " | Tipe: " << root->type 
         << " | Status: " << getPriorityLabel(root->priority) << endl;
    printArsip(root->right);
}

BSTNode* searchBST(BSTNode* root, string fn) {
    if (root == nullptr || root->flightNum == fn) return root;
    if (fn < root->flightNum) return searchBST(root->left, fn);
    return searchBST(root->right, fn);
}