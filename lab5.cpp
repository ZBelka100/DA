#include <bits/stdc++.h>

using namespace std;

const char SENTINEL = '$';

class TNode {
public:
    unordered_map<char, TNode*> children; // Дети текущего узла
    int leftBound, rightBound, suffixNum;      // Границы подстроки и номер суффикса
    TNode* suffixLink;                         // Суффиксная ссылка

    TNode(int num, int left, int right)
        : leftBound(left), rightBound(right), suffixNum(num), suffixLink(nullptr) {}

    ~TNode() {
        for (auto& elem : children) {
            delete elem.second;
        }
    }
};

class TSuffixTree {
public:
    explicit TSuffixTree(const string& inputText);
    ~TSuffixTree();

    set<int> Search(const string& word); // Поиск слова в суффиксном дереве
    void PrintSuffixes();                          // Печать всех суффиксов

private:
    TNode* root_;             // Корень дерева
    string text_;        // Исходный текст с добавленным сентинелом
    TNode* activeNode_;       // Активный узел
    int activeEdge_;          // Активное ребро
    int activeLength_;        // Длина активного суффикса
    int remainingSuffixCount_; // Количество оставшихся суффиксов для добавления
    int nodeCount_;           // Счетчик узлов

    bool WalkDown(TNode* node);                         // Спуск по ребру
    void Add(int pos);                     // Расширение суффиксного дерева
    void FindLeafNodes(TNode* node, set<int>& result); // Поиск листовых узлов
    void SearchSubstring(const string& word, set<int>& result); // Поиск подстроки
    void PrintSuffixesHelper(TNode* node, string& currentString);    // Помощник для печати суффиксов
};

TSuffixTree::TSuffixTree(const string& inputText) {
    text_ = inputText + SENTINEL;
    root_ = new TNode(-1, -1, -1);
    activeNode_ = root_;
    activeEdge_ = -1;
    activeLength_ = 0;
    remainingSuffixCount_ = 0;
    nodeCount_ = 0;

    for (size_t i = 0; i < text_.length(); ++i) {
        Add(static_cast<int>(i));
    }
}

TSuffixTree::~TSuffixTree() {
    delete root_;
}

bool TSuffixTree::WalkDown(TNode* node) {
    int edgeLength = node->rightBound - node->leftBound + 1;
    if (activeLength_ >= edgeLength) {
        activeEdge_ += edgeLength;
        activeLength_ -= edgeLength;
        activeNode_ = node;
        return true;
    }
    return false;
}

void TSuffixTree::Add(int pos) {
    ++remainingSuffixCount_;
    TNode* lastNewNode = nullptr;

    while (remainingSuffixCount_ > 0) {
        if (activeLength_ == 0) {
            activeEdge_ = pos;
        }

        char currentChar = text_[activeEdge_];
        auto it = activeNode_->children.find(currentChar);
        if (it == activeNode_->children.end()) {
            TNode* leaf = new TNode(pos - remainingSuffixCount_ + 1, pos, static_cast<int>(text_.length()) - 1);
            activeNode_->children[currentChar] = leaf;

            if (lastNewNode != nullptr) {
                lastNewNode->suffixLink = activeNode_;
                lastNewNode = nullptr;
            }
        } else {
            TNode* nextNode = it->second;
            if (WalkDown(nextNode)) {
                continue;
            }
            if (text_[nextNode->leftBound + activeLength_] == text_[pos]) {
                ++activeLength_;
                if (lastNewNode != nullptr && activeNode_ != root_) {
                    lastNewNode->suffixLink = activeNode_;
                }
                break;
            }
            // Разбиваем ребро и создаем новый узел
            TNode* splitNode = new TNode(-1, nextNode->leftBound, nextNode->leftBound + activeLength_ - 1);
            activeNode_->children[currentChar] = splitNode;
            nextNode->leftBound += activeLength_;
            splitNode->children[text_[nextNode->leftBound]] = nextNode;
            // Создаем новый листовой узел
            TNode* leaf = new TNode(pos - remainingSuffixCount_ + 1, pos, static_cast<int>(text_.length()) - 1);
            splitNode->children[text_[pos]] = leaf;
            if (lastNewNode != nullptr) {
                lastNewNode->suffixLink = splitNode;
            }
            lastNewNode = splitNode;
        }
        --remainingSuffixCount_;
        if (activeNode_ == root_ && activeLength_ > 0) {
            --activeLength_;
            activeEdge_ = pos - remainingSuffixCount_ + 1;
        } else if (activeNode_ != root_) {
            activeNode_ = activeNode_->suffixLink != nullptr ? activeNode_->suffixLink : root_;
        }
    }
}

void TSuffixTree::FindLeafNodes(TNode* node, set<int>& result) {
    queue<TNode*> nodeQueue;
    nodeQueue.push(node);

    while (!nodeQueue.empty()) {
        TNode* currentNode = nodeQueue.front();
        nodeQueue.pop();
        if (currentNode->suffixNum != -1) {
            if (currentNode->suffixNum < static_cast<int>(text_.size()) - 1) { // Исключаем сентинел
                result.insert(currentNode->suffixNum);
            }
        } else {
            for (auto& child : currentNode->children) {
                nodeQueue.push(child.second);
            }
        }
    }
}

void TSuffixTree::SearchSubstring(const string& word, set<int>& result) {
    TNode* currentNode = root_;
    int wordIndex = 0;

    while (wordIndex < static_cast<int>(word.length())) {
        char currentChar = word[wordIndex];
        auto it = currentNode->children.find(currentChar);
        if (it == currentNode->children.end()) {
            return; 
        }
        currentNode = it->second;
        int edgeLength = currentNode->rightBound - currentNode->leftBound + 1;
        int i = 0;
        while (i < edgeLength && wordIndex < static_cast<int>(word.length())) {
            if (text_[currentNode->leftBound + i] != word[wordIndex]) {
                return; 
            }
            ++i;
            ++wordIndex;
        }
    }
    FindLeafNodes(currentNode, result);
}

set<int> TSuffixTree::Search(const string& word) {
    set<int> result;
    SearchSubstring(word, result);
    return result;
}

void TSuffixTree::PrintSuffixes() {
    string currentString;
    PrintSuffixesHelper(root_, currentString);
}

void TSuffixTree::PrintSuffixesHelper(TNode* node, string& currentString) {
    if (node == nullptr) return;

    if (node->leftBound != -1 && node->rightBound != -1) {
        currentString += text_.substr(node->leftBound, node->rightBound - node->leftBound + 1);
    }

    if (node->children.empty()) {
        cout << "Суффикс, начинающийся с позиции " << node->suffixNum + 1 << ": " << currentString << endl;
    } else {
        for (auto& child : node->children) {
            PrintSuffixesHelper(child.second, currentString);
        }
    }

    if (node->leftBound != -1 && node->rightBound != -1) {
        currentString.erase(currentString.size() - (node->rightBound - node->leftBound + 1));
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    cin >> text;
    TSuffixTree tree(text);

    string word;
    int wordCount = 1;
    while (cin >> word) {
        set<int> occurrences = tree.Search(word);
        if (!occurrences.empty()) {
            cout << wordCount << ": ";
            for (auto it = occurrences.begin(); it != occurrences.end(); ++it) {
                cout << *it + 1; 
                if (next(it) != occurrences.end()) {
                    cout << ", ";
                } else {
                    cout << '\n';
                }
            }
        }
        ++wordCount;
    }

    return 0;
}