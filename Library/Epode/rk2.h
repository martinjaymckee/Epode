//
//
// File - Epode/rk2.h:
//
//      Implementation of a selection of Runge-Kutta 2nd-order methods.  Initially, a generic
//  Runge-Kutta 2nd-order class is implemented and the remainder of the methods -- Heun's,
//  Ralston's and Explicit Midpoint -- are derived from that.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_RK2_H
#define EPODE_RK2_H

#include <iostream>

#include "core.h"

namespace epode
{
namespace method
{

template<typename Value, size_t N>
class GenericRK2 : public internal::Fixed<Value, 2>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        explicit constexpr GenericRK2(const value_t& _eta)
            : c0(_eta),
              c2(value_t(1) / (value_t(2)*_eta)),
              c1(value_t(1) - c2)
        {}

        return_t operator () (auto func, value_t dv, value_t v, state_t y, auto) {
            const auto k0 = func(v, y);
            const auto k1 = func(v+(c0*dv), y+(c0*dv*k0));
            return return_t{dv, dv, y + (dv * (c1*k0 + c2*k1)), 2};
        }

    private:
        const value_t c0;
        const value_t c2;
        const value_t c1;
};

//
// Implement specific second-order Runge-Kutta methods
//

//  Heun's Method, eta = 1
template<typename Value, size_t N>
class Heuns : public GenericRK2<Value, N>
{
    public:
        Heuns() : GenericRK2<Value, N>(Value(1)) {}
};

//  Explicit Midpoint Method, eta = 1/2
template<typename Value, size_t N>
class Midpoint : public GenericRK2<Value, N>
{
    public:
        Midpoint() : GenericRK2<Value, N>(Value(1)/Value(2)) {}
};

//  Ralston's Method, eta = 2/3
template<typename Value, size_t N>
class Ralstons : public GenericRK2<Value, N>
{
    public:
        Ralstons() : GenericRK2<Value, N>(Value(2)/Value(3)) {}
};

} /*namespace method*/
} /*namespace epode*/

#endif // EPODE_RK2_H

