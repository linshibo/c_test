#include <iostream>
using namespace std;

class Foo
{
public:
        Foo() { cout << "Foo constructor" << endl; }
        Foo(const Foo &foo) { cout << "Foo copy constructor" << endl; }
        ~Foo() { cout << "Foo deconstructor" << endl; }
};

class Bar
{
public:
        Bar() { cout << "Bar constructor" << endl; }
        Bar(const Bar &bar) { cout << "Bar copy constructor" << endl; }
        ~Bar() { cout << "Bar deconstructor" << endl; }
};

class Base
{
public:
        Base() { cout << "Base constructor" << endl; }
        ~Base() { cout << "Base deconstructor" << endl; }
};

class Derived : public Base
{
public:
        Derived() { cout << "Derived constructor without arguments" << endl; }
        Derived(const Foo &foo, const Bar &bar);
        Derived(const Bar &bar, const Foo &foo);
        ~Derived() { cout << "Derived deconstructor" << endl; }

private:
        Foo m_foo;
        Bar m_bar;
};

Derived::Derived(const Foo &foo, const Bar &bar) :
        m_foo(foo),
        m_bar(bar)
{
        cout << "Derived constructor with argument[Foo foo, Bar bar] passed by references" << endl;
}

Derived::Derived(const Bar &bar, const Foo &foo) :
        m_bar(bar),
        m_foo(foo)
{
        cout << "Derived constructor with argument[Bar bar, Foo foo] passed by references" << endl;
}

int main (int argc, char** argv)
{
        Foo foo;
        Bar bar;

        cout << "test case 1:" << endl;
        Derived deri_1;  //  (1)

        cout << "test case 2:" << endl;
        Derived deri_2(foo, bar);   //  (2)

        cout << "test case 3:" << endl;
        Derived deri_3(bar, foo);   //  (3)

        cout << "test case end" << endl;

        return 0;
}
