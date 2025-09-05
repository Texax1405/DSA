    /*
    * File:   DLinkedList.h
    */

    #ifndef DLINKEDLIST_H
    #define DLINKEDLIST_H

    #include "list/IList.h"

    #include <sstream>
    #include <iostream>
    #include <type_traits>
    using namespace std;

    template <class T>
    class DLinkedList : public IList<T>
    {
    public:
        class Node;        // Forward declaration
        class Iterator;    // Forward declaration
        class BWDIterator; // Forward declaration

    protected:
        Node *dummyHead; // this node does not contain user's data
        Node *tail;      // this node does not contain user's data
        int count;
        bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
        void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

    public:
        DLinkedList(
            void (*deleteUserData)(DLinkedList<T> *) = 0,
            bool (*itemEqual)(T &, T &) = 0);
        DLinkedList(const DLinkedList<T> &list);
        DLinkedList<T> &operator=(const DLinkedList<T> &list);
        ~DLinkedList();

        // Inherit from IList: BEGIN
        void add(T e);
        void add(int index, T e);
        T removeAt(int index);
        bool removeItem(T item, void (*removeItemData)(T) = 0);
        bool empty();
        int size();
        void clear();
        T &get(int index);
        int indexOf(T item);
        bool contains(T item);
        string toString(string (*item2str)(T &) = 0);
        // Inherit from IList: END

        void println(string (*item2str)(T &) = 0)
        {
            cout << toString(item2str) << endl;
        }
        void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
        {
            this->deleteUserData = deleteUserData;
        }

        bool contains(T array[], int size)
        {
            int idx = 0;
            for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
            {
                if (!equals(*it, array[idx++], this->itemEqual))
                    return false;
            }
            return true;
        }

        /*
        * free(DLinkedList<T> *list):
        *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
        *  + if users want a DLinkedList removing their data,
        *      he/she must pass "free" to constructor of DLinkedList
        *      Example:
        *      DLinkedList<T> list(&DLinkedList<T>::free);
        */
        static void free(DLinkedList<T> *list)
        {
            typename DLinkedList<T>::Iterator it = list->begin();
            while (it != list->end())
            {
                delete *it;
                it++;
            }
        }

        /* begin, end and Iterator helps user to traverse a list forwardly
        * Example: assume "list" is object of DLinkedList

        DLinkedList<char>::Iterator it;
        for(it = list.begin(); it != list.end(); it++){
                char item = *it;
                std::cout << item; //print the item
        }
        */
        Iterator begin()
        {
            return Iterator(this, true);
        }
        Iterator end()
        {
            return Iterator(this, false);
        }

        /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
        * Example: assume "list" is object of DLinkedList

        DLinkedList<char>::BWDIterator it;
        for(it = list.last(); it != list.beforeFirst(); it--){
                char item = *it;
                std::cout << item; //print the item
        }
        */
        BWDIterator bbegin()
        {
            return BWDIterator(this, true);
        }
        BWDIterator bend()
        {
            return BWDIterator(this, false);
        }

    protected:
        static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
        {
            if (itemEqual == nullptr)
                return lhs == rhs;
            else
                return itemEqual(lhs, rhs);
        }
        void copyFrom(const DLinkedList<T> &list);
        void removeInternalData();
        void checkIndex(int index);
        Node *getPreviousNodeOf(int index);

        //////////////////////////////////////////////////////////////////////
        ////////////////////////  INNER CLASSES DEFNITION ////////////////////
        //////////////////////////////////////////////////////////////////////
    public:
        class Node
        {
        public:
            T data;
            Node *next;
            Node *prev;
            friend class DLinkedList<T>;

        public:
            Node(Node *next = 0, Node *prev = 0)
            {
                this->next = next;
                this->prev = prev;
            }
            Node(T data, Node *next = 0, Node *prev = 0)
            {
                this->data = data;
                this->next = next;
                this->prev = prev;
            }
        };

        //////////////////////////////////////////////////////////////////////
        class Iterator
        {
        private:
            DLinkedList<T> *pList;
            Node *pNode;

        public:
            Iterator(DLinkedList<T> *pList = 0, bool begin = true)
            {
                if (begin)
                {
                    this->pNode = pList->dummyHead->next;
                }
                else
                {
                    this->pNode = nullptr;
                }
                this->pList = pList;
            }

            Iterator &operator=(const Iterator &iterator)
            {
                this->pNode = iterator.pNode;
                this->pList = iterator.pList;
                return *this;
            }
            void remove(void (*removeItemData)(T) = 0)
            {
                Node *prev = pNode->prev;
                Node *next = pNode->next;

                if (prev != nullptr) prev->next = next;
                if (next != nullptr) next->prev = prev;

                if (pNode == pList->tail) pList->tail = prev;
                if (removeItemData != nullptr) removeItemData(pNode->data);

                delete pNode;
                pNode = next;
                pList->count--;
            }

            T &operator*()
            {
                return pNode->data;
            }
            bool operator!=(const Iterator &iterator)
            {
                return pNode != iterator.pNode;
            }
            // Prefix ++ overload
            Iterator &operator++()
            {
                pNode = pNode->next;
                return *this;
            }
            // Postfix ++ overload
            Iterator operator++(int)
            {
                Iterator iterator = *this;
                ++*this;
                return iterator;
            }
        };
    };
    //////////////////////////////////////////////////////////////////////
    // Define a shorter name for DLinkedList:

    template <class T>
    using List = DLinkedList<T>;

    //////////////////////////////////////////////////////////////////////
    ////////////////////////     METHOD DEFNITION      ///////////////////
    //////////////////////////////////////////////////////////////////////

    template <class T>
    DLinkedList<T>::DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *),
        bool (*itemEqual)(T &, T &))
    {
        this->dummyHead = new Node();
        this->tail = dummyHead;
        this->count = 0;
        this->deleteUserData = deleteUserData;
        this->itemEqual = itemEqual;
    }

    template <class T>
    DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
    {
        this->dummyHead = new Node();
        this->tail = dummyHead;
        this->count = 0;
    }

    template <class T>
    DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
    {
        DLinkedList<T> temp(list);
        swap(temp.dummyHead, dummyHead);
        return *this;
    }

    template <class T>
    DLinkedList<T>::~DLinkedList()
    {
        this->clear();
    }
    template <class T>
    void DLinkedList<T>::add(T e)
    {
        Node *newNode = new Node(e);

        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;

        count++;
    }
    template <class T>
    void DLinkedList<T>::add(int index, T e)
    {
        this->checkIndex(index);
        Node *curr = getPreviousNodeOf(index);
        Node *newNode = new Node(e, curr->next, curr);
        if (curr->next != nullptr){
            (curr->next)->prev = newNode;
        } else {
            tail = newNode;
        }
        curr->next = newNode;
        count++;
    }

    template <class T>
    typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
    {
        /**
         * Returns the node preceding the specified index in the doubly linked list.
         * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
         * Efficiently navigates to the node by choosing the shorter path based on the index's position.
         */
        Node *curr = dummyHead;
        for (int i = 0; i < index; i++){
            curr = curr->next;
        }

        return curr;
    }

    template <class T>
    T DLinkedList<T>::removeAt(int index)
    {
        this->checkIndex(index);
        Node *curr = getPreviousNodeOf(index);
        Node *nodeToRemove = curr->next;
        T removedData = nodeToRemove->data;

        curr->next = nodeToRemove->next;

        if (nodeToRemove->next != nullptr){
            nodeToRemove->next->prev = curr;
        } else {
            tail = curr;
        }

        delete nodeToRemove;
        count--;
        return removedData;
    }

    template <class T>
    bool DLinkedList<T>::empty()
    {
        return this->count == 0;
    }

    template <class T>
    int DLinkedList<T>::size()
    {
        return this->count;
    }

    template <class T>
    void DLinkedList<T>::clear()
    {
        Node *curr = dummyHead->next;
        while (curr != nullptr){
            delete curr;
            curr = curr->next;
        }

        dummyHead->next = nullptr;
        tail = dummyHead;
        count = 0;
    }

    template <class T>
    T &DLinkedList<T>::get(int index)
    {
        this->checkIndex(index);
        Node *curr = dummyHead->next;

        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }

        return curr->data;
    }

    template <class T>
    int DLinkedList<T>::indexOf(T item)
    {
        Node* curr = dummyHead->next;

        if (curr == nullptr){
            return -1;
        }
        
        for (int i = 0; i < count; i++){
            if (equals(curr->data, item, this->itemEqual)){
                return i;
            }
            curr = curr->next;
        }

        return -1;
    }

    template <class T>
    bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
    {
        Node *curr = dummyHead->next;
        while (curr != nullptr) {
            if (equals(curr->data, item, this->itemEqual)) {
                if (curr->prev != nullptr) {
                    curr->prev->next = curr->next;
                }
                if (curr->next != nullptr) {
                    curr->next->prev = curr->prev;
                }
                if (removeItemData) {
                    removeItemData(curr->data);
                }
                delete curr;
                count--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    template <class T>
    bool DLinkedList<T>::contains(T item)
    {
        return this->indexOf(item) != -1;
    }

    template <class T>
    string DLinkedList<T>::toString(string (*item2str)(T &))
    {
        /**
         * Converts the list into a string representation, where each element is formatted using a user-provided function.
         * If no custom function is provided, it directly uses the element's default string representation.
         * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
         *
         * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
         * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
         */
        stringstream ss;
        ss << "[";
        Node *curr = dummyHead->next; 
        while (curr){
            if (item2str != nullptr){
                ss << item2str(curr->data);
            } else {
                ss << (curr->data);
            }

            if (curr->next != nullptr){
                ss << ", ";
            }
            curr = curr->next;
        }

        ss << "]";
        return ss.str();
    }

    template <class T>
    void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
    {
        /**
         * Copies the contents of another doubly linked list into this list.
         * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
         * Iterates through the source list and adds each element, preserving the order of the nodes.
         */
        this->removeInternalData();
        if (list.dummyHead->next == nullptr)
            return;

        Node *curr = list.dummyHead->next;
        while (curr != nullptr)
        {
            Node *newNode = new Node(curr->data);
            if (this->dummyHead->next == nullptr)
            {
                this->dummyHead->next = newNode;
                newNode->prev = this->dummyHead;
                this->tail = newNode;
            }
            else
            {
                // head -> [A] <-> [B] <- tail
                this->tail->next = newNode;
                newNode->prev = this->tail;
                this->tail = newNode;
            }

            curr = curr->next;
        }

        this->count = list.count;
    }

    template <class T>
    void DLinkedList<T>::removeInternalData()
    {
        /**
         * Clears the internal data of the list by deleting all nodes and user-defined data.
         * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
         * Traverses and deletes each node between the head and tail to release memory.
         */
        Node *curr = this->dummyHead->next;

        while (curr != nullptr)
        {
            if (this->deleteUserData)
            {
                this->deleteUserData(curr->data);
            }

            delete curr;
            curr = curr->next;
        }

        this->dummyHead->next = nullptr;
        this->tail = dummyHead;
        this->count = 0;
    }

    template <class T>
    void DLinkedList<T>::checkIndex(int index){
        if (index < 0 || index > this->count){
            throw out_of_range("Index is out of range!");
        }
    }

    #endif /* DLINKEDLIST_H */
