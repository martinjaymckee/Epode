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
    Epode/Util \
    Epode/Integrator \
    Epode/ODE \
    Epode/Triggers \
    Epode/Step \
    Epode/RK2 \
    Epode/Solve \
    Epode/Butcher \
    Epode/RKF \
    Epode/BogackiShampine \
    Epode/Euler

DISTFILES += \
    ../../Tests/C++/ODE_Integration/Epode/MPL_2_0.txt \
    ../../Tests/C++/ODE_Integration/Epode/README.md \
    parseCSV.py
