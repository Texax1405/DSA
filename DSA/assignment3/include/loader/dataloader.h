/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), 
                batch_size(batch_size), 
                shuffle(shuffle),
                m_seed(seed){
            nbatch = ptr_dataset->len()/batch_size;
            item_indices = xt::arange(0, ptr_dataset->len());

            if (shuffle == true){
                if (seed >= 0){
                    xt::random::seed(m_seed);
                }
                xt::random::shuffle(item_indices); 
            }
    }
    virtual ~DataLoader(){}
    
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin(){
        //YOUR CODE IS HERE
        return Iterator(this, 0);
    }
    Iterator end(){
        //YOUR CODE IS HERE
        return Iterator(this, nbatch);
    }
    
    //BEGIN of Iterator

    //YOUR CODE IS HERE: to define Iterator
    class Iterator {
    private:
        DataLoader* loader;
        int currBatch;
    public:
        Iterator(DataLoader* loader = 0, int currBatch = 0) {
            this->loader = loader;
            this->currBatch = currBatch;
        }

        Iterator& operator=(const Iterator& Iterator) {
            this->loader = Iterator.loader;
            this->currBatch = Iterator.currBatch;
            return *this;
        }

        bool operator != (const Iterator &Iterator) const {
            return currBatch != Iterator.currBatch;
        }

        Batch<DType, LType> operator*() {
            int startIdx = currBatch * loader->get_batch_size();
            int total_samples = loader->get_sample_count();
            int batchSize;

            if (currBatch == loader->get_total_batch()){
                batchSize = total_samples - startIdx;
            } else {
                batchSize = loader->get_batch_size();
            }

            auto dataShape = loader->ptr_dataset->get_data_shape();
            auto labelShape = loader->ptr_dataset->get_label_shape();

            dataShape[0] = batchSize;
            if (labelShape.size() > 0){
                labelShape[0] = batchSize;
            }

            xt::xarray<DType> X = xt::empty<DType>(dataShape);
            xt::xarray<LType> t = xt::empty<LType>(labelShape);

            for (int i = startIdx; i < startIdx + batchSize; i++){
                auto dataL = loader->ptr_dataset->getitem(loader->item_indices[i]);
                xt::view(X, i - startIdx) = dataL.getData();

                if (dataL.getLabel().size() == 0){
                    t = {};
                } else {
                    xt::view(t, i - startIdx) = dataL.getLabel();
                }
            }
            return Batch<DType, LType>(X, t);
        }        
        
        Iterator& operator++() {
            this->currBatch++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
    };

    //END of Iterator
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

