#Overview
Epode is a header-only C++14 library for the solution of systems of ordinary differental equations,
specifically, of initial value problems.

#Project Goals
The goal of the Epode library is to provide a light-weight ode solving solution in modern C++ which
remains flexible and fast.

Epode strives to provide a wide range of solution methods.  As it undergoes API stabilization, many
of the solution methods will be taken from the Wikipidia page "List of Runge–Kutta methods".  These
methods are most useful for non-stiff odes however and it would be nice to have some methods capable
of handling stiff systems of equations as well.

#What's with the name anyway?
Epode is C++14 compatible library for integration of ordinary differential equations (odes). The
connection is simple, an ode is a lyric poem written to address a particular subject.  This library
was written to address the need for a clean, easy to use and efficient library for the solution of
ordinary differental equations.  The final link (as simply naming the library "ode" would be too
boring) is that the word epode is synonomous to ode and refers to a specific form of lyric poem
typified by couplets composed of lines with, typically, varying length (not that that matters in any
way to this library).  The Epode library is an ode to the new features of C++ 11/14 which make such
a library much easier to create and use.  It doesn't hurt that it's a short name and contains the
letters 'o', 'd', 'e', either!

#Basic coding conventions
Epode is a header-only library.  Class names are capitalized.  Member types are suffixed with "_t".
Documentation is in the

#Dependencies
Epode uses objects of Eigen Matrix class to pass around state vectors.

#Documentation
The Epode manual is written in LaTex and will cover everything from basic usage to internal details.
It is very much "in process" at the moment.
