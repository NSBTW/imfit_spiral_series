// Code for setting up ModelObject instances with data

#ifndef _SETUP_MODEL_OBJECT_H_
#define _SETUP_MODEL_OBJECT_H_

#include <vector>

#include "options_base.h"
#include "model_object.h"

using namespace std;

ModelObject* SetupModelObject( OptionsBase *options, vector<int> nColumnsRowsVector, 
					double *dataPixels, double *psfPixels=NULL, double *maskPixels=NULL, 
					double *errorPixels=NULL, double *psfOversampledPixels=NULL, 
					vector<int> xyOversamplePos=vector<int>() );


#endif  // _SETUP_MODEL_OBJECT_H_