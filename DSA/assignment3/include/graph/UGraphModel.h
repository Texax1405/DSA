/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
//#include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* vertexFrom = AbstractGraph<T>::getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* vertexTo = AbstractGraph<T>::getVertexNode(to);

        if (vertexFrom == nullptr) {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        } else if (vertexTo == nullptr){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }

        if (vertexFrom->equals(vertexTo)){
            vertexFrom->connect(vertexTo, weight);
        } 

        vertexFrom->connect(vertexTo, weight);
        vertexTo->connect(vertexFrom, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* vertexFrom = AbstractGraph<T>::getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* vertexTo = AbstractGraph<T>::getVertexNode(to);

        if (vertexFrom == nullptr) {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        } else if (vertexTo == nullptr){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }

        if (vertexFrom == vertexTo){
            vertexFrom->removeTo(vertexTo);
        } 

        vertexFrom->removeTo(vertexTo);
        vertexTo->removeTo(vertexFrom);
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* vertexNode = AbstractGraph<T>::getVertexNode(vertex);

        if (vertexNode == 0){
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(vertex));
        }

        for (auto it = AbstractGraph<T>::nodeList.begin(); it != AbstractGraph<T>::nodeList.end(); it++){
            (*it)->removeTo(vertexNode);
            vertexNode->removeTo(*it);
        }

        this->nodeList.removeItem(vertexNode);
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T>* graph = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++){
            graph->add(vertices[i]);
        }

        for (int i = 0; i < nedges; i++){
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
