#ifndef CAT_H
#define CAT_H
#include <odb/core.hxx>
#include <string>
#include <iostream>
#include "creature.h"

#pragma db object
class cat : public creature
{
public:
    cat(size_t id, const std::string &name);
    const size_t &id() const;
    void print() const;

private:
    cat() = default;
    friend class odb::access;
#pragma db id
    size_t id_;
};

#endif // CAT_H
