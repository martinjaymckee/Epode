//
//
// File - Epode/core.h:
//
//      Basic type definitions required to implement the whole of the Epode library.  These are
//  mostly basic types (and type aliases) which are used for data forwarding and storage or are
//  available as base classes for generic functionality.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef CORE
#define CORE

#include <tuple>

#include <Eigen/Dense>

namespace epode
{

// Wrap a number of functions in a tuple of functions
//  TODO: THIS NEEDS TO VALIDATE THE FUNCTION ARGUMENTS
template<typename... Ts>
auto fns(Ts&&... _args) { return std::make_tuple(_args...); }

namespace internal
{
template<typename Value, size_t N>
using State = Eigen::Matrix<Value, 1, N>;

template<typename Value, size_t _N>
struct StateProperties {
        using value_t = Value;
        static constexpr size_t N = static_cast<size_t>(_N);
};


template<typename Value, int N, int _Options, int _MaxRows, int _MaxCols>
constexpr auto stateProperties(const Eigen::Matrix<Value, 1, N, _Options, _MaxRows, _MaxCols>&) {
    return StateProperties<Value, N>{};
}

template<typename Value, int N, int _Options, int _MaxRows, int _MaxCols>
constexpr auto stateProperties(Eigen::Matrix<Value, 1, N, _Options, _MaxRows, _MaxCols>&) {
	return StateProperties<Value, N>{};
}

// NOTE: THE FINAL THREE PARAMETERS IN THE STATEPROPERTIES FUNCTION
//	(_OPTIONS, _MAXROWS, _MAXCOLS) 
//	MUST BE INCLUDED OR THE CODE FAILS TO DEDUCE CORRECTLY.  FIGURE OUT IF THIS IS
//	A C++ COMPLIANCE THING OR IF IT IS SOMETHING THAT VISUAL STUDIO DOES.  IT CERTAINLY
//	WORKED FINE WITHOUT SPECIFYING THEM UNDER GCC.

template<typename Value, typename State>
struct MethodReturn
{
        using value_t = Value;
        using state_t = State;

        MethodReturn(value_t _dv, value_t _dv_next, state_t _y, size_t _evals)
            : dv(_dv), dv_next(_dv_next), y(_y), evals(_evals) {}

        value_t dv;
        value_t dv_next;
        state_t y;
        size_t evals;
};

template<typename Value, typename State, typename Stats>
struct StepPoint
{
        using value_t = Value;
        using state_t = State;
        using stats_t = Stats;

        StepPoint(value_t _dv, value_t _v, state_t _y, stats_t _stats)
            : dv(_dv), v(_v), y(_y), stats(_stats) {}

        value_t dv;
        value_t v;
        state_t y;
        stats_t stats;
};

template<typename Value, bool Adaptive, bool Implicit, unsigned int Order>
class MethodInfo
{
    public:
        constexpr static bool adaptive = Adaptive;
        constexpr static bool implicit = Implicit;
        constexpr static unsigned int order = Order;
};

template<typename Value>
constexpr Value defaultTolerance(const Value base_tolerance, size_t Order) {
    return (Order == 0) ? base_tolerance : defaultTolerance(base_tolerance, Order-1) / Value(5);
}

template<typename Value, size_t Order, bool Implicit = false>
class Fixed : public MethodInfo<Value, false, Implicit, Order> {};

template<typename Value, size_t Order>
using ImplicitFixed = Fixed<Value, Order, true>;

template<typename Value, size_t Order, bool Implicit = false>
class Adaptive : public MethodInfo<Value, true, Implicit, Order> {
    public:
        Adaptive(const Value& _tolerance = defaultTolerance(1e-6, Order)) :
            tolerance(_tolerance) {}

    protected:
        Value tolerance;
};

template<typename Value, size_t Order>
using ImplicitAdaptive = Adaptive<Value, Order, true>;


// Make sure that the application functions are in a template
template<typename... Ts> auto Functions(std::tuple<Ts...> fs) { return fs; }
template<typename F> auto Functions(F&& f) -> decltype(fns(f)) { return fns(f); }
} /* namespace internal */

} /* namespace epode */

#endif // CORE

