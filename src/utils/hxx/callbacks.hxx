#pragma once
#include <memory>
#include <set>
#include <iostream>

class Broadcaster
{
    using TSafeCallbackWPtr = std::weak_ptr<SafeCallback>;
public:
    void AddCallback(SafeCallback& callback)
    {
        m_callbacks.insert(callback.GetHandle());
    }
private:
    std::set<TSafeCallbackWPtr> m_callbacks;
};

class SafeCallback : std::enable_shared_from_this<SafeCallback>
{
    friend class Broadcaster;
    std::weak_ptr<SafeCallback> GetHandle() { return shared_from_this(); }
};

template <typename TBaseClass, typename TCallback>
class Emitter : std::enable_shared_from_this<TBaseClass>
{
    using TCallbackCPtr = std::shared_ptr<const TCallbackType>;
    using TCallbackWPtr = std::weak_ptr<const TCallbackType>;
public:
    void AddCallback(const TCallback& callback)
    {
        m_subscribers.insert(callback);
    }
    void RemoveCallback(const TCallback& callback)
    {

    }
    void Notify()
    {

    }


    std::set<TCallbackWPtr> m_subscribers;
};

class EmitterImpl : public Emitter<EmitterImpl, EmitterImpl::ICallback>
{
public:
    struct ICallback
    {
        virtual void Callback() = 0;
    };
};

class RecieverImpl : protected EmitterImpl::ICallback
{
public:
    virtual void Callback() override
    {
        std::cout << "Callback" << std::endl;
    }
};

void main()
{
    EmitterImpl emitter;
    {
        RecieverImpl reciever;
        emitter.AddCallback(reciever);
        emitter.Notify();
    }
    emitter.Notify();

}