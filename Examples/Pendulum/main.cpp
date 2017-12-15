#include <iostream>
#include <sstream>

#include <Eigen/Dense>
#include <Epode/ode.h>
#include <Epode/util.h>

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

void statistics(int run, auto results) {
    auto stats = results.back().stats;
    cout << "Run #" << run << ":\n";
    cout << "\tIteration Steps = " << stats.steps << "\n";
    cout << "\tFunction Evals = " << stats.evals << "\n";
}

std::string filename(int idx) {
    ostringstream s;
    s << "pendulum_" << idx << ".csv";
    return s.str();
}

std::string header(int idx, auto lambda) {
    ostringstream s;
    s << "#name:Pendulum Example Run #" << idx << "\n";
    s << "#label:$\\lambda=" << lambda << "$";
    return s.str();
}

int main()
{
    auto dt = 0.01;
    auto t_end = 2;
    auto y0 = State{0, .35}; // rad/s, rad
    auto lambda_min = 0;
    auto lambda_max = 5;
    auto N = 5;

    cout << "Run the Pendulum Model\n";
    auto lambda_range = lambda_max - lambda_min;
    auto lambda_step = lambda_range / (N-1);

    for(int idx=0; idx < N; ++idx) {
        auto lambda = lambda_min + (idx * lambda_step);

        auto results = epode::solve(
            pendulumSystem(1.0, 0.1, lambda),   // system parameters: m, L, lambda
            dt,                                 // Initial step size
            0,                                  // Start time
            t_end,                              // End time
            y0                                  // Initial system state
        );

        epode::util::resultsToCSV(filename(idx), results, header(idx, lambda));
        statistics(idx, results);
    }

    return 0;
}
