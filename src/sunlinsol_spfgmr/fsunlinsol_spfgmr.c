/*
 * -----------------------------------------------------------------
 * Programmer(s): Daniel Reynolds @ SMU
 *                David Gardner, Carol Woodward, Slaven Peles @ LLNL
 * -----------------------------------------------------------------
 * LLNS/SMU Copyright Start
 * Copyright (c) 2017, Southern Methodist University and 
 * Lawrence Livermore National Security
 *
 * This work was performed under the auspices of the U.S. Department 
 * of Energy by Southern Methodist University and Lawrence Livermore 
 * National Laboratory under Contract DE-AC52-07NA27344.
 * Produced at Southern Methodist University and the Lawrence 
 * Livermore National Laboratory.
 *
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS/SMU Copyright End
 * -----------------------------------------------------------------
 * This file (companion of fsunlinsol_spfgmr.h) contains the
 * implementation needed for the Fortran initialization of SPFGMR
 * linear solver operations.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "fsunlinsol_spfgmr.h"

/* Define global linsol variables */

SUNLinearSolver F2C_CVODE_linsol;
SUNLinearSolver F2C_IDA_linsol;
SUNLinearSolver F2C_KINSOL_linsol;
SUNLinearSolver F2C_ARKODE_linsol;

/* Declarations of external global variables */

extern SUNMatrix F2C_CVODE_matrix;
extern SUNMatrix F2C_IDA_matrix;
extern SUNMatrix F2C_KINSOL_matrix;
extern SUNMatrix F2C_ARKODE_matrix;

extern N_Vector F2C_CVODE_vec;
extern N_Vector F2C_IDA_vec;
extern N_Vector F2C_KINSOL_vec;
extern N_Vector F2C_ARKODE_vec;

/* Fortran callable interfaces */

void FSUNSPFGMR_INIT(int *code, int *pretype, int *maxl, int *ier)
{
  *ier = 0;

  switch(*code) {
  case FCMIX_CVODE:
    F2C_CVODE_linsol = NULL;
    F2C_CVODE_linsol = SUNSPFGMR(F2C_CVODE_vec, *pretype, *maxl);
    if (F2C_CVODE_linsol == NULL) *ier = -1;
    break;
  case FCMIX_IDA:
    F2C_IDA_linsol = NULL;
    F2C_IDA_linsol = SUNSPFGMR(F2C_IDA_vec, *pretype, *maxl);
    if (F2C_IDA_linsol == NULL) *ier = -1;
    break;
  case FCMIX_KINSOL:
    F2C_KINSOL_linsol = NULL;
    F2C_KINSOL_linsol = SUNSPFGMR(F2C_KINSOL_vec, *pretype, *maxl);
    if (F2C_KINSOL_linsol == NULL) *ier = -1;
    break;
  case FCMIX_ARKODE:
    F2C_ARKODE_linsol = NULL;
    F2C_ARKODE_linsol = SUNSPFGMR(F2C_ARKODE_vec, *pretype, *maxl);
    if (F2C_ARKODE_linsol == NULL) *ier = -1;
    break;
  default:
    *ier = -1;
  }
}
