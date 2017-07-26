/*
 * -----------------------------------------------------------------
 * $Revision$
 * $Date$
 * ----------------------------------------------------------------- 
 * Programmer(s): Radu Serban @ LLNL
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
 * This is the implementation file for the CVSDENSE linear solver.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include <cvodes/cvodes_dense.h>
#include "cvodes_direct_impl.h"
#include "cvodes_impl.h"

#include <sundials/sundials_math.h>

/* Constants */

#define ZERO         RCONST(0.0)
#define ONE          RCONST(1.0)
#define TWO          RCONST(2.0)

/* CVSDENSE linit, lsetup, lsolve, and lfree routines */
static int cvDenseInit(CVodeMem cv_mem);
static int cvDenseSetup(CVodeMem cv_mem, int convfail, N_Vector ypred,
                        N_Vector fpred, booleantype *jcurPtr, 
                        N_Vector vtemp1, N_Vector vtemp2, N_Vector vtemp3);
static int cvDenseSolve(CVodeMem cv_mem, N_Vector b, N_Vector weight,
                        N_Vector ycur, N_Vector fcur);
static int cvDenseFree(CVodeMem cv_mem);

/* CVSDENSE lfreeB function */
static int cvDenseFreeB(CVodeBMem cvb_mem);

/* 
 * ================================================================
 *
 *                   PART I - forward problems
 *
 * ================================================================
 */

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
 * TRUE, and the d_jac field to the default CVDenseDQJac.
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
    cvProcessError(NULL, CVDLS_MEM_NULL, "CVSDENSE", "CVDense", MSGD_CVMEM_NULL);
    return(CVDLS_MEM_NULL);
  }
  cv_mem = (CVodeMem) cvode_mem;

  /* Test if the NVECTOR package is compatible with the DENSE solver */
  if (cv_mem->cv_tempv->ops->nvgetarraypointer == NULL ||
      cv_mem->cv_tempv->ops->nvsetarraypointer == NULL) {
    cvProcessError(cv_mem, CVDLS_ILL_INPUT, "CVSDENSE", "CVDense", MSGD_BAD_NVECTOR);
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
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVSDENSE", "CVDense", MSGD_MEM_FAIL);
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
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVSDENSE", "CVDense", MSGD_MEM_FAIL);
    free(cvdls_mem); cvdls_mem = NULL;
    return(CVDLS_MEM_FAIL);
  }
  cvdls_mem->d_savedJ = NULL;
  cvdls_mem->d_savedJ = NewDenseMat(N, N);
  if (cvdls_mem->d_savedJ == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVSDENSE", "CVDense", MSGD_MEM_FAIL);
    DestroyMat(cvdls_mem->d_M);
    free(cvdls_mem); cvdls_mem = NULL;
    return(CVDLS_MEM_FAIL);
  }
  cvdls_mem->d_lpivots = NULL;
  cvdls_mem->d_lpivots = NewIndexArray(N);
  if (cvdls_mem->d_lpivots == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVSDENSE", "CVDense", MSGD_MEM_FAIL);
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

  /*
   nje   = 0;
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

static int cvDenseSetup(CVodeMem cv_mem, int convfail, N_Vector ypred,
                        N_Vector fpred, booleantype *jcurPtr, 
                        N_Vector vtemp1, N_Vector vtemp2, N_Vector vtemp3)
{
  CVDlsMem cvdls_mem;
  booleantype jbad, jok;
  realtype dgamma;
  int retval;
  sunindextype ier;

  cvdls_mem = (CVDlsMem) cv_mem->cv_lmem;
 
  /* Use nst, gamma/gammap, and convfail to set J eval. flag jok */
 
  dgamma = SUNRabs((cv_mem->cv_gamma/cv_mem->cv_gammap) - ONE);
  jbad = (cv_mem->cv_nst == 0) ||
    (cv_mem->cv_nst > cvdls_mem->d_nstlj + CVD_MSBJ) ||
    ((convfail == CV_FAIL_BAD_J) && (dgamma < CVD_DGMAX)) ||
    (convfail == CV_FAIL_OTHER);
  jok = !jbad;
 
  if (jok) {

    /* If jok = TRUE, use saved copy of J */
    *jcurPtr = FALSE;
    DenseCopy(cvdls_mem->d_savedJ, cvdls_mem->d_M);

  } else {

    /* If jok = FALSE, call jac routine for new J value */
    cvdls_mem->d_nje++;
    cvdls_mem->d_nstlj = cv_mem->cv_nst;
    *jcurPtr = TRUE;
    SetToZero(cvdls_mem->d_M);

    retval = cvdls_mem->d_djac(cvdls_mem->d_n, cv_mem->cv_tn, ypred,
                               fpred, cvdls_mem->d_M, cvdls_mem->d_J_data,
                               vtemp1, vtemp2, vtemp3);
    if (retval < 0) {
      cvProcessError(cv_mem, CVDLS_JACFUNC_UNRECVR, "CVSDENSE", "cvDenseSetup", MSGD_JACFUNC_FAILED);
      cvdls_mem->d_last_flag = CVDLS_JACFUNC_UNRECVR;
      return(-1);
    }
    if (retval > 0) {
      cvdls_mem->d_last_flag = CVDLS_JACFUNC_RECVR;
      return(1);
    }

    DenseCopy(cvdls_mem->d_M, cvdls_mem->d_savedJ);

  }
  
  /* Scale and add I to get M = I - gamma*J */
  DenseScale(-cv_mem->cv_gamma, cvdls_mem->d_M);
  AddIdentity(cvdls_mem->d_M);

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

/* 
 * ================================================================
 *
 *                   PART II - backward problems
 *
 * ================================================================
 */

/*
 * CVDenseB is a wrapper around CVDense. It attaches the CVSDENSE linear solver
 * to the backward problem memory block.
 */

int CVDenseB(void *cvode_mem, int which, sunindextype nB)
{
  CVodeMem cv_mem;
  CVadjMem ca_mem;
  CVodeBMem cvB_mem;
  void *cvodeB_mem;
  CVDlsMemB cvdlsB_mem;
  int flag;

  /* Check if cvode_mem exists */
  if (cvode_mem == NULL) {
    cvProcessError(NULL, CVDLS_MEM_NULL, "CVSDENSE", "CVDenseB", MSGD_CVMEM_NULL);
    return(CVDLS_MEM_NULL);
  }
  cv_mem = (CVodeMem) cvode_mem;

  /* Was ASA initialized? */
  if (cv_mem->cv_adjMallocDone == FALSE) {
    cvProcessError(cv_mem, CVDLS_NO_ADJ, "CVSDENSE", "CVDenseB", MSGD_NO_ADJ);
    return(CVDLS_NO_ADJ);
  } 
  ca_mem = cv_mem->cv_adj_mem;

  /* Check which */
  if ( which >= ca_mem->ca_nbckpbs ) {
    cvProcessError(cv_mem, CVDLS_ILL_INPUT, "CVSDENSE", "CVDenseB", MSGD_BAD_WHICH);
    return(CVDLS_ILL_INPUT);
  }

  /* Find the CVodeBMem entry in the linked list corresponding to which */
  cvB_mem = ca_mem->cvB_mem;
  while (cvB_mem != NULL) {
    if ( which == cvB_mem->cv_index ) break;
    cvB_mem = cvB_mem->cv_next;
  }

  cvodeB_mem = (void *) (cvB_mem->cv_mem);

  /* Get memory for CVDlsMemRecB */
  cvdlsB_mem = (CVDlsMemB) malloc(sizeof(struct CVDlsMemRecB));
  if (cvdlsB_mem == NULL) {
    cvProcessError(cv_mem, CVDLS_MEM_FAIL, "CVSDENSE", "CVDenseB", MSGD_MEM_FAIL);
    return(CVDLS_MEM_FAIL);
  }

  /* set matrix type */
  cvdlsB_mem->d_typeB = SUNDIALS_DENSE;

  /* initialize Jacobian function */
  cvdlsB_mem->d_djacB = NULL;

  /* attach lmemB and lfreeB */
  cvB_mem->cv_lmem = cvdlsB_mem;
  cvB_mem->cv_lfree = cvDenseFreeB;

  flag = CVDense(cvodeB_mem, nB);

  if (flag != CVDLS_SUCCESS) {
    free(cvdlsB_mem);
    cvdlsB_mem = NULL;
  }

  return(flag);
}

/*
 * cvDenseFreeB frees the memory associated with the CVSDENSE linear
 * solver for backward integration.
 */

static int cvDenseFreeB(CVodeBMem cvB_mem)
{
  CVDlsMemB cvdlsB_mem;

  cvdlsB_mem = (CVDlsMemB) (cvB_mem->cv_lmem);

  free(cvdlsB_mem);

  return(0);
}

