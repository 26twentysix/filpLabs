#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


template<typename T>
struct BuddyBlock {
    bool free = true;
    BuddyBlock *parentBlock = nullptr;
    BuddyBlock *leftChild = nullptr;
    BuddyBlock *rightChild = nullptr;
    void *startPtr;
    size_t blockSize;
    size_t freeMemory;

    BuddyBlock(void *startPtr, size_t blockSize) {
        this->startPtr = startPtr;
        this->blockSize = blockSize;
        this->freeMemory = blockSize;
    }

    BuddyBlock() {}


};

template<typename T>
class BuddyAllocator {
    size_t memorySize;
    void *memoryStart;
    BuddyBlock<T> rootBlock;
public:
    BuddyAllocator(size_t memSize) {
        memorySize = memSize;
        memoryStart = malloc(memSize);
        rootBlock = BuddyBlock<T>(memoryStart, memSize);
    }

    ~BuddyAllocator() {
        free(memoryStart);
    }

    BuddyBlock<T> *FindFreeBlock(size_t size) {
        auto currBlock = &rootBlock;
        if (size > currBlock->blockSize) {
            cerr << "Cannot allocate memory";
            exit(404);
        }
        size_t currSize = rootBlock.blockSize;
        while (true) {
            if (currBlock->blockSize / 2 < size && currBlock->leftChild == nullptr &&
                currBlock->rightChild == nullptr) {
                break;
            } else {
                if (currBlock->leftChild == nullptr) {
                    currSize = currSize / 2;
                    BuddyBlock<T> *newLeftChild = new BuddyBlock<T>(currBlock->startPtr, currSize);
                    newLeftChild->parentBlock = currBlock;
                    currBlock->leftChild = newLeftChild;
                    currBlock = newLeftChild;
                } else if (currBlock->leftChild->free && currBlock->leftChild->freeMemory >= size) {
                    currBlock = currBlock->leftChild;
                } else if (currBlock->rightChild == nullptr) {
                    currSize = currSize / 2;
                    void *newRightPointer = (void *) (((char *) currBlock->startPtr) + (currBlock->blockSize) / 2);
                    BuddyBlock<T> *newRightChild = new BuddyBlock<T>(newRightPointer, currSize);
                    newRightChild->parentBlock = currBlock;
                    currBlock->rightChild = newRightChild;
                    currBlock = newRightChild;
                } else if (currBlock->rightChild->free && currBlock->rightChild->freeMemory >= size) {
                    currBlock = currBlock->rightChild;
                } else {
                    cerr << "Cannot allocate memory";
                    exit(505);
                }
            }
        }
        return currBlock;
    }

    void *InsertData(T *data, size_t sz) {
        auto freeBlockPtr = FindFreeBlock(sz);
        freeBlockPtr->freeMemory = 0;
        memcpy(freeBlockPtr->startPtr, data, sz);
        freeBlockPtr->free = false;
        auto parentBlock = freeBlockPtr->parentBlock;
        while (parentBlock != nullptr) {
            parentBlock->freeMemory -= sz;
            parentBlock = parentBlock->parentBlock;
        }
        return freeBlockPtr->startPtr;
    }

    void RemoveData(void *data, size_t sz) {
        RemoveDataRecursive(data, &rootBlock, sz);
    }

    void Print() {
        PrintAllDfs(rootBlock);
    }

private:
    void RemoveDataRecursive(void *data, BuddyBlock<T> *current, size_t sz) {

        if (current->startPtr == data && current->leftChild == nullptr) {
            current->free = true;
            current->freeMemory = current->blockSize;
            auto parentBlock = current->parentBlock;
            if (parentBlock != nullptr &&
                parentBlock->leftChild->free && parentBlock->leftChild->freeMemory == parentBlock->blockSize &&
                parentBlock->rightChild->free && parentBlock->rightChild->freeMemory == parentBlock->blockSize) {
                parentBlock->leftChild = nullptr;
                parentBlock->rightChild = nullptr;
            }
        }
        if (current->leftChild != nullptr && current->leftChild->blockSize >= sz) {
            RemoveDataRecursive(data, current->leftChild, sz);
        }
        if (current->rightChild != nullptr && current->rightChild->blockSize >= sz) {
            RemoveDataRecursive(data, current->rightChild, sz);
        }
    }


    void PrintAllRecursive(BuddyBlock<T> current) {
        if (!current.free) {
            T result[999];
            memcpy((void *) result, current.startPtr, current.blockSize - current.freeMemory);
            stringstream ss;
            for (int i = 0; i < sizeof(T) / current.blockSize - current.freeMemory / current.blockSize; i++)
                ss << result[i];
            string r = ss.str();
            cout << r << " : " << current.startPtr << endl;
        }
        if (current.leftChild != nullptr) {
            PrintAllRecursive(*current.leftChild);
        }
        if (current.rightChild != nullptr) {
            PrintAllRecursive(*current.rightChild);
        }
    }


};

int main() {
    auto intBuddyAllocator = new BuddyAllocator<char>(16);
    char a = 'a';
    char b = 'b';
    char c = 'c';
    auto p_a = intBuddyAllocator->InsertData(&a, sizeof(a));
    auto p_b = intBuddyAllocator->InsertData(&b, sizeof(b));
    auto p_c = intBuddyAllocator->InsertData(&c, sizeof(c));
    intBuddyAllocator->Print();
    intBuddyAllocator->RemoveData(p_a, sizeof(a));
    intBuddyAllocator->Print();

}