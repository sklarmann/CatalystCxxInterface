


#include <management.h>
#include <string>


int main() {

	
	managementClass testClass;

	testClass.addPoint(0, 0, 1, 0.0, 0.0, 0.0);
	testClass.addPoint(0, 0, 8, 1.0, 0.0, 0.0);
	testClass.addPoint(0, 0, 110, 1.0, 1.0, 0.0);
	testClass.addPoint(0, 0, 4, 0.0, 1.0, 0.0);


	testClass.addPoint(0, 1, 8, 1.0, 0.0, 0.0);
	testClass.addPoint(0, 1, 9, 2.0, 0.0, 0.0);
	testClass.addPoint(0, 1, 10, 2.0, 1.0, 0.0);
	testClass.addPoint(0, 1, 110, 1.0, 1.0, 0.0);

	int arr[4], numpt, vtkNum, cellNum, cellSubNum;
	arr[0] = 8; arr[1] = 9; arr[2] = 10; arr[3] = 110;
	numpt = 4;
	vtkNum = 9;
	cellNum = 2;
	cellSubNum = 2;
	testClass.addCell(0, 1, cellNum, cellSubNum, arr, &numpt, &vtkNum);
	arr[0] = 1; arr[1] = 8; arr[2] = 110; arr[3] = 4;
	numpt = 4;
	vtkNum = 9;
	cellNum = 2;
	cellSubNum = 2;
	testClass.addCell(0, 0, cellNum, cellSubNum, arr, &numpt, &vtkNum);

	int data[220];
	for (auto i = 0; i < 220; ++i) {
		data[i] = i;
	}

	char name[7] = {'t','e','s','t','A','r','r'};
	int len = 7;
	int numNodes = 110;
	int num_comp = 2;
	testClass.addCompleteFeapPointArray<int>(0, 1, data, num_comp, numNodes, name, len);
	testClass.addCompleteFeapPointArray<int>(0, 0, data, num_comp, numNodes, name, len);
	testClass.addCompleteFeapPointArray<int>(0, 0, data, num_comp, numNodes, name, len);

	int bla[2];
	bla[0] = 100; bla[1]=99;
	testClass.setPointData<int>(0, 0, 1, &bla, 2, name, len);

	std::string sname = "D:/build/cmfeap/coproc.py";

	int slen = sname.length();
	char *w = new char[slen];
	sname.copy(w, slen);

	testClass.initializePythonCoProcessing(w,&slen);
	//testClass.CoProcessTest();
	testClass.CoProcess();
	std::cin >> sname;
	double time = 1.0;
	testClass.TimeUpdate(time);
	testClass.CoProcess();
	std::cin >> sname;
	testClass.finalizeCoProcessing();
}