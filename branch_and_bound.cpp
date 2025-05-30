#include <iostream>
#include <vector>
#include <queue>

struct Item {
    int value, weight;
    double ratio;
    int index;
};

bool cmp(const Item& a, const Item& b) {
    return a.ratio > b.ratio;
}

struct Node {
    int level;
    int profit;
    int weight;
    double bound;
    std::vector<bool> taken;

    bool operator< (const Node& other) const {
        return bound < other.bound;
    }
};

double bound(Node u, int n, int W, std::vector<Item>& items) {
    if (u.weight >= W) return 0;

    double result = u.profit;
    int totalWeight = u.weight;
    int i = u.level + 1;

    while (i < n && totalWeight + items[i].weight <= W) {
        totalWeight += items[i].weight;
        result += items[i].value;
        i++;
    }

    if (i < n)
        result += (W - totalWeight) * items[i].ratio;

    return result;
}

int main() {
    int n, W;
    std::cin >> n >> W;

    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> items[i].value >> items[i].weight;
        items[i].index = i;
        items[i].ratio = (double)items[i].value / items[i].weight;
    }

    sort(items.begin(), items.end(), cmp);

    std::priority_queue<Node> queue;
    Node u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = bound(u, n, W, items);
    u.taken = std::vector<bool>(n, false);

    queue.push(u);

    int max_profit = 0;
    std::vector<bool> bestTaken(n);

    while (!queue.empty()) {
        u = queue.top(); queue.pop();

        if (u.bound > max_profit)
        {
            v.level = u.level + 1;
            v.taken = u.taken;

            v.weight = u.weight + items[v.level].weight;
            v.profit = u.profit + items[v.level].value;
            v.taken[items[v.level].index] = true;

            if (v.weight <= W && v.profit > max_profit) {
                max_profit = v.profit;
                bestTaken = v.taken;
            }

            v.bound = bound(v, n, W, items);
            if (v.bound > max_profit)
                queue.push(v);

            v.weight = u.weight;
            v.profit = u.profit;
            v.taken = u.taken;
            v.taken[items[v.level].index] = false;
            v.bound = bound(v, n, W, items);
            if (v.bound > max_profit)
                queue.push(v);
        }
    }

    std::cout << max_profit << std::endl;
    for (int i = 0; i < n; ++i)
        if (bestTaken[i])
            std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}