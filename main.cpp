#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
#include "library/pugixml.hpp"
using namespace std;
struct Node {
    long long id;
    double lat;
    double lon;
};
struct edge {
    long long to;
    double weight;
};

void addEdgeNotOneWay(vector<vector<edge>>& graph,int source,int destination,double weight
)
{
    graph[source].push_back({destination, weight});
    graph[destination].push_back({source, weight});
}
void addEdgeOneWay(vector<vector<edge>>& graph, int source, int destination, double weight)
{
    graph[source].push_back({destination, weight});
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


int main()
{

    /*Chuyển data xml thành data máy đọc được*/
    pugi::xml_document doc;
    doc.load_file("Data/Quan 1 (1).osm");
    pugi::xml_node osm = doc.child("osm");

    unordered_map<long long, Node> nodes;
    unordered_map<long long,int> osmToGraph;
    int graphid=0;

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
        graphid++;
    }

    vector<vector<edge>> graph(graphid);
    for (pugi::xml_node way : osm.children("way"))
    {
        string highway="";
        
        for (pugi::xml_node tag : way.children("tag"))
        {
            string key = tag.attribute("k").as_string();
            string value = tag.attribute("v").as_string();
            if (key == "highway")
            {
                highway = value;
                break;
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
                addEdgeOneWay(graph, source, destination, distance);
            }else addEdgeNotOneWay(graph, source, destination, distance);
        }
    }
    for (int i = 0; i < 10; i++)
    {
    cout << "Node " << i << ": ";

    for (const auto& [to, weight] : graph[i])
    {
        cout << to << " (" << weight << "m) ";
    }

    cout << "\n";
}
    


    return 0;
}
