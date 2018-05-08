

#include <management.h>

#define EXPORTSTD 
#ifdef __SALFORD_
#define EXPORTSTD __stdcall
#endif 



static managementClass paraviewHandler;


extern "C" __declspec(dllexport) void EXPORTSTD initializeCoProcessing(
	char *inputFileName, int *nameLength) {

	char *buffer = new char[*nameLength + 1];
	for (auto i = 0; i < *nameLength; ++i)
		buffer[i] = inputFileName[i];

	buffer[*nameLength] = '\0';


	paraviewHandler.initialize(buffer);
	delete[] buffer;
}

extern "C" __declspec(dllexport) void EXPORTSTD CoProcess() {
	paraviewHandler.CoProcess();
}

extern "C" __declspec(dllexport) void EXPORTSTD isRVEfunc() {
	paraviewHandler.isRVEfunc();
}

extern "C" __declspec(dllexport) void EXPORTSTD finalizeCoProcessing() {
	paraviewHandler.finalizeCoProcessing();
}

extern "C" __declspec(dllexport) void EXPORTSTD addPoint(
	int *main, int*part, int *id, double *x, double *y, double *z) {
	paraviewHandler.addPoint(*main, *part, *id, *x, *y, *z);
}

extern "C" __declspec(dllexport) void EXPORTSTD addCell(
	int *main, int *part, int *FeapCellNumber, int *FeapSubCellNumber, int *FeapPoints, int *numpts, int *vtkNumber) {
	paraviewHandler.addCell(*main,*part,*FeapCellNumber,*FeapSubCellNumber,FeapPoints,numpts,vtkNumber);
}


extern "C" __declspec(dllexport) void EXPORTSTD addIntPointDataAll(
	int*main, int *data, int *num_comp, int *numpoints, char *name, int *namelen) {
	
	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i) {
		buff[i] = name[i];
	}

	buff[*namelen] = '\0';

	paraviewHandler.addCompleteFeapPointArrayToAll<int>(
		*main, data, *num_comp, *numpoints, buff);

	delete[] buff;
}

extern "C" __declspec(dllexport) void EXPORTSTD addDoublePointDataAll(
	int* main, double *data, int *num_comp, int *numpoints, char *name, int *namelen) {

	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i) {
		buff[i] = name[i];
	}

	buff[*namelen] = '\0';

	paraviewHandler.addCompleteFeapPointArrayToAll<double>(
		*main, data, *num_comp, *numpoints, buff);

	delete[] buff;
}


extern "C" __declspec(dllexport) void EXPORTSTD addIntPointDataPart(
	int*main, int* part, int *data, int *num_comp, int *numpoints, char *name, int *namelen) {

	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i) {
		buff[i] = name[i];
	}

	buff[*namelen] = '\0';

	paraviewHandler.addCompleteFeapPointArray<int>(
		*main, *part, data, *num_comp, *numpoints, buff
		);
	delete[] buff;
}

extern "C" __declspec(dllexport) void EXPORTSTD addDoublePointDataPart(
	int* main, int* part, double *data, int *num_comp, int *numpoints, char *name, int *namelen) {

	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i) {
		buff[i] = name[i];
	}

	buff[*namelen] = '\0';

	paraviewHandler.addCompleteFeapPointArray<double>(
		*main, *part, data, *num_comp, *numpoints, buff
		);


	delete[] buff;
}


extern "C" __declspec(dllexport) void EXPORTSTD writeFile() {
	paraviewHandler.writeFile();
}

extern "C" __declspec(dllexport) void EXPORTSTD timeUpdate(double *time) {
	paraviewHandler.TimeUpdate(*time);
}


extern "C" __declspec(dllexport) void EXPORTSTD SetIntFieldData(int *main, int *data, int *num_comp, char *name, int *namelen) {
	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i)
		buff[i] = name[i];

	buff[*namelen] = '\0';
	paraviewHandler.SetFieldData<int>(*main, data, *num_comp, buff);

	delete[] buff;
}

extern "C" __declspec(dllexport) void EXPORTSTD SetDoubleFieldData(int *main, double *data, int *num_comp, char *name, int *namelen) {
	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i)
		buff[i] = name[i];

	buff[*namelen] = '\0';
	paraviewHandler.SetFieldData<double>(*main, data, *num_comp,buff);

	delete[] buff;
}


extern "C" __declspec(dllexport) void EXPORTSTD SetDoubleCellData(int *main, int *part, 
		int *feapCellNumber, int *feapSubCellNumber, 
		double *data, int *num_comp, char *name, int *namelen) {
	char *buff = new char[*namelen + 1];
	for (auto i = 0; i < *namelen; ++i)
		buff[i] = name[i];

	buff[*namelen] = '\0';
	paraviewHandler.setCellData<double>(*main, *part, *feapCellNumber, *feapSubCellNumber, data, *num_comp, buff);

	delete[] buff;
}