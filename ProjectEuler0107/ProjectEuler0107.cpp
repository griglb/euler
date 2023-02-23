// The following undirected network consists of seven verticesand twelve edges with a total weight of 243.
//
//                  B    20   E
//                  *---------*
//                 / \       / \
//             16 /   \     /   \11
//               /   17\   /18   \
//              /       \ /       \
//            A*---------*---------*G
//              \  21   /D\   23  /
//               \     /   \     /
//              12\   /28 19\   /27
//                 \ /       \ /
//                  *---------*
//                  C    31   F
//
// The same network can be represented by the matrix below.
//              A   B   C   D   E   F   G
//          A   -  16  12  21   -   -   -
//          B  16   -   -  17  20   -   -
//          C  12   -   -  28   -  31   -
//          D  21  17  28   -  18  19  23
//          E   -  20   -  18   -   -  11
//          F   -   -  31  19   -   -  27
//          G   -   -   -  23  11  27   -
//
// However, it is possible to optimise the network by removing some edges
// and still ensure that all points on the network remain connected.
// The network which achieves the maximum saving is shown below.
// It has a weight of 93, representing a saving of 243 - 93  = 150 from the original network.
//
//                  B         E
//                  *         *
//                 / \       / \
//             16 /   \     /   \11
//               /   17\   /18   \
//              /       \ /       \
//            A*         *         *G
//              \        D\
//               \         \
//              12\       19\
//                 \         \
//                  *         *
//                  C         F
//
// Using network.txt(right click and 'Save Link/Target As...'), a 6K text
// file containing a network with forty vertices, and given in matrix form,
// find the maximum saving which can be achieved by removing redundant
// edges whilst ensuring that the network remains connected.


#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>


using Cost = int64_t;
using Vector = std::vector<Cost>;
using Matrix = std::vector<Vector>;


Matrix get_small_network() {
    return { { -1, 16, 12, 21, -1, -1, -1 },
             { 16, -1, -1, 17, 20, -1, -1 },
             { 12, -1, -1, 28, -1, 31, -1 },
             { 21, 17, 28, -1, 18, 19, 23 },
             { -1, 20, -1, 18, -1, -1, 11 },
             { -1, -1, 31, 19, -1, -1, 27 },
             { -1, -1, -1, 23, 11, 27, -1 } };
}


Matrix get_big_network() {
#ifdef _CONSOLE
    std::ifstream infile("./p107_network.txt");
#else
    std::ifstream infile("./ProjectEuler0107/p107_network.txt");
#endif

    Matrix ret;

    while (!infile.eof()) {
        std::string line;
        infile >> line;
        std::stringstream str{ line };

        Vector this_row;

        while (!str.eof()) {
            char symbol;
            str >> symbol;
            if ('-' == symbol)
                this_row.push_back(-1);
            else {
                str.putback(symbol);
                Cost value;
                str >> value;
                this_row.push_back(value);
            }
            char comma;
            str >> comma;
        }
        ret.push_back(this_row);
    }
    ret.pop_back();

    for (size_t r = 0; r < ret.size(); ++r) {
        if (ret[r].size() != ret.size())
            std::cout << "Row " << r << " has wrong number of elements" << std::endl;
        for (size_t c = 0; c < r; ++c)
            if (ret[r][c] != ret[c][r])
                std::cout << "What?!, row = " << r << ", column = " << c << std::endl;
    }

    return ret;
}


Cost get_network_cost(const Matrix& graph) {
    Cost cost{ 0 };
    for (size_t r = 0; r < graph.size(); ++r) {
        for (size_t c = 0; c < r; ++c) {
            if (graph[r][c] > 0)
                cost += graph[r][c];
        }
    }

    return cost;
}


bool found_cycle(const Matrix& network, std::vector<Cost>& path, std::vector<bool>& visited, size_t &num_nodes) {
    size_t graph_size = network.size();

    const auto curr_node = path[num_nodes - 1];
    const auto& row = network[curr_node];

    // See if any there are any edges from last_node to a node in nodes_left to complete a cycle
    for (size_t ind = 0; ind < graph_size; ++ind) {
        // No double backs to the node that got me to last_node
        if ((num_nodes > 1) && (path[num_nodes - 2] == ind))
            continue;
        // Can we move from curr_node to ind?
        if (row[ind] > 0) {
            // Have we already been to ind?
            if (visited[ind])
                return true;

            path[num_nodes++] = ind;
            visited[ind] = true;

            if (found_cycle(network, path, visited, num_nodes))
                return true;

            path[--num_nodes] = -1;
            visited[ind] = false;
        }
    }

    return false;
}


bool is_spanning_tree(const Matrix& network) {
    size_t graph_size = network.size();

    std::vector<Cost> path(graph_size, -1);
    std::vector<bool> visited(graph_size, false);

    for (size_t start = 0; start < graph_size; ++start) {
        size_t num_nodes{ 0 };
        path[num_nodes++] = start;
        visited[start] = true;

        if (found_cycle(network, path, visited, num_nodes))
            return false;

        path[--num_nodes] = -1;
        visited[start] = false;
    }

    return true;
}


bool is_connected(const Matrix& network) {
    for (const auto& row : network) {
        bool connected{ false };
        for (const auto& el : row) {
            connected |= el > 0;
        }
        if (!connected)
            return false;
    }

    return true;

}


using MatrixStack = std::vector<Matrix>;
bool try_remove_edge(MatrixStack& graph_stack, Cost cost_limit) {
    std::cout << "stack depth = " << graph_stack.size() << std::endl;
    using Cell = std::pair<size_t, size_t>;
    std::set<Cell> most_expensive_cells;
    Cost max_cost{ 0 };

    Matrix next_graph{ graph_stack.back() };

    size_t graph_size = next_graph.size();
    for (size_t r = 0; r < graph_size; ++r) {
        for (size_t c = 0; c < r; ++c) {
            const Cost& cost = next_graph[r][c];
            if ((cost < 0) || (cost > cost_limit))
                continue;
            if (cost > max_cost) {
                max_cost = cost;
                most_expensive_cells.clear();
                most_expensive_cells.insert({ r, c });
            }
            else if (cost == max_cost) {
                most_expensive_cells.insert({ r, c });
            }
        }
    }

    for (const auto& cell : most_expensive_cells) {
        // Try removing the most expensive edge
        next_graph[cell.first][cell.second] = -1;
        next_graph[cell.second][cell.first] = -1;

        if (!is_connected(next_graph))
            continue;

        graph_stack.push_back(next_graph);

        // See if removing this edge makes the graph a spanning tree.
        if (is_spanning_tree(next_graph))
            return true;

        // Recurse
        if (try_remove_edge(graph_stack, max_cost - 1))
            return true;

        // Removing this edge failed, mark it as such and restore it
        graph_stack.pop_back();
    }

    return false;
}


Matrix get_minimal_spanning_tree(const Matrix& in_network) {
    MatrixStack stack;
    stack.push_back(in_network);

    try_remove_edge(stack, 9999999);

    return stack.back();
}


Matrix get_minimal_spanning_tree_prim(const Matrix& in_network) {
    size_t graph_size = in_network.size();
    // Initialize graph as all disconnected vertices.
    Matrix ret(graph_size, Vector(graph_size, -1));

    // Can start with random, so use first vertex.
    std::set<size_t> verts_used{ 0 } , verts_left;
    for (size_t i = 1; i < graph_size; ++i)
        verts_left.insert(i);

    for (size_t i = 1; i < graph_size; ++i) {
        Cost min_cost{ 9999999 };
        std::pair<size_t, size_t> min_edge{ 0, 0 };
        for (const auto& out: verts_left) {
            for (const auto& in : verts_used) {
                Cost cost = in_network[in][out];
                if ((cost > 0) && (cost < min_cost)) {
                    min_cost = cost;
                    min_edge = { in, out };
                }
            }
        }

        ret[min_edge.first][min_edge.second] = in_network[min_edge.first][min_edge.second];
        ret[min_edge.second][min_edge.first] = in_network[min_edge.second][min_edge.first];

        verts_left.erase(min_edge.second);
        verts_used.insert(min_edge.second);
    }

    return ret;
}


int main()
{
    std::cout << "Hello World!\n";

    {
        auto mat = get_small_network();
        for (const auto& row : mat) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }
        std::cout << "cost = " << get_network_cost(mat) << std::endl;
        std::cout << "is spanning_tree = " << is_spanning_tree(mat) << std::endl;

        auto tree = get_minimal_spanning_tree_prim(mat);
        for (const auto& row : tree) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }
        std::cout << "cost = " << get_network_cost(tree) << std::endl;
    }

    //{
    //    Matrix solved{ { -1, 16, 12, -1, -1, -1, -1 },
    //                   { 16, -1, -1, 17, -1, -1, -1 },
    //                   { 12, -1, -1, -1, -1, -1, -1 },
    //                   { -1, 17, -1, -1, 18, 19, -1 },
    //                   { -1, -1, -1, 18, -1, -1, 11 },
    //                   { -1, -1, -1, 19, -1, -1, -1 },
    //                   { -1, -1, -1, -1, 11, -1, -1 } };
    //    std::cout << "cost = " << get_network_cost(solved) << std::endl;
    //    std::cout << "is spanning_tree = " << is_spanning_tree(solved) << std::endl;
    //}

    {
        auto mat = get_big_network();
        for (const auto& row : mat) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }
        std::cout << "cost = " << get_network_cost(mat) << std::endl;

        auto tree = get_minimal_spanning_tree_prim(mat);
        for (const auto& row : tree) {
            for (const auto& el : row)
                std::cout << el << "  ";
            std::cout << std::endl;
        }
        std::cout << "cost = " << get_network_cost(tree) << std::endl;
        std::cout << "savings = " << get_network_cost(mat) - get_network_cost(tree) << std::endl;
    }
}
