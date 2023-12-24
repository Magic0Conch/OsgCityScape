#pragma once
#include <atomic>
#include <limits>

namespace CSEditor::ECS{
using ObjectID = std::size_t;
constexpr ObjectID invalidObjectID = std::numeric_limits<std::size_t>::max();

class ObjectIDAllocator
{
public:
    static ObjectID alloc();

private:
    static std::atomic<ObjectID> m_nextID;
};
} 
