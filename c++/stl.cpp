#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <deque>

void print(int a){
    std::cout<<a<<" ";
}
class Print:public std::unary_function<int, void>{
    public:
    void operator()(int a) const
    {
        std::cout<<a<<" ";
    }
};

class Print2:public std::binary_function<int,int, void>{
    public:
    void operator()(int a, int b) const
    {
        std::cout<<a<<" "<<b<<" ";
    }
};

int op_nothing (int i) { return i; }


int main(int argc, const char *argv[])
{
    //Initialization
    std::vector<int> vec={1,1,2,2,3,3,4,4,5,6}; 
    std::list<int> l={1,1,2,2,3,3,4,4,5,6}; 
    std::deque<int> d={1,1,2,2,3,3,4,4,5,6}; 
    std::map<int, int> m;
    std::set<int> s;
    for(int i=0; i<=10;i++){
        m[i]=i;
        s.insert(i);
    }
    std::string str="aabbcdefgh";
    Print pp;
    Print2 pp2;
    std::cout<<"vec :\n";
    std::for_each(vec.begin(),vec.end(), print);
    std::cout<<"\n";
    std::cout<<"list :\n";
    std::for_each(l.begin(),l.end(), pp);
    std::cout<<"\n";
    std::cout<<"set :\n";
    std::transform(s.begin(),s.end(), std::ostream_iterator<int>(std::cout, " "),  op_nothing);
    std::cout<<"\n";

    //erase one
    vec.erase(std::remove(vec.begin(),vec.end(), 3), vec.end());
    std::cout<<"vec erase 3\n";
    std::cout<<"vec :\n";
    std::for_each(vec.begin(),vec.end(), print);
    std::cout<<"\n";;
    std::cout<<"vec erase 4\n";
    for(auto it = vec.begin(); it != vec.end();){
        if(*it==4) {
            vec.erase(it);
        }else{
            ++it; 
        }
    }
    std::cout<<"vec :\n";
    std::for_each(vec.begin(),vec.end(), print);
    std::cout<<"\n";;
    std::cout<<"list erase 3\n";
    l.remove(3);
    std::cout<<"list :\n";
    std::for_each(l.begin(),l.end(), pp);
    std::cout<<"\n";

    std::cout<<"vec size "<<vec.size()<<" capcity "<<vec.capacity()<<std::endl;
    std::vector<int>(vec.begin(), vec.end()).swap(vec); 
    std::cout<<"vec size "<<vec.size()<<" capcity "<<vec.capacity()<<std::endl;
    vec.reserve(100);
    std::cout<<"after reserve vec size "<<vec.size()<<" capcity "<<vec.capacity()<<std::endl;
    std::vector<int>().swap(vec); 
    std::cout<<"vec size "<<vec.size()<<" capcity "<<vec.capacity()<<std::endl;
    
      
    return 0;
}
