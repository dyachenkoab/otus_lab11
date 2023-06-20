#include "person.h"

person::person(size_t id, const std::string &name) : creature(name), id_(id) { }

const size_t &person::id() const
{
    return id_;
}

void person::print() const
{
    std::cout << "Person: " << name() << ". with id: " << id_ << std::endl;
}
