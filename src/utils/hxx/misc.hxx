#pragma once
#include <memory>
#include <string>

template <typename TBase>
struct Shared
{
	using Ptr = std::shared_ptr<TBase>;
	using CPtr = std::shared_ptr<TBase>;
};

class ObjectType
{
public:
    template<typename T>
    ObjectType(const T&)
        : m_type(typeid(T).name()) {}
    bool operator==(const ObjectType& right)
    {
        return m_type == right.m_type;
    }
private:
    std::string m_type;
};

struct QueryEnabled
{
    template<typename T>
    bool Query(T** out_object)
    {
        T* ret = dynamic_cast<T>(this);
        *out_object = ret;
        return !!ret;
    }

    ObjectType GetType() { return ObjectType(*this); }
};