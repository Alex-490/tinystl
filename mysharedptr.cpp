#include <algorithm>

template <class T>
class mysharedptr{
public:
    mysharedptr():ptr_(nullptr),ref_count_(nullptr){}
    explicit mysharedptr(T* ptr):ptr_(ptr),ref_count_(new size_t(1)){}
    ~mysharedptr(){
        release();
    }

    mysharedptr(const mysharedptr& other):ptr_(other.ptr_),ref_count_(other.ref_count_){
        if(ref_count_){
            ++(*ref_count_);
        }
    }

    mysharedptr(mysharedptr&& other):ptr_(other.ptr_),ref_count_(other.ref_count_){
        other.ptr_=nullptr;
        other.ref_count_=nullptr;
    }

    mysharedptr& operator=(mysharedptr other){
        swap(other);
        return *this;
    }

    //访问

    T& operator*()const{
        return *ptr_;
    }

    T* operator->()const{
        return ptr_;
    }

    T* get()const{
        return ptr_;
    }

    size_t use_count()const{
        return ref_count_?*ref_count_:0;
    }

    //修改
    void reset(T* ptr=nullptr){
        release();
        if(ptr){
            ptr_=ptr;
            ref_count_=new size_t(1);
        }else{
            ptr_=nullptr;
            ref_count_=nullptr;
        }
    }

    //工具
    void swap(mysharedptr& other){
        std::swap(ptr_,other.ptr_);
        std::swap(ref_count_,other.ref_count_);
    }


private:
    T* ptr_;
    size_t* ref_count_;

    void release(){
        if (ref_count_){
            if(--(*ref_count_)==0){
                delete ptr_;
                delete ref_count_;
            }
            ptr_=nullptr;
            ref_count_=nullptr;
        }
    }
};



// #include <cstddef>
// #include <utility>

// template <typename T>
// class mysharedptr {
// private:
//     T* ptr_;
//     size_t* ref_count_;

//     void release() {
//         if (ref_count_) {
//             if (--(*ref_count_) == 0) {
//                 delete ptr_;
//                 delete ref_count_;
//             }
//         }
//     }

// public:
//     // 1. 默认构造
//     mysharedptr() : ptr_(nullptr), ref_count_(nullptr) {}

//     // 2. 从裸指针构造
//     explicit mysharedptr(T* ptr)
//         : ptr_(ptr), ref_count_(new size_t(1)) {}

//     // 3. 拷贝构造
//     mysharedptr(const mysharedptr& other)
//         : ptr_(other.ptr_), ref_count_(other.ref_count_) {
//         if (ref_count_) {
//             ++(*ref_count_);
//         }
//     }

//     // 4. 移动构造
//     mysharedptr(mysharedptr&& other) noexcept
//         : ptr_(other.ptr_), ref_count_(other.ref_count_) {
//         other.ptr_ = nullptr;
//         other.ref_count_ = nullptr;
//     }

//     // 5. 拷贝赋值（copy-and-swap 思想）
//     mysharedptr& operator=(mysharedptr other) {
//         swap(other);
//         return *this;
//     }

//     // 6. 析构
//     ~mysharedptr() {
//         release();
//     }

//     // 7. swap
//     void swap(mysharedptr& other) noexcept {
//         std::swap(ptr_, other.ptr_);
//         std::swap(ref_count_, other.ref_count_);
//     }

//     // 8. 访问
//     T& operator*() const { return *ptr_; }
//     T* operator->() const { return ptr_; }
//     T* get() const { return ptr_; }

//     // 9. 引用计数
//     size_t use_count() const {
//         return ref_count_ ? *ref_count_ : 0;
//     }

//     // 10. reset
//     void reset(T* ptr = nullptr) {
//         release();
//         if (ptr) {
//             ptr_ = ptr;
//             ref_count_ = new size_t(1);
//         } else {
//             ptr_ = nullptr;
//             ref_count_ = nullptr;
//         }
//     }
// };
