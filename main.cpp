#include <iostream>
#include <map>
#include <fstream>
#include <filesystem>
#include <cstring>

using namespace std;

class Text {
public:
    static const int blockSize = 1000;
    char block[blockSize];
    Text *nextBlock = nullptr;
};

Text *readText(ifstream *file) {
    Text *firstBlock = new Text();
    Text *currentBlock = firstBlock;
    while (file->is_open()) {
        if (file->eof()) break;
        for (size_t i = 0; i < firstBlock->blockSize; i++) {
            if (file->eof()) {
                currentBlock->block[i] = '\0';
                break;
            }
            file->get(currentBlock->block[i]);
        }
        Text *nextBlock = new Text();
        currentBlock->nextBlock = nextBlock;
        currentBlock = nextBlock;
    }
    return firstBlock;
}

char *appendCharToCharArray(char *array, char a, int len) {
    char *ret = new char[len + 2];
    strcpy(ret, array);
    ret[len] = a;
    ret[len + 1] = '\0';

    return ret;
}

struct cmp_str {
    bool operator()(char const *a, char const *b) const {
        return std::strcmp(a, b) < 0;
    }
};

void bufferToMap(Text *buffer, map<char *, int, cmp_str> &dict) {
    size_t i = 0;
    char *tmpStr;
    int strSize = 0;
    while (buffer->block[i] != '\0') {
        if (isalpha(buffer->block[i])) {
            tmpStr = appendCharToCharArray(tmpStr, tolower(buffer->block[i]), strSize);
            strSize++;
        } else {
            dict[tmpStr] += 1;
            tmpStr = "";
            strSize = 0;
        }
        if (i < Text::blockSize - 1) {
            i++;
        } else {
            buffer = buffer->nextBlock;
            i = 0;
        }
    }
    dict.erase(dict.find("\000"));
}

void printCharDict(map<char *, int, cmp_str> &dict) {
    for (auto const &pair: dict) {
        cout << pair.first << ": " << pair.second << "\n";
    }
}

void printStrDict(map<string, int> &dict) {
    for (auto const &pair: dict) {
        cout << pair.first << ": " << pair.second << "\n";
    }
}

void createDictFromText(ifstream *file, map<string, int> &dict) {
    string tmpStr;
    while (file->is_open()) {
        if (file->eof()) {
            break;
        }
        auto tmpCh = file->get();
        if (isalpha(tmpCh)) {
            tmpStr += tolower(tmpCh);
        } else {
            if (!tmpStr.empty()) {
                dict[tmpStr] += 1;
                tmpStr.clear();
            }
        }

    }
}

int main() {
    ifstream file("testText.txt");
    auto start_time = chrono::steady_clock::now();
    Text *textBuffer = readText(&file);
    map<char *, int, cmp_str> charDict = map<char *, int, cmp_str>();
    bufferToMap(textBuffer, charDict);
    printCharDict(charDict);
    auto end_time = chrono::steady_clock::now();
    auto elapsed_ns = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    ifstream file1("testText.txt");
    auto start_time1 = chrono::steady_clock::now();
    map<string, int> strDict = map<string, int>();
    createDictFromText(&file1, strDict);
    printStrDict(strDict);
    auto end_time1 = chrono::steady_clock::now();
    auto elapsed_ns1 = chrono::duration_cast<chrono::milliseconds>(end_time1 - start_time1);
    cout << elapsed_ns.count() << " ms\n";
    cout << elapsed_ns1.count() << " ms\n";

}
