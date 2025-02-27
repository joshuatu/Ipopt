// Copyright (C) 2004, 2010 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

#include "IpOptErrorConvCheck.hpp"

#ifdef HAVE_CMATH
# include <cmath>
#else
# ifdef HAVE_MATH_H
#  include <math.h>
# else
#  error "don't have header file for math"
# endif
#endif

namespace Ipopt
{
#if COIN_IPOPT_VERBOSITY > 0
static const Index dbg_verbosity = 0;
#endif

OptimalityErrorConvergenceCheck::OptimalityErrorConvergenceCheck()
{ }

OptimalityErrorConvergenceCheck::~OptimalityErrorConvergenceCheck()
{ }

void OptimalityErrorConvergenceCheck::RegisterOptions(
   SmartPtr<RegisteredOptions> roptions
)
{
   roptions->AddLowerBoundedIntegerOption(
      "max_iter",
      "Maximum number of iterations.",
      0,
      3000,
      "The algorithm terminates with an error message if the number of iterations exceeded this number.");
   roptions->AddLowerBoundedNumberOption(
      "max_cpu_time",
      "Maximum number of CPU seconds.",
      0.0, true,
      1e6,
      "A limit on CPU seconds that Ipopt can use to solve one problem. "
      "If during the convergence check this limit is exceeded, Ipopt will terminate with a corresponding error message.");
   roptions->AddLowerBoundedNumberOption(
      "dual_inf_tol",
      "Desired threshold for the dual infeasibility.",
      0., true,
      1.,
      "Absolute tolerance on the dual infeasibility. "
      "Successful termination requires that the max-norm of the (unscaled) dual infeasibility is less than this threshold.");
   roptions->AddLowerBoundedNumberOption(
      "constr_viol_tol",
      "Desired threshold for the constraint violation.",
      0.0, true,
      1e-4,
      "Absolute tolerance on the constraint violation. "
      "Successful termination requires that the max-norm of the (unscaled) constraint violation is less than this threshold.");
   roptions->AddLowerBoundedNumberOption(
      "compl_inf_tol",
      "Desired threshold for the complementarity conditions.",
      0., true,
      1e-4,
      "Absolute tolerance on the complementarity. "
      "Successful termination requires that the max-norm of the (unscaled) complementarity is less than this threshold.");
   roptions->AddLowerBoundedNumberOption(
      "acceptable_tol",
      "\"Acceptable\" convergence tolerance (relative).",
      0., true,
      1e-6,
      "Determines which (scaled) overall optimality error is considered to be \"acceptable\". "
      "There are two levels of termination criteria. "
      "If the usual \"desired\" tolerances (see tol, dual_inf_tol etc) are satisfied at an iteration, "
      "the algorithm immediately terminates with a success message. "
      "On the other hand, if the algorithm encounters \"acceptable_iter\" many iterations in a row that are considered \"acceptable\", "
      "it will terminate before the desired convergence tolerance is met. "
      "This is useful in cases where the algorithm might not be able to achieve the \"desired\" level of accuracy.");
   roptions->AddLowerBoundedIntegerOption(
      "acceptable_iter",
      "Number of \"acceptable\" iterates before triggering termination.",
      0,
      15,
      "If the algorithm encounters this many successive \"acceptable\" iterates (see \"acceptable_tol\"), "
      "it terminates, assuming that the problem has been solved to best possible accuracy given round-off. "
      "If it is set to zero, this heuristic is disabled.");
   roptions->AddLowerBoundedNumberOption(
      "acceptable_dual_inf_tol",
      "\"Acceptance\" threshold for the dual infeasibility.",
      0., true,
      1e10,
      "Absolute tolerance on the dual infeasibility. "
      "\"Acceptable\" termination requires that the (max-norm of the unscaled) dual infeasibility is less than this threshold; "
      "see also acceptable_tol.");
   roptions->AddLowerBoundedNumberOption(
      "acceptable_constr_viol_tol",
      "\"Acceptance\" threshold for the constraint violation.",
      0., true,
      1e-2,
      "Absolute tolerance on the constraint violation. "
      "\"Acceptable\" termination requires that the max-norm of the (unscaled) constraint violation is less than this threshold; "
      "see also acceptable_tol.");
   roptions->AddLowerBoundedNumberOption(
      "acceptable_compl_inf_tol",
      "\"Acceptance\" threshold for the complementarity conditions.",
      0., true,
      1e-2,
      "Absolute tolerance on the complementarity. "
      "\"Acceptable\" termination requires that the max-norm of the (unscaled) complementarity is less than this threshold; "
      "see also acceptable_tol.");
   roptions->AddLowerBoundedNumberOption(
      "acceptable_obj_change_tol",
      "\"Acceptance\" stopping criterion based on objective function change.",
      0., false,
      1e20,
      "If the relative change of the objective function (scaled by Max(1,|f(x)|)) is less than this value, "
      "this part of the acceptable tolerance termination is satisfied; see also acceptable_tol. "
      "This is useful for the quasi-Newton option, which has trouble to bring down the dual infeasibility.");
   roptions->AddLowerBoundedNumberOption(
      "diverging_iterates_tol",
      "Threshold for maximal value of primal iterates.",
      0., true,
      1e20,
      "If any component of the primal iterates exceeded this value (in absolute terms), "
      "the optimization is aborted with the exit message that the iterates seem to be diverging.");
   roptions->AddLowerBoundedNumberOption(
      "mu_target",
      "Desired value of complementarity.",
      0., false,
      0.,
      "Usually, the barrier parameter is driven to zero and "
      "the termination test for complementarity is measured with respect to zero complementarity. "
      "However, in some cases it might be desired to have Ipopt solve barrier problem for strictly positive value of the barrier parameter. "
      "In this case, the value of \"mu_target\" specifies the final value of the barrier parameter, and "
      "the termination tests are then defined with respect to the barrier problem for this value of the barrier parameter.");
}

bool OptimalityErrorConvergenceCheck::InitializeImpl(
   const OptionsList& options,
   const std::string& prefix
)
{
   options.GetIntegerValue("max_iter", max_iterations_, prefix);
   options.GetNumericValue("max_cpu_time", max_cpu_time_, prefix);
   options.GetNumericValue("dual_inf_tol", dual_inf_tol_, prefix);
   options.GetNumericValue("constr_viol_tol", constr_viol_tol_, prefix);
   options.GetNumericValue("compl_inf_tol", compl_inf_tol_, prefix);
   options.GetIntegerValue("acceptable_iter", acceptable_iter_, prefix);
   options.GetNumericValue("acceptable_tol", acceptable_tol_, prefix);
   options.GetNumericValue("acceptable_dual_inf_tol", acceptable_dual_inf_tol_, prefix);
   options.GetNumericValue("acceptable_constr_viol_tol", acceptable_constr_viol_tol_, prefix);
   options.GetNumericValue("acceptable_compl_inf_tol", acceptable_compl_inf_tol_, prefix);
   options.GetNumericValue("acceptable_obj_change_tol", acceptable_obj_change_tol_, prefix);
   options.GetNumericValue("diverging_iterates_tol", diverging_iterates_tol_, prefix);
   options.GetNumericValue("mu_target", mu_target_, prefix);
   acceptable_counter_ = 0;
   curr_obj_val_ = -1e50;
   last_obj_val_iter_ = -1;

   return true;
}

ConvergenceCheck::ConvergenceStatus OptimalityErrorConvergenceCheck::CheckConvergence(
   bool call_intermediate_callback /*= true*/
)
{
   DBG_START_METH("OptimalityErrorConvergenceCheck::CheckConvergence", dbg_verbosity);

   if( call_intermediate_callback )
   {
      // Check if user requested termination by calling the intermediate
      // user callback function
      AlgorithmMode mode = RegularMode;
      // Gather the information also used in the iteration output
      Index iter = IpData().iter_count();
      Number inf_pr = IpCq().curr_primal_infeasibility(NORM_MAX);
      Number inf_du = IpCq().curr_dual_infeasibility(NORM_MAX);
      Number mu = IpData().curr_mu();
      Number dnrm;
      if( IsValid(IpData().delta()) && IsValid(IpData().delta()->x()) && IsValid(IpData().delta()->s()) )
      {
         dnrm = Max(IpData().delta()->x()->Amax(), IpData().delta()->s()->Amax());
      }
      else
      {
         // This is the first iteration - no search direction has been
         // computed yet.
         dnrm = 0.;
      }
      Number alpha_primal = IpData().info_alpha_primal();
      Number alpha_dual = IpData().info_alpha_dual();
      Number regu_x = IpData().info_regu_x();
      Number unscaled_f = IpCq().unscaled_curr_f();
      Index ls_count = IpData().info_ls_count();
      bool request_stop = !IpNLP().IntermediateCallBack(mode, iter, unscaled_f, inf_pr, inf_du, mu, dnrm, regu_x,
                          alpha_dual, alpha_primal, ls_count, &IpData(), &IpCq());

      if( request_stop )
      {
         return ConvergenceCheck::USER_STOP;
      }
   }

   Number overall_error = IpCq().curr_nlp_error();
   Number dual_inf = IpCq().unscaled_curr_dual_infeasibility(NORM_MAX);
   Number constr_viol = IpCq().unscaled_curr_nlp_constraint_violation(NORM_MAX);
   Number compl_inf = IpCq().unscaled_curr_complementarity(mu_target_, NORM_MAX);

   if( IpData().curr()->x()->Dim() == IpData().curr()->y_c()->Dim() )
   {
      // the problem is square, there is no point in looking at dual
      // infeasibility and complementarity as termination criterion
      dual_inf_tol_ = 1e300;
      compl_inf_tol_ = 1e300;
   }

   if( Jnlst().ProduceOutput(J_MOREDETAILED, J_MAIN) )
   {
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "Convergence Check:\n");
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  overall_error = %23.16e   IpData().tol()   = %23.16e\n", overall_error, IpData().tol());
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  dual_inf      = %23.16e   dual_inf_tol_    = %23.16e\n", dual_inf, dual_inf_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  constr_viol   = %23.16e   constr_viol_tol_ = %23.16e\n", constr_viol, constr_viol_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  compl_inf     = %23.16e   compl_inf_tol_   = %23.16e\n", compl_inf,  compl_inf_tol_);
   }
   if( overall_error <= IpData().tol() && dual_inf <= dual_inf_tol_ && constr_viol <= constr_viol_tol_
       && compl_inf <= compl_inf_tol_ )
   {
      return ConvergenceCheck::CONVERGED;
   }

   if( acceptable_iter_ > 0 && CurrentIsAcceptable() )
   {
      IpData().Append_info_string("A");
      acceptable_counter_++;
      if( acceptable_counter_ >= acceptable_iter_ )
      {
         return ConvergenceCheck::CONVERGED_TO_ACCEPTABLE_POINT;
      }
   }
   else
   {
      acceptable_counter_ = 0;
   }

   if( IpData().curr()->x()->Amax() > diverging_iterates_tol_ )
   {
      return ConvergenceCheck::DIVERGING;
   }

   if( IpData().iter_count() >= max_iterations_ )
   {
      return ConvergenceCheck::MAXITER_EXCEEDED;
   }

   Number curr_cpu_time = CpuTime();
   if( max_cpu_time_ < 999999. && curr_cpu_time - IpData().cpu_time_start() > max_cpu_time_ )
   {
      return ConvergenceCheck::CPUTIME_EXCEEDED;
   }

   return ConvergenceCheck::CONTINUE;
}

bool OptimalityErrorConvergenceCheck::CurrentIsAcceptable()
{
   DBG_START_METH("OptimalityErrorConvergenceCheck::CurrentIsAcceptable",
                  dbg_verbosity);

   Number overall_error = IpCq().curr_nlp_error();
   Number dual_inf = IpCq().unscaled_curr_dual_infeasibility(NORM_MAX);
   Number constr_viol = IpCq().unscaled_curr_nlp_constraint_violation(NORM_MAX);
   Number compl_inf = IpCq().unscaled_curr_complementarity(mu_target_, NORM_MAX);

   if( IpData().iter_count() != last_obj_val_iter_ )
   {
      // DELETEME
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "obj val update iter = %d\n", IpData().iter_count());
      last_obj_val_ = curr_obj_val_;
      curr_obj_val_ = IpCq().curr_f();
      last_obj_val_iter_ = IpData().iter_count();
   }

   DBG_PRINT((1, "overall_error = %e\n", overall_error));
   DBG_PRINT((1, "dual_inf = %e\n", dual_inf));
   DBG_PRINT((1, "constr_viol = %e\n", constr_viol));
   DBG_PRINT((1, "compl_inf = %e\n", compl_inf));

   DBG_PRINT((1, "acceptable_tol_ = %e\n", acceptable_tol_));
   DBG_PRINT((1, "acceptable_dual_inf_tol_ = %e\n", acceptable_dual_inf_tol_));
   DBG_PRINT((1, "acceptable_constr_viol_tol_ = %e\n", acceptable_constr_viol_tol_));
   DBG_PRINT((1, "acceptable_compl_inf_tol_ = %e\n", acceptable_compl_inf_tol_));

   if( IpData().curr()->x()->Dim() == IpData().curr()->y_c()->Dim() )
   {
      // the problem is square, there is no point in looking at dual
      // infeasibility and complementarity as termination criterion
      acceptable_dual_inf_tol_ = 1e300;
      acceptable_compl_inf_tol_ = 1e300;
   }

   if( Jnlst().ProduceOutput(J_MOREDETAILED, J_MAIN) )
   {
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "Acceptable Check:\n");
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  overall_error = %23.16e   acceptable_tol_             = %23.16e\n", overall_error, acceptable_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  dual_inf      = %23.16e   acceptable_dual_inf_tol_    = %23.16e\n", dual_inf, acceptable_dual_inf_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  constr_viol   = %23.16e   acceptable_constr_viol_tol_ = %23.16e\n", constr_viol, acceptable_constr_viol_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  compl_inf     = %23.16e   acceptable_compl_inf_tol_   = %23.16e\n", compl_inf, acceptable_compl_inf_tol_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  curr_obj_val_ = %23.16e   last_obj_val                = %23.16e\n", curr_obj_val_, last_obj_val_);
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "  fabs(curr_obj_val_-last_obj_val_)/Max(1., fabs(curr_obj_val_)) = %23.16e acceptable_obj_change_tol_ = %23.16e\n",
                     fabs(curr_obj_val_ - last_obj_val_) / Max(1., fabs(curr_obj_val_)), acceptable_obj_change_tol_);
      // DELETEME
      Jnlst().Printf(J_MOREDETAILED, J_MAIN,
                     "test iter = %d\n", IpData().iter_count());
   }

   return (overall_error <= acceptable_tol_ && dual_inf <= acceptable_dual_inf_tol_
           && constr_viol <= acceptable_constr_viol_tol_ && compl_inf <= acceptable_compl_inf_tol_
           && fabs(curr_obj_val_ - last_obj_val_) / Max(1., fabs(curr_obj_val_)) <= acceptable_obj_change_tol_);
}

} // namespace Ipopt
