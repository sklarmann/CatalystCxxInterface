
#include <vtkLiveInsituLink.h>
#include <vtkCommunicationErrorCatcher.h>
#include <vtkSmartPointer.h>
#include <vtkMultiProcessController.h>

class insitulink : public vtkLiveInsituLink {
public:
	//void InsituUpdateFix(double &time, vtkIdType &tstep) {
	//	vtkCommunicationErrorCatcher catcher(this->Proc0NodesController);
	//
	//	int drop_connection = catcher.GetErrorsRaised() ? 1 : 0;
	//	if (drop_connection)
	//	{
	//		this->DropLiveInsituConnection();
	//	}
	//	else {
	//		//std::cout << "Live changed " << this->WaitForLiveChange() << std::endl;
	//		this->InsituUpdate(time, tstep);
	//	}
	//}
};