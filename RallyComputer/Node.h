#pragma once
template <typename T>
struct Node {
    T val;
    Node<T>* next;
    Node<T>* prev;

    Node(T obj):
        val(obj)
        , next(nullptr)
        , prev(nullptr)
    {}
};

template <typename T>
class List {
    public:
        using NodeType = Node<T>;
        NodeType* head;
        NodeType* tail;
        List(bool circular = false):
            head(nullptr)
            , tail(nullptr)
        {}
        void append(NodeType* n) {
            if(!n) {
                return;
            }
            if(!head) {
                head = n;
                tail = n;
                if(circular_) {
                    n->next = n;
                    n->prev = n;
                }
                return;
            }
            tail->next = n;
            n->prev = tail;
            tail = n;
            while(tail->next) {
                tail = tail->next;
            }
            if(circular_) {
                head->prev = tail;
                tail->next = head;
            }
        }
        void append(T obj) {
            NodeType* n = new NodeType(obj);
            append(n);
        }
        bool empty() {
            return !head;
        }
    private:
        bool circular_;
};
