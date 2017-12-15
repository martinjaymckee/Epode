TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/eigen3/

SOURCES += main.cpp

HEADERS += \
    Epode/Midpoint \
    Epode/Heun \
    Epode/Ralston \
    Epode/Kutta3rd \
    Epode/Kutta4th \
    Epode/RKF45 \
    Epode/butcher.h \
    Epode/core.h \
    Epode/euler.h \
    Epode/bogacki_shampine.h \
    Epode/integrator.h \
    Epode/ode.h \
    Epode/solve.h \
    Epode/step.h \
    Epode/triggers.h \
    Epode/util.h \
    Epode/rk2.h \
    Epode/rkf.h

DISTFILES += \
    ../../Tests/C++/ODE_Integration/Epode/MPL_2_0.txt \
    ../../Tests/C++/ODE_Integration/Epode/README.md \
    parseCSV.py \
    plotCSV.py \
    otree.py \
    auto_label.py
