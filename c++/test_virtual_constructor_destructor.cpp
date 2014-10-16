#include <iostream>
class base{
    public:
        base(){
            std::cout<<"base constructor\n";
            init();
        }
        virtual void init(){
            std::cout<<"base init\n";
        }
        virtual void des(){
            std::cout<<"base des\n";
        }
        virtual  ~base(){
            std::cout<<"base destructor\n";
            des();
        }
} ;

class derive:public base{
    public:
        derive(){
            std::cout<<"derive constructor\n";
        }
        ~derive(){
            std::cout<<"derive destructor\n";
            des();
        }
        virtual void init(){
            std::cout<<"derive init\n";
        }
        virtual void des(){
            std::cout<<"derive des\n";
        }

};
int main(int argc, const char *argv[])
{
    base* p= new derive();
    delete p;
    return 0;
}
