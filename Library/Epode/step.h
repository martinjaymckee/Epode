//
//
// File - Epode/step.h:
//
//      Data types and functions to handle single steps of the integration variable.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef STEP
#define STEP
#include <cmath>

namespace epode
{
namespace step
{

template<typename Value>
struct StepLimits
{
        using value_t = Value;

        StepLimits(const value_t& _max = value_t(1e6), const value_t& _min = value_t(1e-6))
            : max(_max), min(_min) {}

        constexpr value_t constrain(const value_t& dv) const {
            if(dv > max) return max;
            if(dv < min) return min;
            return dv;
        }

        value_t max;
        value_t min;
};

namespace internal
{

//
// Calculate the stepsize scaling for an adaptive method of order N
//
//  Implementtion in the case of N > 1
template<size_t N>
struct stepScalingImpl
{
        template<typename Value>
        constexpr static Value calc(const Value& tolerance, const Value& error) {
            return pow(abs(error / tolerance), (Value(1) / Value(N-1)));
        }
};

//  Implementation in the case of first-order (N = 1)
template<>
struct stepScalingImpl<1>
{
        template<typename Value>
        constexpr static Value calc(const Value& tolerance, const Value& error) {
            return abs(error / tolerance);
        }
};

//  Wrapper Method
template<size_t N, typename Value>
Value stepScaling(const Value& tolerence, const Value& error) {
    return stepScalingImpl<N>::calc(tolerence, error);
}

//  Method to update the step size
//      TODO: FIGURE OUT IF THERE'S A GOOD WAY TO MAKE THESE VALUES CONST GIVEN THE STRUCTURE OF THE BS45 IMPLEMENTATION
template<typename Value>
struct StepSizeUpdate {
        /*const*/ bool done;
        /*const*/ Value dv;
};

template<size_t N, typename Value>
StepSizeUpdate<Value> updateStepSize(
            const Value& dv, const Value& dv_min,
            auto y, auto z, const Value& tolerance,
            const Value& scale_min = Value(0.33),
            const Value& scale_max = Value(3.0)
        ) {
    constexpr auto scale_threshold = Value(1.0);
    const auto error = (z-y).norm();
    const auto scale = stepScalingImpl<N>::calc(error, tolerance);
    return [=]() -> StepSizeUpdate<Value> {
        if(scale < scale_threshold) {
            if(dv <= dv_min) {
                return {true, dv_min};
            } else {
                return {false, dv * ((scale < scale_min) ? scale_min : scale)};
            }
        }
        return {true, dv * ((scale > scale_max) ? scale_max : scale)};
    }();
}

//
// Construct a step limiter object based on the input "end" specification
//

//  Single end-point limiter
template<typename LimitsType, typename Value>
auto constructLimiter(auto v1) {
    return [=](auto, auto v) {return LimitsType(v1-v); };
}

} /*namespace internal*/
} /*namespace step*/
} /*namespace epode*/

#endif // STEP
