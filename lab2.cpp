#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

const uint8_t STORAGE_INCREASE_SIZE = 2;
const uint16_t STR_BUF_SIZE = 257;

class TString {
   private:
    int size;
    int capacity;
    char* storage;

   public:
    TString() : size(0), capacity(0), storage(nullptr) {}

    TString(int n) : size(n), capacity(n), storage(new char[n + 1]) {
        // Выделение памяти под строку размером n + 1 для завершающего нуля
        // Установка начального размера строки
        // Установка начальной вместимости строки
        // Инициализация массива символов нулями
        memset(storage, 0, n + 1);
    }

    TString(const char* str) {
        size = strlen(str);
        capacity = size * STORAGE_INCREASE_SIZE;
        storage = new char[capacity];
        std::copy(str, str + size, storage);
        storage[size] = '\0';
    }

    TString(const TString& str) {
        size = str.size;
        capacity = size * STORAGE_INCREASE_SIZE;
        storage = new char[capacity];
        std::copy(str.storage, str.storage + str.size, storage);
        storage[size] = '\0';
    }

    TString(TString&& str) noexcept {
        size = str.size;
        capacity = str.capacity;
        storage = str.storage;
        str.storage = nullptr;
    }

    TString& operator=(const TString& str) {
        delete[] storage;
        size = str.size;
        capacity = str.capacity;
        storage = new char[capacity];
        std::copy(str.storage, str.storage + size + 1, storage);
        return *this;
    }

    TString& operator=(TString&& str) noexcept {
        delete[] storage;
        size = str.size;
        capacity = str.capacity;
        storage = str.storage;
        str.storage = nullptr;
        return *this;
    }

    TString& operator=(const char* str) {
        delete[] storage;
        size = strlen(str);
        capacity = size * STORAGE_INCREASE_SIZE;
        storage = new char[capacity];
        std::copy(str, str + size + 1, storage);
        return *this;
    }

    void CStrMove(char* str) {
        delete[] storage;
        storage = str;
        size = strlen(str);
        capacity = size + 1;
    }

    ~TString() {
        delete[] storage;
        size = 0;
        capacity = 0;
        storage = nullptr;
    }

    char* begin() noexcept { return storage; }

    const char* begin() const noexcept { return storage; }

    char* end() noexcept {
        if (storage) return storage + size;
        return nullptr;
    }

    const char* end() const noexcept {
        if (storage) return storage + size;
        return nullptr;
    }

    const char* CStr() const { return storage; }

    void toLower() {
        for (int i = 0; i < size; ++i) {
            storage[i] = std::tolower(storage[i]);
        }
    }

    short Size() const noexcept { return size; }

    const char& operator[](int index) const {
        if (index > -1 && index < size) {
            return storage[index];
        }
        throw std::out_of_range("Out of range");
    }

    char& operator[](int index) {
        if (index > -1 && index < size) {
            return storage[index];
        }
        throw std::out_of_range("Out of range");
    }

    friend std::ostream& operator<<(std::ostream& os, const TString& str);
    friend std::istream& operator>>(std::istream& is, TString& str);
};

std::ostream& operator<<(std::ostream& os, const TString& str) {
    for (auto ch : str) os << ch;
    return os;
}

std::istream& operator>>(std::istream& is, TString& str) {
    char buf[STR_BUF_SIZE];
    if (is >> buf) str = buf;
    return is;
}

bool operator<(const TString& lhs, const TString& rhs) {
    int minSize = std::min(lhs.Size(), rhs.Size());
    for (int i = 0; i < minSize; ++i) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }
    return lhs.Size() < rhs.Size();
}

bool operator>(const TString& lhs, const TString& rhs) { return rhs < lhs; }

bool operator<=(const TString& lhs, const TString& rhs) { return !(lhs > rhs); }

bool operator>=(const TString& lhs, const TString& rhs) { return !(lhs < rhs); }

bool operator==(const TString& lhs, const TString& rhs) {
    return !(lhs > rhs) && !(lhs < rhs);
}

bool operator!=(const TString& lhs, const TString& rhs) {
    return !(lhs == rhs);
}

enum RBcolor { red, black };

struct TRBNode {
    TRBNode* left;
    TRBNode* right;
    TRBNode* parent;
    uint64_t value;
    TString key;
    RBcolor color;

    TRBNode()
        : color(black),
          key(),
          value(),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    TRBNode(TString& key, uint64_t value)
        : color(red),
          key(key),
          value(value),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}
};

class TRBTree {
   public:
    TRBTree() : size(0) {
        nil = new TRBNode();
        root = nil;
    }

    ~TRBTree() {
        size = 0;
        treeClean(root);
        delete nil;
    }

    void treeClean(TRBNode* root) {
        if (root == nil) return;
        treeClean(root->left);
        treeClean(root->right);
        delete root;
    }

    void serializeFind(const TString& raw_key) {
        TString input_key(raw_key);
        input_key.toLower();
        uint64_t value = 0;

        if (find(input_key, value)) {
            std::cout << "OK: " << value << '\n';
        } else {
            std::cout << "NoSuchWord\n";
        }
    }

    void serializeInsert() {
        TString input_key;
        uint64_t input_value;
        std::cin >> input_key >> input_value;
        input_key.toLower();
        if (find(input_key, input_value)) {
            std::cout << "Exist\n";
        } else {
            insert(input_key, input_value);
            std::cout << "OK\n";
        }
    }

    void serializeRemove() {
        TString input_key;
        std::cin >> input_key;
        input_key.toLower();
        uint64_t value = 0;
        if (find(input_key, value)) {
            remove(input_key);
            std::cout << "OK\n";
        } else {
            std::cout << "NoSuchWord\n";
        }
    }

    void fileOperation() {
        TString filename, command;
        std::cin >> command >> filename;
        if (command[0] == 'S') {  // Save
            if (!trySave(filename)) {
                std::cout << "ERROR: File doesn't open\n";
                return;
            }
        } else {  // Load
            if (!tryLoad(filename)) {
                std::cout << "ERROR: File doesn't open\n";
                return;
            }
        }
        std::cout << "OK\n";
    }

    bool trySave(const TString& filename) {
        std::ofstream output;
        output.open(filename.CStr(),
                    std::ios_base::binary | std::ios_base::out);
        toBinary(root, output);
        output.close();
        return true;
    }

    bool tryLoad(const TString& filename) {
        std::ifstream input;
        input.open(filename.CStr(), std::ios_base::binary | std::ios_base::in);
        treeClean(root);
        root = fromBinary(input, nil);
        input.close();
        return true;
    }

    void toBinary(const TRBNode* root, std::ostream& output) const {
        if (root == nil) return;
        uint16_t keySize = root->key.Size();
        output.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        output.write(root->key.CStr(), keySize);
        output.write((char*)&root->value, sizeof(root->value));
        output.write((char*)&root->color, sizeof(root->color));

        bool hasLeft = root->left != nil;
        bool hasRight = root->right != nil;

        output.write(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
        output.write(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

        if (hasLeft) toBinary(root->left, output);
        if (hasRight) toBinary(root->right, output);
    }

    TRBNode* fromBinary(std::istream& is, TRBNode* parent) {
        TRBNode* root = nil;

        uint16_t keySize;
        is.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

        if (is.gcount() == 0) return root;

        char* key = new char[keySize + 1];
        key[keySize] = 0;
        is.read(key, keySize);

        uint64_t value;
        is.read(reinterpret_cast<char*>(&value), sizeof(value));

        RBcolor color;
        is.read(reinterpret_cast<char*>(&color), sizeof(color));

        bool hasLeft = false;
        bool hasRight = false;
        is.read(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
        is.read(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

        root = new TRBNode();
        root->key.CStrMove(key);
        key = nullptr;
        root->value = value;
        root->color = color;
        root->parent = parent;

        if (hasLeft)
            root->left = fromBinary(is, root);
        else
            root->left = nil;
        if (hasRight)
            root->right = fromBinary(is, root);
        else
            root->right = nil;

        return root;
    }

    TRBNode* root;
    TRBNode* nil;
    size_t size;

    void clean(TRBNode* root) {
        if (root == nil) return;
        clean(root->left);
        clean(root->right);
        delete root;
    }

    void insert(TString& key, uint64_t value) {
        TRBNode* node = new TRBNode(key, value);
        TRBNode* tmp = root;
        TRBNode* predok = nil;
        while (tmp != nil) {
            predok = tmp;
            tmp = (key < tmp->key) ? tmp->left : tmp->right;
        }
        node->parent = predok;
        if (predok == nil) {  //вставили корень
            root = node;
        } else if (key < predok->key) {
            predok->left = node;
        } else {
            predok->right = node;
        }
        node->color = red;
        node->left = nil;
        node->right = nil;
        insertBalance(node);
        ++size;
    }

    void remove(TString& key) {
        TRBNode* tmp = root;
        while (key != tmp->key) {
            tmp = (key < tmp->key) ? tmp->left : tmp->right;
        }
        deleteWithFix(tmp);
        --size;
    }

    bool find(TString& key, uint64_t& value) {
        TRBNode* tmp = root;
        while (tmp != nil) {
            if (key == tmp->key) {
                value = tmp->value;
                return true;
            } else if (key < tmp->key) {
                tmp = tmp->left;
            } else {
                tmp = tmp->right;
            }
        }
        return false;
    }

    TRBNode* rotateLeft(TRBNode* node) {
        TRBNode* res = node->right;
        node->right = res->left;
        if (res->left != nil) res->left->parent = node;
        res->parent = node->parent;
        if (node->parent == nil)
            root = res;
        else if (node == node->parent->left)
            node->parent->left = res;
        else
            node->parent->right = res;
        res->left = node;
        node->parent = res;
        return res;
    }

    TRBNode* rotateRight(TRBNode* node) {
        TRBNode* res = node->left;
        node->left = res->right;
        if (res->right != nil) res->right->parent = node;
        res->parent = node->parent;
        if (node->parent == nil)
            root = res;
        else if (node->parent->left == node)
            node->parent->left = res;
        else
            node->parent->right = res;
        res->right = node;
        node->parent = res;
        return res;
    }

    void leftSonBalance(TRBNode** node) {
        TRBNode* uncle = (*node)->parent->parent->right;
        if (uncle->color == red) {
            (*node)->parent->color = black;
            uncle->color = black;
            (*node)->parent->parent->color = red;
            (*node) = (*node)->parent->parent;
        } else {
            if ((*node) == (*node)->parent->right) {
                (*node) = (*node)->parent;
                rotateLeft(*node);
            }
            (*node)->parent->color = black;
            (*node)->parent->parent->color = red;
            rotateRight((*node)->parent->parent);
        }
    }

    void rightSonBalance(TRBNode** node) {
        TRBNode* uncle = (*node)->parent->parent->left;
        if (uncle->color == red) {
            (*node)->parent->color = black;
            uncle->color = black;
            (*node)->parent->parent->color = red;
            (*node) = (*node)->parent->parent;
        } else {
            if ((*node) == (*node)->parent->left) {
                (*node) = (*node)->parent;
                rotateRight(*node);
            }
            (*node)->parent->color = black;
            (*node)->parent->parent->color = red;
            rotateLeft((*node)->parent->parent);
        }
    }

    void insertBalance(TRBNode* node) {
        while (node != root && node->parent->color == red) {
            if (node->parent->parent->left == node->parent) {
                leftSonBalance(&node);
            } else {
                rightSonBalance(&node);
            }
        }
        root->color = black;
        return;
    }

    void replacement(TRBNode* p, TRBNode* s) {
        if (p->parent == nil)
            root = s;
        else if (p->parent->left == p)
            p->parent->left = s;
        else
            p->parent->right = s;
        s->parent = p->parent;
    }

    TRBNode* findExchange(TRBNode* node) {
        TRBNode* tmp = node;
        TRBNode* res = nil;
        while (tmp != nil) {
            res = tmp;
            tmp = tmp->left;
        }
        return res;
    }

    void deleteLeft(TRBNode** node) {
        TRBNode* brat = (*node)->parent->right;
        if (brat->color == red) {
            brat->color = black;
            brat->parent->color = red;
            rotateLeft((*node)->parent);
            brat = (*node)->parent->right;
        }
        if (brat->left->color == black && brat->right->color == black) {
            brat->color = red;
            *node = (*node)->parent;
        } else {
            if (brat->right->color == black) {
                brat->left->color = black;
                brat->color = red;
                rotateRight(brat);
                brat = (*node)->parent->right;
            }
            brat->color = brat->parent->color;
            brat->parent->color = black;
            brat->right->color = black;
            rotateLeft(brat->parent);
            *node = root;
        }
    }

    void deleteRight(TRBNode** node) {
        TRBNode* brat = (*node)->parent->left;
        if (brat->color == red) {
            brat->color = black;
            brat->parent->color = red;
            rotateRight((*node)->parent);
            brat = (*node)->parent->left;
        }
        if (brat->left->color == black && brat->right->color == black) {
            brat->color = red;
            *node = (*node)->parent;
        } else {
            if (brat->left->color == black) {
                brat->right->color = black;
                brat->color = red;
                rotateLeft(brat);
                brat = (*node)->parent->left;
            }
            brat->color = brat->parent->color;
            brat->parent->color = black;
            brat->left->color = black;
            rotateRight(brat->parent);
            *node = root;
        }
    }

    void deleteUp(TRBNode* node) {
        while (node->color == black && node != root) {
            if (node == node->parent->left) {
                deleteLeft(&node);
            } else {
                deleteRight(&node);
            }
        }
        node->color = black;
    }

    void deleteWithFix(TRBNode* node) {
        TRBNode* deleting_node = node;
        RBcolor deleting_node_color = deleting_node->color;
        TRBNode* deleting_node_son = nil;
        if (node->left == nil) {
            deleting_node_son = node->right;
            replacement(node, node->right);
        } else if (node->right == nil) {
            deleting_node_son = node->left;
            replacement(node, node->left);
        } else {
            deleting_node = findExchange(node->right);
            deleting_node_color = deleting_node->color;
            deleting_node_son = deleting_node->right;
            if (deleting_node->parent == node)
                deleting_node_son->parent = deleting_node;
            else {
                replacement(deleting_node, deleting_node->right);
                deleting_node->right = node->right;
                deleting_node->right->parent = deleting_node;
            }
            replacement(node, deleting_node);
            deleting_node->left = node->left;
            deleting_node->left->parent = deleting_node;
            deleting_node->color = node->color;
        }
        if (deleting_node_color == black) deleteUp(deleting_node_son);
        delete node;
    }

    TRBNode* GetRoot() { return root; }

    TRBNode* GetNil() { return nil; }
};

void inorderTraversal(TRBNode* node, TRBNode* nil) {
    if (node != nil) {
        inorderTraversal(node->left, nil);
        std::cout << node->key << ' ' << node->color << '\n';
        inorderTraversal(node->right, nil);
    } else {
        std::cout << "null\n";
    }
}

/*
int main() {
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(nullptr);
    TString command;
    TRBTree tree;
    while (std::cin >> command) {
        if (command[0] == '+') {
            tree.serializeInsert();
        } else if (command[0] == '-') {
            tree.serializeRemove();
        } else if (command[0] == '!') {
            tree.fileOperation();
        } else {
            tree.serializeFind(command);
        }
    }
    return 0;
}

/**/

#include <chrono>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

class LogDuration {
public:
    explicit LogDuration(const std::string& msg = "")
            : message(msg + ": ")
            , start(std::chrono::steady_clock::now())
    {
    }

    ~LogDuration() {
        auto finish = std::chrono::steady_clock::now();
        auto dur = finish - start;
        std::cerr << message
             <<
std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
             << " ms" << std::endl;
    }
private:
    std::string message;
    std::chrono::steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) LogDuration UNIQ_ID(__LINE__){message};
default_random_engine rng;

uint64_t get_number(uint64_t min = 0, uint64_t max = numeric_limits<unsigned
long long>::max()) { uniform_int_distribution<unsigned long long> dist_ab(min,
max); return dist_ab(rng);
}

TString get_string() {
    int string_size = get_number(1, 256);
    TString str(string_size);
    for (size_t i = 0; i < string_size; ++i) {
        str[i] = 'a' + get_number(0, 25);
    }
    return str;
}

int main() {
    rng.seed(chrono::system_clock::now().time_since_epoch().count());   

    size_t count;
    std::cout << "Enter the number of operations: ";
    cin >> count;

    // Generate test data
    vector<pair<TString, uint64_t>> test_data(count);
    for (size_t i = 0; i < count; ++i) {
        test_data[i] = {get_string(), get_number()};
    }
    TString filename = 'f';
    TRBTree tree;
    for (size_t i = 0; i < count; ++i) {
        tree.insert(test_data[i].first, test_data[i].second);
    }
    for (size_t i = 0; i < count; ++i) {
        tree.find(test_data[i].first, test_data[i].second);
    }
    for (size_t i = 0; i < count; ++i) {
        tree.remove(test_data[i].first);
    }
    return 0;
}
