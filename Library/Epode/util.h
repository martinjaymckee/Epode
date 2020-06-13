//
//
// File - Epode/util.h:
//
//      Functions to simplify the implementation of other Epode features.
//
//  TODO: DECIDE IF THIS FILE IS REALLY REQUIRED... OR IF THE FUNCTIONALITY CAN GO ELSEWHERE.
//
// License:
//
//      This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
//  If a copy of the MPL was not distributed with this file, You can obtain one
//  at http://mozilla.org/MPL/2.0/.
//
//

#ifndef EPODE_UTIL_H
#define EPODE_UTIL_H

#include <fstream>

#include "core.h"

namespace epode
{
namespace util
{

template<typename Value>
inline Value lastInitElement(std::initializer_list<Value> _init) {
    auto it = _init.begin();
    Value last = *it;
    for(; it != _init.end(); ++it) {
        last = *it;
    }
    return last;
}

template<typename Results>
bool resultsToCSV(
        const std::string& filename, Results results,
        const std::string& header = std::string()
        ) {
    auto file = std::ofstream{};
    file.open(filename, std::ofstream::out);

    if(file) {
        file << header << "\n";

        for(auto result: results) {
            using properties_t = decltype(internal::stateProperties(result.y));
            constexpr size_t N = properties_t::N;

            file << result.dv << ", ";
            file << result.v << ", ";

            for(size_t idx = 0; idx < N; ++idx) {
                file << result.y[idx];
                if(idx < (N-1)) file << ", ";
            }

            file << "\n";
        }

        file.close();
        return true;
    }

    // TODO: SOME SORT OF ERROR HANDLING
    return false;
}

} /*namespace util*/
} /*namespace epode*/

#endif // EPODE_UTIL_H

