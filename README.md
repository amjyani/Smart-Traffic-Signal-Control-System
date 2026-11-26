# 🚦 Smart Traffic Signal Control System

A terminal-based C++ project that models a city's roads as a **graph** and uses classic Data Structures & Algorithms — graph traversal, shortest path, searching, and sorting — to find routes, detect congestion, and recommend traffic signal timings.


---

## 📌 Overview

Most real traffic signals run on **fixed timings**, regardless of how busy an intersection actually is. This project simulates a smarter alternative: every intersection is a **node**, every road is a **weighted edge**, and each road carries a traffic level — `Low`, `Medium`, or `High`. The program then uses that graph to find the fastest routes, flag congested roads, and recommend signal durations based on real traffic conditions.

Built as a Data Structures & Algorithm Design (C++) course project.

## ✨ Features

| # | Feature | DSA Concept |
|---|---------|-------------|
| 1 | Display City Road Network | Graph — Adjacency List |
| 2 | Add New Road | Linear Search (duplicate check) |
| 3 | Breadth First Search (BFS) | Queue-based traversal |
| 4 | Depth First Search (DFS) | Stack-based traversal |
| 5 | Find Shortest Route | Dijkstra's Algorithm (Greedy + Priority Queue) |
| 6 | Update Traffic Density | Linear Search |
| 7 | Show Congested Roads | Selection Sort |
| 8 | Recommend Signal Timing | Rule-based lookup |
| 9 | Search Intersection | Linear Search |
| 10 | System Report | Aggregation / Statistics |
| 11 | Exit | — |

## 🛠️ Tech Stack

- **Language:** C++ (C++17)
- **Compiler:** GNU G++
- **STL used:** `queue`, `stack`, `priority_queue`, `vector`, `string`
- No external dependencies — a single `.cpp` file, compiles anywhere `g++` is available.

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (`g++`) supporting C++17

### Build & Run
```bash
git clone https://github.com/<your-username>/smart-traffic-signal-control-system.git
cd smart-traffic-signal-control-system
g++ -std=c++17 src/main.cpp -o traffic_sim
./traffic_sim
```

A sample city with 6 intersections and 7 roads loads automatically, so every feature can be tried immediately.

## 🧠 How the City is Stored

- Every **intersection** is an integer id with a name (e.g. `0 → MG Road`).
- Every **road** is one entry in a single master list (`vector<Road>`), holding its two endpoints, distance, and traffic level.
- Each intersection keeps a list of the **indices** of the roads that touch it — this is the adjacency list that powers BFS, DFS, and Dijkstra.

```cpp
struct Road {
    int u, v;              // the two intersections this road connects
    int distance;          // distance / travel weight
    TrafficLevel traffic;  // Low / Medium / High
};
```

## 📂 Project Structure

```
smart-traffic-signal-control-system/
├── src/
│   └── main.cpp     # entire project — single file, heavily commented
├── LICENSE
├── .gitignore
└── README.md

```

## 🎯 Signal Timing Logic

A simple, explainable rule-based lookup — deliberately kept simple rather than a complex formula:

| Traffic Level | Recommended Green Time |
|---|---|
| Low | 15 seconds |
| Medium | 30 seconds |
| High | 45 seconds |

## 👤 Author

**Anshika Dubey**

## 🙏 Acknowledgements

Developed under the guidance of **Mr. Gaurav Sharma**, School of Computer Science and Engineering, Lovely Professional University, as part of the *Data Structures and Algorithm Design using C++* course.
