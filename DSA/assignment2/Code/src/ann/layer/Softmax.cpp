/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Softmax.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name): m_nAxis(axis) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    //YOUR CODE IS HERE
    m_aCached_Y = softmax(X, m_nAxis);

    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    //YOUR CODE IS HERE

    xt::xarray<double> Y = m_aCached_Y;
    xt::xarray<double> DZ = xt::zeros_like(DY);

    if (DY.dimension() == 1){
        xt::xarray<double> func1 = xt::diag(Y) - xt::linalg::outer(Y, Y);
        DZ = xt::linalg::dot(DY, func1);
    } else {
        xt::xarray<double> func2 = diag_stack(Y) - outer_stack(Y, Y);
        DZ = matmul_on_stack(DY, func2);
    }

    return DZ; 
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}
