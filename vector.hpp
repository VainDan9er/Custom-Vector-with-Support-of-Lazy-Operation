#ifndef VECTOR_HPP
#define VECTOR_HPP

#include<initializer_list>





template<typename L, typename R>
class AddExp
{
public:
    AddExp(const L& left, const R& right) :  left(left), right(right)
    {}
    auto operator [] (size_t index) const
    {
        return left[index] + right[index];
    }
    size_t get_size() const {return left.get_size();}
private:
    const L& left;
    const R& right;
};

template<typename L, typename R>
class SubExp
{
public:
    SubExp(const L& left, const R& right) :  left(left), right(right)
    {}
    auto operator [] (size_t index) const
    {
        return left[index] - right[index];
    }
    size_t get_size() const {return left.get_size();}
private:
    const L& left;
    const R& right;
};

template<typename L, typename R>
AddExp<L,R> operator + (const L& left, const R& right)
{
    return AddExp<L,R>{left,right};
}

template<typename L, typename R>
SubExp<L,R> operator - (const L& left, const R& right)
{
    return SubExp<L,R>{left,right};
}






template<typename T>
class Vector
{
public:
    Vector();
    Vector(const size_t n);
    Vector(const size_t n, const T& value);
    Vector(std::initializer_list<T> other);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    const T& operator [] (std::ptrdiff_t index) const;
    T& operator [] (std::ptrdiff_t index);
    template<typename Expr>
    Vector(const Expr& expression);
    Vector<T>& operator = (const Vector<T>& other);
    template<typename Exp>
    Vector<T>& operator = (const Exp& other);
    ~Vector();
    
    size_t get_size() const;
    size_t get_capacity() const;
    T* begin()  const;
    T* end()  const;
    void print()    const;
    bool is_empty() const;
    
    void reserve(size_t new_capacity);
    void resize(size_t new_size, const T& value = T());
    void shrink_to_fit();
    void clear();
    void push_back(const T&& value);
    
private:
    T* data;
    T* end_data;
    size_t size;
    size_t capacity;
    
    T* allocator(const size_t n);
    void deallocator(T* data);
    void construct(T* data, const T& value = T());
    void destroy(T* data);
    void reallocate(size_t new_capacity);
};



//Vector()
template<typename T>
Vector<T>::Vector() : data(nullptr), size(0), capacity(0), end_data(nullptr)
{}
//Vector(n)
template<typename T>
Vector<T>::Vector(const size_t n) : Vector()
{
    std::cout<<"Constructor(n)\n";
    data = allocator(n);
    capacity = n;
}

//Vector(n,value)
template<typename T>
Vector<T>::Vector(const size_t n, const T& value) : Vector(n)
{
    std::cout<<"Constructor(n,value)\n";
    size = capacity;
    for(size_t i=0; i<size; i++)
    {
        construct(data+i, value);
    }
    end_data = data+size;
}

//Intitializer_list
template<typename T>
Vector<T>::Vector(std::initializer_list<T> other) : Vector(other.size())
{
    std::cout<<"Initializer Constructor\n";
    size = capacity;
    int counter = 0;
    for(const T& element : other)
    {
        construct(data+counter, element);
        counter++;
    }
    
    end_data = data+size;
}

//Copy Constructor
template<typename T>
Vector<T>::Vector(const Vector& other) : Vector(other.capacity)
{
    std::cout<<"Copy Constructor\n";
    size = other.size;
    for(size_t i=0; i<size; i++)
    {
        construct(this->data+i, *(other.data+i));
    }
    
    end_data = data+size;
}

//Move Constructor
template<typename T>
Vector<T>::Vector(Vector&& other) noexcept : size(other.size), capacity(other.capacity), data(other.data), end_data(other.end_data)
{
    std::cout<<"Move Constructor\n";
    other.size = 0;
    other.capacity = 0;
    other.data = nullptr;
    other.end_data = nullptr;
}

//Operator [] const
template<typename T>
const T& Vector<T>::operator[](std::ptrdiff_t index)    const
{
    if (index < 0)  index += static_cast<std::ptrdiff_t>(size);
    if (index < 0 || static_cast<std::size_t>(index) >= size)   throw std::out_of_range("index out of range");
    return *(data + index);
}

//Operator []
template<typename T>
T& Vector<T>::operator[](std::ptrdiff_t index)
{
    if (index < 0)  index += static_cast<std::ptrdiff_t>(size);
    if (index < 0 || static_cast<std::size_t>(index) >= size)   throw std::out_of_range("index out of range");
    return *(data + index);
}

//Lazy Operation Constructor
template<typename T>
template<typename Expr>
Vector<T>::Vector(const Expr& expression) : size(expression.get_size()), capacity(expression.get_size())
{
    std::cout<<"Lazy Operation Constructor\n";
    data = allocator(size);
    for(size_t i=0; i<size; i++)
    {
        construct(data+i);
        data[i] = expression[i];
    }
    end_data = data + size;
}

//Operator =
template<typename T>
Vector<T>& Vector<T>::operator = (const Vector<T>& other)
{
    if(data == other.data)   return *this;
    size = other.size;
    capacity = other.capacity;
    if(data!=nullptr)
    {
        for(size_t i=0; i<capacity; i++)
        {
            destroy(data+i);
        }
        end_data = nullptr;
    }
    data = allocator(capacity);
    for(size_t i=0; i<size; i++)
    {
        construct(data+i, *(other.data+i));
    }
    end_data = data + size;
    return *this;
}

//Lazy Operation for Operator =
template<typename T>
template<typename Exp>
Vector<T>& Vector<T>::operator = (const Exp& expression)
{
    if(data!=nullptr)
    {
        for(size_t i=0; i<capacity; i++)
        {
            destroy(data+i);
        }
        end_data = nullptr;
    }
    
    for(size_t i=0; i<size; i++)
    {
        construct(data+i);
        data[i] = expression[i];
    }
    end_data = data + size;
    return *this;
}

//Destructor
template<typename T>
Vector<T>::~Vector()
{
    std::cout<<"Destructor\n";
    for(size_t i=0; i<size; i++)
    {
        destroy(data+i);
    }
    end_data = data;
    deallocator(data);
}









//Allocator
template<typename T>
T* Vector<T>::allocator(const size_t n)
{
    if(n==0)    return nullptr;
    //std::cout<<"Allocator\n";
    return static_cast<T*>(operator new(n*sizeof(T)));
}

//Deallocator
template<typename T>
void Vector<T>::deallocator(T* data)
{
    if(data==nullptr)   return;
    //std::cout<<"Deallocator\n";
    operator delete(data);
}

template<typename T>
void Vector<T>::construct(T* data, const T& value)
{
    new (data) T(value);
}

template<typename T>
void Vector<T>::destroy(T* data)
{
    (data)->~T();
}

template<typename T>
void Vector<T>::reallocate(size_t new_capacity)
{
    T* new_data = allocator(new_capacity);
    capacity = new_capacity;
    for(size_t i=0; i<size; i++)
    {
        construct(new_data+i, std::move(*(data+i)));
        destroy(data+i);
    }
    deallocator(data);
    data = new_data;
    end_data = data+size;
}






template<typename T>
void Vector<T>::reserve(size_t new_capacity)
{
    if(new_capacity>capacity)
    {
        reallocate(new_capacity);
    }
}

template<typename T>
void Vector<T>::resize(size_t new_size, const T& value)
{
    if(new_size>size)
    {
        size_t difference = new_size-size;
        if(new_size>capacity)
        {
            reallocate(new_size);
        }
        for(size_t i=0; i<difference; i++)
        {
            construct(end_data+i, value);
        }
        end_data+=difference;
    }
    else if(new_size<size)
    {
        size_t difference = size-new_size;
        for(size_t i=0; i<difference; i++)
        {
            destroy(end_data);
            end_data--;
        }
    }
    size = new_size;
}

template<typename T>
void Vector<T>::shrink_to_fit()
{
    if(capacity>size)
    {
        reallocate(size);
        capacity = size;
    }
}

template<typename T>
void Vector<T>::clear()
{
    for(size_t i=0; i<size; i++)
    {
        destroy(data+i);
    }
    size = 0;
    end_data = data;
}

template<typename T>
void Vector<T>::push_back(const T&& value)
{
    if(capacity==size)
    {
        capacity = size*2;
        reallocate(capacity);
    }
    *end_data = value;
    end_data++;
    size++;
}



//Begin
template<typename T>
T* Vector<T>::begin()   const
{
    return data;
}

//End
template<typename T>
T* Vector<T>::end()   const
{
    return end_data;
}

//Size
template<typename T>
size_t Vector<T>::get_size() const
{
    return size;
}

//Capacity
template<typename T>
size_t Vector<T>::get_capacity() const
{
    return capacity;
}

template<typename T>
bool Vector<T>::is_empty() const
{
    if(data==nullptr)    return 1;
    else    return 0;
}

//Print
template<typename T>
void Vector<T>::print()   const
{
    for(T* begin=data; begin!=end_data; begin++)   std::cout<<*(begin)<<' ';
    std::cout<<"Size: "<<size<<' '<<"Cap: "<<capacity<<'\n';
}


#endif



