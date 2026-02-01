#include <utility>
#include <cstring>
#include <stdexcept>
#include <algorithm>

class mystring{
public:
    mystring():data_(new char[1]{'\0'}),size_(0),capacity_(0){}

    mystring(const char* s){
        size_=std::strlen(s);
        capacity_=size_;
        data_=new char[capacity_+1];
        std::memcpy(data_,s,size_+1);
    }

    mystring(mystring& other):size_(other.size_),capacity_(other.capacity_){
        data_=new char[capacity_+1];
        std::memcpy(data_,other.data_,size_+1);
    }

    mystring(mystring&& other):size_(other.size_),capacity_(other.capacity_),data_(other.data_){
        other.size_=0;
        other.capacity_=0;
        other.data_=new char[1]{'\0'};
    }

    mystring& operator=(mystring other){
        swap(other);
        return *this;
    }

    ~mystring(){
        delete[] data_;
    }

    //访问
    char& operator[](size_t i) noexcept{
        return data_[i];
    }

    const char& operator[](size_t i)const noexcept{
        return data_[i];
    }

    char& at(size_t i){
        if(i>=size_){
            throw std::out_of_range("mystring:at");
        }
        return data_[i];
    }


    const char* c_str()const noexcept{
        return data_;   
    }

    size_t capacity()const noexcept{
        return capacity_;
    }

    size_t size()const noexcept{
        return size_;
    }

    bool empty()const noexcept{
        return size_==0;
    }

    //修改
    void reserve(int newCap){
        if(newCap<capacity_)    return;

        capacity_=newCap;
        char* newMem=new char[newCap+1];
        std::memcpy(newMem,data_,size_);
        delete[] data_;
        data_=newMem;
    }

    void push_back(char c){
        if(size_==capacity_){
            reserve(capacity_==0?1:capacity_*2);
        }

        data_[size_]=c;
        ++size_;
    }

    void pop_back(){
        if(size_==0)    return;
        --size_;
        data_[size_]='\0';
    }

    void clear(){
        size_=0;
        data_[0]='\0';
    }

    //工具
    void swap(mystring& other){
        std::swap(data_,other.data_);
        std::swap(capacity_,other.capacity_);
        std::swap(size_,other.size_);
    }

private:
    int dummy;
    char* data_;
    size_t capacity_;
    size_t size_;
};



// ~T();
// T(const T&);//拷贝构造
// T& operator=(const T&);//拷贝复制运算符
// T(T&&) noexcept;//移动构造
// T& operator=(T&&) noexcept;//移动赋值运算符



// #include <cstring>
// #include <algorithm>
// #include <stdexcept>

// class mystring {
// public:
//     // ---------- 构造 / 析构 ----------
//     mystring()
//         : data_(new char[1]{'\0'}), size_(0), capacity_(0) {}

//     mystring(const char* s) {
//         size_ = std::strlen(s);
//         capacity_ = size_;
//         data_ = new char[capacity_ + 1];
//         std::memcpy(data_, s, size_ + 1);
//     }

//     mystring(const mystring& other)
//         : size_(other.size_), capacity_(other.capacity_) {
//         data_ = new char[capacity_ + 1];
//         std::memcpy(data_, other.data_, size_ + 1);
//     }

//     mystring(mystring&& other) noexcept
//         : data_(other.data_),
//           size_(other.size_),
//           capacity_(other.capacity_) {
//         other.data_ = new char[1]{'\0'};
//         other.size_ = 0;
//         other.capacity_ = 0;
//     }

//     mystring& operator=(mystring other) {
//         swap(other);
//         return *this;
//     }

//     ~mystring() {
//         delete[] data_;
//     }

//     // ---------- 访问 ----------
//     char& operator[](size_t i) noexcept {
//         return data_[i];
//     }

//     const char& operator[](size_t i) const noexcept {
//         return data_[i];
//     }

//     char& at(size_t i) {
//         if (i >= size_)
//             throw std::out_of_range("mystring::at");
//         return data_[i];
//     }

//     const char* c_str() const noexcept {
//         return data_;
//     }

//     size_t size() const noexcept {
//         return size_;
//     }

//     size_t capacity() const noexcept {
//         return capacity_;
//     }

//     bool empty() const noexcept {
//         return size_ == 0;
//     }

//     // ---------- 修改 ----------
//     void reserve(size_t new_cap) {
//         if (new_cap <= capacity_) return;

//         char* new_data = new char[new_cap + 1];
//         std::memcpy(new_data, data_, size_ + 1);

//         delete[] data_;
//         data_ = new_data;
//         capacity_ = new_cap;
//     }

//     void push_back(char c) {
//         if (size_ == capacity_) {
//             reserve(capacity_ == 0 ? 1 : capacity_ * 2);
//         }
//         data_[size_] = c;
//         ++size_;
//         data_[size_] = '\0';
//     }

//     void pop_back() {
//         if (size_ == 0) return;
//         --size_;
//         data_[size_] = '\0';
//     }

//     void clear() noexcept {
//         size_ = 0;
//         data_[0] = '\0';
//     }

//     // ---------- 工具 ----------
//     void swap(mystring& other) noexcept {
//         std::swap(data_, other.data_);
//         std::swap(size_, other.size_);
//         std::swap(capacity_, other.capacity_);
//     }

// private:
//     char*  data_;
//     size_t size_;
//     size_t capacity_;
// };
