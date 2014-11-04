#ifndef NONCOPYABLE_HPP 
#define NONCOPYABLE_HPP

class noncopyable{
    public:
    noncopyable(){}
    noncopyable(const noncopyable&)=delete;
    void operator=(const noncopyable&)=delete;
    ~noncopyable(){}
};

#endif /*NONCOPYABLE_HPP*/
