/*=========================================================================

  Program:   VMTK
  Module:    $RCSfile: vtkvmtkUnstructuredGridFELaplaceAssembler.cxx,v $
  Language:  C++
  Date:      $Date: 2005/11/15 17:39:25 $
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

#include "vtkvmtkUnstructuredGridFELaplaceAssembler.h"
#include "vtkvmtkGaussQuadrature.h"
#include "vtkvmtkFEShapeFunctions.h"
#include "vtkCell.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkvmtkUnstructuredGridFELaplaceAssembler, "$Revision: 1.3 $");
vtkStandardNewMacro(vtkvmtkUnstructuredGridFELaplaceAssembler);

vtkvmtkUnstructuredGridFELaplaceAssembler::vtkvmtkUnstructuredGridFELaplaceAssembler()
{
}

vtkvmtkUnstructuredGridFELaplaceAssembler::~vtkvmtkUnstructuredGridFELaplaceAssembler()
{
}

void vtkvmtkUnstructuredGridFELaplaceAssembler::Build()
{
  int numberOfVariables = 1;
  this->Initialize(numberOfVariables);

  vtkvmtkGaussQuadrature* gaussQuadrature = vtkvmtkGaussQuadrature::New();
  gaussQuadrature->SetOrder(this->QuadratureOrder);

  vtkvmtkFEShapeFunctions* feShapeFunctions = vtkvmtkFEShapeFunctions::New();

  int dimension = 3;

  int numberOfCells = this->DataSet->GetNumberOfCells();
  int k;
  for (k=0; k<numberOfCells; k++)
    {
    vtkCell* cell = this->DataSet->GetCell(k);
    if (cell->GetCellDimension() != dimension)
      {
      continue;
      } 
    gaussQuadrature->Initialize(cell->GetCellType());
    feShapeFunctions->Initialize(cell,gaussQuadrature->GetQuadraturePoints());
    int numberOfQuadraturePoints = gaussQuadrature->GetNumberOfQuadraturePoints();
    double quadraturePCoords[3], quadraturePoint[3];
    int numberOfCellPoints = cell->GetNumberOfPoints();
    double* weights = new double[numberOfCellPoints];
    double* derivs = new double[dimension*numberOfCellPoints];
    int i, j;
    int q;
    for (q=0; q<numberOfQuadraturePoints; q++)
      {
      gaussQuadrature->GetQuadraturePoint(q,quadraturePCoords);
      double quadratureWeight = gaussQuadrature->GetQuadratureWeight(q);
      double jacobian = feShapeFunctions->GetJacobian(q);
      double dphii[3], dphij[3];
      for (i=0; i<numberOfCellPoints; i++)
        {
        vtkIdType iId = cell->GetPointId(i);
        vtkvmtkSparseMatrixRow* row = this->Matrix->GetRow(iId);
        feShapeFunctions->GetDPhi(q,i,dphii);
        for (j=0; j<numberOfCellPoints; j++)
          {
          vtkIdType jId = cell->GetPointId(j);
          feShapeFunctions->GetDPhi(q,j,dphij);
          double gradphii_gradphij = vtkMath::Dot(dphii,dphij);
          double value = jacobian * quadratureWeight * gradphii_gradphij;
          double currentValue = this->Matrix->GetElement(iId,jId);
          this->Matrix->SetElement(iId,jId,currentValue+value);
          }
        }
      }
    delete[] weights;
    delete[] derivs;
    }

  gaussQuadrature->Delete();
  feShapeFunctions->Delete();
}
