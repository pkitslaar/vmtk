#!/usr/bin/env python

## Program:   VMTK
## Module:    $RCSfile: vmtkbranchclipper.py,v $
## Language:  Python
## Date:      $Date: 2006/02/23 09:31:39 $
## Version:   $Revision: 1.1 $

##   Copyright (c) Luca Antiga, David Steinman. All rights reserved.
##   See LICENCE file for details.

##      This software is distributed WITHOUT ANY WARRANTY; without even 
##      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
##      PURPOSE.  See the above copyright notices for more information.


import vtk
import vtkvmtk
import sys

import pypes

vmtkbranchclipper = 'vmtkBranchClipper'

class vmtkBranchClipper(pypes.pypeScript):

    def __init__(self):

        pypes.pypeScript.__init__(self)
        
        self.Surface = None
        self.Centerlines = None

        self.RadiusArrayName = ''
        self.CutoffRadiusFactor = 1E16
	
        self.BlankingArrayName = ''
        self.GroupIdsArrayName = ''

        self.GroupIds = []
        self.InsideOut = 0

        self.SetScriptName('vmtkbranchclipper')
        self.SetInputMembers([
            ['Surface','i','vtkPolyData',1,'','vmtksurfacereader'],
            ['Centerlines','centerlines','vtkPolyData',1,'','vmtksurfacereader'],
            ['GroupIdsArrayName','groupidsarray','str',1],
            ['GroupIds','groupids','int',-1],
            ['InsideOut','insideout','int',1],
            ['RadiusArrayName','radiusarray','str',1],
				    ['BlankingArrayName','blankingarray','str',1],
						['CutoffRadiusFactor','cutoffradiusfactor','float',1]
            ])
        self.SetOutputMembers([
            ['Surface','o','vtkPolyData',1,'','vmtksurfacewriter'],
            ['Centerlines','ocenterlines','vtkPolyData',1,'','vmtksurfacewriter']
            ])

    def Execute(self):

        if not self.Surface:
            self.PrintError('Error: No input surface.')

        if not self.Centerlines:
            self.PrintError('Error: No input centerlines.')

        clipper = vtkvmtk.vtkvmtkPolyDataCenterlineGroupsClipper()
        clipper.SetInput(self.Surface)
        clipper.SetCenterlines(self.Centerlines)
        clipper.SetCenterlineGroupIdsArrayName(self.GroupIdsArrayName)
        clipper.SetGroupIdsArrayName(self.GroupIdsArrayName)
        clipper.SetCenterlineRadiusArrayName(self.RadiusArrayName)
        clipper.SetBlankingArrayName(self.BlankingArrayName)
        clipper.SetCutoffRadiusFactor(self.CutoffRadiusFactor)
        if self.GroupIds:
            groupIds = vtk.vtkIdList()
            for groupId in self.GroupIds:
                groupIds.InsertNextId(groupId)
            clipper.SetCenterlineGroupIds(groupIds)
            clipper.ClipAllCenterlineGroupIdsOff()
        else:
            clipper.ClipAllCenterlineGroupIdsOn()
        if not self.InsideOut:
            clipper.GenerateClippedOutputOff()
        else:
            clipper.GenerateClippedOutputOn()
        clipper.Update()

        if not self.InsideOut:
            self.Surface = clipper.GetOutput()
        else:
            self.Surface = clipper.GetClippedOutput()

        if self.Surface:
            if self.Surface.GetSource():
                self.Surface.GetSource().UnRegisterAllOutputs()

        if self.Centerlines.GetSource():
            self.Centerlines.GetSource().UnRegisterAllOutputs()


if __name__=='__main__':

    main = pypes.pypeMain()
    main.Arguments = sys.argv
    main.Execute()