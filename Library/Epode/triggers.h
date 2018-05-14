//
//
// File - Epode/triggers.h:
//
//      Implementation of end trigger objects and related object creation functions.  Included
//  are value, statistics and time triggers in addition to the placeholder types, conditional
//  operators and boolean operators to combine them.
//
//  TODO: THIS NEEDS TO BE IMPLEMENTED FROM THE TEST CODE NOT IN THE EPODE REPO
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//
#ifndef EPODE_TRIGGERS_H
#define EPODE_TRIGGERS_H

#include <initializer_list>
#include <type_traits>

namespace epode
{
namespace triggers
{
namespace internal
{
    // TODO: THE MAIN TRIGGER TYPES SHOULD GO HERE
} /*namespace internal*/

// TODO: THE MAIN TRIGGER FUNCTIONS SHOULD GO HERE... OR EVEN IN THE BASE EPODE NAMESPACE...?

namespace internal
{

// Construct an End Trigger Function from a Single Value
template<typename Value>
auto constructEndTrigger(Value v1) {
    return [=](auto /*dv*/, auto v, auto /*y*/, auto /*stats*/, auto limits) -> bool {
        return v > (v1 - limits.min);
    };
}

// Construct an End Trigger Function from an Initializer List
template<typename Value>
auto constructEndTrigger(std::initializer_list<Value> vs) {
    Value v1;
    for (auto it = vs.begin(); it != vs.end(); ++it) {
        v1 = *it;
    }
    return [=](auto /*dv*/, auto v, auto /*y*/, auto /*stats*/, auto limits) -> bool {
        return v > (v1 - limits.min);
    };
}

// Construct an End Trigger as a Copy -- Disable of trig is convertible to Value
template<typename Value,
         typename Trigger,
         typename = std::enable_if_t<!std::is_convertible<Trigger, Value>::value> >
Trigger constructEndTrigger(Trigger trig) {
    return trig;
}

} /*namespace internal*/


} /*namespace triggers*/
} /*namespace epode*/

#endif // EPODE_TRIGGERS_H

