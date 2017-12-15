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
#include "butcher.h" // TODO: THE DEFAULT SOLVER SHOULD BECOME RKF45 EVENTUALLY
#include "rkf.h"
#include "core.h"
#include "integrator.h"

namespace epode
{

template<typename System, typename State>
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

    //using Solver = Integrator<value_t, system_properties_t::N, method::Butcher5th>;
    using Solver = Integrator<value_t, system_properties_t::N, method::RKF12>;
    //using Solver = Integrator<value_t, system_properties_t::N, method::RKF23>;
    //using Solver = Integrator<value_t, system_properties_t::N, method::RKF34>;
    //using Solver = Integrator<value_t, system_properties_t::N, method::RKF45>;
    //using Solver = Integrator<value_t, system_properties_t::N, method::BS32>;
    //using Solver = Integrator<value_t, system_properties_t::N, method::BS45>;

    auto solver = Solver(dv, 1e-10);

    return solver(system, v0, end, y0);
}

} /*namespace epode*/

#endif // EPODE_SOLVE_H

