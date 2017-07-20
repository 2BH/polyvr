#include "graph.h"
#include "core/utils/toString.h"

#include <algorithm>

using namespace OSG;

string toString(Graph::edge& e) {
    return toString(Vec3i(e.from, e.to, e.connection));
}

template<> int toValue(stringstream& ss, Graph::edge& e) {
    Vec3i tmp;
    auto b = toValue(ss, tmp);
    e.from = tmp[0];
    e.to = tmp[1];
    e.connection = Graph::CONNECTION(tmp[2]);
    return b;
}

string toString(Graph::node& n) {
    return toString(n.box) + " " + toString(n.p);
}

template<> int toValue(stringstream& ss, Graph::node& n) {
    bool b = toValue(ss, n.box);
    b = b?toValue(ss, n.p):b;
    return b;
}

#include "core/utils/VRStorage_template.h"


Graph::Graph() {
    storeVecVec("edges", edges);
    storeVec("edgesByID", edgesByID);
    storeVec("nodes", nodes);
}

Graph::~Graph() {}

int Graph::connect(int i, int j, CONNECTION c) {
    //if (i >= int(nodes.size()) || j >= int(nodes.size())) return edge;
    while (i >= int(edges.size())) edges.push_back( vector<edge>() );
    edges[i].push_back(edge(i,j,c,edgesByID.size()));
    edgesByID.push_back(Vec2i(i,j));
    return edgesByID.size()-1;
}

void Graph::disconnect(int i, int j) {
    if (i >= int(nodes.size()) || j >= int(nodes.size())) return;
    if (i >= int(edges.size())) return;
    auto& v = edges[i];
    for (uint k=0; k<v.size(); k++) {
        if (v[k].to == j) {
            v.erase(v.begin()+k);
            break;
        }
    }
}

bool Graph::connected(int i, int j) {
    if (!hasNode(i) || !hasNode(j)) return false;
    if (i >= int(edges.size())) return false;
    for (auto& e : edges[i]) if (e.to == j) return true;
    return false;
}

int Graph::size() { return nodes.size(); }
bool Graph::hasNode(int i) { return (i >= 0 && i < int(nodes.size())); }
bool Graph::hasEdge(int i) { return (i >= 0 && i < int(edgesByID.size())); }
vector< vector< Graph::edge > >& Graph::getEdges() { return edges; }
vector< Graph::node >& Graph::getNodes() { return nodes; }
Graph::node& Graph::getNode(int i) { return nodes[i]; }
Graph::edge& Graph::getEdge(int i) { Vec2i e = edgesByID[i]; return edges[e[0]][e[1]]; }

Graph::edge& Graph::getEdge(int n1, int n2) {
    if (!connected(n1,n2)) return nullEdge;
    for (auto& e : edges[n1]) if (e.to == n2) return e;
    return nullEdge;
}

int Graph::getEdgeID(int n1, int n2) {
    if (!connected(n1,n2)) return -1;
    for (auto& e : edges[n1]) if (e.to == n2) return e.ID;
    return -1;
}

int Graph::getNEdges() {
    int N = 0;
    for (auto& n : edges) N += n.size();
    return N;
}

void Graph::setPosition(int i, posePtr p) {
    if (!p || i >= int(nodes.size()) || i < 0) return;
    nodes[i].p = *p;
    update(i, true);
}

posePtr Graph::getPosition(int i) { auto p = pose::create(); *p = nodes[i].p; return p; }

int Graph::addNode() { nodes.push_back(node()); return nodes.size()-1; }
void Graph::clear() { nodes.clear(); edges.clear(); }
void Graph::update(int i, bool changed) {}
void Graph::remNode(int i) { nodes.erase(nodes.begin() + i); }

Graph::edge::edge(int i, int j, CONNECTION c, int ID) : from(i), to(j), connection(c), ID(ID) {}

//vector<Graph::node>::iterator Graph::begin() { return nodes.begin(); }
//vector<Graph::node>::iterator Graph::end() { return nodes.end(); }
