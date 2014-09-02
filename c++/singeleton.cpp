#include <iostream>
//.h
class QMManager
{
    protected:
        struct object_creator
        {
            object_creator()
            {
                QMManager::instance();
            }
            inline void do_nothing() const {}
        };
        static object_creator create_object_;

        QMManager();
        ~QMManager(){};
    public:
        static QMManager *instance()
        {
            static QMManager instance;
            return &instance;
        }
};

class QMSqlite
{
    protected:
        QMSqlite();
        ~QMSqlite(){};
        struct object_creator
        {
            object_creator()
            {
                QMSqlite::instance();
            }
            inline void do_nothing() const {}
        };
        static object_creator create_object_;
    public:
        static QMSqlite *instance()
        {
            static QMSqlite instance;
            return &instance;
        }
        void do_something();
};
QMManager::QMManager()
{
    printf("QMManager constructor\n");
    QMSqlite::instance()->do_something();
}

QMSqlite::QMSqlite()
{
    printf("QMSqlite constructor\n");
}
void QMSqlite::do_something()
{
    printf("QMSqlite do_something\n");
}


QMManager::object_creator QMManager::create_object_;


QMSqlite::object_creator QMSqlite::create_object_;

int main(int argc, const char *argv[])
{
    /* code */
    return 0;
}
