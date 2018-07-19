#pragma once
#include "fmt.h"
#include "list.h"
#include "log.h"
#include "sort.h"
#include "utf8.h"
namespace bidi {
constexpr Class AL{13};
constexpr Class AN{5};
moku::slice<uint8_t> AppendReverse(moku::slice<uint8_t> out,
                                   moku::slice<uint8_t> in);
constexpr Class B{7};
constexpr Class BN{11};
constexpr Class CS{6};

struct Class : public moku::basic<unsigned int> {
    Class() : moku::basic<unsigned int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<Class>(const Class &class)
{
    return moku::is_nil<unsigned int>(unsigned int(class));
}
inline bool in(moku::slice<Class> set)
{
    Class class = *this;
    return class._inByValue(moku::slice<Class> set);
}
bool _inByValue(moku::slice<Class> set);
constexpr Class Control{14};
Option DefaultDirection(Direction d);

struct Direction : public moku::basic<int> {
    Direction() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<Direction>(const Direction &direction)
{
    return moku::is_nil<int>(int(direction));
}
constexpr Class EN{2};
constexpr Class ES{3};
constexpr Class ET{4};
constexpr Class FSI{23};
constexpr Class L{0};
constexpr Class LRE{18};
constexpr Class LRI{21};
constexpr Class LRO{16};
constexpr Direction LeftToRight{0};
std::tuple<Properties, int> Lookup(moku::slice<uint8_t> s);
std::tuple<Properties, int> LookupRune(int32_t r);
std::tuple<Properties, int> LookupString(std::string s);
constexpr Direction Mixed{2};
constexpr Class NSM{12};
constexpr Direction Neutral{3};
constexpr Class ON{10};
} // namespace bidi
