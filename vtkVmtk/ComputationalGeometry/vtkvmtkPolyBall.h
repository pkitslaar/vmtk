/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkPolyBall.h,v $
Language:  C++
Date:      $Date: 2006/04/06 16:46:43 $
Version:   $Revision: 1.3 $

  Copyright (c) Luca Antiga, David Steinman. All rights reserved.
  See LICENCE file for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm 
  for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
  // .NAME vtkvmtkPolyBall - 
  // .SECTION Description
  // ..

#ifndef __vtkvmtkPolyBall_h
#define __vtkvmtkPolyBall_h

#include "vtkImplicitFunction.h"
#include "vtkPolyData.h"
//#include "vtkvmtkComputationalGeometryWin32Header.h"
#include "vtkvmtkWin32Header.h"

class VTK_VMTK_COMPUTATIONAL_GEOMETRY_EXPORT vtkvmtkPolyBall : public vtkImplicitFunction
{
  public:
  vtkTypeRevisionMacro(vtkvmtkPolyBall,vtkImplicitFunction);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkvmtkPolyBall *New();

  // Description
  // Evaluate polyball.
  double EvaluateFunction(double x[3]);
  double EvaluateFunction(double x, double y, double z)
  {return this->vtkImplicitFunction::EvaluateFunction(x, y, z); } ;

  // Description
  // Evaluate polyball gradient.
  void EvaluateGradient(double x[3], double n[3]);

  // Description:
  // Set / get input poly data.
  vtkSetObjectMacro(Input,vtkPolyData);
  vtkGetObjectMacro(Input,vtkPolyData);

  // Description:
  // Set / get poly ball radius array name.
  vtkSetStringMacro(PolyBallRadiusArrayName);
  vtkGetStringMacro(PolyBallRadiusArrayName);

  // Description:
  // Get the id of the last nearest poly ball center.
  vtkGetMacro(LastPolyBallCenterId,vtkIdType);

  protected:
  vtkvmtkPolyBall();
  ~vtkvmtkPolyBall();

  vtkPolyData* Input;
  char* PolyBallRadiusArrayName;
  vtkIdType LastPolyBallCenterId;

  private:
  vtkvmtkPolyBall(const vtkvmtkPolyBall&);  // Not implemented.
  void operator=(const vtkvmtkPolyBall&);  // Not implemented.
};

#endif


