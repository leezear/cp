#include <iostream>
#include "Model.h"
#include "MAC.h"
#include "XMLBuilder.h"
#include "BuildModel.h"
#include <string>
using namespace std;
using namespace cp;
using namespace cp::model;
using namespace cp::parse;

const string X_PATH = "BMPath.xml";

int main()
{
	cout << X_PATH << endl;
	XMLBuilder path_builder(X_PATH, XRT_BM_PATH);
	string bm_path = path_builder.GetBMFile();
	cout << bm_path << endl;
	XMLBuilder builder(bm_path, XRT_BM);
	XMLModel* xml_model = new XMLModel();
	builder.GenerateModelFromXml(xml_model);
	Network* nt =new Network();
	BuildModel(xml_model, nt);
	MAC* mac = new MAC(nt, AC_3);
	mac->enforce();

	//for (int i = 0; i < 3; ++i)
	//{
	//	delete[] tuple0[i];
	//	tuple0[i] = NULL;
	//}

	//for (int i = 0; i < 3; ++i)
	//{
	//	delete[] tuple1[i];
	//	tuple1[i] = NULL;
	//}

	delete mac;
	delete nt;

	return 0;
}