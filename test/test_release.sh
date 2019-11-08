#!/bin/bash
# ------------------------------------------------------------------------------
# Programmer(s): David J. Gardner @ LLNL
# ------------------------------------------------------------------------------
# SUNDIALS Copyright Start
# Copyright (c) 2002-2019, Lawrence Livermore National Security
# and Southern Methodist University.
# All rights reserved.
#
# See the top-level LICENSE and NOTICE files for details.
#
# SPDX-License-Identifier: BSD-3-Clause
# SUNDIALS Copyright End
# ------------------------------------------------------------------------------
# SUNDIALS release testing driver script
#
# Usage: ./test_release.sh <num build threads>
#
# Inputs (all optional):
#   <num build threads> = number of threads in parallel build
# ------------------------------------------------------------------------------

# Number of build threads for parallel build (default 1)
bt=1
if [ "$#" -ge 1 ]; then
    bt=$1
fi

# ==============================================================================
# Test SUNDIALS tarball with TPLs and DEVTESTS ON
# ==============================================================================

# run tests
./suntest_tarscript.sh sundials all both each ON DEV $bt

# check return flag
if [ $? -ne 0 ]; then
    echo "FAILED: ./suntest_tarscript.sh sundials all both each ON DEV $bt" | tee -a suntest.log
    exit 1
fi

# ==============================================================================
# Test with xSDK options with TPLs and DEVTESTS ON
# ==============================================================================

realtype=( "single" "double" "extended" )
indexsize=( "32" "64" )

for rt in "${realtype[@]}"; do
    for is in "${indexsize[@]}"; do

        # print test label for Jenkins section collapsing
        echo "TEST: ./suntest_xsdk.sh $rt $is both ON DEV $bt"

        # run tests using xSDK CMake options
        ./suntest_xsdk.sh $rt $is both ON DEV $bt

        # check return flag
        if [ $? -ne 0 ]; then
            echo "FAILED: ./suntest_xsdk.sh $rt $is both ON DEV $bt" | tee -a suntest.log
            exit 1
        else
            echo "PASSED"
        fi

    done
done
