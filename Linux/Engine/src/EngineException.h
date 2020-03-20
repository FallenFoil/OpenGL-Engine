//
// Created by cesar on 4/3/19.
//

#ifndef ENGINE_ENGINEEXCEPTION_H
#define ENGINE_ENGINEEXCEPTION_H

#include <exception>
#include <string>

class EngineException : public std::exception{
private:
    std::string message;
public:
    explicit EngineException (const std::string& message){this->message = message;};
    virtual const char* what() const throw() {
        return message.c_str();
    }
};


#endif //ENGINE_ENGINEEXCEPTION_H
