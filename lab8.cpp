#include <bits/stdc++.h>

using namespace std;

const int MAX_NUM = 3; //по условию числа до 3

int64_t NextPosition(int64_t num, int64_t pos, const vector<int64_t>& sequence, bool reverse = false) {
    if (sequence.empty() || pos < 0 || pos >= sequence.size()) {
        return 0;
    }

    if (!reverse) {
        while (sequence[pos] != num && pos + 1< sequence.size()) {
            ++pos;
        }
    } else {
        while (pos -1 >= 0 && sequence[pos] != num) {
            --pos;
        }
    }
    
    return pos;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int64_t n;
    cin >> n;
    vector<int64_t> sequence(n);
    vector<int64_t> count(MAX_NUM + 1, 0); 

    for (int64_t i = 0; i < n; ++i) {
        cin >> sequence[i];
        ++count[sequence[i]];
    }

    

    int64_t pos1_end = count[1];           
    int64_t pos2_end = pos1_end + count[2]; 
    int64_t swaps = 0;

    int64_t pos3_1 = NextPosition(1, n - 1, sequence, true);       // Поиск '1' с конца массива
    int64_t pos3_2 = NextPosition(2, pos2_end, sequence);          // Поиск '2' в сегменте троек
    int64_t pos2_1 = NextPosition(1, pos1_end, sequence);          // Поиск '1' в сегменте двоек

    for (int64_t i = 0; i < n; ++i) {
        if (i >= pos1_end && i < pos2_end && sequence[i] != 2) {
            if (pos3_2 != -1) {
                swap(sequence[i], sequence[pos3_2]);
                ++swaps;
                pos3_2 = NextPosition(2, pos3_2, sequence);
            }
        }
        else if (i < pos1_end && sequence[i] != 1) {
            if (sequence[i] == 2) {
                if (pos2_1 != -1) {
                    swap(sequence[i], sequence[pos2_1]);
                    ++swaps;
                    pos2_1 = NextPosition(1, pos2_1, sequence);
                }
            } else if (sequence[i] == 3) {
                if (pos3_1 != -1) {
                    swap(sequence[i], sequence[pos3_1]);
                    ++swaps;
                    pos3_1 = NextPosition(1, pos3_1, sequence, true);
                }
            }
        }
    }
    
    cout << swaps << "\n";
    return 0;
}
