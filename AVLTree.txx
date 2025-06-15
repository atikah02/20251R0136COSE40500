//
// Implementation
//
template <typename E>					// constructor
  AVLTree<E>::AVLTree() : ST() { }

template <typename E>					// node height utility
  int AVLTree<E>::height(const TPos& v) const
    { return (v.isExternal() ? 0 : (*v).height()); }

template <typename E>					// set height utility
  void AVLTree<E>::setHeight(TPos v) {
    int hl = height(v.left());
    int hr = height(v.right());
    (*v).setHeight(1 + std::max(hl, hr));			// max of left & right
  }

template <typename E>					// is v balanced?
  bool AVLTree<E>::isBalanced(const TPos& v) const {	
    int bal = height(v.left()) - height(v.right());
    return ((-1 <= bal) && (bal <= 1));
  }

template <typename E>					// get tallest grandchild
  typename AVLTree<E>::TPos AVLTree<E>::tallGrandchild(const TPos& z) const {
    TPos zl = z.left();
    TPos zr = z.right();
    if (height(zl) >= height(zr))			// left child taller
      if (height(zl.left()) >= height(zl.right()))
        return zl.left();
      else
        return zl.right();
    else 						// right child taller
      if (height(zr.right()) >= height(zr.left()))
        return zr.right();
      else
        return zr.left();
  }


//
// ToDo
//

// erase the node with key k
template <typename E>					// remove key k entry
  void AVLTree<E>::erase(const K& k) {

    TPos u = ST::finder(k, ST::root()); // find node with key k
    if (u.isExternal()) return;         // key not found
    TPos p = u.parent();                // save parent before deletion
    ST::eraser(u);                      // delete node using base class method
    rebalance(p);                       
  }


// insert a (key, value) pair into the AVL tree
template <typename E>					// insert (k,x)
  typename AVLTree<E>::Iterator AVLTree<E>::insert(const K& k, const V& x) {
    TPos v = ST::finder(k, ST::root());  // find key or insertion point

    if (v.isInternal()) {
        (*v).setValue(x);                // key exists → replace value
        return Iterator(v);
    }

    ST::inserter(k, x);                 // insert new key-value pair
    rebalance(v.parent());              // rebalance from parent
    return Iterator(v);                 // return iterator to inserted node
  }
  

// restore AVL property
template <typename E>					// rebalancing utility
  void AVLTree<E>::rebalance(const TPos& v) {
    TPos z = v;
    TPos lastZ = nullptr;

    while (!z.isRoot()) {
        TPos nextZ = z.parent();
        if (lastZ == z) {
            std::cout << "[rebalance] Infinite loop detected at key: " << (*z).key() << std::endl;
            break;
        }

        // std::cout << "  [rebalance] Checking node with key: " << (*z).key() << std::endl;
        setHeight(z);

        if (!isBalanced(z)) {
            // std::cout << "  [rebalance] Unbalanced at key: " << (*z).key() << std::endl;
            TPos x = tallGrandchild(z);
            // std::cout << "  [rebalance] Rotating around grandchild with key: " << (*x).key() << std::endl;
            z = restructure(x);
            setHeight(z.left());
            setHeight(z.right());
            setHeight(z);
        }

        lastZ = z;
        z = nextZ;
    }
  }


// fix imbalance at grandparent z by rotating around child y and grandchild x = v
template <typename E>				// Binary Search Tree Rotation
  typename AVLTree<E>::TPos AVLTree<E>::restructure(const TPos& v) {
    TPos x = v;
    TPos y = x.parent();
    TPos z = y.parent();

    // Identify a, b, c and T0, T1, T2, T3
    TPos a, b, c;
    TPos T0, T1, T2, T3;

    // Case: x < y < z (Left-Left)
    if ((x == y.left()) && (y == z.left())) {
        a = x; b = y; c = z;
        T0 = a.left(); T1 = a.right();
        T2 = b.right(); T3 = c.right();
    }
    // Case: z.left() > x (Left-Right)
    else if ((x == y.right()) && (y == z.left())) {
        a = y; b = x; c = z;
        T0 = a.left(); T1 = b.left();
        T2 = b.right(); T3 = c.right();
    }
    // Case: z.right() < x (Right-Right)
    else if ((x == y.right()) && (y == z.right())) {
        a = z; b = y; c = x;
        T0 = a.left(); T1 = b.left();
        T2 = c.left(); T3 = c.right();
    }
    // Case: x < y < z.right() (Right-Left)
    else {
        a = z; b = x; c = y;
        T0 = a.left(); T1 = b.left();
        T2 = b.right(); T3 = c.right();
    }

    // Relink b as new root
    if (z.isRoot()) {
      b.v->par = nullptr;       // special case: rotating at root
    }
    else {
        TPos zParent = z.parent();
        if (z == zParent.left()) zParent.v->left = b.v;
        else zParent.v->right = b.v;
        b.v->par = zParent.v;
    }

    // Relink children
    b.v->left = a.v; a.v->par = b.v;
    b.v->right = c.v; c.v->par = b.v;

    a.v->left = T0.v; if (T0.v) T0.v->par = a.v;
    a.v->right = T1.v; if (T1.v) T1.v->par = a.v;
    c.v->left = T2.v; if (T2.v) T2.v->par = c.v;
    c.v->right = T3.v; if (T3.v) T3.v->par = c.v;

    return b; // new subtree root
  }

