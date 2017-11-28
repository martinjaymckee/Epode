#include <iostream>

#include <Eigen/Dense>
#include <Epode/ODE>
#include <Epode/Util>

using namespace std;

using State = Eigen::Matrix<double, 1, 2>;

auto pendulumSystem = [] (auto m, auto L, auto lambda) {
    return [=](auto, auto y) {
        return State{
            y[1],
            -(((lambda*y[1]) + ((9.81 * m) / L) * sin(y[0])) / m)
        };
    };
};

int main()
{
    auto dt = 0.001; /*s*/
    auto y0 = State{0/*rad/s*/, .35 /*rad*/};

    cout << "Run the Pendulum Model\n";
    // TODO: THE CURRENT IMPLEMENTATION OF SOLVE IS UNABLE TO HANDLE INPUTS WHICH VARY IN TYPE... IT IS
    //  GOING TO BE NECESSARY TO FIND SOME WAY TO GET THE "CORRECT" TYPES TO PASS INTO THE SYSTEM
    //  CALL OPERATOR WITHOUT EXPLICITLY NAMING TYPES (OR IMPLICITLY, AS THE CASE MAY BE BY ADDING
    //  THE DECIMAL-POINT, ZERO.
    auto results = epode::solve(
        pendulumSystem(1.0/*kg*/, 0.1 /*m*/, 1.6),
        dt, 0.0, 2.0, y0
    );
    epode::util::resultsToCSV("pendulum_1.csv", results);

    results = epode::solve(
        pendulumSystem(1.0/*kg*/, 0.1 /*m*/, 4.8),
        dt, 0.0, 2.0, y0
    );
    epode::util::resultsToCSV("pendulum_2.csv", results);
    return 0;
}

