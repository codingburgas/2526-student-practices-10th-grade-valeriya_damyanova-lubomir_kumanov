#include <iostream>
#include "BLL.h"
using namespace std;

void BLL::showBLL() {
    cout << "BLL is working!" << endl;

    DAL d;
    d.showDAL();  
}
