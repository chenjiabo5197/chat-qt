#ifndef SINGLETON_H
#define SINGLETON_H
#include <global.h>

/******************************************************************************
 *
 * @file       singleton.h
 * @brief      单例的基类
 *
 * @author     陈佳波
 * @date       2024/12/09
 * @history
 *****************************************************************************/

template <typename T>
class Singleton{
// 使用protected修饰构造函数是为了在继承singleton的类中可以构造基类
protected:
    Singleton() = default;
    // 不允许拷贝构造和拷贝赋值
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;
    // 智能指针的单例实例，可以自动回收，避免手动回收
    // 静态成员变量是为了在使用时只返回一个实例
    static std::shared_ptr<T> _instance;

public:
    // 静态成员函数可以用来实现线程安全的实例创建和访问。避免不必要的资源消耗和重复创建对象的开销
    static std::shared_ptr<T> GetInstance(){
        // 用于确保某个特定的函数或代码块在多线程环境中只被执行一次
        static std::once_flag s_flag;
        // 调用是线程安全的，它内部使用互斥锁来保护对 std::once_flag 状态的访问。
        std::call_once(s_flag, [&](){
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress(){
        // 获取 std::shared_ptr 所管理的原始指针（即裸指针）
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton(){
        std::cout << "singleton destory" << std::endl;
    }
};

// 类的静态成员变量必须初始化
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;


#endif // SINGLETON_H
