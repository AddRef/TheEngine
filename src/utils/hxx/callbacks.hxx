#pragma once
#include <memory>
#include <set>

template <typename TCallback>
class Broadcaster
{
public:
    class Callback
        : public TCallback
        , protected std::enable_shared_from_this<TCallback>
    {
        template <typename T>
        friend class Emitter;
        
        using Ptr = std::shared_ptr<TCallback>;
        using WPtr = std::weak_ptr<TCallback>;
        
        WPtr& GetWPtr()
        {
            return this->shared_from_this();
        }
        
    };

    
    void AddCallback(Receiver<TCallback>& receiver)
    {
        m_callbacks.insert(receiver.GetWPtr());
    }
    
    void RemoveCallback(Receiver<TCallback>& receiver)
    {
        if (!m_callbacks.empty())
        {
            m_callbacks.erase(receiver.GetWPtr());
        }
    }
    
private:
    std::set< typename Receiver<TCallback>::WPtr > m_callbacks;

};

