#include <iostream>
#include "Model.h"
#include "MAC.h"
#include "XMLBuilder.h"
#include "BuildModel.h"
#include <string>
#include <time.h>
//#include <windows.h> 
//#include <Mmsystem.h>             //timeGetTime()  
//#pragma comment(lib, "Winmm.lib")   //timeGetTime()  
using namespace std;
using namespace cp;
using namespace cp::model;
using namespace cp::parse;
const string X_PATH = "BMPath.xml";

int main()
{
	clock_t  begin, end;
	XMLBuilder path_builder(X_PATH, XRT_BM_PATH);
	string bm_path = path_builder.GetBMFile();
	cout << bm_path << endl;
	XMLBuilder builder(bm_path, XRT_BM);
	XMLModel* xml_model = new XMLModel();
	builder.GenerateModelFromXml(xml_model);
	Network* nt = new Network();
	BuildModel(xml_model, nt);
	MAC* mac = new MAC(nt, AC_3);
	begin = clock_t();
	mac->enforce();
	end = clock_t();
	cout << "solutions = " << mac->sol_count() << endl;
	int time = end - begin;
	cout << "execute time = " << time << endl;
	delete mac;
	delete nt;

	return 0;
}