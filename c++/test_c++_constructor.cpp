#include <iostream>
class base{
    public:
        base(){
            init();
        }
        void init(){
            func();
        }
        virtual void func(){
            std::cout<<"base func\n";
        }
};

class derive:public base{
    public:
        derive(){
        }
        virtual void func(){
            std::cout<<"derive func\n";
        }
};
int main(int argc, const char *argv[])
{
    derive d;
    d.init();
    return 0;
}
