#ifndef NONCOPYABLE_HPP 
#define NONCOPYABLE_HPP

class noncopyable{
    public:
    noncopyable()=delete;
    noncopyable(const noncopyable&)=delete;
    void operator=(const noncopyable&)=delete;
    ~noncopyable()=delete;
};

#endif /*NONCOPYABLE_HPP*/
