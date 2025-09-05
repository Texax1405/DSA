/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "hash/xMap.h"
#include <stacknqueue/Queue.h>
#include <stacknqueue/Stack.h>

template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0){
        //TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        //TODO
        if (mode == 1){
            return bfsSort(sorted);
        } 

        return dfsSort(sorted);
        
    }
    DLinkedList<T> bfsSort(bool sorted=true){ 
        //TODO
        Queue<T> q;
        DLinkedList<T> results;
        xMap<T, bool> visited(hash_code);

        //add first vertex with no indegree
        DLinkedList<T> zeroInDegree = listOfZeroInDegrees();
        for (auto it = zeroInDegree.begin(); it != zeroInDegree.end(); it++){
            T vertex = *it;
            q.push(vertex);
            visited.put(vertex, true);
        }

        while (!q.empty()) {
            T currNode = q.peek();
            q.pop();
            results.add(currNode);

            for (auto it = graph->getOutwardEdges(currNode).begin(); it != graph->getOutwardEdges(currNode).end(); it++) {
                T neighbor = *it;
                if (visited.get(neighbor) == false) {
                    q.push(neighbor);
                    visited.put(neighbor, true);
                }
            }
        }

        return results;
    }

    DLinkedList<T> dfsSort(bool sorted=true){
        //TODO
        Stack<T> q;
        DLinkedList<T> results;
        xMap<T, bool> visited(hash_code);

        //add first vertex with no indegree
        DLinkedList<T> zeroInDegree = this->listOfZeroInDegrees();
        for (auto it = zeroInDegree.begin(); it != zeroInDegree.end(); it++){
            T vertex = *it;
            q.push(vertex);
            visited.put(vertex, true);
        }

        while (!q.empty()) {
            T currNode = q.peek();
            q.pop();
            results.add(currNode);

            auto edges = graph->getOutwardEdges(currNode);
            if (edges.empty()) continue;

            for (auto it = edges.begin(); it !=edges.end(); it++) {
                T neighbor = *it;
                if (!visited.containsKey(neighbor) || !visited.get(neighbor)) {
                    q.push(neighbor);
                    visited.put(neighbor, true);
                }
            }
        }

        return results;
    }

protected:
    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int)){
        xMap<T, int> inDegree(hash);

        for (auto it = graph->vertices().begin(); it != graph->vertices().end(); it++){
            T vertex = *it;
            inDegree.put(vertex, graph->inDegree(vertex));
        }

        return inDegree;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int)){
        xMap<T, int> outDegree(hash);
        for (auto it = graph->vertices().begin(); it != graph->vertices().end(); it++){
            T vertex = *it;
            outDegree.put(vertex, graph->outDegree(vertex));
        }

        return outDegree;
    }
    DLinkedList<T> listOfZeroInDegrees(){
        // DLinkedList<T> zeroOutDegrees;
        // cout << "code ngu" << endl;
        // for (auto it = graph->vertices().begin(); it != graph->vertices().end(); ++it) {
        //     T vertex = *it;
        //     cout << "code ngu1" << endl;
        //     if (graph->inDegree(vertex) == 0 || graph->outDegree(vertex) == 0){
        //         cout << "code ngu2" << endl;
        //         zeroOutDegrees.add(vertex);
        //     }
        //     cout << "code ngu3" << endl;
        // }
        // return zeroOutDegrees;

        DLinkedList<T> zeroInDegrees;

        xMap<T, int> in_degree(this->hash_code, 0.75, nullptr, nullptr, nullptr, nullptr);
        DLinkedList<T> verticesList = graph->vertices();

        for (auto vit = verticesList.begin(); vit != verticesList.end(); vit++){
            in_degree.put(*vit, 0);
        }

        for (auto vit = verticesList.begin(); vit != verticesList.end(); vit++){
            DLinkedList<T> adjVertices = this->graph->getInwardEdges(*vit);
            for (auto adjIt = adjVertices.begin(); adjIt != adjVertices.end(); adjIt++){
                in_degree.put(*adjIt, in_degree.get(*adjIt) + 1);
            }
        }

        for (auto vit = verticesList.begin(); vit != verticesList.end(); vit++){
            if (in_degree.get(*vit) == 0){
                zeroInDegrees.add(*vit);
            }
        }

        return zeroInDegrees;
    }

}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */

