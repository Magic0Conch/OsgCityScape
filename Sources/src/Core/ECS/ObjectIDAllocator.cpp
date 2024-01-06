#include <assert.h>
#include <atomic>
#include <limits>
#include "Core/ECS/ObjectIDAllocator.h"
#include "spdlog/spdlog.h"
using namespace CSEditor::ECS;

std::atomic<ObjectID> ObjectIDAllocator::ObjectIDAllocator::m_nextID {0};

ObjectID ObjectIDAllocator::alloc(){
    std::atomic<ObjectID> newObjectID = m_nextID.load();
    ++m_nextID;
    // if (m_nextID >= invalidObjectID){
    //     spdlog::error("Object id overflow!");
    //     assert(0);
    // }

    return newObjectID;
}