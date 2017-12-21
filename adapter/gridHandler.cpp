#include "gridHandler.h"

#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include <vtkIdList.h>
#include <vtkFieldData.h>

gridHandler::gridHandler()
{
	this->grid = vtkUnstructuredGrid::New();
	vtkPoints *points =
		vtkPoints::New();
	this->grid->SetPoints(points);
	vtkIntArray *feapIds =
		vtkIntArray::New();
	feapIds->SetName(pointIdName);
	feapIds->SetNumberOfComponents(1);
	vtkIntArray *feapCellIds =
		vtkIntArray::New();
	feapCellIds->SetName(cellIdName);
	feapCellIds->SetNumberOfComponents(2);
	this->grid->GetPointData()->AddArray(feapIds);
	this->grid->GetCellData()->AddArray(feapCellIds);


	vtkDoubleArray *test =
		vtkDoubleArray::New();

	test->SetNumberOfComponents(1);
	test->SetName("Timetest");
	test->InsertNextValue(10.2);
	this->grid->GetFieldData()->AddArray(test);



}

gridHandler::~gridHandler()
{
}

vtkUnstructuredGrid * gridHandler::getGrid()
{
	return this->grid;
}

void gridHandler::addPoint(const int & FeapId,const double & x,const double & y,const double & z)
{
	auto search = this->FeapToParvMapPoints.find(FeapId);
	if (search != this->FeapToParvMapPoints.end()) {

	}
	else {
		this->grid->GetPoints()->InsertNextPoint(x, y, z);
		vtkIdType parvnum = this->grid->GetPoints()->GetNumberOfPoints();
		this->FeapToParvMapPoints[FeapId] = parvnum-1;
		this->grid->GetPointData()->GetArray(pointIdName)->InsertComponent(parvnum-1, 0, FeapId);
	}
		
}

void gridHandler::addCell(int FeapCellNumber, int FeapCellSubNumber, int * pointIds, int numpoints, int vtkNumber)
{


	auto vecSize = this->FeapToParvMapCells[FeapCellNumber].size();

	while (vecSize < FeapCellSubNumber) {
		this->FeapToParvMapCells[FeapCellNumber].emplace_back(-1);
		++vecSize;
	}

	if (this->FeapToParvMapCells[FeapCellNumber][FeapCellSubNumber - 1] == -1) {

		vtkIdList *list =
			vtkIdList::New();
		for (auto i = 0; i < numpoints; ++i)
		{
			list->InsertNextId(this->FeapToParvMapPoints[pointIds[i]]);
		}
		this->grid->InsertNextCell(vtkNumber, list);
		int numcells = this->grid->GetNumberOfCells();
		this->grid->GetCellData()->GetArray(cellIdName)->InsertComponent(numcells - 1, 0, FeapCellNumber);
		this->grid->GetCellData()->GetArray(cellIdName)->InsertComponent(numcells - 1, 1, FeapCellSubNumber);
	}
}

bool gridHandler::hasField(const char * name, const DataType &type)
{

	switch (type)
	{
	case DataType::PointData:
		if (this->grid->GetPointData()->GetArray(name)) {
			return true;
		}
		break;
	case DataType::CellData:
		if (this->grid->GetCellData()->GetArray(name)) {
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}


