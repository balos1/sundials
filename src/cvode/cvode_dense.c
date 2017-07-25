/*
 * -----------------------------------------------------------------
 * $Revision$
 * $Date$
 * ----------------------------------------------------------------- 
 * Programmer(s): Scott D. Cohen, Alan C. Hindmarsh and
 *                Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * LLNS Copyright Start
 * Copyright (c) 2014, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department 
 * of Energy by Lawrence Livermore National Laboratory in part under 
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 * -----------------------------------------------------------------
 * This is the impleentation file for the CVDENSE linear solver.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include <cvode/cvode_dense.h>
#include "cvode_direct_impl.h"
#include "cvode_impl.h"

#include <sundials/sundials_math.h>

/* Constants */

#define ZERO         RCONST(0.0)
#define ONE          RCONST(1.0)
#define TWO          RCONST(2.0)

/* CVDENSE linit, lsetup, lsolve, and lfree routines */
 
static int cvDenseInit(CVodeMem cv_mem);

static int cvDenseSetup(CVodeMem cv_mem, N_Vector vtemp1,
                        N_Vector vtemp2, N_Vector vtemp3);

static int cvDenseSolve(CVodeMem cv_mem, N_Vector b, N_Vector weight,
                        N_Vector ycur, N_Vector fcur);

static int cvDenseFree(CVodeMem cv_mem);

/*
 * -----------------------------------------------------------------
 * CVDense
 * -----------------------------------------------------------------
 * This routine initializes the memory record and sets various function
 * fields specific to the dense linear solver module.  CVDense first
 * calls the existing lfree routine if this is not NULL.  Then it sets
 * the cv_linit, cv_lsetup, cv_lsolve, cv_lfree fields in (*cvode_mem)
 * to be cvDenseInit, cvDenseSetup, cvDenseSolve, and cvDenseFree,
 * respectively.  It allocates memory for a structure of type
 * CVDlsMemRec and sets the cv_lmem field in (*cvode_mem) to the
 * address of this structure.  It sets setupNonNull in (*cvode_mem) to
 * TRUE, and the d_jac field to the default cvDlsDenseDQJac.
 * Finally, it allocates memory for M, savedJ, and lpivots.
 * The return value is SUCCESS = 0, or LMEM_FAIL = -1.
 *
 * NOTE: The dense linear solver assumes a serial implementation
 *       of the NVECTOR package. Therefore, CVDense will first 
 *       test for compatible a compatible N_Vector internal
 *       representation by checking that N_VGetArrayPointer and
 *       N_VSetArrayPointer exist.
 * -----------------------------------------------------------------
 */

int CVDense(void *cvode_mem, sunindextype N)
{
  CVodeMem cv_mem;
  CVDlsMem cvdls_mem;

  /* Return immediately if cvode_mem is NULL */
  if (cvode_mem == NULL) {
    cvProcessError(NULL, CVDLS_MEM_NULL, "CVDENSE", "CVDense", MSGD_CVMEM_NULL);
    return(CVDLS_MEM_NULL);
  }
  cv_mem = (CVodeMem) cvode_mem;

  /* Test if the NVECTOR package is compatible with the DENSE solver */
  if (cv_mem->cv_tempv->ops->nvgetarraypointer == NULL ||
      cv_mem->cv_tempv->ops->nvsetarraypointer == NULL) {
    cvProcessError(cv_mem, CVDLS_ILL_INPUT, "CVDENSE", "CVDense", MSGD_BAD_NVECTOR);
    return(CVDLS_ILL_INPUT);
  }

  if (cv_mem->cv_lfree !=NULL) cv_mem->cv_lfree(cv_mem);

  /* Set four main function fields in cv_mem */
  cv_mem->cv_linit  = cvDenseInit;
  cv_mem->cv_lsetup = cvDenseSetup;
  cv_mem->cv_lsolve = cvDenseSolve;
  cv_mem->cv_lfree  = cvDenseFree;

  /* Get memory for CVDlsMemRec */
  cvdls_mem = NULL;
  cvdls_mem = (CVDlsMem) malloc(sizeof(struct CVDlsMemRec));
  if (cvdls_mem == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVDENSE", "CVDense", MSGD_MEM_FAIL);
    return(CVDLS_MEM_FAIL);
  }

  /* Set matrix type */
  cvdls_mem->d_type = SUNDIALS_DENSE;

  /* Initialize Jacobian-related data */
  cvdls_mem->d_jacDQ = TRUE;
  cvdls_mem->d_djac = NULL;
  cvdls_mem->d_J_data = NULL;

  cvdls_mem->d_last_flag = CVDLS_SUCCESS;

  cvDlsInitializeCounters(cvdls_mem);

  cv_mem->cv_setupNonNull = TRUE;

  /* Set problem dimension */
  cvdls_mem->d_n = N;

  /* Allocate memory for M, savedJ, and pivot array */

  cvdls_mem->d_M = NULL;
  cvdls_mem->d_M = NewDenseMat(N, N);
  if (cvdls_mem->d_M == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVDENSE", "CVDense", MSGD_MEM_FAIL);
    free(cvdls_mem); cvdls_mem = NULL;
    return(CVDLS_MEM_FAIL);
  }
  cvdls_mem->d_savedJ = NULL;
  cvdls_mem->d_savedJ = NewDenseMat(N, N);
  if (cvdls_mem->d_savedJ == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVDENSE", "CVDense", MSGD_MEM_FAIL);
    DestroyMat(cvdls_mem->d_M);
    free(cvdls_mem); cvdls_mem = NULL;
    return(CVDLS_MEM_FAIL);
  }
  cvdls_mem->d_lpivots = NULL;
  cvdls_mem->d_lpivots = NewLintArray(N);
  if (cvdls_mem->d_lpivots == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVDENSE", "CVDense", MSGD_MEM_FAIL);
    DestroyMat(cvdls_mem->d_M);
    DestroyMat(cvdls_mem->d_savedJ);
    free(cvdls_mem); cvdls_mem = NULL;
    return(CVDLS_MEM_FAIL);
  }

  /* Attach linear solver memory to integrator memory */
  cv_mem->cv_lmem = cvdls_mem;

  return(CVDLS_SUCCESS);
}

/*
 * -----------------------------------------------------------------
 * cvDenseInit
 * -----------------------------------------------------------------
 * This routine does remaining initializations specific to the dense
 * linear solver.
 * -----------------------------------------------------------------
 */

static int cvDenseInit(CVodeMem cv_mem)
{
  CVDlsMem cvdls_mem;

  cvdls_mem = (CVDlsMem) cv_mem->cv_lmem;
  
  cvDlsInitializeCounters(cvdls_mem);
  /*   nje   = 0;
   nfeDQ = 0;
   nstlj = 0;
  */

  /* Set Jacobian function and data, depending on jacDQ */
  if (cvdls_mem->d_jacDQ) {
    cvdls_mem->d_djac = cvDlsDenseDQJac;
    cvdls_mem->d_J_data = cv_mem;
  } else {
    cvdls_mem->d_J_data = cv_mem->cv_user_data;
  }

  cvdls_mem->d_last_flag = CVDLS_SUCCESS;
  return(0);
}

/*
 * -----------------------------------------------------------------
 * cvDenseSetup
 * -----------------------------------------------------------------
 * This routine does the setup operations for the dense linear solver.
 * It makes a decision whether or not to call the Jacobian evaluation
 * routine based on various state variables, and if not it uses the 
 * saved copy.  In any case, it constructs the Newton matrix 
 * M = I - gamma*J, updates counters, and calls the dense LU 
 * factorization routine.
 * -----------------------------------------------------------------
 */

static int cvDenseSetup(CVodeMem cv_mem, N_Vector vtemp1, 
                        N_Vector vtemp2, N_Vector vtemp3)
{
  sunindextype ier;
  CVDlsMem cvdls_mem;
  
  cvdls_mem = (CVDlsMem) cv_mem->cv_lmem;

  /* setup system matrix */
  ier = CVDlsSetupMatrix(cv_mem, vtemp1, vtemp2, vtemp3);
  if (ier < 0)  return(-1);
  if (ier > 0)  return(1);

  /* Do LU factorization of M */
  ier = DenseGETRF(cvdls_mem->d_M, cvdls_mem->d_lpivots); 

  /* Return 0 if the LU was complete; otherwise return 1 */
  cvdls_mem->d_last_flag = (long int) ier;
  if (ier > 0) return(1);
  return(0);
}

/*
 * -----------------------------------------------------------------
 * cvDenseSolve
 * -----------------------------------------------------------------
 * This routine handles the solve operation for the dense linear solver
 * by calling the dense backsolve routine.  The returned value is 0.
 * -----------------------------------------------------------------
 */

static int cvDenseSolve(CVodeMem cv_mem, N_Vector b, N_Vector weight,
                        N_Vector ycur, N_Vector fcur)
{
  CVDlsMem cvdls_mem;
  realtype *bd;

  cvdls_mem = (CVDlsMem) cv_mem->cv_lmem;
  
  bd = N_VGetArrayPointer(b);

  DenseGETRS(cvdls_mem->d_M, cvdls_mem->d_lpivots, bd);

  /* If CV_BDF, scale the correction to account for change in gamma */
  if ((cv_mem->cv_lmm == CV_BDF) && (cv_mem->cv_gamrat != ONE)) {
    N_VScale(TWO/(ONE + cv_mem->cv_gamrat), b, b);
  }
  
  cvdls_mem->d_last_flag = CVDLS_SUCCESS;
  return(0);
}

/*
 * -----------------------------------------------------------------
 * cvDenseFree
 * -----------------------------------------------------------------
 * This routine frees memory specific to the dense linear solver.
 * -----------------------------------------------------------------
 */

static int cvDenseFree(CVodeMem cv_mem)
{
  CVDlsMem  cvdls_mem;

  cvdls_mem = (CVDlsMem) cv_mem->cv_lmem;
  
  DestroyMat(cvdls_mem->d_M);
  DestroyMat(cvdls_mem->d_savedJ);
  DestroyArray(cvdls_mem->d_lpivots);
  free(cvdls_mem);
  cv_mem->cv_lmem = NULL;

  return(0);
}

