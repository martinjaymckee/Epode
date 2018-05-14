#include <iostream>
#include <sstream>

#include <Eigen/Dense>
#include <Epode/ode.h>
#include <Epode/util.h>

using namespace std;

using State = Eigen::Matrix<double, 1, 1>;

auto capacitorSystem = [] (auto C, auto R) {
    return [=](auto, auto y) {
        return State{-y[0]/(R*C)};
    };
};

auto endTrigger(double V_end) {
    return [=](auto /*dv*/, auto /*v*/, auto y, auto /*stats*/, auto /*limits*/) -> bool {
        return y[0] <= V_end;
    };
}

template<typename T>
struct TD;

int main()
{
    auto dt = 0.01;
    auto R = 1e6;
    auto C = 1e-6;
    auto V0 = 5.0;
    auto y0 = State{V0}; // volts
    auto p = 0.01;
    auto end = endTrigger(V0*p);
    auto results = epode::solve(
        capacitorSystem(C, R),   // system parameters: m, L, lambda
        dt,                                 // Initial step size
        0,                                  // Start time
        end,                                // End Trigger
        y0                                  // Initial system state
    );

    epode::util::resultsToCSV("capacitor_discharge.csv", results);
    std::cout << "Discharge Time = " << results.back().v << " seconds.\n";

    return 0;
}
