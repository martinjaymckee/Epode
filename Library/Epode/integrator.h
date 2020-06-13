//
//
// File - Epode/integrator.h:
//
//      Implementation of the Integrator wrapper class to handle creation of event triggers,
//  function objects, etc. and to run the integration.
//
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef INTEGRATOR
#define INTEGRATOR

#include <iostream>
#include <initializer_list>
#include <vector>

#include <cmath>

#include "core.h"
#include "step.h"
#include "triggers.h"

namespace epode
{
namespace internal
{
struct IntegratorStatistics
{
        IntegratorStatistics() : steps(0), evals(0) {}
        IntegratorStatistics(const IntegratorStatistics& _other)
            : steps(_other.steps), evals(_other.evals) {}

        IntegratorStatistics& update(size_t _steps, size_t _evals ) {
            steps += _steps;
            evals += _evals;
            return *this;
        }

        size_t steps;
        size_t evals;
};

struct NullOutputTransformer
{
        template<typename Value, typename State, typename Stats>
        constexpr State operator() (
                const Value&,       // dv
                const Value&,       // v
                const State& state, // y
                const Stats&        // statistics
        ) const {
            return state;
        }
};
} /*namespace internal*/

template<typename Value, size_t N, template<typename V, size_t N2> class Method>
class Integrator
{
    public:
        using value_t = Value;
        using method_t = Method<value_t, N>;
        using state_t = internal::State<value_t, N>;

    protected:
        using limits_t = step::StepLimits<value_t>;
        using stats_t = internal::IntegratorStatistics;

    public:
		template<typename Funcs, typename Results>
		struct IntegratorLoopState
		{
			value_t dv;
			value_t v;
			Funcs funcs;
			state_t y;
			stats_t stats = stats_t{};
			Results results;
			limits_t limits;
			method_t method;
			bool complete = false;
		};

		template<typename... Args>
        Integrator(value_t _dv0, Args... args) : dv0(_dv0), method(args...) {}

        template<typename Funcs, typename YState, typename Transformer=internal::NullOutputTransformer>
        auto operator() (Funcs funcs,
                             value_t v0,
                             std::initializer_list<value_t> _end,
                             YState y0,
                             const Transformer& transformer = Transformer{}) {
            auto end = triggers::internal::constructEndTrigger<value_t>(_end);
            auto store = [](auto, auto, auto, auto) {return true;};
            auto limiter = step::internal::constructLimiter<limits_t, value_t>(_end);

            return this->operator ()(
                internal::Functions(funcs), // Ensure that a single function is wrapped
                v0, // Initial Integration Variable Value
                y0, // Initial State Value
                end, // Integration End Trigger
                store, // Integration Step Store Trigger
                limiter,
                transformer // Storage Transformer Type
            );
        }

        template<typename Funcs, typename Ender, typename YState, typename Transformer=internal::NullOutputTransformer>
        auto operator() (Funcs funcs, value_t v0, Ender _end, YState y0,
                             const Transformer& transformer = Transformer{}) {
            auto end = triggers::internal::constructEndTrigger<value_t>(_end);
            auto store = [](auto, auto, auto, auto) {return true;};
            auto limiter = step::internal::constructLimiter<limits_t, value_t>(_end);

            return this->operator ()(
                internal::Functions(funcs), // Ensure that a single function is wrapped
                v0, // Initial Integration Variable Value
                y0, // Initial State Value
                end, // Integration End Trigger
                store, // Integration Step Store Trigger
                limiter,
                transformer // Storage Transformer Type
            );
        }

		template<typename Funcs, typename Transformer>
		auto initializeLoopState(
			Funcs funcs,
			value_t v0, state_t y0,
			Transformer transformer) {
			using transformed_state_t = decltype(transformer(value_t{}, value_t{}, state_t{}, stats_t{}));
			using point_t = internal::StepPoint<value_t, transformed_state_t, stats_t>;
			using results_t = std::vector<point_t>;

			IntegratorLoopState<Funcs, results_t, limits_t> state;
			state.dv = dv0;
			state.v = v0;
			state.y = y0;
			state.funcs = funcs;
			state.method = method; // COPY FROM THE CONSTRUCTED METHOD
			

			auto f0 = std::get<0>(funcs);
			initMethod(state.dv, state.v, state.y, f0, state.method);

			return state;
		}


    protected:
        //
        // These overloads determine if the integration method object has an init member function
        //  and, if so, it calls it.
        //
        template<typename V, typename S, typename... Ts>
        void initMethod(V, V, S, Ts...) {}

        template<typename V, typename S, typename Func, typename Method>
        auto initMethod(V dv, V v, S y, Func _func, Method _method)
        -> decltype(_method.init(dv, v, y, _func)) {
            return _method.init(dv, v, y, _func);
        }

		//
		// The LoopStateInitialization
		//

        //
        // The generic call operator which contains the implementaion.
        //
        template<typename Funcs, typename Ender, typename Storer, typename Limiter, typename Transformer>
        auto operator() (
                Funcs funcs,
                value_t v0, state_t y0,
                Ender end, Storer store, Limiter limiter,
                Transformer transformer) {
            using transformed_state_t = decltype(transformer(value_t{}, value_t{}, state_t{}, stats_t{}));
            using point_t = internal::StepPoint<value_t, transformed_state_t, stats_t>;
            using return_t = std::vector<point_t>;

            return_t results;

            auto stats = stats_t(); // Create inital statistics object

            auto dv = dv0;
            auto f0 = std::get<0>(funcs);
            auto y = y0;
            auto v = v0;

            initMethod(dv, v, y, f0, method);

            auto limits = limiter(dv, v);

            // DEBUG HACK
            while(!end(dv, v, y, stats, limits)){
                // Always constrain the integration variable step
                dv = limits.constrain(dv);

                // Do a step of the selected integration method and update the integration state
                const auto result = method(f0, dv, v, y, limits);
                v += result.dv;
                dv = result.dv_next;
                y = result.y;
                stats.update(1, result.evals);

             /*   if(std::isnan(dv)) {
                    std::cout << "limits = [" << limits.min << ", " << limits.max << "], dv = " << result.dv << std::endl;
                    std::cout << "steps = " << stats.steps << ", evals = " << stats.evals << std::endl;
                    exit(-1);
                }*/

                // Store the correctly transformed results of the step, if desired
                //  TODO: IT MAKES SENSE TO CONSIDER INTERPOLATION HERE....
                //  INTERPOLATION WILL, LIKELY, REQUIRE A REVAMPING OF THE METHOD API TO RETURN
                //  MORE DATA, BUT, IT WOULD MAKE IT POSSIBLE TO GET A DENSE OUTPUT WITH LESS
                //  COMPUTATIONAL EFFORT -- NOT A BAD THING.  AS A BASE LINE, IT WOULD BE POSSIBLE
                //  TO DO A THIRD ORDER INTERPOLATION USING JUST THE BEGIN AND END POINTS AND THEIR
                //  SLOPES.  WITH THE MID-POINT ALSO, IT WOULD BE POSSIBLE TO DO A QUARTIC
                //  (4TH ORDER) INTERPOLATION.  IT SEEMS THAT THESE TWO OPTIONS ARE REASONABLE TO
                //  CONSIDER AND IMPLEMENT (AS A START). THERE NEEDS TO BE API IN PLACE TO CONTROL
                //  THE DENSITY OF THE OUTPUT THOUGH....
                if(store(result.dv, v, y, stats)){
                    const auto y_transformed = transformer(result.dv, v, y, stats);
                    results.emplace_back(result.dv, v, y_transformed, stats);
                }

                // Update the integration variable limits
                limits = limiter(dv, v);
            }

            return results;
        }

        //
        // Integrator data members
        //
        value_t dv0; // Initial step size... is this the best way to do this for all methods?
        method_t method; // The method object which will be used in calculations
};

} /*namespace epode*/

#endif // INTEGRATOR

