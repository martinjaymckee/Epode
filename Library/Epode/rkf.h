//
//
// File - Epode/rkf.h:
//
//      Implementation of the adaptive Runge-Kutta methods introduced by Fehlberg in his 1968 paper.
//  The four methods currently included are: RKF 1(2), RKF 2(3), RKF 3(4) and RKF 4(5).  For the
//  RKF 4(5) implementation, the most typically used coefficients (formula 2) have been chosen.
//  Likewise, the formula 1 coefficients have been chosen for the RKF 3(4) implementation as the
//  coefficients are substantially less complicated and the truncation errors are very similar.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_RKF_H
#define EPODE_RKF_H
#include "core.h"
#include "step.h"

namespace epode
{
namespace method
{

template<typename Value, size_t N>
class RKF12 : public internal::Adaptive<Value, 1>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 1>::Adaptive; // Inherit Construtors

        void init(value_t /*dv*/, value_t v0, value_t y0, auto func) {
            k0 = func(v0, y0);
        }

        return_t operator () (auto func, value_t dv, value_t v, state_t y0, auto limiter) {
            constexpr auto c0 = value_t(1) / value_t(2);
            constexpr auto c1 = value_t(1) / value_t(256);
            constexpr auto c2 = value_t(255) / value_t(256);
            constexpr auto c3 = value_t(1) / value_t(512);

            size_t evals = 0;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;
            state_t k2{};

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(c0*dv*k0));
                y1 = y0 + dv*(c1*k0 + c2*k1);
                k2 = func(v+dv, y1);
                evals += 2;

                const auto z1 = y0 + dv*(c3*(k0+k2) + c2*k1);
                const auto update = step::internal::updateStepSize<1>(
                            dv, limiter.min, y1, z1, this->tolerance
                    );
                done = update.done;
                dv_next = update.dv;
            } while(!done);

            k0 = k2;
            return return_t{dv, dv_next, y1, evals};
        }

    protected:
        state_t k0;
};

template<typename Value, size_t N>
class RKF23 : public internal::Adaptive<Value, 2>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 2>::Adaptive; // Inherit Construtors

        void init(value_t /*dv*/, value_t v0, value_t y0, auto func) {
            k0 = func(v0, y0);
        }

        return_t operator () (auto func, value_t dv, value_t v, state_t y0, auto limiter) {
            constexpr auto c0 = value_t(1) / value_t(4);
            constexpr auto c1 = value_t(27) / value_t(40);
            constexpr auto c2 = value_t(-189) / value_t(800);
            constexpr auto c3 = value_t(729) / value_t(800);
            constexpr auto c4 = value_t(214) / value_t(891);
            constexpr auto c5 = value_t(1) / value_t(33);
            constexpr auto c6 = value_t(650) / value_t(891);
            constexpr auto c7 = value_t(533) / value_t(2106);
            constexpr auto c8 = value_t(800) / value_t(1053);
            constexpr auto c9 = value_t(-1) / value_t(78);

            size_t evals = 0;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;
            state_t k3{};

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(c0*dv*k0));
                const auto k2 = func(v+(c1*dv), y0+(dv*(c2*k0 + c3*k1)));
                y1 = y0 + dv*(c4*k0 + c5*k1 + c6*k2);
                k3 = func(v+dv, y1);
                evals += 3;

                const auto z1 = y0 + dv*(c7*k0 + c8*k2 + c9*k3);
                const auto update = step::internal::updateStepSize<2>(
                            dv, limiter.min, y1, z1, this->tolerance
                    );
                done = update.done;
                dv_next = update.dv;
            } while(!done);
            k0 = k3;
            return return_t{dv, dv_next, y1, evals};
        }

    protected:
        state_t k0;
};

template<typename Value, size_t N>
class RKF34 : public internal::Adaptive<Value, 3>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 3>::Adaptive; // Inherit Construtors

        void init(value_t /*dv*/, value_t v0, value_t y0, auto func) {
            k0 = func(v0, y0);
        }

        return_t operator () (auto func, value_t dv, value_t v, state_t y0, auto limiter) {
            constexpr auto c0 = value_t(1) / value_t(4);
            constexpr auto c1 = value_t(4) / value_t(9);
            constexpr auto c2 = value_t(4) / value_t(81);
            constexpr auto c3 = value_t(32) / value_t(81);
            constexpr auto c4 = value_t(6) / value_t(7);
            constexpr auto c5 = value_t(57) / value_t(98);
            constexpr auto c6 = value_t(-432) / value_t(343);
            constexpr auto c7 = value_t(1053) / value_t(686);
            constexpr auto c8 = value_t(1) / value_t(6);
            constexpr auto c9 = value_t(27) / value_t(52);
            constexpr auto c10 = value_t(49) / value_t(156);
            constexpr auto c11 = value_t(43) / value_t(288);
            constexpr auto c12 = value_t(243) / value_t(416);
            constexpr auto c13 = value_t(343) / value_t(1872);
            constexpr auto c14 = value_t(1) / value_t(12);

            size_t evals = 0;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;
            state_t k4{};

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(c0*dv*k0));
                const auto k2 = func(v+(c1*dv), y0+(dv*(c2*k0 + c3*k1)));
                const auto k3 = func(v+(c4*dv), y0+(dv*(c5*k0 + c6*k1 + c7*k2)));
                y1 = y0 + dv*(c8*k0 + c9*k2 + c10*k3);
                k4 = func(v+dv, y1);

                evals += 4;
                const auto z1 = y0 + dv*(c11*k0 + c12*k2 + c13*k3 + c14*k4);
                const auto update = step::internal::updateStepSize<3>(
                            dv, limiter.min, y1, z1, this->tolerance
                    );
                done = update.done;
                dv_next = update.dv;
            } while(!done);
            k0 = k4;
            return return_t{dv, dv_next, y1, evals};
        }

    protected:
        state_t k0;
};

template<typename Value, size_t N>
class RKF45 : public internal::Adaptive<Value, 4>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 4>::Adaptive; // Inherit Construtors

        return_t operator () (auto func, value_t dv, value_t v, state_t y0, auto limiter) {
            constexpr auto c0 = value_t(1) / value_t(4);
            constexpr auto c1 = value_t(3) / value_t(8);
            constexpr auto c2 = value_t(3) / value_t(32);
            constexpr auto c3 = value_t(9) / value_t(32);
            constexpr auto c4 = value_t(12) / value_t(13);
            constexpr auto c5 = value_t(1932) / value_t(2197);
            constexpr auto c6 = value_t(-7200) / value_t(2197);
            constexpr auto c7 = value_t(7296) / value_t(2197);
            constexpr auto c8 = value_t(439) / value_t(216);
            constexpr auto c9 = value_t(-8);
            constexpr auto c10 = value_t(3680) / value_t(513);
            constexpr auto c11 = value_t(-845) / value_t(4104);
            constexpr auto c12 = value_t(1) / value_t(2);
            constexpr auto c13 = value_t(-8) / value_t(27);
            constexpr auto c14 = value_t(2);
            constexpr auto c15 = value_t(-3544) / value_t(2565);
            constexpr auto c16 = value_t(1859) / value_t(4104);
            constexpr auto c17 = value_t(-11) / value_t(40);
            constexpr auto c18 = value_t(25) / value_t(216);
            constexpr auto c19 = value_t(1408) / value_t(2565);
            constexpr auto c20 = value_t(2197) / value_t(4104);
            constexpr auto c21 = value_t(-1) / value_t(5);
            constexpr auto c22 = value_t(16) / value_t(135);
            constexpr auto c23 = value_t(6656) / value_t(12825);
            constexpr auto c24 = value_t(28561) / value_t(56430);
            constexpr auto c25 = value_t(-9) / value_t(50);
            constexpr auto c26 = value_t(2) / value_t(55);

            const auto k0 = func(v, y0);
            size_t evals = 1;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(c0*dv*k0));
                const auto k2 = func(v+(c1*dv), y0+(dv*(c2*k0 + c3*k1)));
                const auto k3 = func(v+(c4*dv), y0+(dv*(c5*k0 + c6*k1 + c7*k2)));
                const auto k4 = func(v+dv, y0+(dv*(c8*k0 + c9*k1 + c10*k2 + c11*k3)));
                const auto k5 = func(v+(c12*dv), y0+(dv*(c13*k0 + c14*k1 + c15*k2 + c16*k3 + c17*k4)));

                evals += 5;
                y1 = y0 + dv*(c18*k0 + c19*k2 + c20*k3 + c21*k4);
                const auto z1 = y0 + dv*(c22*k0 + c23*k2 + c24*k3 + c25*k4 + c26*k5);
                const auto update = step::internal::updateStepSize<4>(
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

#endif // EPODE_RKF_H

