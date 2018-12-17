#include <iostream>
#include <map>
#include <vector>
#include <fstream>

std::string str;
const size_t MAX_STRING_LENGTH = 400000 + 50;
const size_t MAX_ALPHABET_SIZE = 160;
std::vector<int> pos(MAX_STRING_LENGTH);
std::vector<int> lcp(MAX_STRING_LENGTH);
std::vector<int> mSuffArray(MAX_STRING_LENGTH);
std::vector<int> mCnt(MAX_STRING_LENGTH);
std::vector<int> mClasses(MAX_STRING_LENGTH);

int getCharacterCode(char ch) {
    return ch - '$';
}

void buildArray(std::string str) {

    std::vector<int> suffArrayTemp;
    std::vector<int> classesTemp;

    suffArrayTemp.resize(MAX_STRING_LENGTH);
    classesTemp.resize(MAX_STRING_LENGTH);

    // Zero phase

    for (size_t i = 0; i < str.size(); ++i) {
        ++mCnt[getCharacterCode(str[i])];
    }
    for (size_t i = 1; i < MAX_ALPHABET_SIZE; ++i) {
        mCnt[i] += mCnt[i - 1];
    }
    for (int i = 0; i < str.size(); ++i) {
        mSuffArray[--mCnt[getCharacterCode(str[i])]] = i;
    }
    mClasses[mSuffArray[0]] = 0;
    int count = 1;
    for (size_t i = 1; i < str.size(); ++i) {
        if (str[mSuffArray[i - 1]] != str[mSuffArray[i]]) {
            ++count;
        }
        mClasses[mSuffArray[i]] = count - 1;
    }

    //Other phases

    for (int k = 0; (1 << k) < str.size(); ++k) {
        int cur = 1 << k;
        for (int i = 0; i < str.size(); ++i) {
            suffArrayTemp[i] = mSuffArray[i] - cur;
            if (suffArrayTemp[i] < 0) {
                suffArrayTemp[i] += str.size();
            }
        }
        mCnt.resize(static_cast<unsigned long>(count));
        std::fill(mCnt.begin(), mCnt.end(), 0);
        for (int i = 0; i < str.size(); ++i) {
            ++mCnt[mClasses[suffArrayTemp[i]]];
        }
        for (int i = 1; i < count; ++i) {
            mCnt[i] += mCnt[i - 1];
        }
        for (auto i = static_cast<int>(str.size() - 1); i >= 0; --i) {
            mSuffArray[--mCnt[mClasses[suffArrayTemp[i]]]] = suffArrayTemp[i];
        }
        classesTemp[mSuffArray[0]] = 0;
        count = 1;
        for (int i = 1; i < str.size(); ++i) {
            size_t mid1 = (mSuffArray[i] + cur) % str.size();
            size_t mid2 = (mSuffArray[i - 1] + cur) % str.size();
            if (mClasses[mSuffArray[i]] != mClasses[mSuffArray[i - 1]] || mClasses[mid1] != mClasses[mid2]) {
                ++count;
            }
            classesTemp[mSuffArray[i]] = count - 1;
        }

        mClasses = classesTemp;


    }
}

std::vector<int> calculateLCP() {
    for (int i = 0; i < str.size(); ++i) {
        pos[mSuffArray[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < str.size(); ++i) {
        if (k > 0) {
            --k;
        }
        if (pos[i] == static_cast<int>(str.size() - 1)) {
            lcp[str.size() - 1] = -1;
            k = 0;
        } else {
            int cur = mSuffArray[pos[i] + 1];
            while (std::max(i + k, cur + k) < str.size() &&
                   str[i + k] == str[cur + k]) {
                ++k;
            }
            lcp[pos[i]] = k;
        }

    }
    return lcp;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::cin >> str;
    str += '$';
    buildArray(str);
    for (int i = 1; i < str.length(); ++i) {
        std::cout << mSuffArray[i] + 1 << " ";
    }
    std::cout << std::endl;
    std::vector<int> lcp = calculateLCP();
    for (int i = 1; i < str.length() - 1; ++i) {
        std::cout << lcp[i] << " ";
    }

    return 0;
}
