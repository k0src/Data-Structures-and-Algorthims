#include "QueueReverser.h"

void QueueReverser::reverse(std::queue<int>& q) {
    std::stack<int> s;

    while (!q.empty()) {
        s.push(q.front());
        q.pop();
    }

    while (!s.empty()) {
        q.push(s.top());
        s.pop();
    }
}