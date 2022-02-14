#pragma once

// Implement this class in this header file

#include <iostream>


using namespace std;

template <typename T>
class dictionary
{
private:
    struct Node
    {
        // A, B and C are the actual data values in the node
        T* A;
        T* B;
        T* C;

        // Child pointers
        Node* L, * M1, * M2, * R;

        // Constructor to initialize the node with one value. The other two value
        // pointers will be set to null and all child pointers will be set to null.
        // The pointer given as the parameter is directly copied to A.
        Node(T* valueForA)
        {
            A = valueForA;
            B = C = NULL;
            L = M1 = M2 = R = NULL;
        }

        // Constructor to initialize the node with one value and two child pointers.
        Node(T* valueForA, Node* leftChildOfA, Node* rightChildOfA)
        {
            A = valueForA;
            L = leftChildOfA;
            M1 = rightChildOfA;

            // Everything else is NULL
            B = C = NULL;
            M2 = R = NULL;
        }

        Node(T* aVal, T* bVal, T* cVal, Node* left, Node* m1, Node* m2, Node* right)
        {
            A = aVal;
            B = bVal;
            C = cVal;
            L = left;
            M1 = m1;
            M2 = m2;
            R = right;
        }

        bool ContainsKey(T key)
        {
            if (C && *C == key) { return true; }
            if (B && *B == key) { return true; }
            if (*A == key) { return true; }
            return false;
        }

        T* FindsKey(T key)
        {
            if (C && *C == key) { return C; }
            if (B && *B == key) { return B; }
            if (*A == key) { return A; }
            return NULL;
        }

        void DeleteData()
        {
            if (A) { delete A; A = NULL; }
            if (B) { delete B; B = NULL; }
            if (C) { delete C; C = NULL; }
        }

        // Returns true if all three data-value pointers, A, B, and C, are non-null,
        // false otherwise.
        bool IsFull()
        {
            return (A != NULL && B != NULL && C != NULL);
        }

        // Returns true if all 4 child pointers are null,
    // implying that the node is a leaf
        bool IsLeaf()
        {
            return (NULL == L && NULL == M1 && NULL == M2 && NULL == R);
        }

        int ValueCount()
        {
            if (C) { return 3; }
            else if (B) { return 2; }
            else if (A) { return 1; }

            // Should never come here
            return 0;
        }
    };

    Node* m_root;

    void DeleteTree(Node* root)
    {
        if (root)
        {
            root->DeleteData();
            DeleteTree(root->L);
            DeleteTree(root->M1);
            DeleteTree(root->M2);
            DeleteTree(root->R);
            delete root;
        }
    }

    void Display(Node* n)
    {
        if (!n) { return; }
        // There will always be an A value with a subtree to left and right of it
        Display(n->L);
         
        cout << *n->A << endl;
        Display(n->M1);

        // There may or may not be a B and C value
        if (n->B)
        {
            
            cout << *n->B << endl;
            Display(n->M2);

            if (n->C)
            {
                
                cout << *n->C << endl;
                Display(n->R);
            }
        }
    }


public:
    dictionary(void)
    {
        m_root = NULL;
    }

    virtual ~dictionary(void)
    {
        DeleteTree(m_root);
        m_root = NULL;
    }

    /*****************************
     * IMPLEMENTED THIS FUNCTION *
     *****************************/
    bool Add(T& toCopyAndAdd)
    {
        // Create root node if 2-3-4 tree is empty
        if (!m_root)
        {
            m_root = new Node(new T(toCopyAndAdd));
            return true;
        }

        T key = toCopyAndAdd;

        Node* n_ptr = m_root;  // traversing pointer
        Node* parent = n_ptr;  // previous pointer

        while (n_ptr)
        {
            // Check for duplicates
            if (n_ptr->ContainsKey(key)) { return false; }


            /*** CASE 1: If it's 3 nodes split it ***/
            if (n_ptr->IsFull())
            {
                // Case 1a: If parent of node has 1 key
                if (1 == parent->ValueCount())
                {
                    if (*parent->A < *n_ptr->B)
                    {
                        parent->B = n_ptr->B;
                        parent->M1 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                        parent->M2 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    }
                    else
                    {
                        parent->B = parent->A;
                        parent->M2 = parent->M1;
                        parent->A = n_ptr->B;
                        parent->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                        parent->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    }

                    delete n_ptr;

                    if (key < *parent->A)
                        n_ptr = parent->L;
                    else if (key < *parent->B)
                        n_ptr = parent->M1;
                    else if (key > *parent->B)
                        n_ptr = parent->M2;
                    else
                        return false; // duplicate

                } // end Case 1a

                // Case 1b: If parent of node has 2 keys
                else if (2 == parent->ValueCount())
                {
                    if (*parent->B < *n_ptr->B)
                    {
                        parent->C = n_ptr->B;
                        parent->M2 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                        parent->R = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    }
                    else if (*parent->A < *n_ptr->B)
                    {
                        parent->C = parent->B;
                        parent->R = parent->M2;
                        parent->B = n_ptr->B;
                        parent->M1 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                        parent->M2 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    }
                    else
                    {
                        parent->C = parent->B;
                        parent->R = parent->M2;
                        parent->B = parent->A;
                        parent->M2 = parent->M1;
                        parent->A = n_ptr->B;
                        parent->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                        parent->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    }

                    delete n_ptr;

                    if (key < *parent->A)
                        n_ptr = parent->L;
                    else if (key < *parent->B)
                        n_ptr = parent->M1;
                    else if (key < *parent->C)
                        n_ptr = parent->M2;
                    else if (key > *parent->C)
                        n_ptr = parent->R;
                    else
                        return false; // duplicate
                } // end of Case 1b

                // Case 1c: If parent of node has 3 keys
                else
                {
                    m_root = new Node(n_ptr->B);
                    m_root->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
                    m_root->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
                    delete n_ptr;
                    parent = m_root;
                    if (key < *m_root->A)
                        n_ptr = m_root->L;
                    else if (key > *m_root->A)
                        n_ptr = m_root->M1;
                    else
                        return false; // duplicate
                } // end Case 1c

            } // end Case 1


            /*** CASE 2: Is it a leaf node ***/
            if (n_ptr->IsLeaf())
            {
                // Case 2a: Has 1 key
                if (1 == n_ptr->ValueCount())
                {
                    if (key > *n_ptr->A)
                    {
                        n_ptr->B = new T(toCopyAndAdd);
                        return true;
                    }
                    else if (key < *n_ptr->A)
                    {
                        T* temp = new T(toCopyAndAdd);
                        n_ptr->B = n_ptr->A;
                        n_ptr->A = temp;
                        return true;
                    }
                } // end Case 2a

                // Case 2b: Has 2 keys
                else if (2 == n_ptr->ValueCount())
                {
                    if (key > *n_ptr->B)
                    {
                        n_ptr->C = new T(toCopyAndAdd);
                        return true;
                    }
                    else if (key > *n_ptr->A)
                    {
                        T* temp = new T(toCopyAndAdd);
                        n_ptr->C = n_ptr->B;
                        n_ptr->B = temp;
                        return true;
                    }
                    else if (key < *n_ptr->A)
                    {
                        T* temp = new T(toCopyAndAdd);
                        n_ptr->C = n_ptr->B;
                        n_ptr->B = n_ptr->A;
                        n_ptr->A = temp;
                        return true;
                    }
                } // end Case 2b
            } // end Case 2


            /*** CASE 3: Not a leaf node ***/
            else
            {
                // Assigns previous pointer
                parent = n_ptr;

                // Case 3a: Has 1 key
                if (1 == n_ptr->ValueCount())
                {
                    if (key < *n_ptr->A)
                        n_ptr = n_ptr->L;
                    else
                        n_ptr = n_ptr->M1;
                }

                // Case 3b: Has 2 keys
                else if (2 == n_ptr->ValueCount())
                {
                    if (key < *n_ptr->A)
                        n_ptr = n_ptr->L;
                    else if (key < *n_ptr->B)
                        n_ptr = n_ptr->M1;
                    else
                        n_ptr = n_ptr->M2;
                }
            } // end Case 3

        } // end while


        // Should never get here!
        return false;
    }

    void Display(ostream& outstream)
    {
        if (!m_root)
        {
            outstream << "(tree is empty)";
        }
        Display(m_root);
    }

    /*****************************
     * IMPLEMENTED THIS FUNCTION *
     *****************************/
    
    T* Find(T key)
    {
        Node* n_ptr = m_root;

        T* result = n_ptr->FindsKey(key);

        if (result) { return result; }
        else
        {
            if (n_ptr->C && *n_ptr->C < key)
            {
                return Find(key, n_ptr->R);
            }
            else if (n_ptr->B && *n_ptr->B < key)
            {
                return Find(key, n_ptr->M2);
            }
            else if (*n_ptr->A < key)
            {
                return Find(key, n_ptr->M1);
            }
            else
            {
                return Find(key, n_ptr->L);
            }
        }
        
        return NULL;
    }

    T* Find(T const& key, Node*& n_ptr)
    {
        if (!n_ptr) {
            
            return NULL;
        }
        T* result = n_ptr->FindsKey(key);
        if (result) { return result; }
        else
        {
            if (n_ptr->C && *n_ptr->C < key)
            {
                return Find(key, n_ptr->R);
            }
            else if (n_ptr->B && *n_ptr->B < key)
            {
                return Find(key, n_ptr->M2);
            }
            else if (*n_ptr->A < key)
            {
                return Find(key, n_ptr->M1);
            }
            else
            {
                return Find(key, n_ptr->L);
            }
        }
    }




    /*****************************
     * IMPLEMENTED THIS FUNCTION *
     *****************************/
    bool Remove(T key)
    {
        if (!m_root) { return false; }

        // Implement the rest of this function for extra credit
        // Return false if the value is not present in the tree
        // Don't forget the rules for fusing nodes

        Node* n_ptr = m_root;
        Node* parent = m_root;

        T* the_item = NULL;
        Node* n_item = NULL;

        while (n_ptr)
        {
            // Case 1: If root and root has single key
            // and both descendents has single keys
            if (n_ptr == m_root && n_ptr->ValueCount() == 1 &&
                n_ptr->L->ValueCount() == 1 &&
                n_ptr->M1->ValueCount() == 1)
            {
                // Set value for C and R & M2 pointers
                Node* right = n_ptr->M1;
                n_ptr->C = right->A;
                n_ptr->M2 = right->L;
                n_ptr->R = right->M1;
                delete right;

                // Set value for B
                n_ptr->B = n_ptr->A;

                // Set value for A and L & M1 pointers
                Node* left = n_ptr->L;
                n_ptr->A = left->A;
                n_ptr->L = left->L;
                n_ptr->M1 = left->M1;
                delete left;

                parent = m_root;
                n_ptr = m_root;
            } // end Case 1

            // Cases 2: Transformations (Ensure the_item does not occur in a 2-node)
            // a) Transform each node encountered into a 3 node or a 4 node
            if (n_ptr->ValueCount() == 1)
            {
                // single key on L pointer
                if (parent->L == n_ptr)
                {
                    // Parent has more than 1 key and M1 has 1 key, FUSE IT
                    if (parent->M1->ValueCount() == 1)
                    {
                        // set C and pointers R & M2
                        parent->M1->C = parent->M1->A;
                        parent->M1->R = parent->M1->M1;
                        parent->M1->M2 = parent->M1->L;
                        // set B
                        parent->M1->B = parent->A;
                        // set A and pointer L & M1
                        parent->M1->A = n_ptr->A;
                        parent->M1->L = n_ptr->L;
                        parent->M1->M1 = n_ptr->M1;

                        delete parent->L;
                        parent->A = parent->B;
                        parent->B = parent->C;
                        parent->C = NULL;
                        parent->L = parent->M1;
                        parent->M1 = parent->M2;
                        parent->M2 = parent->R;
                        parent->R = NULL;

                        n_ptr = parent->L;
                    }

                    // Sibling has more than 1 key. Borrow
                    else if (parent->M1->ValueCount() >= 2)
                    {
                        // set B and pointer M2
                        n_ptr->B = parent->A;
                        n_ptr->M2 = parent->M1->L;

                        // set parent A
                        parent->A = parent->M1->A;

                        // set A, B, C of pointer M1 and its pointers
                        parent->M1->A = parent->M1->B;
                        parent->M1->B = parent->M1->C;
                        parent->M1->C = NULL;
                        parent->M1->L = parent->M1->M1;
                        parent->M1->M1 = parent->M1->M2;
                        parent->M1->M2 = parent->M1->R;
                        parent->M1->R = NULL;
                    }
                } // end single key on L pointer

                // single key on M1 pointer
                else if (parent->M1 == n_ptr)
                {
                    // L & M2 have single key, FUSE L & M1
                    if (parent->L->ValueCount() == 1 && parent->M2->ValueCount() == 1)
                    {
                        // set C and pointers R & M2
                        n_ptr->C = n_ptr->A;
                        n_ptr->R = n_ptr->M1;
                        n_ptr->M2 = n_ptr->L;
                        // set B
                        n_ptr->B = parent->A;
                        // set A and pointers L & M1
                        n_ptr->A = parent->L->A;
                        n_ptr->M1 = parent->L->M1;
                        n_ptr->L = parent->L->L;

                        delete parent->L;
                        parent->A = parent->B;
                        parent->B = parent->C;
                        parent->C = NULL;
                        parent->L = parent->M1;
                        parent->M1 = parent->M2;
                        parent->M2 = parent->R;
                        parent->R = NULL;

                        // n_ptr already good
                    }

                    // borrow from M2
                    else if (parent->L->ValueCount() < parent->M2->ValueCount())
                    {
                        // set B and pointer M2
                        n_ptr->B = parent->B;
                        n_ptr->M2 = parent->M2->L;

                        // set parent B
                        parent->B = parent->M2->A;

                        // set M2 values and pointers
                        parent->M2->A = parent->M2->B;
                        parent->M2->B = parent->M2->C;
                        parent->M2->C = NULL;
                        parent->M2->L = parent->M2->M1;
                        parent->M2->M1 = parent->M2->M2;
                        parent->M2->M2 = parent->M2->R;
                        parent->M2->R = NULL;
                    }

                    // borrow from L
                    else
                    {
                        // set B and pointer M1 & M2
                        n_ptr->B = n_ptr->A;
                        n_ptr->M2 = n_ptr->M1;
                        n_ptr->M1 = n_ptr->L;

                        // set A, parent A and pointer L
                        n_ptr->A = parent->A;
                        if (parent->L->C)
                        {
                            n_ptr->L = parent->L->R;
                            parent->A = parent->L->C;
                            parent->L->C = NULL;
                            parent->L->R = NULL;
                        }
                        else
                        {
                            n_ptr->L = parent->L->M2;
                            parent->A = parent->L->B;
                            parent->L->B = NULL;
                            parent->L->M2 = NULL;
                        }
                    }

                } // end single key on M1 pointer

                // single key on M2 pointer
                else if (parent->M2 == n_ptr)
                {
                    // M1 & R have single key, FUSE M2 & R
                    if (parent->R && parent->M1->ValueCount() == 1 &&
                        parent->R->ValueCount() == 1)
                    {
                        // set B
                        n_ptr->B = parent->C;
                        parent->C = NULL;

                        // set C and pointer M2 & R
                        n_ptr->C = parent->R->A;
                        n_ptr->M2 = parent->R->L;
                        n_ptr->R = parent->R->M1;

                        delete parent->R;
                        parent->R = NULL;

                        // n_ptr is good
                    }

                    // borrow from R
                    else if (parent->R &&
                        parent->R->ValueCount() >= parent->M1->ValueCount())
                    {
                        // set B and pointer M2
                        n_ptr->B = parent->C;
                        n_ptr->M2 = parent->R->L;

                        // set parent C
                        parent->C = parent->R->A;

                        // set R values and pointers
                        parent->R->A = parent->R->B;
                        parent->R->B = parent->R->C;
                        parent->R->C = NULL;
                        parent->R->L = parent->R->M1;
                        parent->R->M1 = parent->R->M2;
                        parent->R->M2 = parent->R->R;
                        parent->R->R = NULL;

                        // n_ptr is good
                    }

                    // borrow from M1
                    else
                    {
                        // set B and pointer M1 & M2
                        n_ptr->B = n_ptr->A;
                        n_ptr->M2 = n_ptr->M1;
                        n_ptr->M1 = n_ptr->L;

                        // set A, parent B and pointer L
                        n_ptr->A = parent->B;
                        if (parent->M1->C)
                        {
                            n_ptr->L = parent->M1->R;
                            parent->B = parent->M1->C;
                            parent->M1->R = NULL;
                            parent->M1->C = NULL;
                        }
                        else
                        {
                            n_ptr->L = parent->M1->M2;
                            parent->B = parent->M1->B;
                            parent->M1->M2 = NULL;
                            parent->M1->B = NULL;
                        }
                    }

                } // end single key on M2 pointer

                // single key on R pointer
                else if (parent->R == n_ptr)
                {
                    // M2 has a single key, FUSE M2 & R
                    if (parent->M2->ValueCount() == 1)
                    {
                        // set B
                        parent->M2->B = parent->C;
                        parent->C = NULL;

                        // set C and pointer M2 & R
                        parent->M2->C = n_ptr->A;
                        parent->M2->M2 = n_ptr->L;
                        parent->M2->R = n_ptr->M1;

                        n_ptr->A = NULL;
                        n_ptr->L = NULL;
                        n_ptr->M1 = NULL;
                        delete parent->R;

                        n_ptr = parent->M2;
                    }

                    // borrow from M2
                    else if (parent->M2->ValueCount() >= 2)
                    {
                        // set B and pointer M1 & M2
                        n_ptr->B = n_ptr->A;
                        n_ptr->M2 = n_ptr->M1;
                        n_ptr->M1 = n_ptr->L;

                        // set A, parent C and pointer L
                        n_ptr->A = parent->C;
                        if (parent->M2->C)
                        {
                            n_ptr->L = parent->M2->R;
                            parent->C = parent->M2->C;
                            parent->M2->R = NULL;
                            parent->M2->C = NULL;
                        }
                        else
                        {
                            n_ptr->L = parent->M2->M2;
                            parent->C = parent->M2->B;
                            parent->M2->M2 = NULL;
                            parent->M2->C = NULL;
                        }
                    }

                } // end single key on R pointer
            }

            // Case 3: Item hasn't been found
            // but this node contains key
            if (!the_item && n_ptr->ContainsKey(key) && !n_ptr->IsLeaf())
            {
                parent = n_ptr;
                n_item = n_ptr;      // points to the node where the key is found
                if (n_ptr->C && key == *n_ptr->C)
                {
                    the_item = n_ptr->C;
                    n_ptr = n_ptr->R;  // go to right of key
                }
                else if (n_ptr->B && key == *n_ptr->B)
                {
                    the_item = n_ptr->B;
                    n_ptr = n_ptr->M2; // go to right of key
                }
                else
                {
                    the_item = n_ptr->A;
                    n_ptr = n_ptr->M1; // go to right of key
                }
            } // Case 3

            // Case 4: Item already found
            // and node contains key (assume its a leaf)
            // The key here is the inorder successor key
            else if ((the_item && n_ptr->ContainsKey(key)) ||
                (n_ptr->ContainsKey(key) && n_ptr->IsLeaf()))
            {
                while (1)
                {
                    // # of keys greater than or equal to 2 keys
                    // delete the key
                    if (n_ptr->ValueCount() >= 2)
                    {
                        // key is C
                        if (n_ptr->C && key == *n_ptr->C)
                        {
                            delete n_ptr->C;
                            n_ptr->C = NULL;
                        }
                        // key is B
                        else if (n_ptr->B && key == *n_ptr->B)
                        {
                            delete n_ptr->B;
                            n_ptr->B = n_ptr->C;
                            n_ptr->C = NULL;
                        }
                        // key is A
                        else // <----- PROBLEM MIGHT BE HERE
                        {
                            delete n_ptr->A;
                            n_ptr->A = n_ptr->B;
                            n_ptr->B = n_ptr->C;
                            n_ptr->C = NULL;
                        }
                        return true;
                    }

                    // key on R pointer
                    else if (parent->R && parent->R == n_ptr)
                    {
                        // M2 & R has single keys. FUSE IT
                        if (parent->M2->ValueCount() == 1 && parent->R->ValueCount() == 1)
                        {
                            parent->M2->B = parent->C;
                            parent->M2->C = n_ptr->A;
                            n_ptr->A = NULL;
                            delete parent->R;
                            parent->R = NULL;
                            parent->C = NULL;
                            n_ptr = parent->M2;
                        }
                        // R has one key and M2 has more than one key
                        else
                        {
                            // borrow from parent C
                            n_ptr->B = n_ptr->A;
                            n_ptr->A = parent->C;
                            // parent C borrow C from M2
                            if (parent->M2->C)
                            {
                                parent->C = parent->M2->C;
                                parent->M2->C = NULL;
                            }
                            // parent C borrow B from M2
                            else
                            {
                                parent->C = parent->M2->B;
                                parent->M2->B = NULL;
                            }
                        }
                    }

                    // key on M2 pointer
                    else if (parent->M2 && parent->M2 == n_ptr)
                    {
                        // M1, M2 & R has single keys. FUSE M2 & R
                        if ((parent->M1->ValueCount() == 1 &&
                            parent->M2->ValueCount() == 1 && !parent->C) ||
                            (parent->R && parent->M1->ValueCount() == 1 &&
                                parent->M2->ValueCount() == 1 && parent->R->ValueCount() == 1))
                        {
                            n_ptr->B = parent->C;
                            n_ptr->C = parent->R->A;
                            parent->R->A = NULL;
                            delete parent->R;
                            parent->R = NULL;
                            parent->C = NULL;
                        }
                        // M2 has one key and R has more keys than M1
                        else if (parent->R &&
                            parent->R->ValueCount() >= parent->M1->ValueCount())
                        {
                            // borrow from parent C
                            n_ptr->B = parent->C;
                            // parent C borrow A from R
                            parent->C = parent->R->A;
                            parent->R->A = parent->R->B;
                            parent->R->B = parent->R->C;
                            parent->R->C = NULL;
                        }
                        // M2 has one key and M1 has more keys than R
                        else
                        {
                            // borrow from parent B
                            n_ptr->B = n_ptr->A;
                            n_ptr->A = parent->B;
                            // parent B borrow C from M1
                            if (parent->M1->C)
                            {
                                parent->B = parent->M1->C;
                                parent->M1->C = NULL;
                            }
                            // parent B borrow B from M1
                            else
                            {
                                parent->B = parent->M1->B;
                                parent->M1->B = NULL;
                            }
                        }
                    }

                    // key on M1 pointer
                    else if (parent->M1 && parent->M1 == n_ptr)
                    {
                        // L, M1, & M2 has single keys. FUSE L & M1
                        if (parent->L->ValueCount() == 1 &&
                            parent->M1->ValueCount() == 1 &&
                            parent->M2->ValueCount() == 1)
                        {
                            n_ptr->C = n_ptr->A;
                            n_ptr->B = parent->A;
                            n_ptr->A = parent->L->A;
                            parent->L->A = NULL;
                            delete parent->L;
                            parent->A = parent->B;
                            parent->B = parent->C;
                            parent->L = parent->M1;
                            parent->M1 = parent->M2;
                            parent->M2 = parent->R;
                            parent->R = NULL;
                        }
                        // M1 has one key and M2 has more keys than L
                        else if (parent->L->ValueCount() < parent->M2->ValueCount())
                        {
                            // borrow from parent B
                            n_ptr->B = parent->B;
                            // parent B borrow A from M2
                            parent->B = parent->M2->A;
                            parent->M2->A = parent->M2->B;
                            parent->M2->B = parent->M2->C;
                            parent->M2->C = NULL;
                        }
                        // M1 has one key and L has more keys than M2
                        else
                        {
                            // borrow from parent A
                            n_ptr->B = n_ptr->A;
                            n_ptr->A = parent->A;
                            // parent A borrow C from L
                            if (parent->L->C)
                            {
                                parent->A = parent->L->C;
                                parent->L->C = NULL;
                            }
                            // parent A borrow B from L
                            else
                            {
                                parent->A = parent->L->B;
                                parent->L->B = NULL;
                            }
                        }
                    }

                    // key on L pointer
                    else
                    {
                        // L, M1 has single keys. FUSE IT!
                        if (parent->L->ValueCount() == 1 && parent->M1->ValueCount() == 1)
                        {
                            parent->M1->C = parent->M1->A;
                            parent->M1->B = parent->A;
                            parent->M1->A = parent->L->A;
                            parent->L->A = NULL;
                            delete parent->L;
                            parent->A = parent->B;
                            parent->B = parent->C;
                            parent->L = parent->M1;
                            parent->M1 = parent->M2;
                            parent->M2 = parent->R;
                            parent->R = NULL;

                            n_ptr = parent->M1;
                        }
                        // L has one key, borrow from M1
                        else
                        {
                            // borrow from parent A
                            n_ptr->B = parent->A;
                            // parent A borrow A from M1
                            parent->A = parent->M1->A;
                            parent->M1->A = parent->M1->B;
                            parent->M1->B = parent->M1->C;
                            parent->M1->C = NULL;
                        }
                    }
                } // end while (1)
            } // end Case 4

            // Case: Item not found yet so continue looking
            if (!the_item)
            {
                parent = n_ptr;
                if (n_ptr->C && key > *n_ptr->C) { n_ptr = n_ptr->R; }
                else if (n_ptr->B && key > *n_ptr->B) { n_ptr = n_ptr->M2; }
                else if (key > *n_ptr->A) { n_ptr = n_ptr->M1; }
                else { n_ptr = n_ptr->L; }
            }
            // Case: Item found. Look for inorder successor
            else
            {
                if (n_ptr->L == NULL) // n_ptr key A is the inorder successor
                {
                    if (n_item->A == the_item)
                    {
                        delete n_item->A;
                        n_item->A = new T(*(n_ptr->A));
                    }
                    else if (n_item->B == the_item)
                    {
                        delete n_item->B;
                        n_item->B = new T(*(n_ptr->A));
                    }
                    else
                    {
                        delete n_item->C;
                        n_item->C = new T(*(n_ptr->A));
                    }

                    key = *n_ptr->A;
                }
                else
                {
                    parent = n_ptr;
                    n_ptr = n_ptr->L;   // go to left of key
                }
            }
        }

        return false;
    }

};
