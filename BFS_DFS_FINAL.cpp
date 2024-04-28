#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Define the graph as an adjacency list
class Graph {
public:
    vector<vector<int>> adjList; // Adjacency list

    Graph(int vertices) {
        adjList.resize(vertices);
    }

    void addEdge(int u, int v) {
        // Add an undirected edge between vertices u and v
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
};

// Sequential BFS
void sequentialBFS(Graph& graph, int start) {
    int vertices = graph.adjList.size();
    vector<bool> visited(vertices, false);
    queue<int> q;

    // Start BFS from the starting node
    visited[start] = true;
    q.push(start);

    cout << "Sequential BFS Order: ";

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << current << " ";

        // Visit neighbors of the current node
        for (int neighbor : graph.adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    cout << endl;
}

// Parallel BFS
void parallelBFS(Graph& graph, int start) {
    int vertices = graph.adjList.size();
    vector<bool> visited(vertices, false);
    queue<int> q;

    // Start BFS from the starting node
    visited[start] = true;
    q.push(start);

    cout << "Parallel BFS Order: ";

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << current << " ";

        // Parallelize the for loop using OpenMP
        #pragma omp parallel for
        for (int i = 0; i < graph.adjList[current].size(); i++) {
            int neighbor = graph.adjList[current][i];
            if (!visited[neighbor]) {
                // Use a critical section to protect the modification of visited and queue
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    cout << endl;
}

// Sequential DFS
void sequentialDFS(Graph& graph, int start) {
    int vertices = graph.adjList.size();
    vector<bool> visited(vertices, false);
    stack<int> s;

    // Start DFS from the starting node
    s.push(start);

    cout << "Sequential DFS Order: ";

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        // If not visited, mark as visited and print
        if (!visited[current]) {
            visited[current] = true;
            cout << current << " ";

            // Push the neighbors onto the stack in reverse order
            for (int i = graph.adjList[current].size() - 1; i >= 0; i--) {
                int neighbor = graph.adjList[current][i];
                if (!visited[neighbor]) {
                    s.push(neighbor);
                }
            }
        }
    }

    cout << endl;
}

// Parallel DFS
void parallelDFS(Graph& graph, int start) {
    int vertices = graph.adjList.size();
    vector<bool> visited(vertices, false);
    stack<int> s;

    // Start DFS from the starting node
    s.push(start);

    cout << "Parallel DFS Order: ";

    // Use a mutex for protecting the stack in parallel environment
    #pragma omp parallel
    {
        while (true) {
            int current;

            // Critical section to protect access to the stack
            #pragma omp critical
            {
                if (s.empty()) {
                    current = -1; // No more nodes to process
                } else {
                    current = s.top();
                    s.pop();
                }
            }

            // If current is invalid, break the loop
            if (current == -1) {
                break;
            }

            // If not visited, mark as visited and print
            if (!visited[current]) {
                #pragma omp critical
                {
                    if (!visited[current]) {
                        visited[current] = true;
                        cout << current << " ";
                    }
                }

                // Push the neighbors onto the stack in reverse order
                for (int i = graph.adjList[current].size() - 1; i >= 0; i--) {
                    int neighbor = graph.adjList[current][i];
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    cout << endl;
}

int main() {
    int vertices, edges, start;

    // User input for the graph
    cout << "Enter the number of vertices: ";
    cin >> vertices;
    
    Graph graph(vertices);

    cout << "Enter the number of edges: ";
    cin >> edges;

    cout << "Enter the edges (u v) one by one:\n";
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }

    cout << "Enter the starting node for traversal: ";
    cin >> start;

    // Sequential BFS
    sequentialBFS(graph, start);

    // Parallel BFS
    parallelBFS(graph, start);

    // Sequential DFS
    sequentialDFS(graph, start);

    // Parallel DFS
    parallelDFS(graph, start);

    return 0;
}
