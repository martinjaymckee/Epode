//
//
// File - Epode/ode.h:
//
//      Simply include all the files to do ODE solving with Epode.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_ODE_H
#define EPODE_ODE_H

//
// Core Files
//
#include "core.h"
#include "util.h"

//
// Solver Methods
//
#include "butcher.h"
#include "bogacki_shampine.h"
#include "euler.h"
#include "rkf.h"
#include "rk2.h"

//
// Compositional Triggers
//
#include "triggers.h"


//
// Integrator Wrapper and associated classes
//
#include "step.h"
#include "integrator.h"

//
// Declare Wrapped Integration Methods
//
namespace epode
{
namespace integrator
{

// First-Order Integrators
template<typename Value, size_t N>
using Euler = Integrator<Value, N, method::Euler>;

template<typename Value, size_t N>
using HeunEuler = Integrator<Value, N, method::HeunEuler>;

template<typename Value, size_t N>
using RKF12 = Integrator<Value, N, method::RKF12>;

// Second-Order Integrators
template<typename Value, size_t N>
using GenericRK2 = Integrator<Value, N, method::GenericRK2>;

template<typename Value, size_t N>
using Heuns = Integrator<Value, N, method::Heuns>;

template<typename Value, size_t N>
using Midpoint = Integrator<Value, N, method::Midpoint>;

template<typename Value, size_t N>
using Ralstons = Integrator<Value, N, method::Ralstons>;

template<typename Value, size_t N>
using RKF23 = Integrator<Value, N, method::RKF23>;

// Third-Order Integrators
template<typename Value, size_t N>
using RKF34 = Integrator<Value, N, method::RKF34>;

template<typename Value, size_t N>
using BS32 = Integrator<Value, N, method::BS32>;

// Fourth-Order Integrators
template<typename Value, size_t N>
using RKF45 = Integrator<Value, N, method::RKF45>;

template<typename Value, size_t N>
using BS45 = Integrator<Value, N, method::BS45>;

// Fifth-Order Integrators
template<typename Value, size_t N>
using Butcher5th = Integrator<Value, N, method::Butcher5th>;

} /*namespace integrator*/
} /*namespace epode*/

//
// Import the simplified "solve" syntactic sugar
//
#include "solve.h"

#endif /*EPODE_ODE_H*/

