Ipopt
=====

Introduction
------------

Ipopt (Interior Point OPTimizer, pronounced eye-pea-Opt) is a software package for large-scale [nonlinear optimization](http://wiki.mcs.anl.gov/NEOS/index.php/Nonlinear_Programming_FAQ).
It is designed to find (local) solutions of mathematical optimization problems of the form

```
   min     f(x)
  x ∈ Rⁿ

s.t.       g_L ≤ g(x) ≤ g_U
           x_L ≤  x   ≤ x_U
```
where ```f(x): Rⁿ --> R``` is the objective function, and ```g(x): Rⁿ --> Rᵐ```
are the constraint functions.  The vectors `g_L` and `g_U` denote the lower and upper bounds on the constraints, and the vectors `x_L` and `x_U` are the bounds on the variables `x`.
The functions `f(x)` and `g(x)` can be nonlinear and nonconvex, but should be twice continuously differentiable.
Note that equality constraints can be formulated in the above formulation by setting the corresponding components of `g_L` and `g_U` to the same value.

Ipopt is part of the [COIN-OR Initiative](http://www.coin-or.org).
The Ipopt project webpage is <https://github.com/coin-or/Ipopt>.


Background
----------

Ipopt is written in C++ and is released as open source code under the Eclipse Public License (EPL).
The code has been written by [Andreas Wächter](http://www.mccormick.northwestern.edu/directory/profiles/Andreas-Waechter.html) and [Carl Laird](http://allthingsoptimal.com/biography/).
The COIN-OR project managers for Ipopt are [Andreas Wächter](http://users.iems.northwestern.edu/~andreasw) und [Stefan Vigerske](https://www.gams.com/~stefan).
For a list of **all contributors**, see the [AUTHORS file](Ipopt/AUTHORS).

The C++ version has first been [released on Aug 26, 2005](http://list.coin-or.org/pipermail/ipopt/2005-August/000331.html) as version 3.0.0.
The previously released [pre-3.0 Fortran version](https://github.com/coin-or/Ipopt/tree/stable/2.3) is no longer maintained.


The Ipopt distribution can be used to generate a library that can be linked to one's own C++, C, Fortran, or Java code, as well as a solver executable for the [AMPL](http://www.ampl.com) modeling environment.
The package includes interfaces to [CUTEr](http://cuter.rl.ac.uk/cuter-www/) optimization testing environment, as well as the [MATLAB](http://www.mathworks.com/products/matlab) and [R](http://www.r-project.org/) programming environments.
IPOPT can be used on Linux/UNIX, Mac OS X and Windows platforms.

As open source software, the source code for Ipopt is provided without charge.
You are free to use it, also for commercial purposes.
You are also free to modify the source code (with the restriction that you need to make your changes public if you decide to distribute your version in any way, e.g. as an executable); for details see the EPL license.
And we are certainly very keen on feedback from users, including contributions!

In order to compile Ipopt, certain third party code is required (such as some linear algebra routines).
Those are available under different conditions/licenses.

If you want to learn more about Ipopt, you can find references in the [bibliography of the documentation](https://coin-or.github.io/Ipopt/citelist.html) and this ["Papers about Ipopt" page](https://github.com/coin-or/Ipopt/wiki/IpoptPapers).

For information on projects that use Ipopt, refer to the [Success Stories page](https://github.com/coin-or/Ipopt/wiki/SuccessStories).


Download
--------

**[Download Ipopt source as tarballs.](http://www.coin-or.org/download/source/Ipopt)**

You can also obtain the Ipopt code via Git.
Please refer to the [documentation](https://coin-or.github.io/Ipopt) and the [General Configuration and Installation Instructions for COIN-OR projects](https://projects.coin-or.org/CoinHelp/).

**Please make sure you read the [current issues page](https://projects.coin-or.org/CoinHelp/wiki/current-issues) before you try to install Ipopt.**

Also still available is the no longer maintained older [Fortran version](https://github.com/coin-or/Ipopt/tree/stable/2.3).

Additionally, **[JuliaOpt provides Ipopt binaries](https://github.com/JuliaOpt/IpoptBuilder/releases)**,
**[AMPL provides binaries](http://ampl.com/products/solvers/open-source/#ipopt)** for using Ipopt through AMPL,
and the **[Pardiso project provides binaries](https://pardiso-project.org/index.html#binaries)** for using Ipopt with Pardiso through Matlab.


Documentation
-------------

 * **[Ipopt Documentation](https://coin-or.github.io/Ipopt/)** with installation instructions, options reference, and more
 * **[Ipopt Wiki](https://github.com/coin-or/Ipopt/wiki)** with more compilation hints, current issues, and hints and tricks
 * **[Mailing list archive](http://list.coin-or.org/pipermail/ipopt/)**
 * [Changelog](ChangeLog)
 * [General Configuration and Installation Instructions](https://projects.coin-or.org/CoinHelp/) for COIN-OR projects
 * [short Ipopt tutorial](http://drops.dagstuhl.de/volltexte/2009/2089/pdf/09061.WaechterAndreas.Paper.2089.pdf)


Getting Help
------------

 * **[Mailing list](http://list.coin-or.org/mailman/listinfo/ipopt)**: subscribe to get notifications about updates and to post questions and comments regarding Ipopt
 * **[Issue tracking system](https://github.com/coin-or/Ipopt/issues/)**: If you believe you found a **bug** in the code, please use the issue tracking system.
   Please include as much information as possible, and if possible some (ideally simple) example code so that we can reproduce the error.

 Configure options:
 -----------------
  ```
  softenv
  soft add +gcc-8.2.0
  ../configure ADD_FFLAGS='-fopenmp' ADD_CFLAGS='-fopenmp' ADD_CXXFLAGS='-fopenmp' --with-pardiso="-L$HOME/Pardiso/ -lpardiso600-GNU720-X86-64 -lgomp"
  ```

Run pardiso through julia:
-------------------------

```
using Libdl
println("Loading lapack")
liblapack=Libdl.dlopen("/usr/lib/x86_64-linux-gnu/liblapack.so",RTLD_GLOBAL)
liblapack=Libdl.dlopen("/usr/lib/x86_64-linux-gnu/libomp.so",RTLD_GLOBAL)
```
*use something like: find /usr/lib -name 'liblapack*

option.opt
------
```
linear_solver pardiso
pardiso_iterative yes
pardiso_max_iter 400
pardiso_iter_dropping_factor 0.01
pardiso_iter_dropping_schur 0.005
pardiso_iter_max_row_fill 10
pardiso_iter_inverse_norm_factor 500
```
Please Cite Us
--------------

We provide this program in the hope that it may be useful to others, and we would very much like to hear about your experience with it.
If you found it helpful and are using it within our software, we encourage you to add your feedback to the [Success Stories page](https://github.com/coin-or/Ipopt/wiki/SuccessStories).

Since a lot of time and effort has gone into Ipopt's development, **please cite the following publication if you are using Ipopt for your own research**:

* A. Wächter and L. T. Biegler, **[On the Implementation of a Primal-Dual Interior Point Filter Line Search Algorithm for Large-Scale Nonlinear Programming](http://dx.doi.org/10.1007/s10107-004-0559-y)**, _Mathematical Programming_ 106(1), pp. 25-57, 2006
  ([preprint](http://www.optimization-online.org/DB_HTML/2004/03/836.html))
  
 
 
 
