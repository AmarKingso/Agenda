#include"ErrorReason.hpp"

using namespace std;

ErrReason::ErrReason(){}

ErrReason::ErrReason(string err_msg) : exception(err_msg.c_str()){
}