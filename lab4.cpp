#include <bits/stdc++.h>

using namespace std;

void BadChar(const vector<string> &pattern, map<string, int> &badChar) { 
    for (int i = 0; i < (int)pattern.size() - 1; ++i) {
        badChar[pattern[i]] = i;
    }
}

void GoodSuffix(const vector<string> &pattern, vector<int> &goodSuffix) {
    int n = (int)pattern.size();
    int left = 0;
    int right = 0;

    vector<int> zFunction(n, 0);
    for (int i = 1; i < n; ++i) {
        if (i <= right)
            zFunction[i] = min(right - i + 1, zFunction[i - left]);

        while (i + zFunction[i] < n && (pattern[n - 1 - zFunction[i]] == pattern[n - 1 - (i + zFunction[i])])) {
            zFunction[i]++;
        }

        if (i + zFunction[i] - 1 > right) {
            left = i;
            right = i + zFunction[i] - 1;
        }
    }
    vector<int> N(zFunction.rbegin(), zFunction.rend()); //reverse
    vector<int> L(n + 1, n);
    int j;
    for (int i = 0; i < n - 1; ++i) {
        j = n - N[i];
        L[j] = i;
    }

    std::vector<int> l(n + 1, n);
    for (int i = n - 1; i >= 0; i--) {
        j = n - i;
        if (N[j - 1] == j) {
            l[i] = (j - 1);
        } else {
            l[i] = l[i + 1];
        }
    }

    for (int i = 0; i < n + 1; ++i) {
        if (L[i] == n) {
            L[i] = l[i];
        }
    }
        

    for (auto &x : L) {
        if (x != n)  {
            x = n - 1 - x;
        }
    }

    goodSuffix = L;
}

void Lowercase(string &str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 'a' - 'A';
        }
    }
}

void get(vector<string> &pattern, vector<string> &text, vector<pair<int, int>> &rowCol) {
    string curLine;
    getline(std::cin, curLine, '\n');
    istringstream stringStream(curLine);
    string curWord;
    while (stringStream >> curWord) {
        Lowercase(curWord);
        pattern.push_back(curWord);
    }
    int lineNum = 1;
    int wordNum = 0;
    int lineLen;
    int left;
    pair<int, int> lineNumWordNum;
    while (getline(cin, curLine, '\n')) {
        wordNum = 1;
        lineLen = curLine.size();
        for (int curPos = 0; curPos < lineLen;) {
            if (isalpha(curLine[curPos])) {
                left = curPos;
                while (isalpha(curLine[curPos]) && curPos < lineLen) {
                 ++curPos;
                }
                lineNumWordNum.first = lineNum;
                lineNumWordNum.second = wordNum++;
                rowCol.push_back(lineNumWordNum);

                curWord = curLine.substr(left, curPos - left);
                Lowercase(curWord);
                text.push_back(curWord);
            }
            ++curPos;
        }
        ++lineNum;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> pattern;
    vector<string> text;
    vector<pair<int, int>> rowCol;  
    auto begin = std::chrono::steady_clock::now();
    get(pattern, text, rowCol);

    map<string, int> badChar;
    BadChar(pattern, badChar);

    vector<int> goodSuffix(pattern.size() + 1, pattern.size());
    GoodSuffix(pattern, goodSuffix);

    vector<int> occurPos;
    int j, bound = 0, shift = 0;
    for (int i = 0; i < 1 + (int)text.size() - (int)pattern.size();) {
        for (j = pattern.size() - 1; j >= bound; j--) {
            if (pattern[j] != text[i + j]) {
                break;
            }
        }
        if (j < bound) {
            occurPos.push_back(i);
            bound = pattern.size() - goodSuffix[0];
            j = -1;
        } else {
            bound = 0;
        }
        if (j < bound) {
            shift = goodSuffix[j + 1];
        } else {
            shift =
                max(max(1, goodSuffix[j + 1]), j - badChar[text[i + j]]);
        }
        i += shift;
    }

    //for (const auto &oc : occurPos)
      //  cout << rowCol[oc].first << ",  " << rowCol[oc].second << '\n';
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    return 0;
}