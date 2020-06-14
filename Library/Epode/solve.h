//
//
// File - Epode/solve.h:
//
//      Implementation of a simple calling form for standard initial value problem (IVP) solution.
//  The solve() function handles defaults and creates the solver and integrator objects.  This
//  function should be sufficient for the most basic usages when no special end trigger or method
//  selection is required.  Uses the ???? method for system solution.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_SOLVE_H
#define EPODE_SOLVE_H

#include "bogacki_shampine.h"
#include "core.h"
#include "integrator.h"

namespace epode
{
namespace internal
{
template<typename Solver, bool adaptive>
struct SolverConstructImpl
{
	template<typename V0, typename V1>
	static Solver construct(V0 dv, V1 tol) {
		return { dv, tol };
	}
};

template<typename Solver>
struct SolverConstructImpl<Solver, false>
{
	template<typename V0, typename V1>
	static Solver construct(V0 dv, V1) {
		return { dv };
	}
};

}

template<
	template<typename V, size_t N> class Method,
	typename System, typename DValue, typename Value,
	typename Ender, typename State, 
	typename Tolerance = typename decltype(internal::stateProperties(State()))::value_t>
	auto solve(System system, DValue dv, Value v0, Ender end, State y0, const Tolerance & tol = Tolerance{1e-6})
{
    using system_properties_t = decltype(internal::stateProperties(system(v0, y0)));
    using state_properties_t = decltype(internal::stateProperties(y0));

    static_assert(
        system_properties_t::N == state_properties_t::N,
        "Initial state, y0, passed into solve is incompatible with the specified system of equations."
    );

    using value_t = typename std::common_type<
        typename system_properties_t::value_t,
        typename state_properties_t::value_t
    >::type;

    using Solver = Integrator<value_t, system_properties_t::N, Method>;
		auto solver = internal::SolverConstructImpl<Solver, Solver::method_t::adaptive>::construct(dv, tol);

    return solver(system, v0, end, y0);
}

namespace internal
{
// TODO: EVENTUALLY THIS SHOULD BE REPLACED WITH A METAFUNCTION THAT CALCULATES THE "BEST" METHOD
//  BASED UPON WHATEVER INFORMATION IS AVAILABLE THROUGH THE "SOLVE" FUNCTION PARAMETERS.
template<typename V, size_t N>
using SolveDefaultMethod = method::BS45<V, N>;
} /*namespace internal*/

template<typename System, typename DValue, typename Value, 
	typename Ender, typename State, 
	typename Tolerance = typename decltype(internal::stateProperties(State()))::value_t>
auto solve(System system, DValue dv, Value v0, Ender end, State y0, const Tolerance& tol = Tolerance(1e-6))
{
    return solve<internal::SolveDefaultMethod>(system, dv, v0, end, y0, tol);
}

} /*namespace epode*/

#endif // EPODE_SOLVE_H
