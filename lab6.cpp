#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

class BigInteger {
private:
    static const int BASE = 100000000;
    static const int BASE_DIGITS = 8;
    std::vector<int> digits;

    void removeLeadingZeros() {
        while (!digits.empty() && digits.back() == 0)
            digits.pop_back();
    }

    int compare(const BigInteger& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size() ? -1 : 1;
        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i] ? -1 : 1;
        }
        return 0;
    }

public:
    BigInteger() {}

    BigInteger(const std::string& str) {
        int pos = 0;
        int len = static_cast<int>(str.length());
        while (pos < len && str[pos] == '0') {
            pos++;
        }
        if (pos == len) {
            digits.push_back(0);
            return;
        }
        for (int i = len - 1; i >= pos; i -= BASE_DIGITS) {
            int x = 0;
            for (int j = std::max(pos, i - BASE_DIGITS + 1); j <= i; ++j) {
                if (!isdigit(str[j])) {
                    throw std::invalid_argument("Invalid character in number");
                }
                x = x * 10 + (str[j] - '0');
            }
            digits.push_back(x);
        }
        removeLeadingZeros();
    }

    BigInteger operator+(const BigInteger& other) const {
        BigInteger result;
        int carry = 0;
        size_t n = std::max(digits.size(), other.digits.size());

        result.digits.reserve(n + 1);

        for (size_t i = 0; i < n || carry; ++i) {
            if (i == result.digits.size())
                result.digits.push_back(0);

            long long sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];

            carry = sum >= BASE;
            if (carry)
                sum -= BASE;

            result.digits[i] = static_cast<int>(sum);
        }

        return result;
    }

    BigInteger operator-(const BigInteger& other) const {
        if (*this < other) {
            throw std::invalid_argument("Subtraction would result in negative value");
        }
        BigInteger result;
        int carry = 0;
        result.digits.resize(digits.size());
        for (size_t i = 0; i < digits.size(); ++i) {
            long long diff = digits[i] - carry - (i < other.digits.size() ? other.digits[i] : 0);
            carry = diff < 0;
            if (carry)
                diff += BASE;
            result.digits[i] = static_cast<int>(diff);
        }
        result.removeLeadingZeros();
        return result;
    }

    BigInteger operator*(const BigInteger& other) const {
        BigInteger result;
        result.digits.resize(digits.size() + other.digits.size());
        for (size_t i = 0; i < digits.size(); ++i) {
            long long carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                long long cur = result.digits[i + j] +
                                digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) +
                                carry;
                result.digits[i + j] = static_cast<int>(cur % BASE);
                carry = cur / BASE;
            }
        }
        result.removeLeadingZeros();
        return result;
    }

    BigInteger operator/(const BigInteger& divisor) const {
        if (divisor == BigInteger("0")) {
            throw std::invalid_argument("Division by zero");
        }

        int norm = BASE / (divisor.digits.back() + 1);
        BigInteger dividend = *this * BigInteger(std::to_string(norm));
        BigInteger div = divisor * BigInteger(std::to_string(norm));
        BigInteger result, current;
        result.digits.resize(dividend.digits.size());

        for (int i = static_cast<int>(dividend.digits.size()) - 1; i >= 0; --i) {
            current.shiftRight();
            current.digits[0] = dividend.digits[i];
            current.removeLeadingZeros();

            int qGuess = 0;
            if (current.digits.size() >= div.digits.size()) {
                long long numerator = 0;
                if (current.digits.size() > div.digits.size()) {
                    numerator = static_cast<long long>(current.digits[div.digits.size()]) * BASE + current.digits[div.digits.size() - 1];
                }
                else {
                    numerator = current.digits.back();
                }
                qGuess = static_cast<int>(numerator / div.digits.back());
            }

            BigInteger qTest = div * BigInteger(std::to_string(qGuess));
            while (qTest > current) {
                --qGuess;
                qTest = div * BigInteger(std::to_string(qGuess));
            }

            result.digits[i] = qGuess;
            current = current - qTest;
        }

        result.removeLeadingZeros();
        return result;
    }

    BigInteger operator^(const BigInteger& other) const {
        if (*this == BigInteger("0") && other == BigInteger("0")) {
            throw std::invalid_argument("0^0 is undefined");
        }
        BigInteger result("1"), base = *this;
        BigInteger exponent = other;
        BigInteger two("2");
        while (exponent > BigInteger("0")) {
            if ((exponent.digits[0] % 2) == 1)
                result = result * base;
            base = base * base;
            exponent = exponent / two;
        }
        return result;
    }

    bool operator<(const BigInteger& other) const {
        return compare(other) == -1;
    }

    bool operator>(const BigInteger& other) const {
        return compare(other) == 1;
    }

    bool operator==(const BigInteger& other) const {
        return compare(other) == 0;
    }

    bool operator<=(const BigInteger& other) const {
        int cmp = compare(other);
        return cmp == -1 || cmp == 0;
    }

    bool operator>=(const BigInteger& other) const {
        int cmp = compare(other);
        return cmp == 1 || cmp == 0;
    }

    void shiftRight() {
        digits.insert(digits.begin(), 0);
    }

    friend std::ostream& operator<<(std::ostream& stream, const BigInteger& number) {
        if (number.digits.empty())
            stream << '0';
        else {
            stream << number.digits.back();
            for (int i = static_cast<int>(number.digits.size()) - 2; i >= 0; --i)
                stream << std::setw(BASE_DIGITS) << std::setfill('0') << number.digits[i];
        }
        return stream;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string a_str, b_str, op_str;
    while (std::cin >> a_str >> b_str >> op_str) {
        try {
            BigInteger a(a_str), b(b_str);
            char op = op_str[0];
            if (op == '+') {
                std::cout << (a + b) << "\n";
            }
            else if (op == '-') {
                std::cout << (a - b) << "\n";
            }
            else if (op == '*') {
                std::cout << (a * b) << "\n";
            }
            else if (op == '/') {
                std::cout << (a / b) << "\n";
            }
            else if (op == '^') {
                std::cout << (a ^ b) << "\n";
            }
            else if (op == '>') {
                std::cout << (a > b ? "true" : "false") << "\n";
            }
            else if (op == '<') {
                std::cout << (a < b ? "true" : "false") << "\n";
            }
            else if (op == '=') {
                std::cout << (a == b ? "true" : "false") << "\n";
            }
            else {
                std::cout << "Error\n";
            }
        }
        catch (...) {
            std::cout << "Error\n";
        }
    }
    return 0;
}
