#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
using namespace std;

//Tính khoảng cách trong gps location
double toRadian(double degree) {
    return degree * M_PI / 180.0;
}
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371000.0; 
    double dLat = toRadian(lat2 - lat1);
    double dLon = toRadian(lon2 - lon1);
    lat1 = toRadian(lat1);
    lat2 = toRadian(lat2);
    double a = sin(dLat / 2.0) * sin(dLat / 2.0) +
               cos(lat1) * cos(lat2) *
               sin(dLon / 2.0) * sin(dLon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c; 
}




bool isRoad(string highway) {
    return highway == "motorway" ||highway == "trunk" ||highway == "primary" ||highway == "secondary" ||highway == "tertiary" ||highway == "residential" ||highway == "service";
}
struct Node {
    long long id;
    double lat;
    double lon;
};
struct edge {
    
};
void addEdgeDirected(vector<long long> graph[], int source, int destination)
{
    graph[source].push_back(destination);
    graph[destination].push_back(source);
};
void addEdgeUnDirected(vector<long long> graph[], int source, int destination)
{
    graph[source].push_back(destination);
};




void printGraph(vector <long long> graph[])
{
    for (int source =0;source<graph.size();source++)
    {
        for (int neighbor : graph[source]);
    }
}

int main()
{
    vector<long long> graph[10000];
    addEdgeUnDirected(graph, 0,1);
    addEdgeUnDirected(graph, 1, 2);
}
