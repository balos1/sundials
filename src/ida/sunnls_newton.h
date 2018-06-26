#ifndef _SUN_NLS_H
#define _SUN_NLS_H

#include "sundials/sundials_nvector.h"
#include "sundials/sundials_types.h"

/* NLS return codes */
#define SUN_NLS_MEM_FAIL -10
#define SUN_NLS_VECTOROP_ERR -11

#define SUN_NLS_SUCCESS    0

#define SUN_NLS_SYS_RECVR +1
#define SUN_NLS_SYS_FAIL  -1

#define SUN_NLS_LSETUP_RECVR +2
#define SUN_NLS_LSETUP_FAIL  -2

#define SUN_NLS_LSOLVE_RECVR +3
#define SUN_NLS_LSOLVE_FAIL  -3

#define SUN_NLS_CONV_RECVR    +1
#define SUN_NLS_CONV_CONTINUE +2

/* NLS functions that the integrator will provide */
typedef int (*SUNNonlinSolSysFn)(N_Vector y, N_Vector yp, N_Vector Fval, void* mem);
typedef int (*SUNNonlinSolLSetupFn)(N_Vector y, N_Vector yp, N_Vector Fval, void* mem);
typedef int (*SUNNonlinSolLSolveFn)(N_Vector y, N_Vector yp, N_Vector delta, void* mem);
typedef int (*SUNNonlinSolCTestFn)(int m, realtype delnrm, realtype tol, void* mem);

/* NLS exported functions */
int SUNNonlinSolSolve_Newton(N_Vector yy_predict, N_Vector yp_predict,
                             N_Vector yy, N_Vector yp,
                             N_Vector ee,
                             N_Vector ewt, realtype tol,
                             booleantype callSetup, void* mem);
int SUNNonlinSolInit_Newton(N_Vector tmpl);
int SUNNonlinSolFree_Newton();

int SUNNonlinSolSetAlphaFactor_Newton(realtype alpha);
int SUNNonlinSolSetMaxNonlinIters_Newton(int max);

int SUNNonlinSolSetSysFn(SUNNonlinSolSysFn SysFn);
int SUNNonlinSolSetLSetupFn(SUNNonlinSolLSetupFn SetupFn);
int SUNNonlinSolSetLSolveFn(SUNNonlinSolLSolveFn SolveFn);
int SUNNonlinSolSetConvTestFn(SUNNonlinSolCTestFn CTestFn);

int SUNNonlinSolGetNumIters_Newton(long int *numiters);

#endif
