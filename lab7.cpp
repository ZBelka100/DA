#include <bits/stdc++.h>

using namespace std;

int64_t countMultiplesInRange(int64_t rightLimit, int64_t leftLimit, int64_t divisor) {
    if (rightLimit < leftLimit) {
        return 0;
    }

    int64_t rightBound = rightLimit - rightLimit % divisor;
    int64_t leftBound;
    int64_t remainder = leftLimit % divisor;

    if (remainder == 0) {
        leftBound = leftLimit;
    } else {
        leftBound = leftLimit + divisor - remainder;
    }

    if (leftBound > rightBound || rightBound <= 0) {
        return 0;
    }

    return (rightBound - leftBound) / divisor + 1;
}

int main() {
    string numStr;
    int64_t num = 0, divisor, powerOfTen = 1, totalCount = 0;
    
    cin >> numStr >> divisor;
    
    for (char digit : numStr) {
        num = num * 10 + digit - '0';
        totalCount += countMultiplesInRange(num, powerOfTen, divisor);
        powerOfTen *= 10;
    }
    
    cout << totalCount - (num % divisor == 0) << '\n';
    return 0;
}
