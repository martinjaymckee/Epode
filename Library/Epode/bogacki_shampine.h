//
//
// File - Epode/bogacki_shampine.h:
//
//      Implementation of the Bogacki-Shampine 3(2) and 4(5) adaptive methods.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_BOGACKISHAMPINE_H
#define EPODE_BOGACKISHAMPINE_H

#include "core.h"
#include "step.h"

namespace epode
{
namespace method
{

// TODO: INTRODUCED IN "A 3(2) PAIR OF RUNGE-KUTTA FORMULAS" -- ADD THIS TO THE DOCUMENTATION
template<typename Value, size_t N>
class BS32 : public internal::Adaptive<Value, 3>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 3>::Adaptive; // Inherit Construtors

		template<typename Func>
        void init(value_t /*dv*/, value_t v0, state_t y0, Func func) {
            k0 = func(v0, y0);
        }

		template<typename Func, typename Limiter>
        return_t operator () (Func func, value_t dv, value_t v, state_t y0, Limiter limiter) {
            constexpr auto c0 = value_t(1) / value_t(2);
            constexpr auto c1 = value_t(3) / value_t(4);
            constexpr auto c2 = value_t(2) / value_t(9);
            constexpr auto c3 = value_t(1) / value_t(3);
            constexpr auto c4 = value_t(4) / value_t(9);
            constexpr auto c5 = value_t(7) / value_t(24);
            constexpr auto c6 = value_t(1) / value_t(4);
            constexpr auto c7 = value_t(1) / value_t(8);

            size_t evals = 0;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;
            state_t k3{};

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(dv*c0*k0));
                const auto k2 = func(v+(c1*dv), y0+(dv*c1*k1));
                y1 = y0 + dv*(c2*k0 + c3*k1 + c4*k2);
                k3 = func(v+dv, y1);
                evals += 3;

                const auto z1 = y0 + dv*(c5*k0 + c6*k1 + c3*k2 + c7*k3);
                const auto update = step::internal::updateStepSize<3>(
                            dv, limiter.min, y1, z1, this->tolerance
                    );
                done = update.done;
                dv_next = update.dv;
            } while(!done);
            k0 = k3; // By the FSAL (First Same As Last) property
            return return_t{dv, dv_next, y1, evals};
        }

    protected:
        state_t k0;
};

// TODO: INCLUDE "AN EFFICIENT RUNGE-KUTTA (4,5) PAIR" IN THE DOCUMENTATION
// TODO: INCLUDE "RKSUITE" IN THE DOCUMENTATION (AS THE SOURCE OF THE COEFFICIENTS)
// TODO: ADD PETER STONE OUTLINE
template<typename Value, size_t N>
class BS45 : public internal::Adaptive<Value, 4>
{
    public:
        using value_t = Value;
        using state_t = internal::State<value_t, N>;
        using return_t = internal::MethodReturn<value_t, state_t>;

        using internal::Adaptive<Value, 4>::Adaptive; // Inherit Construtors

		template<typename Func>
        void init(value_t /*dv*/, value_t v0, state_t y0, Func func) {
            k0 = func(v0, y0);
        }

		template<typename Func, typename Limiter>
        return_t operator () (Func func, value_t dv, value_t v, state_t y0, Limiter limiter) {
            constexpr auto c0 = value_t(1) / value_t(6);
            constexpr auto c1 = value_t(2) / value_t(9);
            constexpr auto c2 = value_t(2) / value_t(27);
            constexpr auto c3 = value_t(4) / value_t(27);
            constexpr auto c4 = value_t(3) / value_t(7);
            constexpr auto c5 = value_t(183) / value_t(1372);
            constexpr auto c6 = value_t(-162) / value_t(343);
            constexpr auto c7 = value_t(1053) / value_t(1372);
            constexpr auto c8 = value_t(2) / value_t(3);
            constexpr auto c9 = value_t(68) / value_t(297);
            constexpr auto c10 = value_t(-4) / value_t(11);
            constexpr auto c11 = value_t(42) / value_t(143);
            constexpr auto c12 = value_t(1960) / value_t(3861);
            constexpr auto c13 = value_t(3) / value_t(4);
            constexpr auto c14 = value_t(597) / value_t(22528);
            constexpr auto c15 = value_t(81) / value_t(352);
            constexpr auto c16 = value_t(63099) / value_t(585728);
            constexpr auto c17 = value_t(58653) / value_t(366080);
            constexpr auto c18 = value_t(4617) / value_t(20480);
            constexpr auto c19 = value_t(174197) / value_t(959244);
            constexpr auto c20 = value_t(-30942) / value_t(79937);
            constexpr auto c21 = value_t(8152137) / value_t(19744439);
            constexpr auto c22 = value_t(666106) / value_t(1039181);
            constexpr auto c23 = value_t(-29421) / value_t(29068);
            constexpr auto c24 = value_t(482048) / value_t(414219);
            constexpr auto c25 = value_t(587) / value_t(8064);
            constexpr auto c26 = value_t(4440339) / value_t(15491840);
            constexpr auto c27 = value_t(24353) / value_t(124800);
            constexpr auto c28 = value_t(387) / value_t(44800);
            constexpr auto c29 = value_t(2152) / value_t(5985);
            constexpr auto c30 = value_t(7267) / value_t(94080);
            constexpr auto c31 = value_t(6059) / value_t(80640);
            constexpr auto c32 = value_t(8559189) / value_t(30983680);
            constexpr auto c33 = value_t(26411) / value_t(124800);
            constexpr auto c34 = value_t(-927) / value_t(89600);
            constexpr auto c35 = value_t(443) / value_t(1197);
            constexpr auto c36 = value_t(2479) / value_t(34992);
            constexpr auto c37 = value_t(123) / value_t(416);
            constexpr auto c38 = value_t(612941) / value_t(3411720);
            constexpr auto c39 = value_t(43) / value_t(1440);
            constexpr auto c40 = value_t(2272) / value_t(6561);
            constexpr auto c41 = value_t(79937) / value_t(1113912);
            constexpr auto c42 = value_t(3293) / value_t(556956);

            size_t evals = 0;

            auto y1 = y0;
            auto dv_next = dv;
            bool done = false;
            state_t k7{};

            do {
                dv = limiter.constrain(dv_next);
                const auto k1 = func(v+(c0*dv), y0+(c0*dv*k0));
                const auto k2 = func(v+(c1*dv), y0+dv*(c2*k0 + c3*k1));
                const auto k3 = func(v+(c4*dv), y0+dv*(c5*k0 + c6*k1 + c7*k2));
                const auto k4 = func(v+(c8*dv), y0+dv*(c9*k0 + c10*k1 + c11*k2 + c12*k3));
                const auto k5 = func(v+(c13*dv), y0+dv*(c14*k0 + c15*k1 + c16*k2 + c17*k3 + c18*k4));
                const auto k6 = func(v+dv, y0+dv*(c19*k0 + c20*k1 + c21*k2 + c22*k3 + c23*k4 + c24*k5));
                evals += 6;

                y1 = y0 + dv*(c25*k0 + c26*k2 + c27*k3 + c28*k4 + c29*k5 + c30*k6);
                const auto y1_alt = y0 + dv*(c31*k0 + c32*k2 + c33*k3 + c34*k4 + c35*k5 + c30*k6);

                // Check first error estimate
                auto update = step::internal::updateStepSize<4>(
                            dv, limiter.min, y1, y1_alt, this->tolerance
                    );

                if(update.done) {
                    k7 = func(v+dv, y1);
                    evals += 1;

                    const auto z1 = y0 + dv*(c36*k0 + c37*k2 + c38*k3 + c39*k4 + c40*k5 + c41*k6 + c42*k7);
                    update = step::internal::updateStepSize<4>(
                                dv, limiter.min, y1_alt, z1, this->tolerance
                        );
                }

                done = update.done;
                dv_next = update.dv;
            } while(!done);
            k0 = k7; // By the FSAL (First Same As Last) property
            return return_t{dv, dv_next, y1, evals};
        }

    protected:
        state_t k0;
};

} /*namespace method*/
} /*namespace epode*/

#endif /* EPODE_BOGACKISHAMPINE_H */
