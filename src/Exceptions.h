#pragma once

#include <exception>

class Exception: public std::exception {
protected:
    const char* msg;
public:
    explicit Exception(const char* msg): msg(msg) { }
    virtual ~Exception() = default;
    const char* what() const noexcept override {
            return this->msg;
    }
};

class NegativeMassException : public Exception {
public:
    explicit NegativeMassException(const char* msg) : Exception(msg) { }
};

class MassExceedException : public Exception {
public:
    explicit MassExceedException(const char* msg) : Exception(msg) { }
};