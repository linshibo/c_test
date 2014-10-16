#include <iostream>
class base{
    public:
        base(){
        }
        virtual ~base(){}
};

class derive:public base{
    public:
        derive(){
        }
};

int main(int argc, const char *argv[])
{
    base* p= new derive();
    if(dynamic_cast<derive*>(p) != NULL) {
        std::cout<<"dynamic_cast succ\n";
    }
    return 0;
}
