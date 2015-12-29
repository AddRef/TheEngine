#pragma once
#include <memory>
#include <list>

template <typename TCallback>
class Receiver
    : public TCallback
    , protected std::enable_shared_from_this<TCallback>
{
    friend class Emitter;

    using Ptr = std::shared_ptr<TCallback>;
    using WPtr = std::weak_ptr<TCallback>;

    Ptr _getPtr()
    {
        return this->shared_from_this();
    }

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
        typedef TCallback TCallback;
        virtual ~Holder() {}
        virtual void AddCallback(Receiver<TCallback>& receiver)
        {
            m_callbacks.push_back(receiver._getPtr());
        }
        virtual void RemoveCallback(Receiver<TCallback>& receiver)
        {
            for (auto iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
            {
                Receiver<TCallback>::Ptr ptr = iter->lock();
                if (ptr == receiver._getPtr())
                {
                    iter = m_callbacks.erase(iter);
                }
            }
        }
        std::list<TCallback*> GetCallback()
        {
            std::list<TCallback*> callbacks;
            for (auto iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
            {
                Receiver<TCallback>::Ptr callback = iter->lock();
                if (callback)
                {
                    callbacks.push_back(callback.get());
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

public:
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

    template <typename TCallback>
    std::list<TCallback*> GetCallback()
    {
        auto holder = _findHolder<TCallback>();
        if (!holder) return std::list<TCallback*>();
        return holder->GetCallback();
    }

private:
    template <typename TCallback>
    Holder<TCallback>* _findHolder()
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
