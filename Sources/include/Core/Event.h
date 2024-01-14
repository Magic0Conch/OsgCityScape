#pragma once
#include <functional>
#include <stdint.h>
namespace CSEditor::Core{

template <typename ... ArgsType>
class Event{
public:
    using Callback = std::function<void(ArgsType...)>;
    
    void operator+=(Callback call){
        addListener(call);
    }

    void addListener(Callback call){
        m_calls.push_back(call);
    }
    void clear(){
        m_calls.clear();
    }
    void invoke(ArgsType... args){
        for (auto func:m_calls){
            func(args...);
        }
    }
private:
    std::vector<Callback> m_calls;
};

}
