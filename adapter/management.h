
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkMultiBlockDataSet.h>

#include <vtkLiveInsituLink.h>
#include <vtkCPProcessor.h>

#include <gridHandler.h>
#include <map>
#include <vector>
#include <vtkSMProxy.h>
#include <vtkSMSourceProxy.h>

#include <vtkXMLMultiBlockDataWriter.h>

#include <vtkXMLPVDWriter.h>
#include <vtkXMLPVAnimationWriter.h>

class managementClass {
public:
	managementClass();
	~managementClass();

	//Deprecated
	

	void addPoints(double *coor, int &numpoints, int &ndm);

	void CoProcessTest();
	vtkUnstructuredGrid *getGrid(const int &main, const int &part);

	//

	void initialize(char *sname);
	void CoProcess();
	void finalizeCoProcessing();
	void isRVEfunc();

	void TimeUpdate(double &time);

	void writeFile();

	void addPoint(const int &main,const int &part, const int &id, const double &x, const double &y, const double &z);
	void addCell(const int &main, const int &part, int &FeapCellNumber, int &FeapSubCellNumber, int *FeapPoints, int *numpts, int *vtkNumber);

	template<typename T>
	void addCompleteFeapPointArray(const int &main, const int &part, const T *data, const int &num_comp, const int &numPoints, const char * name);
	template<typename T>
	void addCompleteFeapPointArrayToAll(const int &main, const T *data, const int &num_comp, const int &numPoints, const char *name);
	template<typename T>
	void setPointData(const int &main, const int &part, const int &feapId,
		const T *data, const int &num_comp, const char *name);

	template<typename T>
	void SetFieldData(const int &main, const T *data, const int &num_comp, const char *name);

	void Update();






private:
	bool checkGrid(const int &main,const int &part); 
	void addGrid(const int &main, const int &part);


	vtkSmartPointer<vtkMultiBlockDataSet> m_toplot;
	std::map<int, std::map<int, gridHandler>> gridMapper;

	//int m_numGrids;

	//bool m_connected;

	bool coProcInit;
	bool isRVE;
	//bool m_meshPresent;
	//bool m_doCoProcessing;
	//bool m_dataChanged;


	double m_time;
	int m_tstep, m_needCoProc;

	bool m_paraIsInit;
	vtkCPProcessor *m_proc;
	vtkLiveInsituLink *m_link;
	vtkSMSessionProxyManager *m_spxm;
	vtkSMProxy *m_px;
	vtkSMSourceProxy *m_spx;





	/// File Writer
	void pvdCollectionWriter();
	void pvdCollectionReader();
	std::string m_baseFileName, m_path;
	std::map<double,std::string> writerTimeMap;
	bool fileWritten;
	int  writerStep;


};

template<typename T>
inline void managementClass::addCompleteFeapPointArray(const int & main, const int & part, const T * data, const int & num_comp, const int & numPoints, const char * name)
{
	
	if (this->checkGrid(main, part)) {
		this->gridMapper[main][part].CompleteFeapPointArray<T>(
			data, num_comp, numPoints, name);
	}
	else {
		std::cout << "Error when trying to add complete data field " << name <<
			" to main " << main << ", part " << part << "."
			<< " Plot mesh does not exist!" << std::endl;
	}
	this->Update();
}

template<typename T>
inline void managementClass::addCompleteFeapPointArrayToAll(const int &main,
	const T * data, const int & num_comp, const int & numPoints, const char *name)
{
	auto it = this->gridMapper.find(main);
	if (it != this->gridMapper.end()) {
		for (auto &it2 : it->second) {
			it2.second.CompleteFeapPointArray<T>(data, num_comp, numPoints, name);
		}
	}
	this->Update();
}

template<typename T>
inline void managementClass::setPointData(const int & main, 
	const int & part, const int & feapId, const T * data, 
	const int & num_comp, const char * name)
{
	int tlen = name_len + 1;
	char *buff = new char[name_len + 1];
	for (auto i = 0; i < name_len; ++i) {
		buff[i] = name[i];
	}
	buff[name_len] = '\0';

	if (this->checkGrid(main, part)) {
		this->gridMapper[main][part].SetPointData<T>(
			data, feapId, num_comp, buff);
	}
	else {
		std::cout << "Error when trying to add complete data field " << buff <<
			" to main " << main << ", part " << part << "."
			<< " Plot mesh does not exist!" << std::endl;
	}
	delete buff;
	this->Update();
}

template<typename T>
inline void managementClass::SetFieldData(const int & main, const T * data, const int &num_comp, const char * name)
{
	auto it = this->gridMapper.find(main);
	if (it != this->gridMapper.end()) {
		for (auto &it2 : it->second) {
			it2.second.SetFieldData(data, num_comp, name);
		}
	}

}
