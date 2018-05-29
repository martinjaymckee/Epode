//
//
// File - Epode/butcher.h:
//
//      Implementation of the Butcher's 5th-order method.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_BUTCHERS5TH_H
#define EPODE_BUTCHERS5TH_H

#include "core.h"

namespace epode
{
namespace method
{

template<typename Value, size_t N>
class Butcher5th : public internal::Fixed<Value, 5>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Fixed<Value, 5>::Fixed; // Inherit Construtors

        return_t operator () (auto func, value_t dv, value_t v, state_t y, auto) {
            constexpr auto c0 = value_t(1) / value_t(4);
            constexpr auto c1 = value_t(1) / value_t(8);
            constexpr auto c2 = value_t(1) / value_t(2);
            constexpr auto c3 = value_t(3) / value_t(4);
            constexpr auto c4 = value_t(3) / value_t(16);
            constexpr auto c5 = value_t(9) / value_t(16);
            constexpr auto c6 = value_t(-3) / value_t(7);
            constexpr auto c7 = value_t(2) / value_t(7);
            constexpr auto c8 = value_t(12) / value_t(7);
            constexpr auto c9 = value_t(8) / value_t(7);
            constexpr auto c10 = value_t(1) / value_t(90);
            constexpr auto c11 = value_t(7);
            constexpr auto c12 = value_t(32);
            constexpr auto c13 = value_t(12);

            const auto k0 = func(v, y);
            const auto k1 = func(v+(c0*dv), y+(c0*dv*k0));
            const auto k2 = func(v+(c0*dv), y+(c1*dv*(k0 + k1)));
            const auto k3 = func(v+(c2*dv), y+(dv*(k2 - c2*k1)));
            const auto k4 = func(v+(c3*dv), y+(dv*(c4*k0 + c5*k3)));
            const auto k5 = func(v+dv, y+(dv*(c6*k0 + c7*k1 + (c8*(k2-k3)) + c9*k4)));

            return return_t{dv, dv, y + (dv * c10 * (c11*(k0+k5) + c12*(k2 + k4) + c13*k3)), 6};
        }
};

} /*namespace method*/
} /*namespace epode*/

#endif // EPODE_BUTCHERS5TH_H



