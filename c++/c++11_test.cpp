#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <regex>
#include <random>
#include <tuple>
#include <map>
constexpr int GetFive() {
    return 5;
}
bool adjacent(long x, long y){
    return x>y;
}
void FunctionName(std::initializer_list<float> list){
}
void foo(char *){
    std::cout<<"foo char*\n";
}
void foo(int){
    std::cout<<"foo int\n";
}
struct BasicStruct
{
 int x;
 float y;
};

// 此函數將得到形參 'r' 的引用並對 r 加一
void f (int &r)  { r++; }
 
// 樣板函式
template<class F, class P> 
void g (F f, P t)  { f(t); }
 

struct SomeStruct
{
    auto FuncName(int x, int y) -> int;
};

auto SomeStruct::FuncName(int x, int y) -> int
{
    return x + y;
}

/************对象构造的改良****************/
//C++11允许构造函数调用其他构造函数
class SomeType {
    public:
    int number;
    std::string name;
    SomeType(int i, std::string& s ) : number(i), name(s){}
    SomeType(int i, char* s ) : number(i), name(s){}
    SomeType(): SomeType(0, "invalid" ){}
    SomeType( int i )  : SomeType( i, "guest" ){}
};

//C++11 允许派生类手动继承基底类型的构造函数
class BaseClass
{
public:
  BaseClass(){
      std::cout<<"BaseClass constructor\n";
  }
};
 
class DerivedClass : public BaseClass
{
public:
  using BaseClass::BaseClass;
};

//若是构造函数中没有设置iValue的初始值，则会采用类定义中的成员初始化，令iValue初值为5
class SomeClass
{
public:
  SomeClass() {}
  explicit SomeClass(int iNewValue) : iValue(iNewValue) {}
  int getvalue(){return iValue;}
private:
  int iValue = 5;
};


/*******************显式虚函数重载*********************/
struct Base {
    virtual void some_func(float){
        std::cout<<"Base  some_func float\n";
    }
};
 
struct Derived : Base {
    void some_func(int){
    } 
    virtual void some_func(float) {
        std::cout<<"Derived some_func float\n";
    }; // OK：顯式改寫
};

//错误事例
struct Base1 final { };
struct Base2 {
    virtual void f() final{}
};
 
//struct Derived1 : Base1 { }; //compiler error

struct Derived2 : Base2 {
    //virtual void f(){} //compiler error 
};

/******************模板的别名********************/
template< typename first, typename second, int third>
class SomeType4;

//template< typename second>
//typedef SomeType4<int, second, 5> TypedefName; //compiler error

//为了定义模板的别名，C++11 将会增加以下的语法：
template< typename first, typename second, int third>
class SomeType2;
template< typename second>
using TypedefName = SomeType2<int, second, 5>;

//using 也能在 C++11 中定义一般类型的别名，等同 typedef：
typedef void (*PFD)(double);        // 傳統語法
using PFD = void (*)(double);       // 新增語法



/*********************无限制的unions****************************/
struct point
{
  explicit point() {}
  point(int x, int y): x_(x), y_(y) {}
  int x_, y_;
};
static union
{
  int z;
  double w;
  //??? 编译不过
  //point p;
};

/********************使用或禁用对象的默认函数************************/
//要求类型带有默认构造函数
struct SomeType3
{
  SomeType3() = default;
  SomeType3(int value);
};

//禁止编译器自动产生某些函数
struct NonCopyable
{
  NonCopyable & operator=(const NonCopyable&) = delete;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable() = default;
};

//禁止类型以operator new配置内存
struct NonNewable
{
  void *operator new(std::size_t) = delete;
};

//禁止成员函数以特定的形参调用
struct NoDouble
{
  void f(int i);
  void f(double) = delete;
};

//禁止成员函数以非int形参调用
struct OnlyInt
{
  void f(int i);
  template<class T> void f(T) = delete;
};



template< class T >
struct Check
{
  static_assert( sizeof(int) <= sizeof(T), "T is not big enough!" ) ;
} ;

/***********************元编程**********************/
template<int B, int N>
struct Pow {
    // recursive call and recombination.
    enum{ value = B*Pow<B, N-1>::value };
};
 
template< int B > 
struct Pow<B, 0> { 
    // ''N == 0'' condition of termination.
    enum{ value = 1 };
};

// 演算法一
template< bool B > struct Algorithm {
    template<class T1, class T2> static int do_it (T1 &, T2 &)  { /*...*/ }
};
 
// 演算法二
template<> struct Algorithm<true> {
    template<class T1, class T2> static int do_it (T1, T2)  { /*...*/ }
};
 
// 根據給定的型別，實體化之後的 'elaborate' 會選擇演算法一或二
template<class T1, class T2> 
int elaborate (T1 A, T2 B) 
{
    // 若 T1 為 int 且 T2 為 float，選用演算法二
    // 其它情況選用演算法一
    return Algorithm<std::is_integral<T1>::value && std::is_floating_point<T2>::value>::do_it( A, B ) ;
}

struct Clear {
    int    operator()(int);     // 參數與回返值的型別相同
    double operator()(double);  // 參數與回返值的型別相同
};
 
//用于计算函数对象回返类型的统一方法
template <class Obj> 
class Calculus {
public:
    template<class Arg> Arg operator()(Arg& a) const
    {
        return member(a);
    }
private:
    Obj member;
};

struct Confused {
    double operator()(int);     // 參數與回返值的型別不相同
    int    operator()(double);  // 參數與回返值的型別不相同
};


template< class Obj >
class CalculusVer2 {
public:
    template<class Arg>
    typename std::result_of<Obj(Arg)>::type operator()(Arg& a) const
    { 
        return member(a);
    }
private:
    Obj member;
};


/************************变长参数模板******************************/
template<typename T, typename... Args>
void print(const char* s, T value, Args... args)
{
  while (*s)
  {
    if (*s == '%' && *(++s) != '%')
    {
      std::cout << value;
      printf(*s ? ++s : s, args...); // 即便当 *s == 0 也会产生调用，以检测更多的类型参数。
      return;
    }
    std::cout << *s++;
  }
  throw std::logic_error("extra arguments provided to printf");
}

template<typename... Values> 
class tuple2{
};

struct A{
    A(){
        std::cout<<"A constructor\n";
    }
    ~A(){
        std::cout<<"A destructor\n";
    }
    A(A& a){
        std::cout<<"A copy constructor\n";
    }
};
struct B{
    B(){
        std::cout<<"B constructor\n";
    }
    B(B& b){
        std::cout<<"B copy constructor\n";
    }
};

template <typename... BaseClasses> 
class ClassName : public BaseClasses...
{
    public:
        ClassName (BaseClasses&&... baseClasses) : BaseClasses(baseClasses)... {}
};

template<typename TypeToConstruct> 
struct SharedPtrAllocator
{
  template<typename... Args> 
  std::shared_ptr<TypeToConstruct> ConstructWithSharedPtr(Args&&... params)
  {
    return std::shared_ptr<TypeToConstruct>(new TypeToConstruct(std::forward<Args>(params)...));
  }
};

template<typename ...Args> 
struct SomeStruct2
{
  static const int size = sizeof...(Args);
};

int main(int argc, const char *argv[])
{
    {
        //move 语义
        std::string foo = "foo-string";
        std::string bar = "bar-string";
        std::vector<std::string> myvector;
        printf("foo's addr %x bar's addr %x \n", static_cast<void*>(&foo[0]), static_cast<void*>(&bar[0]));
        myvector.push_back (foo);                    // copies
        myvector.push_back (std::move(bar));         // moves
        printf("in vector %s's addr %x %s's addr %x\n", myvector[0].c_str(), static_cast<void*>(&myvector[0][0]), myvector[1].c_str(), static_cast<void*>(&myvector[1][0]));
        printf("foo is %s  bar is %s\n", foo.c_str(), bar.c_str());

    }
    {
        //泛化的常数表示式
        int some_value[GetFive() + 5];
        double forceOfGravity = 9.8;
        double moonGravity = forceOfGravity / 6.0;
    }
    {
        //初始化列表
        FunctionName({1,2,0});
        std::vector<int> v = {1,2,3};
    }
    {
        //统一的初始化
        BasicStruct var1{5, 3.2f};
    }
    {
        //类型推导
        auto otherVariable = "hello world";
        int someInt;
        decltype(someInt) otherIntegerVariable = 5;

        printf("otherVaiable is %s otherIntegerVariable  is %d\n", otherVariable, otherIntegerVariable );

    }
    {
        //以范围为基础的 for 循环
        std::vector<int> my_vec = {1, 2, 3, 4, 5};
        for (int &x : my_vec)
        {
            x *= 2;
        } 
        printf("my_vec is ");
        for(auto it = my_vec.begin(); it != my_vec.end(); ++it){
            printf("%d ", *it);
        }
        printf("\n");
    }
    {
        //Lambda函数与表示式
        std::vector<int> someList={1,2,3,4};
        int total = 0;
        std::for_each(someList.begin(), someList.end(), [&total](int x) {
                total += x;
                });
        std::cout <<"total is "<< total<<std::endl;  

        total = 0;
        int value = 5;
        [&, value]() { total += (2 * value); };
        std::cout <<"total is "<< total<<std::endl;  

        auto myOnheapLambdaFunc = new auto([=] { /*...*/ });
    }
    {
        //回返类型后置的函数声明
        SomeStruct s;
        std::cout<<"SomeStruct .FuncName is "<<s.FuncName(1, 2)<<std::endl;
    }
    {
        //对象构造的改良
        SomeType a(1, "string"), b(1), c; 
        printf("a %d %s    b %d %s  c  %d %s\n",a.number, a.name.c_str(), b.number, b.name.c_str(), c.number, c.name.c_str());
        DerivedClass d{}; 
        SomeClass e;
        std::cout<<"SomeClass default value is "<<e.getvalue()<<std::endl;
    }
    {
        //显式虚函数重载
        Base *pa = new Derived();
        pa->some_func(1.0);
    }
    {
        //空指针
        char* pc = nullptr;     // OK
        int * pi = nullptr;     // OK
        //int    i = nullptr;     //compiler error
        foo(nullptr);
    }
    {
        //强类型枚举
        //std::cout<<(Enumeration::Val1==100);//compiler error
        enum class Enumeration
        {
            Val1,
            Val2,
            Val3 = 100,
            Val4 /* = 101 */,
        };
        //enum Enum1;                   //compiler error 不合法的 C++ 與 C++11; 無法判別大小
        enum Enum2 : unsigned int;      // 合法的 C++11
        enum class Enum3;               // 合法的 C++11，列舉類別使用預設型別 int 
        enum class Enum4: unsigned int; // 合法的 C++11
        //enum Enum2 : unsigned short;  //compiler error 不合法的 C++11，Enum2 已被聲明為 unsigned int
    }
    {
        //静态assertion
        //static_assert( 3.14 < 5, "GREEKPI is inaccurate!" ) ;//compile error
    }
    {
        //允许sizeof运算符作用在类型的数据成员上，无须明确的对象
        std::cout<<"BasicStruct's y sizeof is  "<<sizeof(BasicStruct::y)<<std::endl;
    }
    //{
         ////角括号
        //template<bool bTest> SomeType2;
        //std::vector<SomeType2<1>2>> x1;   //  std::vector of "SomeType<true> 2>"， 非法的表示式， 整數 1 被轉換為 bool 型別 true
        //std::vector<SomeType2<(1>2)>> x2; // 解讀為 std::vector of "SomeType<false>", 合法的 C++11 表示式， (1>2) 被轉換為 bool 型別 false
    //}
    {
        //变长参数模板
        class tuple2<int, std::vector<int>, std::map<std::string, int > > someInstanceName;
        A a;
        B b;
        std::cout<<"A B init finish\n";
        ClassName<A, B> c(std::move(a),std::move(b));
        SomeStruct2<int, float, char > d;
        std::cout<<"SomeStruct2 size is "<<d.size<<std::endl;
    }
    {
        //使用或禁用对象的默认函数
        NonCopyable a,b;
        //a = b;//comiler error
        
        //NonNewable* c= new NonNewable;//comiler error
        
        NoDouble d;
        //d.f(1.0);//comiler error

        OnlyInt e;
        //e.f("xxx");
    }
    {
        //多元组类别
        typedef std::tuple <int, double, long &, const char *> test_tuple;
        long lengthy = 12;
        test_tuple proof (18, 6.5, lengthy, "Ciao!");
        lengthy = std::get<0>(proof);  // 將 proof 的第一個元素賦值給 lengthy (索引從零開始起跳)
        std::get<3>(proof) = " Beautiful!";  // 修改 proof 的第四個元素 

        typedef std::tuple< int , double, std::string> tuple_1;
        tuple_1 t1;
        typedef std::tuple< char, short , const char *> tuple_2;
        tuple_2 t2 ('X', 2, "Hola!");
        t1 = t2 ;  // 可行。前兩個元素會作型別轉換，
    }
    {
        //正则表达式
        const char *reg_esp = "[ ,.\\t\\n;:]" ;  // 分隔字元列表
        std::regex rgx(reg_esp) ;  // 'regex' 是樣板類 'basic_regex' 以型別為 'char' 
        //  的參數具現化的實體
        std::cmatch match ;  // 'cmatch' 是樣板類 match_results' 以型別為 'const char *'
        // '的參數具現化的實體
        const char *target = "Polytechnic University of Turin " ;
        // 辨別所有被分隔字元所分隔的字
        if( regex_search( target, match, rgx ) )
        {
            // 若此種字存在
            const size_t n = match.size();
            for( size_t a = 0 ; a < n ; a++ )
            {
                std::string str( match[a].first, match[a].second ) ;
                std::cout << str << "\n" ;
            }
        }
    }
    {
        //通用智能指针
        std::shared_ptr<double> p_first(new double) ;

        {
            std::shared_ptr<A> p_copy(new A);
        }  
    }
    {
        //可扩展的随机数功能
        std::uniform_int_distribution<int> distribution(0, 99); // 以離散型均勻分佈方式產生 int 亂數，範圍落在 0 到 99 之間
        std::mt19937 engine; // 建立亂數生成引擎
        auto generator = std::bind(distribution, engine); // 利用 bind 將亂數生成引擎和分布組合成一個亂數生成物件
        int random = generator();  // 產生亂數
    }
    {
        //包装引用
        int i = 0 ;
        g (f, i) ;  
        std::cout << i << std::endl;  // 輸出 0
        g (f, std::ref(i)); 
        std::cout << i << std::endl;  // 輸出 1
    }
    {
        //多态函数对象包装器
        std::function<int (int, int)> func;  // 利用樣板類 'function'
        // 建立包裝器
        std::plus<int> add;  // 'plus' 被宣告為 'template<class T> T plus( T, T ) ;'
        //  因此 'add' 的型別是 'int add( int x, int y )'
        func = add;  // 可行。'add' 的型參和回返值型別與 'func' 相符
        int a = func (1, 2);  // 注意: 若包裝器 'func' 沒有參考到任何函式
        // 會丟出 'std::bad_function_call' 例外
        std::function<bool (short, short)> func2 ;
        if(!func2) { // 因為尚未賦值與 'func2' 任何函式，此條件式為真
            bool adjacent(long x, long y);
            func2 = &adjacent ;  // 可行。'adjacent' 的型參和回返值型別可透過型別轉換進而與 'func2' 相符
            struct Test {
                bool operator()(short x, short y){
                    return true;
                }
            };
            Test car;
            func = std::ref(car);  // 樣板類 'std::ref' 回傳一個 struct 'car'
            // 其成員函式 'operator()' 的包裝
        }
        func = func2;  // 可行。'func2' 的型參和回返值型別可透過型別轉換進而與 'func' 相符
    }
    {
        //元编程
        int quartic_of_three = Pow<3, 4>::value;
    }
    return 0;
}
