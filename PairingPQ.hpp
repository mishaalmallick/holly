// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
    public:
        // Description: Custom constructor that creates a node containing
        //              the given value.
        explicit Node(const TYPE &val)
            : elt { val } {}

        // Description: Allows access to the element at that Node's position.
        //              There are two versions, getElt() and a dereference
        //              operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child = nullptr;
        Node *sibling = nullptr;
        Node *parent = nullptr;
        // TODO: Add and initialize one extra pointer (parent or previous) as desired.
    };  // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp } {
        pq_size = 0;
        this->compare = comp; 
    }  // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp } {
        this->compare = comp; 
        pq_size = 0; 
        while(start != end){
            push(*start);
            ++start;
        }
    }  // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other)
        : BaseClass { other.compare } {
        
        pq_size = 0;
        root_ptr = nullptr;
        this->compare = other.compare;
      if (other.root_ptr) {
        root_ptr = copyNode(other.root_ptr);  // Recursively copy the root
    }
        

        // TODO: Implement this function.
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
    }  // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
      if (this != &rhs) {
        clear(root_ptr);  // Clear the current heap
        this->compare = rhs.compare;
        pq_size = rhs.pq_size;

        if (rhs.root_ptr) {
            root_ptr = copyNode(rhs.root_ptr);  // Recursively copy the rhs root
        }
        else {
            root_ptr = nullptr;  // If rhs is empty
        }
    }
    return *this;


    }  // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        clear(root_ptr); // Delete this line, it prevents linter complaints
    }  // ~PairingPQ()


    // Description: Move constructor and assignment operators don't need any
    //              code, the members will be reused automatically.
    PairingPQ(PairingPQ &&) noexcept = default;
    PairingPQ &operator=(PairingPQ &&) noexcept = default;


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        if(!root_ptr){
            return;
        }
        
        deque<Node*> melder;
        melder.push_back(root_ptr);
        Node *node_ptr;
        while(!melder.empty()){
            node_ptr = melder.front();
            melder.pop_front();
           if(node_ptr->child){
            melder.push_back(node_ptr->child); 
           } 
           if(node_ptr->sibling){
            melder.push_back(node_ptr->sibling);
           }

            meld(node_ptr, melder.front());
            melder.pop_front();
 
        }

        // TODO: Implement this function.
    }  // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) { addNode(val); }  // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
    if (!root_ptr) {
        return;  // Handle the case when the heap is empty
    }
    deque<Node*> melder;
    Node* current = root_ptr->child;
    
    // Push all children of the root to the deque
    while (current) {
        Node* next_sibling = current->sibling;
        current->parent = nullptr;  // Clear parent pointers
        current->sibling = nullptr; // Clear sibling pointers
        melder.push_back(current);
        current = next_sibling;
    }
    
    // Now pairwise merge all the children
    while (melder.size() > 1) {
        Node* first = melder.front(); 
        melder.pop_front();
        Node* second = melder.front(); 
        melder.pop_front();
        melder.push_back(meld(first, second));
    }
    
    root_ptr = melder.empty() ? nullptr : melder.front();  // Set the new root
    --pq_size;  // Decrease the size of the heap
        // TODO: Implement this function.
    }  // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root_ptr->elt;

    }  // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    [[nodiscard]] virtual std::size_t size() const {
        // TODO: Implement this function
        return pq_size;  // TODO: Delete or change this line
    }  // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    [[nodiscard]] virtual bool empty() const {
        // TODO: Implement this function
        return pq_size == 0;  // TODO: Delete or change this line
    }  // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value) {
        if (this->compare(new_value, node->elt)) {
        node->elt = new_value;

        // Detach the node from its parent's child list
        if (node->parent) {
            if (node->parent->child == node) {
                node->parent->child = node->sibling;
            } else {
                Node* sibling = node->parent->child;
                while (sibling->sibling != node) {
                    sibling = sibling->sibling;
                }
                sibling->sibling = node->sibling;
            }
        }

        node->sibling = nullptr;  
        node->parent = nullptr;   
        meld(root_ptr, node);     
    }
    }  // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node *addNode(const TYPE &val) {
	    Node* temp = new Node(val);
        if (root_ptr) {
		root_ptr = meld(temp, root_ptr);
        } else {
		root_ptr = temp;
        }
        ++pq_size;
        return temp;
        
          // TODO: Delete or change this line
    }  // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).
    Node *root_ptr = nullptr;
    Node* meld(Node* left_rootptr, Node* right_rootptr){
        if(this->compare(left_rootptr->elt, right_rootptr->elt)){
            left_rootptr->sibling = right_rootptr->child;
            right_rootptr->child = left_rootptr;
            left_rootptr->parent = right_rootptr;
            return right_rootptr;
        } else {
            right_rootptr->sibling = left_rootptr->child;
            left_rootptr->child = right_rootptr;
            right_rootptr->parent = left_rootptr;
            return left_rootptr;
        }
    }
    size_t pq_size;

        void clear(Node *node) {
        if (!node) {
            return;  // Base case: nothing to clear
        }

        // Recursively clear the child and sibling nodes
        clear(node->child);
        clear(node->sibling);

        // Once children and siblings are cleared, delete the current node
        delete node;
    }

    Node* copyNode(Node* other_node) {
    if (!other_node){
       return nullptr; 
    } 
    
    Node* new_node = new Node(other_node->elt);  // Copy the element
    new_node->child = copyNode(other_node->child);  // Recursively copy child
    new_node->sibling = copyNode(other_node->sibling);  // Recursively copy sibling
    
    if (new_node->child) {
        new_node->child->parent = new_node;
        }  // Set the child's parent
    if (new_node->sibling){
        new_node->sibling->parent = new_node->parent;
    }   // Set the sibling's parent
    
    return new_node;
    }

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif  // PAIRINGPQ_H
