/*
 * -----------------------------------------------------------------
 * $Revision: 1.9 $
 * $Date: 2005-03-19 00:10:45 $
 * -----------------------------------------------------------------
 * Programmer(s): Allan Taylor, Alan Hindmarsh and
 *                Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2002, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials/kinsol/LICENSE.
 * -----------------------------------------------------------------
 * Routines used to interface between KINSOL and a Fortran
 * user-supplied routine FKJTIMES (Jacobian J times vector v).
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "fkinsol.h"        /* prototypes of interfaces and global variables */
#include "kinsol.h"         /* KINSOL constants and prototypes               */
#include "kinspbcg.h"       /* prototypes of KINSPBCG interface routines     */
#include "kinspgmr.h"       /* prototypes of KINSPGMR interface routines     */
#include "nvector.h"        /* definition of type N_Vector                   */
#include "sundialstypes.h"  /* definition of type realtype                   */

/*
 * ----------------------------------------------------------------
 * prototype of the user-supplied fortran routine
 * ----------------------------------------------------------------
 */
#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

extern void FK_JTIMES(realtype*, realtype*, int*, realtype*, int*);

#ifdef __cplusplus
}
#endif

/*
 * ----------------------------------------------------------------
 * Function : FKIN_SPBCGSETJAC
 * ----------------------------------------------------------------
 */

void FKIN_SPBCGSETJAC(int *flag, int *ier)
{
  if ((*flag) == 0) KINSpbcgSetJacTimesVecFn(KIN_mem, NULL);
  else KINSpbcgSetJacTimesVecFn(KIN_mem, FKINJtimes);
}

/*
 * ----------------------------------------------------------------
 * Function : FKIN_SPGMRSETJAC
 * ----------------------------------------------------------------
 */

void FKIN_SPGMRSETJAC(int *flag, int *ier)
{
  if ((*flag) == 0) KINSpgmrSetJacTimesVecFn(KIN_mem, NULL);
  else KINSpgmrSetJacTimesVecFn(KIN_mem, FKINJtimes);
}

/*
 * ----------------------------------------------------------------
 * Function : FKINJtimes
 * ----------------------------------------------------------------
 * C function FKINJtimes is used to interface between
 * KINSpgmr/KINSpgmrJTimes or KINSpbcg/KINSpbcgJTimes and FK_JTIMES
 * (user-supplied Fortran routine).
 * ----------------------------------------------------------------
 */

int FKINJtimes(N_Vector v, N_Vector Jv,
               N_Vector uu, booleantype *new_uu, 
               void *J_data)
{
 int retcode;
 realtype *vdata, *Jvdata, *uudata;
 
 vdata  = N_VGetArrayPointer(v);
 uudata = N_VGetArrayPointer(uu);
 Jvdata = N_VGetArrayPointer(Jv);
 
 FK_JTIMES(vdata, Jvdata, (int *)new_uu, uudata, &retcode);

 return(retcode);
}
