/*
 SMART TRAFFIC SIGNAL OPTIMIZER
 Data Structures & Algorithm Design using C++

 IDEA IN ONE LINE:
   City intersections = graph nodes. Roads = weighted graph edges.
   Each road has a distance AND a traffic level (Low/Medium/High).
   We use graph traversal, shortest path, searching, sorting and a
   simple rule-based lookup to help manage city traffic.

 DSA CONCEPTS USED (so you can point to them in viva):
   - OOP: class, encapsulation
   - Graph: Adjacency List representation
   - Queue          -> BFS Traversal
   - Stack          -> DFS Traversal
   - Priority Queue -> Dijkstra's Algorithm (Shortest Route)
   - Linear Search  -> Update Traffic Density, Search Intersection
   - Selection Sort -> Show Congested Roads (busiest distance first)
   - Simple aggregation / statistics -> System Report

 HOW TO COMPILE:
   g++ -std=c++17 src/main.cpp -o traffic_sim

 HOW TO RUN:
   ./traffic_sim
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <climits>
#include <algorithm>

using namespace std;

// Traffic is kept simple: just three levels instead of raw numbers.
enum TrafficLevel { LOW, MEDIUM, HIGH };

string levelToText(TrafficLevel level) {
    if (level == LOW) return "Low";
    if (level == MEDIUM) return "Medium";
    return "High";
}

// Ask the user to pick Low/Medium/High from a small menu.
TrafficLevel askTrafficLevel() {
    cout << "  1. Low\n  2. Medium\n  3. High\n  Choose traffic level: ";
    int choice;
    cin >> choice;
    if (choice == 1) return LOW;
    if (choice == 2) return MEDIUM;
    return HIGH; 
}

// One ROAD = one edge of the graph. Stored ONCE in a master list.
struct Road {
    int u, v;              // the two intersections this road connects
    int distance;           // distance weight
    TrafficLevel traffic;   // Low / Medium / High
};


// The whole city road network (the Graph).
class CityTrafficGraph {
private:
    int totalIntersections;
    vector<string> names;
    vector<Road> roads;                // master list of every road
    vector<vector<int>> adjRoadIndex;  // node -> list of road indices touching it

public:
    CityTrafficGraph(int n) {
        totalIntersections = n;
        names.assign(n, "");
        adjRoadIndex.assign(n, {});
    }

    void setIntersectionName(int id, string name) { names[id] = name; }
    string nameOf(int id) { return names[id]; }
    int size() { return totalIntersections; }

    int otherEnd(const Road &r, int atNode) {
        return (r.u == atNode) ? r.v : r.u;
    }

    int findRoadIndex(int u, int v) {
        for (int i = 0; i < (int)roads.size(); i++) {
            if ((roads[i].u == u && roads[i].v == v) ||
                (roads[i].u == v && roads[i].v == u)) {
                return i;
            }
        }
        return -1;
    }

    // FEATURE 2: Add New Road
    void addRoad(int u, int v, int distance, TrafficLevel traffic) {
        int existing = findRoadIndex(u, v);
        if (existing != -1) {
            roads[existing].distance = distance;
            roads[existing].traffic = traffic;
            cout << "\nA road already existed between these intersections - updated it instead.\n";
            return;
        }
        roads.push_back({u, v, distance, traffic});
        int idx = (int)roads.size() - 1;
        adjRoadIndex[u].push_back(idx);
        adjRoadIndex[v].push_back(idx);
        cout << "\nRoad added: " << names[u] << " <-> " << names[v]
             << " | distance: " << distance
             << " | traffic: " << levelToText(traffic) << "\n";
    }

    // FEATURE 1: Display City Road Network
    void displayNetwork() {
        cout << "\n----- CITY ROAD NETWORK -----\n";
        for (int node = 0; node < totalIntersections; node++) {
            cout << names[node] << " -> ";
            if (adjRoadIndex[node].empty()) cout << "(no roads yet)";
            for (int idx : adjRoadIndex[node]) {
                Road &r = roads[idx];
                int neighbor = otherEnd(r, node);
                cout << "[" << names[neighbor]
                     << " | distance:" << r.distance
                     << " | traffic:" << levelToText(r.traffic) << "]  ";
            }
            cout << "\n";
        }
    }

    // FEATURE 3: BFS Traversal
    // Visits nearby intersections first, level by level, using a QUEUE.
    void bfsTraversal(int start) {
        vector<bool> visited(totalIntersections, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS starting from " << names[start] << ":\n";
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << names[current] << "  ";

            for (int idx : adjRoadIndex[current]) {
                int neighbor = otherEnd(roads[idx], current);
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << "\n";
    }

    // FEATURE 4: DFS Traversal
    // Goes as deep as possible before backtracking, using a STACK.
    void dfsTraversal(int start) {
        vector<bool> visited(totalIntersections, false);
        stack<int> s;
        s.push(start);

        cout << "\nDFS starting from " << names[start] << ":\n";
        while (!s.empty()) {
            int current = s.top();
            s.pop();

            if (!visited[current]) {
                visited[current] = true;
                cout << names[current] << "  ";
            }

            for (int idx : adjRoadIndex[current]) {
                int neighbor = otherEnd(roads[idx], current);
                if (!visited[neighbor]) s.push(neighbor);
            }
        }
        cout << "\n";
    }

    // FEATURE 5: Find Shortest Route (Dijkstra's Algorithm)
    // A GREEDY algorithm: always expands the closest unvisited node
    // next, using a PRIORITY QUEUE (min-heap).
    void shortestRoute(int source, int destination) {
        vector<int> dist(totalIntersections, INT_MAX);
        vector<int> parent(totalIntersections, -1);
        vector<bool> done(totalIntersections, false);

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (done[u]) continue;
            done[u] = true;

            for (int idx : adjRoadIndex[u]) {
                Road &r = roads[idx];
                int v = otherEnd(r, u);
                int newDist = dist[u] + r.distance;
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    parent[v] = u;
                    pq.push({newDist, v});
                }
            }
        }

        cout << "\n----- SHORTEST ROUTE: " << names[source] << " -> " << names[destination] << " -----\n";
        if (dist[destination] == INT_MAX) {
            cout << "Not reachable.\n";
            return;
        }

        vector<int> path;
        int node = destination;
        while (node != -1) {
            path.push_back(node);
            node = parent[node];
        }
        cout << "Total distance: " << dist[destination] << "\nPath: ";
        for (int i = (int)path.size() - 1; i >= 0; i--) {
            cout << names[path[i]];
            if (i != 0) cout << " -> ";
        }
        cout << "\n";
    }

    // FEATURE 6: Update Traffic Density
    // Finds the road with Linear Search, then updates its level.
    void updateTrafficDensity(int u, int v, TrafficLevel newLevel) {
        int idx = findRoadIndex(u, v); // Linear Search
        if (idx == -1) {
            cout << "\nNo direct road found between those two intersections.\n";
            return;
        }
        roads[idx].traffic = newLevel;
        cout << "\nTraffic on " << names[u] << " <-> " << names[v]
             << " updated to " << levelToText(newLevel) << "\n";
    }

    // FEATURE 7: Show Congested Roads
    // Filters roads with High traffic, then ranks them by distance
    void showCongestedRoads() {
        vector<int> congestedIdx;
        for (int i = 0; i < (int)roads.size(); i++) {
            if (roads[i].traffic == HIGH) congestedIdx.push_back(i);
        }

        // Selection Sort
        int n = (int)congestedIdx.size();
        for (int i = 0; i < n - 1; i++) {
            int maxPos = i;
            for (int j = i + 1; j < n; j++) {
                if (roads[congestedIdx[j]].distance > roads[congestedIdx[maxPos]].distance)
                    maxPos = j;
            }
            swap(congestedIdx[i], congestedIdx[maxPos]);
        }
       

        cout << "\n----- CONGESTED ROADS (High traffic), longest first -----\n";
        if (congestedIdx.empty()) {
            cout << "No congested roads right now.\n";
            return;
        }
        for (int idx : congestedIdx) {
            Road &r = roads[idx];
            cout << names[r.u] << " <-> " << names[r.v]
                 << " | distance: " << r.distance << "\n";
        }
    }

    // FEATURE 8: Recommend Signal Timing
    // A simple rule-based lookup: higher traffic level -> more
    void recommendSignalTiming(int u, int v) {
        int idx = findRoadIndex(u, v); // Linear Search
        if (idx == -1) {
            cout << "\nNo direct road found between those two intersections.\n";
            return;
        }

        int greenTimeSeconds;
        switch (roads[idx].traffic) {
            case LOW:    greenTimeSeconds = 15; break;
            case MEDIUM: greenTimeSeconds = 30; break;
            default:     greenTimeSeconds = 45; break; // HIGH
        }

        cout << "\n----- SIGNAL TIMING RECOMMENDATION -----\n";
        cout << "Road: " << names[u] << " <-> " << names[v] << "\n";
        cout << "Traffic level: " << levelToText(roads[idx].traffic) << "\n";
        cout << "Recommended green signal time: " << greenTimeSeconds << " seconds\n";
    }

    // FEATURE 9: Search Intersection
    // Uses Linear Search to find an intersection by name then shows every road connected to it.
    void searchIntersection(string query) {
        for (auto &c : query) c = tolower(c); // make search case-insensitive

        for (int id = 0; id < totalIntersections; id++) {
            string nameLower = names[id];
            for (auto &c : nameLower) c = tolower(c);

            if (nameLower == query) {
                cout << "\nFound: " << names[id] << " (id " << id << ")\n";
                cout << "Connected roads:\n";
                if (adjRoadIndex[id].empty()) {
                    cout << "  (none yet)\n";
                }
                for (int idx : adjRoadIndex[id]) {
                    Road &r = roads[idx];
                    int neighbor = otherEnd(r, id);
                    cout << "  -> " << names[neighbor]
                         << " | distance: " << r.distance
                         << " | traffic: " << levelToText(r.traffic) << "\n";
                }
                return;
            }
        }
        cout << "\nNo intersection found with that name.\n";
    }

    // FEATURE 10: System Report
    // Simple aggregation/statistics over the whole network.
    void systemReport() {
        int totalRoads = (int)roads.size();
        int totalDistance = 0;
        int lowCount = 0, mediumCount = 0, highCount = 0;

        for (auto &r : roads) {
            totalDistance += r.distance;
            if (r.traffic == LOW) lowCount++;
            else if (r.traffic == MEDIUM) mediumCount++;
            else highCount++;
        }

        // Find the intersection with the most connected roads (highest degree)
        int busiestNode = 0;
        for (int i = 1; i < totalIntersections; i++) {
            if (adjRoadIndex[i].size() > adjRoadIndex[busiestNode].size())
                busiestNode = i;
        }

        cout << "\n----- SYSTEM REPORT -----\n";
        cout << "Total intersections     : " << totalIntersections << "\n";
        cout << "Total roads              : " << totalRoads << "\n";
        cout << "Total road distance      : " << totalDistance << "\n";
        cout << "Low traffic roads        : " << lowCount << "\n";
        cout << "Medium traffic roads     : " << mediumCount << "\n";
        cout << "High traffic roads       : " << highCount << "\n";
        if (totalRoads > 0) {
            double congestedPercent = (highCount * 100.0) / totalRoads;
            cout << "Percentage congested     : " << congestedPercent << " %\n";
        }
        cout << "Most connected intersection: " << names[busiestNode]
             << " (" << adjRoadIndex[busiestNode].size() << " roads)\n";
    }
};

// Builds a small sample city (6 intersections) so the project can be
// demoed immediately without typing everything by hand.

CityTrafficGraph buildSampleCity() {
    CityTrafficGraph city(6);
    city.setIntersectionName(0, "MG Road");
    city.setIntersectionName(1, "Station Square");
    city.setIntersectionName(2, "City Mall");
    city.setIntersectionName(3, "Hospital Junction");
    city.setIntersectionName(4, "College Gate");
    city.setIntersectionName(5, "Railway Crossing");

    // addRoad(u, v, distance, trafficLevel)
    city.addRoad(0, 1, 5, MEDIUM);
    city.addRoad(0, 2, 3, HIGH);
    city.addRoad(1, 3, 4, LOW);
    city.addRoad(2, 3, 6, MEDIUM);
    city.addRoad(2, 4, 2, HIGH);
    city.addRoad(3, 5, 7, LOW);
    city.addRoad(4, 5, 3, HIGH);

    return city;
}

int askNodeId(CityTrafficGraph &city, const string &prompt) {
    int id;
    cout << prompt << " (0 to " << city.size() - 1 << "): ";
    cin >> id;
    return id;
}

// MAIN - terminal menu with all 11 features.

int main() {
    cout << "=====================================================\n";
    cout << "   SMART TRAFFIC SIGNAL OPTIMIZER (Terminal Project)  \n";
    cout << "=====================================================\n";
    cout << "A sample city with 6 intersections has been loaded.\n";

    CityTrafficGraph city = buildSampleCity();

    int choice;
    do {
        cout << "\nChoose one of the following operations:\n";
        cout << "1. Display City Road Network -> View all intersections, connected roads, distances and traffic levels.\n\n";
        cout << "2. Add New Road -> Add a new road between two intersections with distance and traffic density.\n\n";
        cout << "3. Breadth First Search (BFS) -> Traverse the city level by level from a selected intersection.\n\n";
        cout << "4. Depth First Search (DFS) -> Explore all connected intersections using DFS traversal.\n\n";
        cout << "5. Find Shortest Route -> Find the shortest path between two intersections using Dijkstra's Algorithm.\n\n";
        cout << "6. Update Traffic Density -> Change the traffic level (Low / Medium / High) of an existing road.\n\n";
        cout << "7. Show Congested Roads -> Display all roads currently experiencing high traffic.\n\n";
        cout << "8. Recommend Signal Timing -> Suggest green signal duration based on the traffic density of a road.\n\n";
        cout << "9. Search Intersection -> Find an intersection and display all roads connected to it.\n\n";
        cout << "10. System Report -> View overall statistics of the road network and traffic.\n\n";
        cout << "11. Exit -> Close the application.\n\n";
        cout << "Enter your choice (1-11): ";
        cin >> choice;

        switch (choice) {
            case 1:
                city.displayNetwork();
                break;

            case 2: {
                int u = askNodeId(city, "Enter first intersection id");
                int v = askNodeId(city, "Enter second intersection id");
                int distance;
                cout << "Enter road distance: ";
                cin >> distance;
                TrafficLevel level = askTrafficLevel();
                city.addRoad(u, v, distance, level);
                break;
            }

            case 3: {
                int start = askNodeId(city, "Enter starting intersection id");
                city.bfsTraversal(start);
                break;
            }

            case 4: {
                int start = askNodeId(city, "Enter starting intersection id");
                city.dfsTraversal(start);
                break;
            }

            case 5: {
                int source = askNodeId(city, "Enter source intersection id");
                int destination = askNodeId(city, "Enter destination intersection id");
                city.shortestRoute(source, destination);
                break;
            }

            case 6: {
                int u = askNodeId(city, "Enter first intersection id of the road");
                int v = askNodeId(city, "Enter second intersection id of the road");
                TrafficLevel level = askTrafficLevel();
                city.updateTrafficDensity(u, v, level);
                break;
            }

            case 7:
                city.showCongestedRoads();
                break;

            case 8: {
                int u = askNodeId(city, "Enter first intersection id of the road");
                int v = askNodeId(city, "Enter second intersection id of the road");
                city.recommendSignalTiming(u, v);
                break;
            }

            case 9: {
                cin.ignore(); // clear leftover newline before reading a full line
                cout << "Enter intersection name to search: ";
                string query;
                getline(cin, query);
                city.searchIntersection(query);
                break;
            }

            case 10:
                city.systemReport();
                break;

            case 11:
                cout << "Exiting program. Thank you!\n";
                break;

            default:
                cout << "Invalid choice, try again.\n";
        }

    } while (choice != 11);

    return 0;
}
