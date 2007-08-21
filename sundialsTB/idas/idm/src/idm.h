/*
 * -----------------------------------------------------------------
 * $Revision: 1.8 $
 * $Date: 2007-08-21 17:38:45 $
 * -----------------------------------------------------------------
 * Programmer: Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2005, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials-x.y.z/src/idas/LICENSE.
 * -----------------------------------------------------------------
 * Header file for the IDAS Matlab interface.
 * -----------------------------------------------------------------
 */

#ifndef _IDM_H
#define _IDM_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif
  
#include <string.h>
#include <stdlib.h>
#include "mex.h"
#include <idas/idas.h>
#include <idas/idas_dense.h>
#include <idas/idas_band.h>
#include <idas/idas_spgmr.h>
#include <idas/idas_spbcgs.h>
#include <idas/idas_sptfqmr.h>
#include <idas/idas_bbdpre.h>

/*
 * ---------------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------------
 */

/* Tolerance types */

enum {IDA_SS, IDA_SV, IDA_EE};

/* Linear solver types */

enum {LS_DENSE, LS_BAND, LS_SPGMR, LS_SPBCG, LS_SPTFQMR};

/* Preconditioner modules */

enum {PM_NONE, PM_BBDPRE};

/*
 * ---------------------------------------------------------------------------------
 * Types for global data structures
 * ---------------------------------------------------------------------------------
 */


typedef struct idmPbData_ {


  int n;             /* problem dimension */
  N_Vector YY;       /* solution vector */
  N_Vector YP;       /* derivative of solution vector */

  booleantype Quadr; /* integrate quadratures? */
  int nq;            /* number of quadratures */
  N_Vector YQ;       /* quadratures vector */

  booleantype Fsa;   /* integrate sensitivities? */
  int ns;            /* number of sensitivities */
  N_Vector *YYS;     /* sensitivity vectors */
  N_Vector *YPS;     /* derivatives of sensitivity vectors */

  booleantype RootSet;  /* rootfinding active? */
  int ng;               /* number of root functions */

  booleantype TstopSet; /* tstop active? */

  int LS;            /* linear solver type */
  int PM;            /* preconditioner module */

  booleantype Mon;   /* monitoring? */

  /* Matlab functions and data associated with this problem */

  mxArray *RESfct;
  mxArray *QUADfct;

  mxArray *JACfct;

  mxArray *PSETfct;
  mxArray *PSOLfct;

  mxArray *GLOCfct;
  mxArray *GCOMfct;
    
  mxArray *Gfct;
    
  mxArray *SRESfct;

  mxArray *MONfct;
  mxArray *MONdata;

  /* Pointer to the global Matlab user data */

  mxArray *mtlb_data;

  /* Information for backward problems only */

  struct idmPbData_ *fwd;
  int index;               /* index of this problem */
  struct idmPbData_ *next; /* pointer to next problem in linked list */

} *idmPbData;


typedef struct idmInterfaceData_ {

  void *ida_mem;         /* IDAS solver memory */

  booleantype asa;       /* Perform ASA? */
  int Nd;                /* number of data points */
  int Nc;                /* number of check points */

  struct idmPbData_ *fwdPb;
  struct idmPbData_ *bckPb;

  int NbckPb;            /* Number of backward problems in the linked list bckPb */

} *idmInterfaceData;



/*
 * ---------------------------------------------------------------------------------
 * Error handler function
 * ---------------------------------------------------------------------------------
 */

void idmErrHandler(int error_code, 
                   const char *module, const char *function, 
                   char *msg, void *f_data);


/*
 * ---------------------------------------------------------------------------------
 * Wrapper functions
 * ---------------------------------------------------------------------------------
 */

int mxW_IDARes(realtype tt, N_Vector yy, N_Vector yp,
               N_Vector rr, void *res_data);

int mxW_IDAGfct(realtype t, N_Vector y, N_Vector yp,
                realtype *gout, void *g_data);

int mxW_IDAQuadFct(realtype tres, N_Vector yy, N_Vector yp,
                   N_Vector ypQ,
                   void *rdataQ);

int mxW_IDASensRes(int Nsens, realtype tres, 
                   N_Vector yy, N_Vector yp, N_Vector resval,
                   N_Vector *yyS, N_Vector *ypS, N_Vector *resvalS,
                   void *rdataS,
                   N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

int mxW_IDADenseJac(int Neq, 
                    realtype tt, realtype c_j, 
                    N_Vector yy, N_Vector yp, N_Vector rr,
                    DlsMat Jac, void *jac_data, 
                    N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

int mxW_IDABandJac(int Neq, int mupper, int mlower, 
                   realtype tt, realtype c_j, 
                   N_Vector yy, N_Vector yp, N_Vector rr, 
                   DlsMat Jac, void *jac_data,
                   N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);

int mxW_IDASpilsJac(realtype tt,
                    N_Vector yy, N_Vector yp, N_Vector rr,
                    N_Vector v, N_Vector Jv,
                    realtype c_j, void *jac_data,
                    N_Vector tmp1, N_Vector tmp2);
int mxW_IDASpilsPset(realtype tt,
                     N_Vector yy, N_Vector yp, N_Vector rr,
                     realtype c_j, void *prec_data,
                     N_Vector tmp1, N_Vector tmp2,
                     N_Vector tmp3);
int mxW_IDASpilsPsol(realtype tt,
                     N_Vector yy, N_Vector yp, N_Vector rr,
                     N_Vector rvec, N_Vector zvec,
                     realtype c_j, realtype delta, void *prec_data,
                     N_Vector tmp);

int mxW_IDABBDgloc(int Nlocal, realtype tt,
                   N_Vector yy, N_Vector yp, N_Vector gval,
                   void *res_data);
int mxW_IDABBDgcom(int Nlocal, realtype tt,
                   N_Vector yy, N_Vector yp,
                   void *res_data);

void mxW_IDAMonitor(int call, double t, 
                    N_Vector yy,
                    N_Vector yQ, 
                    N_Vector *yyS,
                    idmPbData fwdPb);

int mxW_IDAResB(realtype tt, 
                N_Vector yy, N_Vector yp,
                N_Vector yyB, N_Vector ypB, N_Vector rrB,
                void *rdataB);
int mxW_IDAResBS(realtype tt, 
                 N_Vector yy, N_Vector yp,
                 N_Vector *yyS, N_Vector *ypS,
                 N_Vector yyB, N_Vector ypB, N_Vector rrB,
                 void *rdataB);

int mxW_IDAQuadFctB(realtype tt, 
                    N_Vector yy, N_Vector yp, 
                    N_Vector yyB, N_Vector ypB,
                    N_Vector ypQB,
                    void *rdataQB);
int mxW_IDAQuadFctBS(realtype t, 
                     N_Vector yy, N_Vector yp,
                     N_Vector *yyS, N_Vector *ypS,
                     N_Vector yyB, N_Vector ypB,
                     N_Vector ypQB,
                     void *user_dataB);

int mxW_IDADenseJacB(int NeqB, 
                     realtype tt, realtype c_jB,
                     N_Vector yy, N_Vector yp,
                     N_Vector yyB, N_Vector ypB, N_Vector rrB,
                     DlsMat JacB, void *jac_dataB, 
                     N_Vector tmp1B, N_Vector tmp2B, N_Vector tmp3B);

int mxW_IDABandJacB(int NeqB, int mupperB, int mlowerB, 
                    realtype tt, realtype c_jB, 
                    N_Vector yy, N_Vector yp,
                    N_Vector yyB, N_Vector ypB, N_Vector rrB,
                    DlsMat JacB, void *jac_dataB,
                    N_Vector tmp1B, N_Vector tmp2B, N_Vector tmp3B);

int mxW_IDASpilsJacB(realtype t,
                     N_Vector yy, N_Vector yp,
                     N_Vector yyB, N_Vector ypB, N_Vector rrB,
                     N_Vector vB, N_Vector JvB, 
                     realtype c_jB, void *jac_dataB, 
                     N_Vector tmp1B, N_Vector tmp2B);
int mxW_IDASpilsPsetB(realtype tt, 
                      N_Vector yy, N_Vector yp,
                      N_Vector yyB, N_Vector ypB, N_Vector rrB, 
                      realtype c_jB, void *prec_dataB,
                      N_Vector tmp1B, N_Vector tmp2B, 
                      N_Vector tmp3B);
int mxW_IDASpilsPsolB(realtype tt, 
                      N_Vector yy, N_Vector yp,
                      N_Vector yyB, N_Vector ypB, N_Vector rrB, 
                      N_Vector rvecB, N_Vector zvecB,
                      realtype c_jB, realtype deltaB,
                      void *prec_dataB, N_Vector tmpB);

int mxW_IDABBDglocB(int NlocalB, realtype tt,
                    N_Vector yy, N_Vector yp, 
                    N_Vector yyB, N_Vector ypB, N_Vector gvalB,
                    void *res_dataB);
int mxW_IDABBDgcomB(int NlocalB, realtype tt,
                    N_Vector yy, N_Vector yp,
                    N_Vector yyB, N_Vector ypB,
                    void *res_dataB);

void mxW_IDAMonitorB(int call, int idxB, double tB, 
                     N_Vector yyB,
                     N_Vector yQB,
                     idmPbData bckPb);

/*
 * ---------------------------------------------------------------------------------
 * Option handling functions
 * ---------------------------------------------------------------------------------
 */

void get_IntgrOptions(const mxArray *options, idmPbData thisPb, booleantype fwd,
                      int *maxord,
                      long int *mxsteps,
                      int *itol, realtype *reltol, double *Sabstol, double **Vabstol,
                      double *hin, double *hmax,
                      double *tstop,
                      booleantype *suppress,
                      double **id, double **cnstr,
                      booleantype *res_s);

void get_LinSolvOptions(const mxArray *options, idmPbData thisPb, booleantype fwd,
                        int *mupper, int *mlower,
                        int *mudq, int *mldq, double *dqrely,
                        int *gstype, int *maxl);

void get_QuadOptions(const mxArray *options, idmPbData thisPb, booleantype fwd,
                     int Nq, booleantype *rhs_s,
                     booleantype *errconQ,
                     int *itolQ, double *reltolQ, double *SabstolQ, double **VabstolQ);

void get_FSAOptions(const mxArray *options, idmPbData thisPb,
                    int *ism,
                    char **pfield_name, int **plist, double **pbar,
                    int *dqtype, double *rho,
                    booleantype *errconS, int *itolS, double *reltolS, 
                    double **SabstolS, double **VabstolS);

#ifdef __cplusplus
}
#endif

#endif
