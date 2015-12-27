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


template <typename TBaseClass, typename TCallback>
class Broadcaster
{
public:
    template < typename std::enable_if<std::is_base_of<TCallback, BroadcastCallback> >
    void AddCallback(TCallback& callback)
    {
        BroadcastCallback* broadcast_callback = static_cast<BroadcastCallback>(callback);
        m_callbacks.insert(broadcast_callback->GetPtr());
    }
    void AddCallback(TCallback& callback)
    {
        throw std::runtime_error("invalid callback type has been passed");
    }

    std::set<TCallback> GetCallbacks() const
    {
        for (const auto& callback : m_callbacks)
        {
        }
    }
private:
    std::set< std::weak_ptr<BroadcastCallback> > m_callbacks;
};

class BroadcastCallback : protected std::enable_shared_from_this<BroadcastCallback>
{
    template <typename TBaseClass, typename TCallback>
    friend class Broadcaster;

    using Ptr = std::weak_ptr<const BroadcastCallback>;

    const Ptr& GetPtr() const
    {
        return shared_from_this();
    }
};