#pragma once
#include <exception>
#include <string>

class Exception : public std::exception {
private:
    std::string message;

public:
    explicit Exception(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class NegativeMassException : public Exception {
public:
    explicit NegativeMassException(const std::string& msg) : Exception(msg) {}
};

class MassExceedException : public Exception {
public:
    explicit MassExceedException(const std::string& msg) : Exception(msg) {}
};