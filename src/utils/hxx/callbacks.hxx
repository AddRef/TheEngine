#pragma once
#include <memory>
#include <list>
#include <stdexcept>

template <typename TCallback>
class Receiver : public TCallback
{
    friend class Emitter;

    using TSelfPointer = Receiver<TCallback>*;
    using Ptr = std::shared_ptr< TSelfPointer >;
    using WPtr = std::weak_ptr< TSelfPointer >;

public:
    Receiver()
        : m_self_pointer(new TSelfPointer(static_cast<TSelfPointer>(this)))
    {
    }

    Ptr _getPtr()
    {
        return m_self_pointer;
    }
    // We can't create object of type Receiver but we can create shared pointer on its pointer
    // which will show us if object is still alive
    std::shared_ptr< TSelfPointer > m_self_pointer;
};

class Emitter
{
    // Holder to keep Emitter non-template class. Instead individual functions are templated.
    // That requires if you want to define list of callbacks inside class deriverd from Emitter
    struct IHolder
    {
        using UPtr = std::unique_ptr<IHolder>;
        virtual ~IHolder() {}
    };
    template <typename TCallback>
    class Holder : public IHolder
    {
    public:
        virtual ~Holder() {}
        virtual void AddCallback(Receiver<TCallback>& receiver)
        {
            typename Receiver<TCallback>::Ptr ptr = receiver._getPtr();
            m_callbacks.push_back(ptr);
        }
        virtual void RemoveCallback(Receiver<TCallback>& receiver)
        {
            for (auto iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
            {
                typename Receiver<TCallback>::Ptr ptr = iter->lock();
                if (ptr == receiver._getPtr())
                {
                    iter = m_callbacks.erase(iter);
                }
            }
        }
        std::list<TCallback*> GetCallback()
        {
            std::list<TCallback*> callbacks;
            for (auto iter = m_callbacks.begin(); iter != m_callbacks.end();)
            {
                typename Receiver<TCallback>::Ptr callback = iter->lock();
                if (callback)
                {
                    callbacks.push_back(*callback.get());
                    ++iter;
                }
                else
                {
                    iter = m_callbacks.erase(iter);
                }
            }
            return callbacks;
        }
    private:
        std::list< typename Receiver<TCallback>::WPtr > m_callbacks;
    };

protected:
    // For usage by inherited class
    Emitter()
    {
    }

    // IMPORTANT: Don't forget to call this function before using other stuff
    // This is a cost for keeping Emitter non-template class
    template <typename TCallback>
    void Init()
    {
        auto existing_holder = _findHolder<TCallback>();
        if (!existing_holder)
        {
            IHolder::UPtr holder(new Holder<TCallback>());
            m_holders.push_back(std::move(holder));
        }
    }
    template <typename TCallback>
    std::list<TCallback*> GetCallback() const
    {
        auto holder = _findHolder<TCallback>();
        if (!holder) return std::list<TCallback*>();
        return holder->GetCallback();
    }
public:
    // For public usage
    template <typename TCallback>
    void AddCallback(Receiver<TCallback>& receiver)
    {
        auto holder = _findHolder<TCallback>();
        if (!holder) throw std::runtime_error("Invalid callback type is passed");
        holder->AddCallback(receiver);
    }

    template <typename TCallback>
    void RemoveCallback(Receiver<TCallback>& receiver) 
    {
        auto holder = _findHolder<TCallback>();
        if (!holder) throw std::runtime_error("Invalid callback type is passed");
        holder->RemoveCallback(receiver);
    }
private:
    template <typename TCallback>
    Holder<TCallback>* _findHolder() const
    {
        for (const IHolder::UPtr& holder : m_holders)
        {
            Holder<TCallback>* typed_holder = dynamic_cast<Holder<TCallback>*>(holder.get());
            if (typed_holder)
            {
                return typed_holder;
            }
        }
        return nullptr;
    }

    std::list<IHolder::UPtr> m_holders;
};
