#pragma once
#include <memory>
#include <set>

template <typename TCallback>
class Receiver
    : public TCallback
    , protected std::enable_shared_from_this<TCallback>
{
    template <typename T>
    friend class Emitter;
    
    using Ptr = std::shared_ptr<TCallback>;
    using WPtr = std::WPtr<TCallback>;
    
    Ptr& GetPtr()
    {
        return this->shared_from_this();
    }
    
};


template <typename TCallback>
class Emitter
{
public:
    void AddCallback(Receiver<TCallback>& receiver)
    {
        m_callbacks.insert(receiver.GetPtr());
    }
    
    void RemoveCallback(Receiver<TCallback>& receiver)
    {
        if (!m_callbacks.empty())
        {
            m_callbacks.erase(receiver.GetPtr());
        }
    }
    
private:
    std::set< typename Receiver<TCallback>::WPtr > m_callbacks;

};

