# Epode: ODE solver library

## Overview:
Epode is a library to simplify the solution of systems of ordinary differential
equations (ODEs) in C++.  It is written using C++14 which allows a clean API
that uses free functions, lambdas or function objects to define a system of
ODEs and it also allows for a number of integration control actions based on
function call parameters.

Epode uses Eigen matricies internally to represent state and for the basic
linear algebra operations that are required.  Like Eigen, Epode is a
header-only library and has minimal dependencies.  Indeed, Epode's only
dependencies are Eigen and the C++ standard library.

There are currently several low to medium order (1st-5th) explicit 
Runge-Kutta methods implemented for the solution of non-stiff systems of ODEs.
Both fixed and adaptive step-size methods are available, as is the possibility 
of implementig implicit methods for the solution of stiff systems of equations.

## To Do:
Epode is very much in a state of flux and while the basic API is likely to
change very little, there are several planned improvements and additions. At a
basic level, Epode requires additional solver methods to be added.  These may
be simply additional explicit methods (Runge-Kutta, Multistep, etc.), or they
could be implicit methods.

An additional feature that is not currently implemented but is planned (as I 
need it for my application!) is the ability to trigger integration completion
on the system state.  The end triggers will be a very generic approach to 
constructing such end conditions.  There will be similar options for defining
when data samples should be saved.

As the API has not yet stabilized, there is much work that needs to be done
on the documentation.  The basic framework of a user's manual has been created
but there are many areas that have not even begun to be filled in.  Part of 
the documentation effort will be targeted at implementing a series of example
projects that demonstrate the capabilities of the library and how the API
functions.
