#include <iostream>
#include <vector>

class datas{
    public:
    datas(){
        std::cout<<"[datas()]\n";
    }
    datas(const datas &i){
        std::cout<<"[datas(datas& i)]\n";
    }
    ~datas(){
        std::cout<<"[~datas()]\n";
    }
    void operator =(datas &i){
        std::cout<<"[datas operator =]\n";
    }
    int a;
};
void func(datas tmp){
    std::cout<<"func\n";
}

int main(int argc, const char *argv[])
{
    datas tmp;
    std::cout<<"datas a(tmp):\n";
    datas a(tmp);
    std::cout<<"datas b=tmp:\n";
    datas b=tmp;
    std::cout<<"a=tmp:\n";
    a=tmp;
    std::cout<<"func(tmp):\n";
    func(tmp);
    std::vector< datas > vec;
    //vec.push_back(tmp);
    //vec.push_back(tmp);
    //vec.push_back(tmp);
    std::cout<<"vector resize:\n";
    vec.resize(3);
    std::cout<<"vector erase:\n";
    vec.erase(vec.begin());

    return 0;
}
