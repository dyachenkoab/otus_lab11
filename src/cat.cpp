#include "cat.h"

cat::cat(size_t id, const std::string &name) : creature(name), id_(id) { }

const size_t &cat::id() const
{
    return id_;
}

void cat::print() const
{
    std::cout << "Cat: " << name() << ". with id: " << id_ << std::endl;
}
