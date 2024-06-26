#include "Graph.h"

Graph::Graph() {

}

Graph::~Graph() {

}

void Graph::addNode(const std::string &label) {
    Node* node = new Node(label);
    auto* list = new std::list<Node*>;

    nodes.insert(std::make_pair(label, node));
    adjList.insert(std::make_pair(node, list));
}


void Graph::removeNode(const std::string &label) {
    auto it = nodes.find(label);
    if (it == nodes.end())
        return;

    Node* node = it->second;

    for (auto& pair : adjList) {
        pair.second->remove(node);
    }

    auto adjIt = adjList.find(node);
    if (adjIt != adjList.end()) {
        delete adjIt->second;
        adjList.erase(adjIt);
    }

    nodes.erase(it);
    delete node;
}

void Graph::addEdge(const std::string &from, const std::string &to) {
    auto fromNodeIt = nodes.find(from);
    if (fromNodeIt == nodes.end())
        throw std::invalid_argument("Invalid 'from' node");

    auto toNodeIt = nodes.find(to);
    if (toNodeIt == nodes.end())
        throw std::invalid_argument("Invalid 'to' node");

    Node* fromNode = fromNodeIt->second;
    Node* toNode = toNodeIt->second;

    adjList[fromNode]->push_back(toNode);
}

void Graph::removeEdge(const std::string &from, const std::string &to) {
    auto fromNodeIt = nodes.find(from);
    auto toNodeIt = nodes.find(to);
    if (fromNodeIt == nodes.end() || toNodeIt == nodes.end())
        return;

    adjList[fromNodeIt->second]->remove(toNodeIt->second);
}

void Graph::print() {
    for (const auto& sourcePair : adjList) {
        Node* source = sourcePair.first;
        std::list<Node*>* targets = sourcePair.second;

        if (!targets->empty()) {
            std::cout << *source << " is connected to {";
            bool first = true;
            for (const auto& target : *targets) {
                if (!first) {
                    std::cout << ", ";
                }
                std::cout << *target;
                first = false;
            }
            std::cout << "}" << std::endl;
        }
    }
}

void Graph::traverseDepthFirst(const std::string& root) {
    auto visited = new std::set<Node*>;

    auto rootIt = nodes.find(root);
    if (rootIt == nodes.end())
        return;

    Node* rootNode = rootIt->second;

    traverseDepthFirst(rootNode, visited);
}

void Graph::traverseDepthFirst(Graph::Node* root, std::set<Node*>* visited) {
    std::cout << *root;
    visited->insert(root);

    for (auto node : *adjList[root]) {
        if (!visited->contains(node)) {
            traverseDepthFirst(node, visited);
        }
    }
}

void Graph::traverseDepthFirstIterative(const std::string &root) {
    std::stack<Node*> stack;
    std::set<Node*> visited;

    auto rootIt = nodes.find(root);
    if (rootIt == nodes.end())
        return;

    Node* rootNode = rootIt->second;
    stack.push(rootNode);

    while(!stack.empty()) {
        auto current = stack.top();
        stack.pop();
        if (visited.contains(current))
            continue;
        std::cout << *current;
        visited.insert(current);

        for (auto node : *adjList[current])
            if (!visited.contains(node))
                stack.push(node);
    }
}

void Graph::traverseBreadthFirst(const std::string &root) {
    std::queue<Node*> queue;
    std::set<Node*> visited;

    auto rootIt = nodes.find(root);
    if (rootIt == nodes.end())
        return;

    Node* rootNode = rootIt->second;
    queue.push(rootNode);

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (visited.contains(current))
            continue;

        std::cout << *current;
        visited.insert(current);

        for (auto node : *adjList[current])
            if (!visited.contains(node))
                queue.push(node);
    }
}

std::vector<std::string> Graph::topologicalSort() {
    std::set<Node*> visited;
    std::stack<Node*> stack;

    for (const auto& node : nodes)
        topologicalSort(node.second, visited, stack);

    std::vector<std::string> sorted;

    while (!stack.empty()) {
        sorted.push_back(stack.top()->label);
        stack.pop();
    }
    return sorted;
}

void Graph::topologicalSort(Node* node, std::set<Node*>& visited, std::stack<Node*>& stack) {
    if (visited.contains(node))
        return;

    visited.insert(node);

    for (auto n : *adjList[node])
        topologicalSort(n, visited, stack);

    stack.push(node);
}

bool Graph::hasCycle() {
    std::set<Node*> all;
    std::set<Node*> visiting;
    std::set<Node*> visited;

    for (const auto& node : nodes)
        all.insert(node.second);

    while (!all.empty()) {
        auto current = *all.begin();
        if (hasCycle(current, all, visiting, visited)) {
            return true;
        }
    }
    return false;
}

bool Graph::hasCycle(Node *node, std::set<Node*>& all, std::set<Node*>& visiting, std::set<Node*>& visited) {
    all.erase(node);
    visiting.insert(node);

    for (auto n : *adjList[node]) {
        if (visited.contains(n)) {
            continue;
        }

        if (visiting.contains(n)) {
            return true; // cycle detected
        }

        if (hasCycle(n, all, visiting, visited)) {
            return true; // cycle detected in the recursive call
        }
    }

    visiting.erase(node);
    visited.insert(node);
    return false;
}