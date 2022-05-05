#include <iostream>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <string>

using namespace std;

template<typename T>
class Object {
private:
    T *objPtr = nullptr;
    vector<void *> objSmartPointers;

    void printPointers() {
        for (auto & objSmartPointer : objSmartPointers) {
            cout << "   " << objSmartPointer << "\n";
        }
    }

public:
    Object(T *newObj) {
        setPtr(newObj);
    }

    void setPtr(T *newObj) {
        objPtr = newObj;
    }

    T *getObjPtr() {
        return objPtr;
    }

    size_t getObjSmartPointersCount() {
        return objSmartPointers.size();
    }

    void addSmartPointer(void *smartPtr) {
        objSmartPointers.push_back(smartPtr);
    }

    void removeSmartPointer(void *smartPtr) {
        objSmartPointers.erase(remove(objSmartPointers.begin(),
                                           objSmartPointers.end(), smartPtr),
                               objSmartPointers.end());
    }

    void printObjectInfo() {
        cout << "Object info:\n " << objPtr << " Object(" << *objPtr << ")\n  Pointers count: "<<
        objSmartPointers.size() << "\n  Pointers list:\n";
        printPointers();
    }
};

template<typename T>
class SmartPointer {
private:
    Object<T> *obj;
    size_t count;

    void release(SmartPointer<T> *ptr) {
        obj->removeSmartPointer(ptr);
        obj = nullptr;
    }

    void updateCounter() {
        count = obj->getObjSmartPointersCount();
    }

public:
    SmartPointer(Object<T> &obj) {
        this->obj = &obj;
        obj.addSmartPointer(this);
    }

    Object<T> *getObj() {
        return obj;
    }

    size_t getObjPointersCount() {
        updateCounter();
        return count;
    }

    void release() {
        obj->removeSmartPointer(this);
        obj = nullptr;
    }

    SmartPointer &operator=(SmartPointer &otherPtr) {
        if (this != &otherPtr) {
            release(this);
            this->obj = otherPtr.getObj();
            this->obj->addSmartPointer(this);
        }
        return *this;
    }

    void printPointerInfo() {
        cout << "Pointer info:\n Pointer: " << this << "\n Points on: " << getObj()->getObjPtr() <<
        " Object(" << *getObj()->getObjPtr() << ")\n";
    }

    void printObjectInfo() {
        obj->printObjectInfo();
    }

    void printInfo() {
        printPointerInfo();
        printObjectInfo();
        cout << "\n";
    }
};

int main() {
    Object<int> testIntObj(new int(10));
    Object<int> testNewIntObj(new int(42));
    auto testPtr = new SmartPointer(testIntObj);
    auto testPtr1 = new SmartPointer(testIntObj);
    SmartPointer testPtr2(testIntObj);
    SmartPointer testPtr3(testNewIntObj);
    auto testPtr4 = new SmartPointer(testNewIntObj);
    testPtr->printInfo();
    testPtr1->printInfo();
    testPtr2.printInfo();
    testPtr3.printInfo();
    testPtr4->printInfo();
    cout << "Doing some changes\n\n\n\n";
    testPtr->release();
    testPtr = testPtr1;
    testPtr2 = testPtr3;
    testPtr->printInfo();
    testPtr1->printInfo();
    testPtr2.printInfo();
    testPtr3.printInfo();
    testPtr4->printInfo();

}
