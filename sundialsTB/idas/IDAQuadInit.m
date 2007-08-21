function IDAQuadInit(fctQ, yQ0, options)
%IDAQuadInit allocates and initializes memory for quadrature integration.
%
%   Usage: IDAQuadInit ( QFUN, YQ0 [, OPTIONS ] ) 
%
%   QFUN     is a function defining the righ-hand sides of the quadrature
%            ODEs yQ' = fQ(t,y).
%   YQ0      is the initial conditions vector yQ(t0).
%   OPTIONS  is an (optional) set of QUAD options, created with
%            the IDASetQuadOptions function. 
%
%   See also: IDASetQuadOptions, IDAQuadRhsFn 

% Radu Serban <radu@llnl.gov>
% Copyright (c) 2007, The Regents of the University of California.
% $Revision: 1.1 $Date: 2007/05/11 18:51:32 $

mode = 2;

if nargin < 2
  error('Too few input arguments');
end

if nargin < 3
  options = [];
end

idm(mode, fctQ, yQ0, options);
