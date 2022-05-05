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

    void setPtr(T *newObj) {
        objPtr = newObj;
    }

    void printPointers() {
        for (auto &objSmartPointer: objSmartPointers) {
            if (objSmartPointer != nullptr) {
                cout << "   " << objSmartPointer << "\n";
            }
        }
    }

public:
    Object(T *newObj) {
        setPtr(newObj);
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
        if (objSmartPointers.empty()) {
            delete objPtr;
            delete this;
        }
    }

    void printObjectInfo() {
        cout << "Object info:\n " << objPtr << " Object(" << *objPtr << ")\n  Pointers count: " <<
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
        if (obj) {
            obj->removeSmartPointer(ptr);
            obj = nullptr;
        }
    }

    void updateCounter() {
        count = obj->getObjSmartPointersCount();
    }

public:
    SmartPointer(Object<T> &obj) {
        this->obj = &obj;
        obj.addSmartPointer(this);
    }

    SmartPointer(SmartPointer &ptr) {
        this->obj = ptr.getObj();
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
    SmartPointer testPtr0(testIntObj);
    SmartPointer testPtr1(testIntObj);
    SmartPointer testPtr2(testIntObj);
    SmartPointer testPtr3(testNewIntObj);
    SmartPointer testPtr4(testNewIntObj);
    testPtr0.printInfo();
    testPtr1.printInfo();
    testPtr2.printInfo();
    testPtr3.printInfo();
    testPtr4.printInfo();
    cout << "\n\n\nDoing some changes\n\n\n\n";
    testPtr0 = *new SmartPointer(testPtr2);
    testPtr0 = *new SmartPointer(testPtr3);
    testPtr2 = testPtr4;
    testPtr0.printInfo();
    testPtr1.printInfo();
    testPtr2.printInfo();
    testPtr3.printInfo();
    testPtr4.printInfo();
}
