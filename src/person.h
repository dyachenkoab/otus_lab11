#ifndef PERSON_H
#define PERSON_H
#include <odb/core.hxx>
#include "creature.h"
#include <string>
#include <iostream>

#pragma db object
class person : public creature
{
public:
    person(size_t id, const std::string &name);
    const size_t &id() const;
    void print() const;

private:
    person() = default;
    friend class odb::access;
#pragma db id
    size_t id_;
};

#endif // PERSON_H
