#include <iostream>
#include <chrono>

template<class T>
class TVector{
    private:
        T* data;
        size_t size;
        size_t capacity;
    public:
        TVector();
        TVector(const TVector& other);
        TVector(const TVector&& other) noexcept;
        TVector(const int n);
        ~TVector();

        void operator=(const TVector& other);
        void operator=(const TVector&& other) noexcept; 

        void PushBack(const T& elem);
        void PushBack(const T&& elem) noexcept;
        void PopBack();
        void Reserve(const size_t n);

        size_t Size() const;
        size_t Capacity() const;
        bool Empty() const;

        T& operator[](int i) const;
        

        void Fill();

        void Swap(TVector& other);
};

template<class T>
TVector<T>::TVector(){
    size = 0;
    capacity = 0;
    data = new T[capacity];
}

template<class T>
TVector<T>::TVector(const TVector<T>& other){
    size = other.size;
    capacity = other.capacity;
    delete[] data;
    data = new T[capacity];
    for(int i = 0; i < other.size; i++){
        data[i] = other.data[i];
    }
}

template<class T>
TVector<T>::TVector(const TVector<T>&& other) noexcept{
    size = other.size;
    capacity = other.capacity;
    data = other.data;
}

template<class T>
TVector<T>::TVector(const int n){
    size = 0;
    capacity = n;
    data = new T[capacity];
}

template<class T>
TVector<T>::~TVector(){
    delete[] data;
}

template<class T>
void TVector<T>::operator=(const TVector<T>& other){
    size = other.size;
    capacity = other.capacity;
    delete[] data;
    data = new T[capacity];
    for(int i = 0; i < other.size; i++){
        data[i] = other.data[i];
    }
}

template<class T>
void TVector<T>::operator=(const TVector<T>&& other) noexcept{
    size = other.size;
    capacity = other.capacity;
    data = other.data;
}

template<class T>
void TVector<T>::PushBack(const T& elem){
    if(size >= capacity){
        capacity = capacity * 2 + 1;
        T* newData = new T[capacity];
        for(int i = 0; i < size; i++){
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

    data[size] = elem;
    
    size++;
}

template<class T>
void TVector<T>::PushBack(const T&& elem) noexcept{
    if(size >= capacity){
        capacity = capacity * 2 + 1;
        T* newData = new T[capacity];
        for(int i = 0; i < size; i++){
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
    }
    data[size] = std::move(elem);

    size++;
}

template<class T>
void TVector<T>::Reserve(const size_t n){
    capacity = n;
    T* newData = new T[capacity];
    for(int i = 0; i < size; i++){
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

template<class T>
T& TVector<T>::operator[](int i) const{
    if(i > size){
        throw(std::out_of_range("TVector out of range\n"));
    }
    return data[i]; 
} 

template<class T>
void TVector<T>::Swap(TVector<T>& other){
    size_t temp = size;
    size = other.size;
    other.size = temp;

    temp = capacity;
    capacity = other.capacity;
    other.capacity = temp;

    T* tempData = data;
    data = other.data; 
    other.data = tempData;
}

template<class T>
void TVector<T>::PopBack(){
    size--;
}

template<class T>
size_t TVector<T>::Size() const{
    return size;
}

template<class T>
size_t TVector<T>::Capacity() const{
    return capacity;
}

template<class T>
bool TVector<T>::Empty() const{
    return size == 0;
}

template<class T>
void TVector<T>::Fill(){
    size = capacity;
}


bool IsDigit(char &c){
    return '0' <= c && c <= '9';
}

class TPair{
    public:
        TVector<char> key;
        TVector<char> value;

        TPair();
        bool ReadKey();
        void ReadValue();
        void Print();
        void operator=(const TPair& other);
};

TPair::TPair(){
    key.Reserve(7);
    //value.Reserve(50);
}

bool TPair::ReadKey(){
    char c = '$';
    while(c != '\t' && c != EOF){
        c = getchar_unlocked();
        if(c == EOF){
            return 1;
        }
        if(c == '\n' || c == ' ' || c == '\t'){
            continue;
        }
        key.PushBack(c);
    }
    return 0;
}

void TPair::ReadValue(){
    char c = '$';
    while(c != '\n' && c != EOF){
        c = getchar_unlocked();
        value.PushBack(c);
    }
}

void TPair::Print(){
    // std::cout << key[0] << ' ' << key[1] << key[2] << key[3] << ' ' << key[4] << key[5] << '\t';
    putchar_unlocked(key[0]);
    putchar_unlocked(' ');
    putchar_unlocked(key[1]);
    putchar_unlocked(key[2]);
    putchar_unlocked(key[3]);
    putchar_unlocked(' ');
    putchar_unlocked(key[4]);
    putchar_unlocked(key[5]);
    putchar_unlocked('\t');
    for(int i = 0; i < value.Size(); i++){
        // std::cout << value[i];
        putchar_unlocked(value[i]);
    }
}

void TPair::operator=(const TPair& other){
    key = other.key;
    value = other.value;
}

int main(){

    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    const int KEY_SIZE = 6;
    const int ALPHABET_SIZE = 26;
    const int DEFAULT_SIZE = 100000;

    //auto begin = std::chrono::steady_clock::now();

    TVector<TPair*> the(DEFAULT_SIZE);
    

    
    while(true){
        TPair* temp = new TPair;
        if(temp->ReadKey()){
            break;
            delete temp;
        }
        temp->ReadValue();
        the.PushBack(std::move(temp));
    }
    TVector<TPair*> res(the.Size());
    res.Fill();

    

    TVector<int> count(ALPHABET_SIZE);
    for(int i = 0; i < ALPHABET_SIZE; i++){
        count.PushBack(0);
    }
    


    for(int j = KEY_SIZE - 1; j >= 0; j--){
        if (the.Size() == 0) {
            break;
        }
        if(the[0]->key[j] == ' '){
            continue;
        }

        char ascii_offset;
        if(IsDigit(the[0]->key[j])){
            ascii_offset = '0';
        }
        else{
            ascii_offset = 'A' ;
        }

        
        for(int i = 0; i < the.Size(); i++) {
            count[the[i]->key[j] - ascii_offset]++;
        }
        for(int i = 1; i < ALPHABET_SIZE; i++){
            count[i] += count[i-1];
        }
        
        for(int i = the.Size()-1; i >= 0; i--){
            res[count[the[i]->key[j] - ascii_offset] - 1] = std::move(the[i]);
            count[the[i]->key[j] - ascii_offset]--;
        }

        for(int i = 0; i < ALPHABET_SIZE; i++){
            count[i] = 0;
        }

        the.Swap(res);
        
    }

    
    for(int i = 0 ; i < the.Size(); i++){
        the[i]->Print();
        delete the[i];
    }
 
    // auto end = std::chrono::steady_clock::now();
    // auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    // std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}