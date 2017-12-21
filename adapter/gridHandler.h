
#include <vtkUnstructuredGrid.h>


#include <map>
#include <vector>
#include <vtkAbstractArray.h>
#include <vtkIntArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>


static char pointIdName[12] = "Node Number";
static char cellIdName[15]  = "Element Number";

class gridHandler {
public:
	gridHandler();
	~gridHandler();
	vtkUnstructuredGrid *getGrid();

	void addPoint(const int &FeapId,const double &x,const double &y,const double &z);

	template<typename T>
	void CompleteFeapPointArray(const T *data,const int &num_comp,
		const int &numPoints,const char *name);
	template<typename T>
	void SetPointData(const T *data, const int &feapId, const int &num_comp, const char *name);
	template<typename T>
	void SetFieldData(const T *data, const int &num_comp, const char *name);

	void addCell(int FeapCellNumber, int FeapCellSubNumber , int *pointIds, int numpoints, int vtkNumber);

private:

	enum DataType {PointData, CellData};

	template <typename T>
	void createField(const char *name, const int &numComp);

	bool hasField(const char *name,const DataType &type);

	vtkUnstructuredGrid *grid;

	std::map<int, vtkIdType> FeapToParvMapPoints;			// Maps Feap Coor Id to Paraview Node Id
	std::map<int, std::vector<int>> FeapToParvMapCells; // Maps Feap Element Id to Paraview CellIds

};

template<typename T>
inline void gridHandler::CompleteFeapPointArray(const T * data, const int & num_comp, 
	const int & numPoints, const char *name)
{


	if (!this->hasField(name, DataType::PointData)) {

		this->createField<T>(name, num_comp);

	}

	if (this->hasField(name, DataType::PointData)) {

		vtkDataArray *arr;
		int numnp = this->grid->GetNumberOfPoints();
		arr = this->grid->GetPointData()->GetArray(name);
		vtkAbstractArray *test = this->grid->GetPointData()->GetArray(name);
		vtkDataArray *feapIds =
			vtkDataArray::SafeDownCast(this->grid->GetPointData()->GetArray(pointIdName));

		for (auto i = 0; i < numnp; ++i) {
			int feapId = feapIds->GetComponent(i, 0);
			for (auto j = 0; j < num_comp; ++j) {
				int pos = (feapId - 1)*num_comp;
				this->grid->GetPointData()->GetArray(name)->SetComponent(i, j, data[pos+j]);
				//arr->SetComponent(i, j, data[pos]);
			}
		}
	}
}

template<typename T>
inline void gridHandler::SetPointData(const T * data, const int & feapId
	, const int & num_comp, const char * name)
{
	if (!this->hasField(name, DataType::PointData)) {
		this->createField<T>(name, num_comp);
	}

	auto search = this->FeapToParvMapPoints.find(feapId);
	if (!search == this->FeapToParvMapPoints.end()) {
		vtkIdType vtkPointNumber = search->second;
		vtkDataArray *arr = this->grid->GetPointData()->GetArray(name);
		for (auto i = 0; i < num_comp; ++i) {
			arr->SetComponent(vtkPointNumber, i, *data[i]);
		}
	}
}

template<typename T>
inline void gridHandler::SetFieldData(const T * data, const int & num_comp, const char * name)
{
	vtkDataArray *arr;
	if (!this->grid->GetFieldData()->GetArray(name)) {
		if (std::is_same<T, int>()) {
			vtkIntArray *toadd = vtkIntArray::New();
			toadd->SetName(name);
			toadd->SetNumberOfComponents(num_comp);
			for (auto i = 0; i < num_comp; ++i)
				toadd->InsertNextValue(0);
			this->grid->GetFieldData()->AddArray(toadd);
		}
		else if (std::is_same<T, double>()) {
			vtkDoubleArray *toadd = vtkDoubleArray::New();

			toadd->SetName(name);
			toadd->SetNumberOfComponents(num_comp);
			for (auto i = 0; i < num_comp; ++i)
				toadd->InsertNextValue(0);
			this->grid->GetFieldData()->AddArray(toadd);
		}
	}
	arr = this->grid->GetFieldData()->GetArray(name);
	for (auto i = 0; i < num_comp; ++i) {
		arr->SetComponent(0, i, data[i]);
	}
}

template<typename T>
inline void gridHandler::createField(const char * name, const int &numComp)
{
	if (!this->hasField(name, DataType::PointData)) {

		int numnp = this->grid->GetNumberOfPoints();
		if (std::is_same<T, int>()) {

			vtkIntArray* toAdd =
				vtkIntArray::New();
			toAdd->SetNumberOfComponents(numComp);
			//toAdd->Allocate(numnp*numComp);
			for (auto i = 0; i < numnp; ++i) {
				for (auto j = 0; j < numComp; ++j) {
					toAdd->InsertNextValue(0);
				}
			}
			toAdd->SetName(name);
			this->grid->GetPointData()->AddArray(toAdd);
		}
		else if (std::is_same<T, double>()) {
			vtkDoubleArray *toAdd =
				vtkDoubleArray::New();
			toAdd->SetNumberOfComponents(numComp);
			//toAdd->Allocate(numnp*numComp);
			for (auto i = 0; i < numnp; ++i) {
				for (auto j = 0; j < numComp; ++j) {
					toAdd->InsertNextValue(0);
				}
			}
			toAdd->SetName(name);
			this->grid->GetPointData()->AddArray(toAdd);
		}
	}
}
