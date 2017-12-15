//
//
// File - Epode/euler.h:
//
//      Implementation of the Forward Euler method as well as the adaptive Heun-Euler method.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_EULER_H
#define EPODE_EULER_H
namespace epode
{
namespace method
{

template<typename Value, size_t N>
class Euler : public internal::Fixed<Value, 1>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        return_t operator () (auto func, value_t dv, value_t v, state_t y, auto) {
            return return_t{dv, dv, y + dv * func(v, y), 1};
        }
};

template<typename Value, size_t N>
class HeunEuler : public internal::Adaptive<Value, 1>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        return_t operator () (auto func, value_t dv, value_t v, state_t y0, auto limiter) {
            constexpr auto c0 = value_t(1) / value_t(2);

            const auto k0 = func(v, y0);
            size_t evals = 1;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+dv, y0+ dv*k0);

                evals += 1;
                y1 = y0 + c0*dv*(k0 + k1);
                const auto z1 = y0 + dv*k0;
                const auto update = step::internal::updateStepSize<1>(
                            dv, limiter.min, y1, z1, this->tolerance
                    );
                done = update.done;
                dv_next = update.dv;
            } while(!done);

            return return_t{dv, dv_next, y1, evals};
        }
};

} /*namespace method*/
} /*namespace epode*/

#endif /*EPODE_EULER_H*/
