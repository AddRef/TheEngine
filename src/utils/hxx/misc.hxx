#pragma once
#include <memory>

template <typename TBase>
struct Shared
{
	using Ptr = std::shared_ptr<TBase>;
	using CPtr = std::shared_ptr<TBase>;
};