#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>

std::string str;
std::vector<int> pos(400000 + 50);
std::vector<int> lcp(400000 + 50);

class SuffixArray {
public:
    explicit SuffixArray(std::string& str) {
        mSize = str.size();
        mString = str;
        mSuffArray.resize(MAX_STRING_LENGTH);
        mClasses.resize(MAX_STRING_LENGTH);
        mCnt.resize(MAX_STRING_LENGTH);
        buildArray(str);
    }

    int operator[](size_t i) {
        return mSuffArray[i];
    }

    size_t getSize() {
        return mSize;
    }

    std::string getString() {
        return mString;
    }

private:

    void buildArray(std::string str) {

        std::vector<int> suffArrayTemp;
        std::vector<int> classesTemp;

        suffArrayTemp.resize(MAX_STRING_LENGTH);
        classesTemp.resize(MAX_STRING_LENGTH);

        // Zero phase

        for (size_t i = 0; i < mSize; ++i) {
            ++mCnt[getCharacterCode(str[i])];
        }
        for (size_t i = 1; i < MAX_ALPHABET_SIZE; ++i) {
            mCnt[i] += mCnt[i - 1];
        }
        for (int i = 0; i < mSize; ++i) {
            mSuffArray[--mCnt[getCharacterCode(str[i])]] = i;
        }
        mClasses[mSuffArray[0]] = 0;
        int count = 1;
        for (size_t i = 1; i < mSize; ++i) {
            if (str[mSuffArray[i - 1]] != str[mSuffArray[i]]) {
                ++count;
            }
            mClasses[mSuffArray[i]] = count - 1;
        }

        //Other phases

        for (int k = 0; (1 << k) < mSize; ++k) {
            int cur = 1 << k;
            for (int i = 0; i < mSize; ++i) {
                suffArrayTemp[i] = mSuffArray[i] - cur;
                if (suffArrayTemp[i] < 0) {
                    suffArrayTemp[i] += mSize;
                }
            }
            mCnt.resize(static_cast<unsigned long>(count));
            std::fill(mCnt.begin(), mCnt.end(), 0);
            for (int i = 0; i < mSize; ++i) {
                ++mCnt[mClasses[suffArrayTemp[i]]];
            }
            for (int i = 1; i < count; ++i) {
                mCnt[i] += mCnt[i - 1];
            }
            for (auto i = static_cast<int>(mSize - 1); i >= 0; --i) {
                mSuffArray[--mCnt[mClasses[suffArrayTemp[i]]]] = suffArrayTemp[i];
            }
            classesTemp[mSuffArray[0]] = 0;
            count = 1;
            for (int i = 1; i < mSize; ++i) {
                size_t mid1 = (mSuffArray[i] + cur) % mSize;
                size_t mid2 = (mSuffArray[i - 1] + cur) % mSize;
                if (mClasses[mSuffArray[i]] != mClasses[mSuffArray[i - 1]] || mClasses[mid1] != mClasses[mid2]) {
                    ++count;
                }
                classesTemp[mSuffArray[i]] = count - 1;
            }

            mClasses = classesTemp; /// check


        }
    }

    int getCharacterCode(char ch) {
        return ch - '$';
    }

    const size_t MAX_STRING_LENGTH = 400000 + 50;
    const size_t MAX_ALPHABET_SIZE = 160;
    size_t mSize;
    std::string mString;

    std::vector<int> mSuffArray;
    std::vector<int> mCnt;
    std::vector<int> mClasses;


};

std::vector<int> calculateLCP(SuffixArray& suffixArray) {


    for (int i = 0; i < suffixArray.getSize(); ++i) {
        pos[suffixArray[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < suffixArray.getSize(); ++i) {
        if (k > 0) {
            --k;
        }
        if (pos[i] == static_cast<int>(suffixArray.getSize() - 1)) {
            lcp[suffixArray.getSize() - 1] = -1;
            k = 0;
        } else {
            int cur = suffixArray[pos[i] + 1];
            while (std::max(i + k, cur + k) < suffixArray.getSize() &&
                   suffixArray.getString()[i + k] == suffixArray.getString()[cur + k]) {
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
    SuffixArray suffixArray(str);
    for (int i = 1; i < str.length(); ++i) {
        std::cout << suffixArray[i] + 1 << " ";
    }
    std::cout << std::endl;
    std::vector<int> lcp = calculateLCP(suffixArray);
    for (int i = 1; i < str.length() - 1; ++i) {
        std::cout << lcp[i] << " ";
    }

    return 0;
}
