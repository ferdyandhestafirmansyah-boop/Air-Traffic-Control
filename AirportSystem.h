#ifndef AIRPORT_SYSTEM_H
#define AIRPORT_SYSTEM_H

#include <string>


enum FlightPriority {
    PRIORITY_NORMAL = 1,         // Level 1
    PRIORITY_PAN_PAN_MEDIS = 2,  // Level 2
    PRIORITY_MAYDAY_KRITIS = 3   // Level 3
};


struct AircraftSpec {
    std::string type;
    int minRunway;
};

const int MAX_CATALOG = 20; // Kapasitas Array
extern AircraftSpec catalog[MAX_CATALOG];
extern int catalogCount;

bool addAircraftSpec(const std::string& type, int minRunway);
void displayCatalog();
int getRequiredRunway(const std::string& type);
std::string getPriorityLabel(int priority);


struct QueueNode {
    std::string flightNum;
    std::string type;
    int priority;
    QueueNode* next;
};

struct PriorityQueue {
    QueueNode* head;

    PriorityQueue();
    void enqueue(std::string fn, std::string type, int priority);
    QueueNode* dequeue();
    void display();
    bool isEmpty();
};


struct ChildFlight {
    std::string flightNum;
    std::string type;
    int priority;
};

struct Runway {
    std::string name;
    int length;
    ChildFlight* activeFlight; 
    Runway* next;              
};

void displayRunways(Runway* headRunway);
void allocateRunway(Runway* headRunway, PriorityQueue& pq);
void finishLanding(Runway* headRunway, struct BSTNode*& archiveRoot);


struct BSTNode {
    std::string flightNum;
    std::string type;
    int priority;
    BSTNode* left;
    BSTNode* right;
};

BSTNode* insertBST(BSTNode* root, std::string fn, std::string type, int priority);
void printArsip(BSTNode* root);
BSTNode* searchBST(BSTNode* root, std::string fn);

#endif 