//#include <iostream>
//#include "Model.h"
//#include "MAC.h"
//#include "Builder.h"
//using namespace std;
//using namespace cp;
//
//const string X_PATH = "BMPath.xml";
//
//int main()
//{
//	cout << X_PATH << endl;
//	Builder path_builder(X_PATH, XRT_BM_PATH);
//	string bm_path = path_builder.GetBMFile();
//	cout << bm_path << endl;
//	Network* nt_;
//
//
//	//Network* nt = new Network(2);
//	//int value[] = { 0,1,2 };
//	//nt->MakeVar(0, value, 3);
//	//nt->MakeVar(1, value, 3);
//	//nt->MakeVar(2, value, 3);
//
//	//int** tuple0 = new int*[3];
//	//tuple0[0] = new int[2];
//	//tuple0[1] = new int[2];
//	//tuple0[2] = new int[2];
//	//tuple0[0][0] = 0;
//	//tuple0[0][1] = 0;
//
//	//tuple0[1][0] = 1;
//	//tuple0[1][1] = 1;
//
//	//tuple0[2][0] = 2;
//	//tuple0[2][1] = 2;
//
//	//int** tuple1 = new int*[3];
//	//tuple1[0] = new int[2];
//	//tuple1[1] = new int[2];
//	//tuple1[2] = new int[2];
//	//tuple1[0][0] = 0;
//	//tuple1[0][1] = 1;
//
//	//tuple1[1][0] = 0;
//	//tuple1[1][1] = 2;
//
//	//tuple1[2][0] = 1;
//	//tuple1[2][1] = 2;
//
//	//vector<IntVar*> scp0 = { nt->vars_[0],nt->vars_[1] };
//	//vector<IntVar*> scp1 = { nt->vars_[1],nt->vars_[2] };
//	//nt->MakeTab(0, scp0, tuple0, 3);
//	//nt->MakeTab(1, scp1, tuple1, 3);
//	//MAC* mac = new MAC(nt, AC_3);
//	//mac->enforce();
//
//	//for (int i = 0; i < 3; ++i)
//	//{
//	//	delete[] tuple0[i];
//	//	tuple0[i] = NULL;
//	//}
//
//	//for (int i = 0; i < 3; ++i)
//	//{
//	//	delete[] tuple1[i];
//	//	tuple1[i] = NULL;
//	//}
//
//	//delete mac;
//	//delete nt;
//
//	return 0;
//}