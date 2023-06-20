#ifndef CREATURE_H
#define CREATURE_H
#include <odb/core.hxx>
#include <string>

#pragma db object abstract
class creature
{
public:
    creature(const std::string &name);
    ~creature() = default;

    const std::string &name() const;

protected:
    creature() = default;
    friend class odb::access;

private:
    std::string name_;
};

#endif // CREATURE_H
