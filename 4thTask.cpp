#include <iostream>
#include <list>

using namespace std;

class Heap {
private:
    struct Node {
    public:
        int weight;
        int degree;
        Node *parent;
        Node *child;
        Node *next_node;
        Node *prev_node;

        Node(int weight) {
            this->parent = nullptr;
            this->weight = weight;
            this->next_node = nullptr;
            this->degree = 0;
            this->child = nullptr;
            this->prev_node = nullptr;
        }

        Node(Node *another) {
            this->weight = another->weight;
            this->child = another->child;
            this->degree = another->degree;
            this->parent = nullptr;
            this->next_node = nullptr;
            this->prev_node = nullptr;
        }

        void addChild(Node *node) {
            node->next_node = nullptr;
            if (this->child == nullptr) {
                this->child = node;
                this->degree++;
                return;
            }
            auto last_child = this->child;
            while (last_child->next_node != nullptr) {
                last_child = last_child->next_node;
            }
            node->prev_node = last_child;
            last_child->next_node = node;
            this->degree++;
        }

        bool operator<(const Node &node) const {
            return this->weight < node.weight;
        }
    };

    Node *root;

    Heap() {
        this->root = nullptr;
    }

    Heap(Node *node) {
        this->root = node;
    }

public:
    static Heap *make() {
        return new Heap();
    }

    static Node *getMinimum(Heap *heap) {
        Node *min = new Node(INT32_MAX);
        Node *curNode = heap->root;
        while (curNode != nullptr) {
            if (curNode->weight < min->weight)
                min = curNode;
            curNode = curNode->next_node;
        }
        return min;
    }

    static Heap *merge(Heap *first, Heap *second) {
        if (first->root == nullptr) return second;
        if (second->root == nullptr) return first;
        Node *curH1 = first->root;
        Node *curH2 = second->root;
        Heap *result = new Heap();
        if (curH1->degree < curH2->degree) {
            result->root = new Node(curH1);
            curH1 = curH1->next_node;
        } else {
            result->root = new Node(curH2);
            curH2 = curH2->next_node;
        }
        Node *curH = result->root;
        while (curH1 != nullptr && curH2 != nullptr) {
            if (curH1->degree < curH2->degree) {
                curH->next_node = new Node(curH1);
                curH = curH->next_node;
                curH1 = curH1->next_node;
            } else {
                curH->next_node = new Node(curH2);
                curH = curH->next_node;
                curH2 = curH2->next_node;
            }
        }
        if (curH1 == nullptr) {
            curH->next_node = curH2;
        } else {
            curH->next_node = curH1;
        }

        curH = result->root;
        while (curH != nullptr) {
            if (curH->next_node != nullptr && curH->degree == curH->next_node->degree) {
                if (curH->weight < curH->next_node->weight) {
                    curH->next_node->parent = curH;
                    auto tmp = curH->next_node->next_node;
                    curH->addChild(curH->next_node);
                    curH->next_node = tmp;
                } else {
                    curH->parent = curH->next_node;
                    curH->next_node->addChild(curH);
                    curH = curH->parent;
                }
            } else {
                curH = curH->next_node;
            }
        }

        while (result->root->parent != nullptr) {
            result->root = result->root->parent;
        }

        first->root = nullptr;
        second->root = nullptr;
        return result;
    }

    static Heap *insert(Heap *current, int weight) {
        auto singleNodeHeap = new Heap(new Node(weight));
        return merge(current, singleNodeHeap);
    }

    static Node *extractMin(Heap *current) {
        Node *min = Heap::getMinimum(current);
        if (min->prev_node != nullptr) {
            min->prev_node->next_node = min->next_node;
            current->root = min->prev_node;
        } else {
            current->root = min->next_node;
        }
        Heap *remains = new Heap(min->child);

        current->root = Heap::merge(current, remains)->root;

        return min;
    }

    static Node *decreaseKey(Node *node, int value) {
        if (value > node->weight)
            return node;
        node->weight = value;
        auto curNode = node;
        auto curParent = curNode->parent;
        while (curParent != nullptr and curNode->weight < curParent->weight) {
            auto tmp = curNode->weight;
            curNode->weight = curParent->weight;
            curParent->weight = tmp;
            curNode = curParent;
            curParent = curNode->parent;
        }
        return curNode;
    }

    static Node *makeFullCopy(Node *target, Node *source) {
        target->weight = source->weight;
        target->degree = source->degree;
        target->parent = source->parent;
        target->child = source->child;
        target->next_node = source->next_node;
        target->prev_node = source->prev_node;
        return target;
    }

    void delete_node(Heap *heap, Node *node) {
        node = decreaseKey(node, -1);
        extractMin(heap);
    }
};

int main() {
    Heap *heap1 = Heap::make();
    heap1 = Heap::insert(heap1, 10);
    heap1 = Heap::insert(heap1, 15);
    heap1 = Heap::insert(heap1, 5);
    Heap *heap2 = Heap::make();
    heap2 = Heap::insert(heap2, 5);
    heap2 = Heap::insert(heap2, 2);
    heap2 = Heap::insert(heap2, 1);
    auto heap3 = Heap::merge(heap1, heap2);

    auto min = Heap::getMinimum(heap3);
    auto node = min->child->next_node;

}