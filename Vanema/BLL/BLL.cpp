#include "BLL.h"
#include "../DAL/DAL.h" 

std::string BLLClass::process() {
    DALClass dal;
    return "BLL" + dal.getTestData();
}