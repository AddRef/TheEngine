#pragma once
#include <memory>

template <typename TBase>
struct Shared
{
	using Ptr = std::shared_ptr<TBase>;
	using CPtr = std::shared_ptr<TBase>;
};

struct NotCopyable
{
    NotCopyable() {}
    NotCopyable& operator = (const NotCopyable&) = delete;
    NotCopyable(const NotCopyable&) = delete;
};