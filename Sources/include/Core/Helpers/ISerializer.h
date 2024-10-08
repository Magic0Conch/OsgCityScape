#pragma once
#include "json11.hpp"
#include "Serializer.h"


namespace CSEditor::Helpers{
class ISerializable{
public:
    virtual void serialize(Json& jsonObject) = 0;
    virtual void deserialize(Json& jsonObject) = 0;
    virtual ~ISerializable() = default;
};
}