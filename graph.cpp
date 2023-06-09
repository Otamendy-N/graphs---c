#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
using Vector = vector<T>;

struct node
{
    char name;
    bool checked;
    int distance;
    Vector<node *> connections;
    node *parent;
};

struct connection
{
    char left;
    char right;
    int weight;
};

typedef struct node Node;
typedef Node *NodePtr;

typedef struct connection Connection;

bool is_in_array(char, Vector<NodePtr>);
NodePtr get_smaller_not_checked(Vector<NodePtr>);
NodePtr get_node(char, Vector<NodePtr>);
Vector<NodePtr> generate_graph(Vector<Connection>);
int get_weight_between(char, char, Vector<Connection>);
bool are_all_checked(Vector<NodePtr>);
void calculate_distances_from(char, Vector<NodePtr>, Vector<Connection>);
void calculate_children_distances(NodePtr, Vector<Connection>);
void print_graph(Vector<NodePtr>);
void print_distance_and_path(char, char, Vector<NodePtr> nodes);

int main(int argc, char **argv)
{
    Vector<Connection> connections = {
        {
            .left ='A',
            .right = 'B',
            .weight = 10
        },
        {
            .left ='A',
            .right = 'E',
            .weight = 3
        },
        {
            .left ='B',
            .right = 'C',
            .weight = 2
        },
        {
            .left ='B',
            .right = 'E',
            .weight = 4
        },
        {
            .left ='E',
            .right = 'B',
            .weight = 1
        },
        {
            .left = 'E',
            .right = 'C',
            .weight = 8
        },
        {
            .left = 'E',
            .right = 'D',
            .weight = 2
        },
        {
            .left = 'C',
            .right = 'D',
            .weight = 9
        },
        {
            .left = 'D',
            .right = 'C',
            .weight = 7
        },
    };
    char current_point = 'A';
    char target_point = 'D';

    Vector<NodePtr> nodes = generate_graph(connections);

    // // this is my implementation of dijkstra's algorithm
    calculate_distances_from(current_point, nodes, connections);

    print_distance_and_path(current_point, target_point, nodes);

    return 0;
}

void print_distance_and_path(char start, char target, Vector<NodePtr> nodes)
{
    NodePtr target_node = get_node(target, nodes);
    printf("La distancia mas corta hasta el nodo %c es %d.\n", target, target_node->distance);
    NodePtr current_node = target_node;
    vector<char> path;
    while (current_node != NULL)
    {
        path.push_back(current_node->name);
        current_node = current_node->parent;
    }

    printf("El camino seria:\n");
    reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); i++)
    {
        if (i != path.size() - 1)
            printf("%c->", path[i]);
        else
            printf("%c\n", path[i]);
    }
}

void print_graph(Vector<NodePtr> nodes)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        NodePtr node = nodes[i];
        printf("------------------------------\n");
        printf("name: %c\n", node->name);
        printf("checked: %d\n", node->checked);
        printf("distance: %d\n", node->distance);
        printf("parent: %p\n", (void *)node->parent);

        printf("\nConnections:\n");
        for (Node *connection : node->connections)
        {
            printf("\t%c\n", connection->name);
        }
    }
}

Vector<NodePtr> generate_graph(Vector<Connection> connections)
{
    Vector<NodePtr> nodes;
    for (int i = 0; i < connections.size(); i++)
    {
        NodePtr node_at_left = node_at_left = get_node(connections[i].left, nodes);
        NodePtr node_at_right = node_at_right = get_node(connections[i].right, nodes);
        if (node_at_left == NULL)
        {
            node_at_left = new (Node);
            node_at_left->name = connections[i].left;
            node_at_left->distance = 0;
            node_at_left->checked = false;
            node_at_left->connections = {};
            node_at_left->parent = NULL;
            nodes.push_back(node_at_left);
        }

        if (node_at_right == NULL)
        {
            node_at_right = new (Node);
            node_at_right->name = connections[i].right;
            node_at_right->distance = 0;
            node_at_right->checked = false;
            node_at_right->connections = {};
            node_at_right->parent = NULL;
            nodes.push_back(node_at_right);
        }

        node_at_left->connections.push_back(node_at_right);
    }

    return nodes;
}

bool is_in_array(char character, Vector<NodePtr> nodes)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i] == NULL)
            continue;
        if (nodes[i]->name == character)
            return true;
    }
    return false;
}

NodePtr get_node(char character, Vector<NodePtr> nodes)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i] == NULL)
            continue;
        if (nodes[i]->name == character)
            return nodes[i];
    }
    return NULL;
}

int get_weight_between(char left, char right, Vector<Connection> connections)
{
    for (int i = 0; i < connections.size(); i++)
    {
        if (connections[i].left == left && connections[i].right == right)
            return connections[i].weight;
    }
    return 0;
}

bool are_all_checked(Vector<NodePtr> nodes)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (!nodes[i]->checked)
            return false;
    }
    return true;
}

NodePtr get_smaller_not_checked(Vector<NodePtr> nodes)
{
    NodePtr smaller = NULL;
    for (int i = 0; i < nodes.size(); i++)
    {
        if (smaller == NULL && !nodes[i]->checked)
        {
            smaller = nodes[i];
            continue;
        }
        if (smaller == NULL)
            continue;

        bool is_smaller_not_checked = nodes[i]->distance < smaller->distance && !nodes[i]->checked;

        if (is_smaller_not_checked)
            smaller = nodes[i];
    }
    return smaller;
}

void calculate_distances_from(char current_point, Vector<NodePtr> nodes, Vector<Connection> connections)
{
    NodePtr start = get_node(current_point, nodes);
    if (start == NULL)
        return;
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i]->name == current_point)
        {
            nodes[i]->distance = 0;
            nodes[i]->checked = true;
            nodes[i]->parent = NULL;
            continue;
        }

        if (!is_in_array(nodes[i]->name, start->connections))
            nodes[i]->distance = 10000; // this acts as infinity
        else
        {
            nodes[i]->distance = get_weight_between(current_point, nodes[i]->name, connections);
            nodes[i]->parent = start;
        }
    }

    while (!are_all_checked(nodes))
    {
        NodePtr node = get_smaller_not_checked(nodes);
        if (node == NULL)
            break;
        node->checked = true;
        calculate_children_distances(node, connections);
    }
}

void calculate_children_distances(NodePtr node, Vector<Connection> connections)
{
    for (int i = 0; i < node->connections.size(); i++)
    {
        NodePtr next_node = node->connections[i];
        int distance_through_this_node = node->distance + get_weight_between(node->name, next_node->name, connections);
        if (next_node->distance < distance_through_this_node)
            continue;
        next_node->distance = distance_through_this_node;
        next_node->parent = node;
    }
}
