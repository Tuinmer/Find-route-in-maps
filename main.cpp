#include <bits/stdc++.h>
#include <math.h>
#include <windows.h>
#include <cmath>
#include "library/pugixml.hpp"
const double INF = 1e18;
using namespace std;
struct Node {
    long long id;
    double lat;
    double lon;
    string name;
};
struct edge {
    long long to;
    double weight;
    string roadName;
};
vector<string> optimizeRoad;
vector<Node> graphNodes;
vector<pair<double, int>> latIndex;
vector<long long> wayNodes;

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
    return highway == "motorway" ||highway == "trunk" ||highway == "primary" ||highway == "secondary" ||highway == "tertiary" ||highway == "residential" ||highway == "service";
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

pair<double, vector<string>> dijkstra(vector<vector<edge>>& graph,int s, int e)
{
    vector<double> d(graph.size(), INF);
    vector<int> pre(graph.size(), -1);
    if (s==e)
    {
        cout<<"Điểm đầu trùng với điểm cuối";
    }
    d[s] = 0;
    pre[s] = s;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> Q;
    //{khoang cach, dinh}
    
    Q.push({0,s});
    while (!Q.empty())
    {
        pair<double,int> top= Q.top();
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
        for (auto it:graph[u]) // lay may cai du lieu trong graph
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
        return {0, {"0"}};
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
    return {shortestDistance, optimizeRoad};
}
int findLatitudePosition(
    vector<pair<double, int>>& latIndex,
    double lat
)
{
    int left = 0;
    int right = latIndex.size();

    while (left < right)
    {
        int mid = left + (right - left) / 2;

        if (latIndex[mid].first < lat)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }

    return left;
}
int closestNode(vector<Node>& graphNodes,vector<pair<double, int>>& latIndex,double lat,double lon)
{
    int pos = findLatitudePosition(lat);
    int range = 100;
    int start = max(0, pos - range);
    int end = min(
        (int)latIndex.size(),
        pos + range
    );
    int nearest = -1;
    double minDistance = INF;
    for (int i = start; i < end; i++)
    {
        int graphID = latIndex[i].second;
        double distance = calculateDistance(lat,lon,graphNodes[graphID].lat,graphNodes[graphID].lon);
        if (distance < minDistance)
        {
            minDistance = distance;
            nearest = graphID;
        }
    }
    return nearest;
}

int TakeLocFromGps(vector<Node>& graphNodes)
{
    HANDLE hSerial = CreateFileA(
        "\\\\.\\COM7",
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        cout << "Khong mo duoc COM7\n";
        return -1;
    }
    cout << "Da mo COM7!\n";
    cout<<"ok";
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        cout << "Khong lay duoc cau hinh COM7\n";
        CloseHandle(hSerial);
        return -1;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        cout << "Khong set duoc baud COM7\n";
        CloseHandle(hSerial);
        return -1;
    }
    cout << "COM7: 115200 baud\n";

    char buffer[128];
    DWORD bytesRead;
    string line = "";
    while (true)
    {
        if (ReadFile(
            hSerial,
            buffer,
            sizeof(buffer) - 1,
            &bytesRead,
            NULL))
        {
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                line += buffer;

                size_t pos;

                while ((pos = line.find('\n')) != string::npos)
                {
                    string oneLine = line.substr(0, pos);
                    line.erase(0, pos + 1);

                    if (!oneLine.empty() &&
                        oneLine.back() == '\r')
                    {
                        oneLine.pop_back();
                    }

                    cout << "Nhan: [" << oneLine << "]\n";

                    double lat, lon;
                    char comma;

                    stringstream ss(oneLine);

                    if (ss >> lat >> comma >> lon)
                    {
                        cout << "GPS: "
                             << fixed << setprecision(6)
                             << lat << ", "
                             << lon << endl;

                        int nearest = closestNode(
                            graphNodes,
                            lat,
                            lon
                        );
                        cout << "Nearest Graph ID: "
                             << nearest << endl;
                        CloseHandle(hSerial);
                        return nearest;
                    }
                }
            }
        }
    }
}

void loadData(vector<Node>& graphNodes, vector<long long> wayNodes)
{
    pugi::xml_document doc;
    doc.load_file("Data/Saigon.osm");
    pugi::xml_node osm = doc.child("osm");
    unordered_map<long long, Node> nodes;
    unordered_map<long long,int> osmToGraph;
    int graphid=0;
    for (pugi::xml_node node : osm.children("node"))
    {
        long long id = node.attribute("id").as_llong();
        double lat = node.attribute("lat").as_double();
        double lon = node.attribute("lon").as_double();
        string name = "";
        for (pugi::xml_node tag : node.children("tag"))
        {
            string key = tag.attribute("k").as_string();
            string value = tag.attribute("v").as_string();

            if (key == "name")
            {
                name = value;
                break;
            }
        }
        Node newNode;
        newNode.id = id;
        newNode.lat = lat;
        newNode.lon = lon;
        newNode.name = name;
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
                addEdgeOneWay(graph, source, destination, distance, roadName);
            }else addEdgeNotOneWay(graph, source, destination, distance, roadName);
        }
    }
    latIndex.resize(graphNodes.size());
    for (int i = 0; i < graphNodes.size(); i++)
    {
        latIndex.push_back({graphNodes[i].lat,i});
    }
    sort(latIndex.begin(), latIndex.end());


}


int main()
{   
    
}