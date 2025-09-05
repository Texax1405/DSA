/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0){
        //TODO
        
        typename AbstractGraph<T>::VertexNode* vertexFrom = AbstractGraph<T>::getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* vertexTo = AbstractGraph<T>::getVertexNode(to);

        if (vertexFrom == nullptr) {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        } else if (vertexTo == nullptr){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }
        
        vertexFrom->connect(vertexTo, weight);
    }
    void disconnect(T from, T to){
        //TODO
        typename AbstractGraph<T>::VertexNode* vertexFrom = AbstractGraph<T>::getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* vertexTo = AbstractGraph<T>::getVertexNode(to);

        if (vertexFrom == nullptr) {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        } else if (vertexTo == nullptr){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }


        vertexFrom->removeTo(vertexTo);
    }
    void remove(T vertex){
        //TODO
        typename AbstractGraph<T>::VertexNode* vertexNode = AbstractGraph<T>::getVertexNode(vertex);

        if (vertexNode == nullptr){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(vertex));
        }

        for (auto it = AbstractGraph<T>::nodeList.begin(); it != AbstractGraph<T>::nodeList.end(); it++){
            (*it)->removeTo(vertexNode);
            vertexNode->removeTo(*it);
        }

        this->nodeList.removeItem(vertexNode);
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++){
            graph->add(vertices[i]);
        }

        for (int i = 0; i < nedges; i++){
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */

