#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

//#include <filesystem> //require C++17
// namespace fs = std::filesystem;

#include "hash/xMap.h"
#include "heap/Heap.h"

#include "demo/hash/xMapDemo.h"
#include "demo/heap/HeapDemo.h"
#include "include/dsaheader.h"
// #include "sformat/fmt_lib.h"
// #include "tensor/xtensor_lib.h"
// #include "ann/annheader.h"
// #include "loader/dataset.h"
// #include "loader/dataloader.h"
// #include "config/Config.h"
// #include "dataset/DSFactory.h"
// #include "optim/Adagrad.h"
// #include "optim/Adam.h"
// #include "modelzoo/twoclasses.h"
// #include "modelzoo/threeclasses.h"


int main(int argc, char** argv) {
    //dataloader:
    //case_data_wo_label_1();
    //case_data_wi_label_1();
    //case_batch_larger_nsamples();

    //cout << "----simpleMap----" << endl;
    //simpleMinHeap();
    //simpleMap();

    cout << "----Test 1----" << endl;
    //hashDemo1();
    heapDemo1();

    cout << "----Test 2----" << endl;
    //hashDemo2();
    heapDemo2();

    cout << "----Test 3----" << endl;
    //hashDemo3();
    heapDemo3();

    cout << "----Test 4----" << endl;
    //hashDemo4();


    cout << "----Test 5----" << endl;
    //hashDemo5();


    cout << "----Test 6----" << endl;
    //hashDemo6();


    cout << "----Test 7----" << endl;
    //hashDemo7();


    cout << "----Test Country----" << endl;
    //countryDemo();

    //Classification:
    //twoclasses_classification();
    //threeclasses_classification();
 
    return 0;
}
