/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CrossEntropy.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction): ILossLayer(reduction){
    
}

CrossEntropy::CrossEntropy(const CrossEntropy& orig):
ILossLayer(orig){
}

CrossEntropy::~CrossEntropy() {
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    //YOUR CODE IS HERE
    m_aYtarget = t;
    m_aCached_Ypred = X;

    return cross_entropy(X, t, m_eReduction == LossReduction::REDUCE_MEAN);
}
xt::xarray<double> CrossEntropy::backward() {
    //YOUR CODE IS HERE
    const double epsilon = 1e-7;

    xt::xarray<double> func2 = -m_aYtarget / (m_aCached_Ypred + epsilon);

    int N_norm = m_aYtarget.shape()[0];
    if (m_eReduction != LossReduction::REDUCE_MEAN){
        N_norm = 1;
    } 

    return func2 / N_norm;
}
