TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/eigen3/

SOURCES += main.cpp

HEADERS += \
    Epode/ODE \
    Epode/Euler \
    Epode/Core \
    Epode/Integrator \
    Epode/Midpoint \
    Epode/Heun \
    Epode/Ralston \
    Epode/Kutta3rd \
    Epode/Kutta4th \
    Epode/RKF45 \
    Epode/Step \
    Epode/Butcher5th \
    Epode/Util \
    Epode/Butcher5th \
    Epode/Integrator \
    Epode/ODE \
    Epode/Triggers \
    Epode/Step

DISTFILES += \
    ../../Tests/C++/ODE_Integration/Epode/MPL_2_0.txt \
    ../../Tests/C++/ODE_Integration/Epode/README.md
