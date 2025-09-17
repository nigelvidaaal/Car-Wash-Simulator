#ifndef LLIST2_HPP
#define LLIST2_HPP

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

template <class T>
class LList {
public:
    struct Node {
        T     data{};
        Node* prev{};
        Node* next{};

        /**
         * @brief Default constructor for Node.
         * @param value data element of type.
         * @param p Pointer to the previous elements.
         * @param n Pointer to the next elements.
         */
        Node(const T& value, Node *p = nullptr, Node *n = nullptr) :
        // just like in previous projects
        // member initialization list, for correctness and initialization
        data(value), prev(p), next(n) {}
    };

    class BiDirectionalIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        friend class LList;

        /**
         * @brief Constructs a bidirectional iterator
         * @param ptr Pointer to the node to iterate on
         */
        BiDirectionalIterator(Node* ptr = nullptr);

        /**
         * @brief Increment operator
         * @return Reference to this iterator after advancing to next node
         */
        BiDirectionalIterator& operator++();

        /**
         * @brief Postfix increment operator
         * @return Copy of iterator before advancing to next node
         */
        BiDirectionalIterator operator++(int);

        /**
         * @brief Prefix decrement operator (--it)
         * @return Reference to this iterator after moving to previous node
         */
        BiDirectionalIterator& operator--();

        /**
         * @brief Postfix decrement operator
         * @return Copy of iterator before moving to previous node
         */
        BiDirectionalIterator operator--(int);

        /**
         * @brief Dereference operator
         * @return Reference to the data in the current node
         */
        reference operator*() const;
        // return current -> data;

        /**
         * @brief Member access operator (pointing finger)
         * @return Pointer to the current node
         */
        Node* operator -> () const;
        // return current

        /**
         * @brief Equality comparison operator
         * @param rhs Iterator to compare with
         */
        bool operator==(const BiDirectionalIterator& rhs) const;

        /**
         * @brief Inequality comparison operator
         * @param rhs Iterator to compare with
         */
        bool operator!=(const BiDirectionalIterator& rhs) const;

    private:
        Node* current;
    };

    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using iterator        = BiDirectionalIterator;
    using const_iterator  = BiDirectionalIterator;

    // *** CONSTRUCTORS ***
    /// look at private section and you can do {} since it's alr nullptr.
    LList() : head(nullptr), tail(nullptr), count(0){
        // set head, tail to nullptr, count to 0.
    }

    // initialize 3 to empty list, which is the one that we just did.
    LList(const LList& other) : LList() {
        // use Node* to traverse starting from head
        const Node* traverse = other.head;
        // as long as it has not reached the end..
        while (traverse != nullptr) {
            // add/push back data
            push_back(traverse -> data);
            // and go to the next one
            traverse = traverse -> next;
        }
    }

    // literally just like the Vector move constructor from pa14
    LList(LList&& other) noexcept :
        head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    // "Initialize head, tail, count"
    LList(const std::initializer_list<T>& init_list) : LList() {
        // for each element in init_list..
        for (const T& value : init_list) {
            // call push back to add the element (value)
            push_back(value);
        }
    }

    virtual ~LList() {
        // calls clear to release memory by removing all elements
        clear();
    }

    // *** ASSIGNMENT ***

    LList& operator=(const LList& other) {
        // self assignment check
        if (this != &other) {
            clear();
            // same code from constructor above
            const Node* traverse = other.head;
            while (traverse != nullptr) {
                // push_back to copy for each
                push_back(traverse -> data);
                // and go to the next one
                traverse = traverse -> next;
            }
        }
        return *this;
    }

    LList& operator=(LList&& other) noexcept {
        // self assignment check
        if (this != &other) {
            clear();
            // transfer ownership of these from other
            head = other.head;
            tail = other.tail;
            count = other.count;
            // set these to nullptr & 0 accordingly
            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;

            /// you can do this for simplicity:
            // other.swap(*this)
        }
        return *this;
    }

    // *** ELEMENT ACCESS ***

    /**
     * @brief Returns reference to first element
     * @return Reference to front element
     */
    reference front() {
        // if it's empty, throw an out of range error
        if (empty()) {
            throw std::out_of_range("Front: Empty list");
        }
        return head -> data;
    }

    /**
     * @brief Returns const reference to first element
     * @return Const reference to front element
     * @throw std::out_of_range if list is empty
     */
    const_reference front() const {
        // same for const
        if (empty()) {
            throw std::out_of_range("Const Front: Empty list");
        }
        return head -> data;
    }

    /**
     * @brief Returns reference to last element
     * @return Reference to back element
     * @throw std::out_of_range if list is empty
     */
    reference back() {
        // if it's empty, throw an out of range error
        if (empty()) {
            throw std::out_of_range("Back: Empty list");
        }
        return tail -> data;
    }

    /**
     * @brief Returns reference to last element
     * @return Reference to back element
     * @throw std::out_of_range if list is empty
     */
    const_reference back() const {
        // same for const
        if (empty()) {
            throw std::out_of_range("Const Back: Empty list");
        }
        return tail -> data;
    }

    // ITERATORS
    // similar to pa14 and straight forward description from the project itself.
    /**
     * @brief Gets iterator to beginning of list
     * @return Iterator pointing to first element
     */
    iterator begin() {
        // return iterator to head
        return iterator(head);
    }

    /**
     * @brief Gets const iterator to beginning of list
     * @return Const iterator pointing to first element
     */
    const_iterator begin() const {
        // do the same
        return const_iterator(head);
    }

    /**
     * @brief Gets iterator to end of list
     * @return Iterator pointing to one past last element
     */
    iterator end() {
        // same goes here as well
        return iterator(nullptr);
    }

    /**
     * @brief Gets const iterator to end of list
     * @return Const iterator pointing to one past last element
     */
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    // CAPACITY
    // very simpe, nuff said
    /**
     * @brief Checks if list is empty
     * @return true if list is empty, false otherwise
     */
    bool empty() const {
        return count == 0;
    }

    /**
     * @brief Gets number of elements in list
     * @return Current size of list or the count itself
     */
    size_type size() const {
        return count;
    }

    // Modifiers

    /**
     * @brief Removes all elements from list
     */
    void clear() noexcept {
        // while not empty, call pop_front
        while (!empty()) {
            pop_front();
        }
    }

    /**
     * @brief Inserts value at specified position
     * @param pos Iterator before which to insert
     * @param value Element value to insert
     * @return Iterator pointing to inserted element
     */
    iterator insert(iterator pos, const T& value) {
        // will keep track of the node we inserted
        Node* inserted = nullptr;

        // if pos is head, call push front(value)
        // but it's not possible to just be "head" it needed to be func
        if (pos == begin()) {
            push_front(value);
            // also insert in the beginning of course
            inserted = head;
        // vice versa
        } else if (pos == end()) {
            push_back(value);
            inserted = tail;
        } else {
            // create a new node, adjust them in designation
            /* Node* current  = pos.current;
             * Node* prev_node = current -> prev_node
             * Node newNode = new Node(value, prev_node, current); */


            Node* newNode = new Node(value, pos -> prev, pos.current);
            // update links
            /* prev_node -> next = newNode;
            current -> prev_node = newNode; */

            pos -> prev -> next = newNode;
            pos -> prev = newNode;
            // increment and return the iterator to the new node.
            ++count;
            inserted = newNode;
        }

        return iterator(inserted);
    }

    /**
     * @brief Removes element at specified position
     * @param pos Iterator to element to remove
     * @return Iterator following last removed element
     */
    iterator erase(iterator pos) {
        /* will be the iterator to return since we can't make multiple
        return statements */
        iterator to_return;

        if (pos == end()){
            // simply return end()
            to_return = end();
        } else if (pos.current == head) {
            // goes to front
            pop_front();
            to_return = begin();
        } else if (pos.current == tail) {
            // to back
            pop_back();
            // also ending the stream if reaches end
            to_return = end();
        } else {
            // if all of it simply passes, then..
            // adjust prev and next's pointers to node pointers

            // deleting such from this current pos
            Node* pos_del = pos.current;
            // visual diagram from page 25 of pa17 deleting the nodes
            Node* prev = pos_del->prev;
            Node* next = pos_del->next;

            /* from the game zuma, it's like just linking the beads after
            shooting the chunks*/
            prev->next = next;
            next->prev = prev;

            // delete pos & decrement count
            delete pos_del;
            --count;

            // return the iterator to the next elemnt
            to_return = iterator(next);
        }

        return to_return;
    }

    /**
     * @brief Adds element to end of list
     * @param value Element value to append
     */
    void push_back(const T& value) {
        // creating a new node with said details
        Node* newNode = new Node(value, tail, nullptr);
        /** this is also:
         * Node* value = new Node;
         * Node* tail = new Node;
         * Node* null = new Node;
         */

        // update tail
        // check for sanity check since we have to insert values only if they're
        // NOT nullptr
        if (tail != nullptr) {
            // current tail ptr next to the new node
            tail -> next = newNode;
        }

        // update new end of the list or the new node
        tail = newNode;

        // from the desc itself, very worded so just following orders
        if (head == nullptr) {
            // also point the head sicne if the tail is null, then head as well
            head = newNode;
        }

        // increment
        ++count;
    }

    /**
     * @brief Removes last element of list, throw if empty
     */
    void pop_back() {
        // just like every function in the project, great instructions
        if (empty()) {
        throw std::underflow_error("Pop_back func: underfow_error");
        }

        // adjust tail pointer, delete old one
        Node* to_delete = tail;
        // this is now a vaslid pointer
        // so now, it points to the node before it (previous)
        tail = tail -> prev;

        // let's make sure it doesnt point to a deleted/nullptr
        if (tail != nullptr) {
            tail -> next = nullptr;
        } else {
            // if it's empty, update head as well
            head = nullptr;
        }

        // remember to delete to avoid leaks and decrement like stated
        delete to_delete;
        --count;
    }

    /**
     * @brief Adds element to front of list
     * @param value Element value to prepend
     */
    void push_front(const T& value) {
        // create a new node just like the previous ones
        Node* newNode = new Node(value, nullptr, head);

        // here, like in push_back but it's head on first now
        if (head != nullptr) {
            head -> prev = newNode;
        }
        head = newNode;

        if (tail == nullptr) {
            tail = newNode;
        }

        ++count;
    }

    /**
     * @brief Adds element to front of list
     * @param value Element value to prepend
     */
    void pop_front() {
        // throw the error like stated
        if (empty()) {
        throw std::underflow_error("Pop_front: underflow_error");
        }

        // very veeery similar to pop_back, but head first.
        Node* to_delete = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }

        delete to_delete;
        --count;
    }

/**
 * @brief Swaps contents with another list
 * @param other List to swap with
 */
    void swap(LList& other) {
        // swap head tail and count
        Node* temp_head = head;
        Node* temp_tail = tail;
        std::size_t temp_count = count;

        // this is like: Vector(Vector&& other)
        // allows us to swap without copying elements
        head = other.head;
        tail = other.tail;
        count = other.count;

        other.head = temp_head;
        other.tail = temp_tail;
        other.count = temp_count;
    }

private:
    // you can do {nullptr} for each var for simplication
    Node*       head;
    Node*       tail;
    std::size_t count;
}; // class LList

// *** BiDirectionalIterator Member Functions ***

template <class T>
inline LList<T>::BiDirectionalIterator::BiDirectionalIterator(Node* ptr) :
current(ptr) {
    // sets current to ptr, initializing to pointer; node or nullptr.
}

// from vector.hpp of pa14:
/* template <class T> Vector<T>& Vector<T>::operator+=(const Vector<T>& other)*/
template <class T>
inline typename LList<T>::BiDirectionalIterator&
LList<T>::BiDirectionalIterator::operator++() {
    // via instructions from the project desc.
    if (current != nullptr) {
        current = current -> next;
    }
    return *this;
}

template <class T>
inline typename LList<T>::BiDirectionalIterator
LList<T>::BiDirectionalIterator::operator++(int) {
    // COPY *this to the iterator
    BiDirectionalIterator temp_iterator = *this;
    // call in advance
    ++(*this);
    // return temporary iterator
    return temp_iterator;
}

template <class T>
inline typename LList<T>::BiDirectionalIterator&
LList<T>::BiDirectionalIterator::operator--() {
    // same as the preincrement of ++ and via instructions
    if (current != nullptr) {
        current = current -> prev;
    }
    return *this;
}

template <class T>
inline typename LList<T>::BiDirectionalIterator
LList<T>::BiDirectionalIterator::operator--(int) {
    BiDirectionalIterator temp = *this;
    if (current == nullptr || current -> prev == nullptr) {
        throw std::out_of_range("Error");
    }
    current = current -> prev;
    return temp;
}

template <class T>
inline typename LList<T>::reference
LList<T>::BiDirectionalIterator::operator*() const {
    if (current == nullptr) {
        throw std::out_of_range("Dereferencing null iterator");
    }
    return current -> data;
}

template <class T>
inline typename LList<T>::Node*
LList<T>::BiDirectionalIterator::operator->() const {
    // direct access to current node's members
    return current;
}

// via instructions
template <class T>
inline bool LList<T>::BiDirectionalIterator::operator==
(const BiDirectionalIterator& rhs) const {
    // returns true if iterators point to the same node
    return current == rhs.current;
}

template <class T>
inline bool LList<T>::BiDirectionalIterator::operator!=
(const BiDirectionalIterator& rhs) const {
    // returns true if iterators point different nodes
    return current != rhs.current;
}

// Non-member functions
template <class U>
bool operator==(const LList<U>& lhs, const LList<U>& rhs) {
    bool is_equal = (lhs.size() == rhs.size());

    typename LList<U>::const_iterator p_lhs = lhs.begin();
    typename LList<U>::const_iterator p_rhs = rhs.begin();

    while (p_lhs != lhs.end() && is_equal) {
        is_equal = (*p_lhs == *p_rhs);
        p_lhs++;
        p_rhs++;
    }

    return is_equal;
}

template <class T>
bool operator!=(const LList<T>& lhs, const LList<T>& rhs) {
    return !(lhs == rhs);
}

#endif
