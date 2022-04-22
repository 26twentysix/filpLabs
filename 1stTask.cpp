#include <iostream>
#include <map>
#include <fstream>
#include <filesystem>
#include <cstring>

using namespace std;

class Text {
public:
    static const int blockSize = 1000;
    char block[blockSize]{};
    Text *nextBlock = nullptr;
};

Text *readText(ifstream *file) {
    Text *firstBlock = new Text();
    Text *currentBlock = firstBlock;
    while (file->is_open()) {
        if (file->eof()) break;
        for (char &i: currentBlock->block) {
            if (file->eof()) {
                i = '\0';
                break;
            }
            file->get(i);
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

void compareSpentTime() {
    ifstream file("testText1stTask.txt");
    auto startTime1 = chrono::steady_clock::now();
    Text *textBuffer = readText(&file);
    map<char *, int, cmp_str> charDict = map<char *, int, cmp_str>();
    bufferToMap(textBuffer, charDict);
    auto endTime1 = chrono::steady_clock::now();
    auto elapsed1 = chrono::duration_cast<chrono::milliseconds>(endTime1 - startTime1);
    file.close();
    file.clear();
    file.open("testText1stTask.txt");
    auto startTime2 = chrono::steady_clock::now();
    map<string, int> strDict = map<string, int>();
    createDictFromText(&file, strDict);
    auto endTime2 = chrono::steady_clock::now();
    auto elapsed2 = chrono::duration_cast<chrono::milliseconds>(endTime2 - startTime2);
    cout << "Time spent by 1st algo without output: " << elapsed1.count() << " ms\n" <<
    "Time spent by 2st algo without output:" << elapsed2.count() << " ms\n";

}

int main() {
    compareSpentTime();
}
