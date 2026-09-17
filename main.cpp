#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
#include "library/pugixml.hpp"
const double INF = 1e18;
using namespace std;
struct Node {
    long long id;
    double lat;
    double lon;
};
struct edge {
    long long to;
    double weight;
    string roadName;
};
vector<string> optimizeRoad;
void addEdgeNotOneWay(vector<vector<edge>>& graph,int source,int destination,double weight,string roadName)
{
    graph[source].push_back({destination, weight,roadName});
    graph[destination].push_back({source, weight,roadName});
}
void addEdgeOneWay(vector<vector<edge>>& graph, int source, int destination, double weight, string roadName)
{
    graph[source].push_back({destination, weight,roadName});
};
bool isRoad(string highway) {
    return highway == "motorway" 
    ||highway == "trunk" 
    ||highway == "primary" 
    ||highway == "secondary" 
    ||highway == "tertiary" 
    ||highway == "residential" 
    ||highway == "service";
}
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

void dijkstra(vector<vector<edge>>& graph, vector<Node>& graphNodes,  int s, int e)
{
    vector<double> d(graph.size(), INF);
    vector<int> pre(graph.size(), -1);
    if (s==e)
    {
        return cout<<"Điểm đầu trùng với điểm cuối";
    }
    d[s] = 0;
    pre[s] = s;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q;
    Q.push({0,s});
    while (!Q.empty())
    {
        auto top = Q.top();
        Q.pop();
        long long u = top.second;
        double kc = top.first;
        if (kc > d[u])
        {
            continue;
        }
        if (u==e)
        {
            break;
        }
        for (auto it:graph[u])
        {
            long long v = it.to;
            double w = it.weight;
            if (d[v] > d[u] + w)
            {
                d[v] = d[u] + w;
                Q.push({d[v], v});
                pre[v] = u; // truoc v la u
            }
        }
    }
    double shortestDistance = d[e];
    vector<int> path;
    if (d[e] == INF)
    {
        cout << "Khong co duong di!\n";
        return;
    }
    while(1)
    {
        path.push_back(e);
        if (e==s) break;
        e = pre[e];
    }
    reverse(begin(path), end(path));
    for (int i = 0; i < path.size() - 1; i++)
    {
        int u = path[i];
        int v = path[i + 1];

        for (auto edge : graph[u])
        {
            if (edge.to == v)
            {
                optimizeRoad.push_back(edge.roadName);
                break;
            }
        }
    }
    cout<<"Khoang cach ngan nhat: "<< shortestDistance<<"m\n";
}


int main()
{

    /*Chuyển data xml thành data máy đọc được*/
    pugi::xml_document doc;
    doc.load_file("Data/Saigon.osm");
    pugi::xml_node osm = doc.child("osm");

    unordered_map<long long, Node> nodes;
    unordered_map<long long,int> osmToGraph;
    int graphid=0;
    vector<Node> graphNodes;
    for (pugi::xml_node node : osm.children("node"))
    {
        long long id = node.attribute("id").as_llong();
        double lat = node.attribute("lat").as_double();
        double lon = node.attribute("lon").as_double();
        Node newNode;
        newNode.id = id;
        newNode.lat = lat;
        newNode.lon = lon;
        nodes[id] = newNode;
        osmToGraph[id] = graphid;
        graphNodes.push_back(newNode);
        graphid++;
    }

    vector<vector<edge>> graph(graphid);
    for (pugi::xml_node way : osm.children("way"))
    {
        string highway="";
        string roadName = "No Name";
        
        for (pugi::xml_node tag : way.children("tag"))
        {
            string key = tag.attribute("k").as_string();
            string value = tag.attribute("v").as_string();
            if (key == "highway")
            {
                highway = value;
            }
            if (key == "name")
            {
                roadName = value;
            }
        }
        if (!isRoad(highway)) continue;
        string oneway="no";
        for (pugi::xml_node tag: way.children("tag"))
        {
            string key = tag.attribute("k").as_string();
            string value = tag.attribute("v").as_string();
            if (key == "oneway")
            {
                oneway = value;
                break;
            }
        }
        vector<long long> wayNodes;
        for (pugi::xml_node nd : way.children("nd"))
        {
            long long ref = nd.attribute("ref").as_llong();
            wayNodes.push_back(ref);
        }
        for (int i=0;i<wayNodes.size()-1;i++)
        {
            long long from = wayNodes[i];
            long long to = wayNodes[i+1];
            Node a = nodes[from];
            Node b = nodes[to];
            double distance = calculateDistance(a.lat, a.lon, b.lat , b.lon);
            long long source = osmToGraph[from];
            long long destination = osmToGraph[to];
            if (oneway == "yes")
            {
                addEdgeOneWay(graph, source, destination, distance,roadName);
            }else addEdgeNotOneWay(graph, source, destination, distance,roadName);
        }
    }
    /*Làm phần algorithm dijkstra   graph[1] = {[1,2], [1,3]}*/
    dijkstra(graph,graphNodes,10,15);
    string temp = optimizeRoad[0];
    cout<<optimizeRoad[0]<<"->";
    for(int i=1;i<optimizeRoad.size();i++)
    {
        if (optimizeRoad[i] != temp)
        {
            temp=optimizeRoad[i];
            cout<<" -> "<<optimizeRoad[i];
        }
    }
    return 0;


}
