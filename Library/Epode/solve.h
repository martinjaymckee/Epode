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

template<template<typename V, size_t N> class Method, typename System, typename State>
auto solve(System system, auto dv, auto v0, auto end, State y0)
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

    // TODO: FIGURE OUT HOW TO DEFAULT THESE ARGUMENTS BETTER
    // TODO: THE TOLERANCE VALUE SHOULD ONLY BE PASSED TO ADAPTIVE METHODS
    auto solver = Solver(dv, 1e-10);

    return solver(system, v0, end, y0);
}

namespace internal
{
// TODO: EVENTUALLY THIS SHOULD BE REPLACED WITH A METAFUNCTION THAT CALCULATES THE "BEST" METHOD
//  BASED UPON WHATEVER INFORMATION IS AVAILABLE THROUGH THE "SOLVE" FUNCTION PARAMETERS.
template<typename V, size_t N>
using SolveDefaultMethod = method::BS45<V, N>;
} /*namespace internal*/

template<typename System, typename State>
auto solve(System system, auto dv, auto v0, auto end, State y0)
{
    return solve<internal::SolveDefaultMethod>(system, dv, v0, end, y0);
}

} /*namespace epode*/

#endif // EPODE_SOLVE_H

