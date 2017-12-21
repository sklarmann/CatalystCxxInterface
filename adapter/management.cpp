#include <management.h>

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

#include <vtkCPAdaptorAPI.h>
#include <vtkCPProcessor.h>
#include <vtkNew.h>
#include <vtkCPDataDescription.h>
#include <vtkCPInputDataDescription.h>

#include <vtkPoints.h>
#include <vtkCellArray.h>

#include <iostream>
#include <vtkLiveInsituLink.h>
#include <vtkSMProxyManager.h>
#include <vtkSMSessionProxyManager.h>
#include <vtkSMInputProperty.h>
#include <vtkAlgorithm.h>
#include <vtkObject.h>
#include <vtkPointData.h>
#include <vtkPVTrivialProducer.h>

#include <vtkCommunicationErrorCatcher.h>

#include <insitulink.h>
#include <thread>
#include <stdlib.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInformation.h>




managementClass::managementClass() {
	vtkSmartPointer<vtkUnstructuredGrid> grid =
		vtkSmartPointer<vtkUnstructuredGrid>::New();

	this->m_toplot =
		vtkSmartPointer<vtkMultiBlockDataSet>::New();

	
	this->coProcInit = false;
	//this->m_meshPresent = false;

	this->m_tstep = 0;
	this->m_time = 0.0;
	this->isRVE = false;
	//this->m_connected = false;


	

}


managementClass::~managementClass() {
	//this->writer->Delete();
}



bool managementClass::checkGrid(const int &main, const int &part)
{
	

	auto search = this->gridMapper.find(main);
	if (search != this->gridMapper.end()) {
		auto search2 = search->second.find(part);
		if (search2 != search->second.end())
			return true;
	}

	return false;
}

void managementClass::addGrid(const int & main, const int & part)
{
	std::string blockname;
	
	if (main == 0) {
		std::stringstream temp;
		temp << "Material " << part;
		
		blockname = temp.str();
	}


	if (!this->checkGrid(main, part)) {
		auto search = this->gridMapper.find(main);
		if (search != this->gridMapper.end()) {


			vtkMultiBlockDataSet *block =
				vtkMultiBlockDataSet::SafeDownCast(this->m_toplot->GetBlock(main));
			block->SetBlock(part, this->gridMapper[main][part].getGrid());
			block->GetMetaData(part)->Set(vtkCompositeDataSet::NAME(), blockname.c_str());
			

		}
		else {
			this->gridMapper[main][part];

			this->m_toplot->SetBlock(main, vtkMultiBlockDataSet::New());
			vtkMultiBlockDataSet *block =
				vtkMultiBlockDataSet::SafeDownCast(this->m_toplot->GetBlock(main));
			block->SetBlock(part, this->gridMapper[main][part].getGrid());
			block->GetMetaData(part)->Set(vtkCompositeDataSet::NAME(), blockname.c_str());

			if (main == 0) {
				std::stringstream temp;
				temp << "Hauptmodell";
				this->m_toplot->GetMetaData(main)->Set(vtkCompositeDataSet::NAME(), temp.str().c_str());
			}
		}

	}
}



void managementClass::initialize(char * sname)
{

	// File writing par
	std::string temp;
	temp = sname;

	std::replace(temp.begin(),temp.end(),'\\','/');
	//std::cout << temp;

	auto pos = temp.find_last_of('/');
	this->m_path = temp.substr(0, pos);
	this->m_baseFileName = temp.substr(pos + 1, temp.length());
	//pos = this->m_baseFileName.find_last_of('.');
	//this->m_baseFileName = this->m_baseFileName.substr(0, pos);
	
	this->m_baseFileName[0] = 'R';
	//std::cout << "\n" << this->m_path << "\n" << this->m_baseFileName << std::endl;

	this->fileWritten = false;
	this->writerStep = 0;
	this->writerTimeMap.clear();
	this->m_time = 0.0;

	this->m_toplot->ReleaseData();
	for (auto it : this->gridMapper) {
		it.second.clear();
	}
	this->gridMapper.clear();
}



void managementClass::CoProcess()
{
	if (!this->coProcInit) {
		//coprocessorinitializewithpython(sname, len);
		this->coProcInit = true;

		this->m_proc = vtkCPProcessor::New();
		this->m_proc->Initialize();
		this->m_link = vtkLiveInsituLink::New();
		this->m_link->SetInsituPort(22222);
		this->m_link->SetHostname("localhost");
		this->m_link->SetProcessType(vtkLiveInsituLink::INSITU);

		this->m_spxm = vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();


		this->m_px = this->m_spxm->NewProxy("sources", "PVTrivialProducer");
		this->m_spx = vtkSMSourceProxy::SafeDownCast(this->m_px);
		this->m_spxm->RegisterProxy("sources", this->m_spx);
		vtkObjectBase *obase = this->m_spx->GetClientSideObject();
		vtkPVTrivialProducer *prod =
			vtkPVTrivialProducer::SafeDownCast(obase);

		prod->SetOutput(this->m_toplot, this->m_time);


		this->m_link->Initialize(this->m_spxm);
		

	}

	this->Update();
}

void managementClass::Update()
{
	if (this->coProcInit) { 
		this->m_link->InsituUpdate(this->m_time, this->m_tstep);
		this->m_spx->UpdatePipeline(this->m_time);
		this->m_link->InsituPostProcess(this->m_time, this->m_tstep);
	}
}


void managementClass::finalizeCoProcessing()
{
	if (this->coProcInit) {
		this->m_link->DropLiveInsituConnection();
	}
}

void managementClass::isRVEfunc()
{
	this->isRVE = true;
}

void managementClass::TimeUpdate(double & time)
{
	++this->m_tstep;
	this->m_time = time;
	if (!this->isRVE) {
		if (this->fileWritten) {
			++this->writerStep;
			this->fileWritten = false;
		}
	}
}

void managementClass::writeFile()
{
	std::string wfilename;
	std::string folder;
	if(this->isRVE) this->pvdCollectionReader();
	
	std::stringstream temp, temp2;
	temp << this->m_path << "/parvout/"; 
	folder = temp.str();
	temp << this->m_baseFileName << this->writerStep << ".vtm";
	temp2 << this->m_baseFileName << this->writerStep << ".vtm";
	wfilename = temp.str();
	
	
	vtkDoubleArray *test =
		vtkDoubleArray::New();
	
	test->SetNumberOfComponents(1);
	test->SetName("Timetest2");
	test->InsertNextValue(10.2);
	this->m_toplot->GetBlock(0)->GetFieldData()->AddArray(test);
	
	vtkSmartPointer<vtkXMLMultiBlockDataWriter> pwriter =
		vtkSmartPointer<vtkXMLMultiBlockDataWriter>::New();
	pwriter->SetFileName(wfilename.c_str());
	pwriter->SetInputData(this->m_toplot);
	//this->writer->SetFileName(wfilename.c_str());
	//this->writer->SetInputData(this->m_toplot); 
	
	//////this->writer->SetInputData(prod);
	//this->writer->AddInputData(this->gridMapper[0][1].getGrid());
	////this->writer->SetPiece(this->m_tstep);
	////this->writer->SetWriteCollectionFile(true);
	//
	//this->writer->SetEncodeAppendedData(0);
	//this->writer->SetDataModeToAscii();
	//this->writer->Write();
	//
	pwriter->SetEncodeAppendedData(0);
	pwriter->SetDataModeToAppended();

	//pwriter->SetDataModeToAscii();
	pwriter->Write();
	
	this->writerTimeMap[this->m_time] = temp2.str();
	this->fileWritten = true;
	
	this->pvdCollectionWriter();
}

void managementClass::pvdCollectionWriter()
{
	std::stringstream fileName;
	fileName << this->m_path << "/parvout/";
	fileName << this->m_baseFileName  << ".pvd";

	std::ofstream file;
	file.open(fileName.str());

	std::string space;
	space = "  ";

	file << "<?xml version =\"1.0\"?>\n"
		<< "<VTKFile type=\"Collection\" version=\"0.1\">\n";

	file << space << "<Collection>\n";
	space = "    ";

	for (auto it : this->writerTimeMap) {
		file << space << "<DataSet timestep=\"" << it.first << "\" group=\"\" part=\"\"\n"
			<< space << "file=\"" << it.second << "\"/>\n";
	}


	space = "  ";
	file << space << "</Collection>\n</VTKFile>" << std::endl;

	file.close();
}

void managementClass::pvdCollectionReader()
{

	std::stringstream fileName;
	fileName << this->m_path << "/parvout/";
	fileName << this->m_baseFileName << ".pvd";


	std::ifstream file(fileName.str());
	if (file.good()) {
		//file.open(fileName.str());
		
		std::vector<std::string> fcon;
		std::string line;

		while (std::getline(file, line)) {
			fcon.push_back(line);
		}

		int start = 3;
		int end = static_cast<int>(fcon.size()) - 3;

		std::regex time("timestep=\"([^\"]*)\"");
		std::regex fname("file=\"([^\"]*)\"");
		std::smatch st, sf;
		while (start < end)
		{
			line = fcon[start];
			if (std::regex_search(line, st, time)) {
				double time = std::stod(st[1]);
				line = fcon[start + 1];
				std::regex_search(line, sf, fname);
				this->writerTimeMap[time] = sf[1];
				++this->writerStep;
			}
			
			//line = fcon[start + 1];
			//std::regex_match(line, sf, fname);
			//for (auto i = 0; i < sf.size(); ++i) {
			//	std::cout << i << ": " << sf[i];
			//}
			start += 2;
		}


		
	}
	file.close();
}

void managementClass::CoProcessTest()
{

	
}

vtkUnstructuredGrid * managementClass::getGrid(const int &main,const int &part)
{
	this->checkGrid(main, part);
	vtkMultiBlockDataSet *mainBlock =
		vtkMultiBlockDataSet::SafeDownCast(this->m_toplot->GetBlock(main));
	vtkUnstructuredGrid *grid =
		vtkUnstructuredGrid::SafeDownCast(mainBlock->GetBlock(part));
	return grid;
}

void managementClass::addPoint(const int & main, const int & part, const int & id, const double & x, const double & y, const double & z)
{
	
	if (!this->checkGrid(main, part))
		this->addGrid(main, part);
	this->gridMapper[main][part].addPoint(id, x, y, z);

	
}

void managementClass::addCell(const int & main, const int & part, int & FeapCellNumber, int & FeapSubCellNumber, int * FeapPoints, int * numpts, int * vtkNumber)
{
	if (this->checkGrid(main, part)) {
		this->gridMapper[main][part].addCell(FeapCellNumber, FeapSubCellNumber, FeapPoints, *numpts, *vtkNumber);
	}
	else {
		std::cout << "Error when trying to add cell to main " << main << ", part " << part
			<< ". First initialize the mesh by adding its points." << std::endl;
	}
}


void managementClass::addPoints(double* coor, int &numpoints, int &ndm)
{
	vtkUnstructuredGrid *grid 
		= vtkUnstructuredGrid::SafeDownCast(this->m_toplot->GetBlock(0));

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	int k = 0;
	for (auto i = 0; i < numpoints; ++i) {
		std::cout << k << " " << coor[k] << " " << coor[k + 1] << " " << coor[k + 2] << std::endl;
		if (ndm == 3) {
			points->InsertNextPoint(coor[k], coor[k + 1], coor[k + 2]);
		}
		else if (ndm == 2) {
			points->InsertNextPoint(coor[k], coor[k + 1], 0.0);
		}
		else if (ndm == 1) {
			points->InsertNextPoint(coor[k], 0.0, 0.0);
		}
		k = k + ndm;
	}

	grid->SetPoints(points);
}



