#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
#include <algorithm>

using namespace std;

template<typename DType, typename LType>
class DataLoader {
public:
    class iterator;
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    xt::xarray<unsigned long> indexArray;
    int currIndex;
    int m_seed;

public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size, bool shuffle=true, bool drop_last=false, int seed = -1) {
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->currIndex = 0;
        this->m_seed = seed;

        indexArray = xt::arange<unsigned long>(ptr_dataset->len());

        if (shuffle == true) {
            if (seed >= 0) {
                xt::random::seed(m_seed);
            }
            xt::random::shuffle(indexArray);
        }
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        int nums_batch = ptr_dataset->len() / batch_size;
        return iterator(this, nums_batch);
    }

    virtual ~DataLoader() {}

    class iterator {
    private:
        DataLoader* loader;
        int currBatch;
    public:
        iterator(DataLoader* loader = 0, int currBatch = 0) {
            this->loader = loader;
            this->currBatch = currBatch;
        }

        iterator& operator=(const iterator& iterator) {
            this->loader = iterator.loader;
            this->currBatch = iterator.currBatch;
            return *this;
        }

        bool operator != (const iterator &iterator) const {
            return currBatch != iterator.currBatch;
        }

        Batch<DType, LType> operator*() {
            int startIdx = currBatch * loader->batch_size;
            int total_samples = loader->ptr_dataset->len();
            int batchSize;

            if (currBatch == (total_samples / loader->batch_size) - 1 && loader->drop_last == false){
                batchSize = total_samples - startIdx;
            } else {
                batchSize = loader->batch_size;
            }

            auto dataShape = loader->ptr_dataset->get_data_shape();
            auto labelShape = loader->ptr_dataset->get_label_shape();

            dataShape[0] = batchSize;
            if (labelShape.size() > 0) {
                labelShape[0] = batchSize;
            }

            xt::xarray<DType> X = xt::empty<DType>(dataShape);
            xt::xarray<LType> t = xt::empty<LType>(labelShape);

            for (int i = startIdx; i < startIdx + batchSize; i++) {
                auto dataL = loader->ptr_dataset->getitem(loader->indexArray[i]);
                xt::view(X, i - startIdx) = dataL.getData();

                if (dataL.getLabel().size() == 0) {
                    t = {};
                } else {
                    xt::view(t, i - startIdx) = dataL.getLabel();
                }
            }

            return Batch<DType, LType>(X, t);

        }        
        
        iterator& operator++() {
            this->currBatch++;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
    };
};

#endif /* DATALOADER_H */
