#ifndef RBTREEDEFINE
#define RBTREEDEFINE

#include <iomanip>
#include <iostream>
using namespace std;

enum RBTColor{RED , BLACK};

template <class T>
class RBTNode
{
public:
    T key;              //关键字(键值)
    RBTColor color;     //颜色
    RBTNode *parent;    //父节点
    RBTNode *left;      //左孩子
    RBTNode *right;     //右孩子

    RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l,RBTNode *r):key(value),color(c),parent(p),left(l),right(r){}
};

template <class T>
class RBTree
{
private:
    RBTNode<T> *mRoot;  //根节点

public:
    RBTree();
    ~RBTree();

    void preOrder();
    void inOrder();
    void postOrder();

    RBTNode<T>* search(T key);      //递归实现，“红黑树”中键值为key的节点
    RBTNode<T>* iterativeSearch(T key); //非递归实现，“红黑树”中键值为key的节点

    T minimum();         //查找最小结点：返回最小结点的键值
    T maximum();         //查找最大结点：返回最大结点的键值

    //找后继、前驱结点
    RBTNode<T>* successor(RBTNode<T> *x);
    RBTNode<T>* predecessor(RBTNode<T> *x);

    void insert(T key);
    void remove(T key);
    void destroy();

    void print();

private:
    void preOrder(RBTNode<T>* tree) const;
    void inOrder(RBTNode<T>* tree) const;
    void postOrder(RBTNode<T>* tree) const;

    RBTNode<T>* search(RBTNode<T>* x, T key) const;
    RBTNode<T>* iterativeSearch(RBTNode<T>* x, T key) const;

    // 查找最小结点：返回tree为根结点的红黑树的最小结点。
    RBTNode<T>* minimum(RBTNode<T>* tree);
    // 查找最大结点：返回tree为根结点的红黑树的最大结点。
    RBTNode<T>* maximum(RBTNode<T>* tree);


    void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
    void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);
    void insert(RBTNode<T>* &root, RBTNode<T>* node);
    void insertFixUp(RBTNode<T>* &root, RBTNode<T>* node);
    void remove(RBTNode<T>* &root, RBTNode<T>* node);
    void removeFixUp(RBTNode<T>* &root, RBTNode<T>* node, RBTNode<T>* parent);

    void destroy(RBTNode<T>* &tree);

    void print(RBTNode<T>* tree, T key, int direction);

#define rb_parent(r) ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)    ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r) do { (r)->color = BLACK;}while(0)
#define rb_set_red(r)   do{(r)->color = RED;}while(0)
#define rb_set_parent(r,p)    do{(r)->parent = (p);} while(0)
#define rb_set_color(r,c)   do{(r)->color = (c);} while(0)
};

template <class T>
RBTree<T>::RBTree():mRoot(nullptr)
{
    mRoot = nullptr;
}

template <class T>
RBTree<T>::~RBTree()
{
    destroy();
}


//前序
template <class T>
void RBTree<T>::preOrder(RBTNode<T> *tree) const
{
    if(tree != nullptr)
    {
        cout<< tree->key << " ";
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

template <class T>
void RBTree<T>::preOrder()
{
    preOrder(mRoot);
}


//中序
template <class T>
void RBTree<T>::inOrder(RBTNode<T> *tree) const
{
    if(tree != nullptr)
    {
        inOrder(tree->left);
        cout<< tree->key << " ";
        inOrder(tree->right);
    }
}

template <class T>
void RBTree<T>::inOrder()
{
    inOrder(mRoot);
}


//后序
template <class T>
void RBTree<T>::postOrder(RBTNode<T> *tree) const
{
    if(tree != nullptr)
    {
        postOrder(tree->left);
        postOrder(tree->right);
        cout<< tree->key << " ";
    }
}

template <class T>
void RBTree<T>::postOrder()
{
    postOrder(mRoot);
}



template <class T>
void RBTree<T>::destroy(RBTNode<T> *&tree)
{
    if(tree == nullptr)
        return;
    if(tree->left != nullptr)
        return destroy(tree->left);
    if(tree->right != nullptr)
        return destroy(tree->right);

    delete tree;
    tree = nullptr;
}

template <class T>
void RBTree<T>::destroy()
{
    destroy(mRoot);
}


/*
* 打印"二叉查找树"
*
* key        -- 节点的键值
* direction  --  0，表示该节点是根节点;
*               -1，表示该节点是它的父结点的左孩子;
*                1，表示该节点是它的父结点的右孩子。
*/

template <class T>
void RBTree<T>::print(RBTNode<T>* tree,T key,int direction)
{
    if(tree != nullptr)
    {
        if(direction == 0)
            cout<< setw(2) << tree->key << "(B) is root" << endl;
        else
        {
            cout<< setw(2) << tree->key << (rb_is_red(tree)?"(R)":"(B)") << " is " << setw(2) <<
                   key << "'s " << setw(12) << (direction == 1? "right child":"left child") << endl;
        }
        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}

template <class T>
void RBTree<T>::print()
{
    if(mRoot != nullptr)
        print(mRoot, mRoot->key, 0);
}



//-----------------------------------------------------------------insert

/*
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *	 root 红黑树的根结点
 *	 node 插入的结点		// 对应《算法导论》中的node
 */

template <class T>
void RBTree<T>::insert(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T>* y = nullptr;
    RBTNode<T>* x = root;

    while(x != nullptr)
    {
        y = x;
        if(node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;

    if(y != nullptr)
    {
        if(node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
    else
        root = node;

    node->color = RED;

    //修正为正式二叉查找树
    insertFixUp(root,node);
}


/*
 * 将结点(key为节点键值)插入到红黑树中
 *
 * 参数说明：
 *	 tree 红黑树的根结点
 *	 key 插入结点的键值
 */

template <class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T>* z = nullptr;

    if((z = new RBTNode<T>(key,BLACK,nullptr,nullptr,nullptr)) == nullptr)
        return;

    insert(mRoot,z);
}



/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *	 root 红黑树的根
 *	 node 插入的结点		// 对应《算法导论》中的z
 */

template <class T>
void RBTree<T>::insertFixUp(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *parent,*gparent;

    //若父节点存在且为红色
    while((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        if(parent == gparent->left)
        {
            //case1:叔叔节点为红
            {
                RBTNode<T>* uncle = gparent->right;
                if(uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }


            //case2:叔叔黑色，当前节点右孩子
            if(parent->right == node)
            {
                RBTNode<T>* tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            //case3：叔叔黑色，当前节点是左孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root,gparent);
        }
        //若z的父节点是z的祖父节点的右孩子
        else
        {
            //case1:叔叔节点为红色
            {
                RBTNode<T>* uncle = gparent->left;
                if(uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            if(parent->left == node )
            {
                RBTNode<T>* tmp;
                rightRotate(root,parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root,gparent);
        }
    }

    rb_set_black(root);
}


//------------------------------------------------------------------remove
/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *	 root 红黑树的根结点
 *	 node 删除的结点
 */

template <class T>
void RBTree<T>::remove(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T>* child,*parent;
    RBTColor color;

    //被删除节点的“左右孩子都不为空”
    if((node->left != nullptr) && (node->right != nullptr))
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBTNode<T>* replace = node;

        replace = replace->right;
        while(replace->left != nullptr)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if(rb_parent(node))
        {
            if(rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            root = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        color = rb_color(replace);

        if(parent == node)
        {
            parent = replace;
        }
        else
        {
            if(child)
                rb_set_parent(child, parent);

            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if(color == BLACK)
            removeFixUp(root,child,parent);

        delete node;
        return;
    }

    if(node->left != nullptr)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    color = node->color;

    if(child)
        child->parent = parent;

    if(parent)
    {
        if(parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if(color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/*
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *	 tree 红黑树的根结点
 */

template <class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T> *node;

    if((node = search(mRoot, key)) != nullptr)
        remove(mRoot, node);
}



/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *	 root 红黑树的根
 *	 node 待修正的节点
 */

template <class T>
void RBTree<T>::removeFixUp(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent)
{
    RBTNode<T> *other;

    while((!node || rb_is_black(node)) && node != root)
    {
        if(parent->left == node)
        {
            other = parent->right;
            if(rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root,parent);
                other = parent->right;
            }
            if((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if(!other->right || rb_is_black(other->right))
                {
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }

                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if(rb_is_red(other))
            {
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right)))
            {
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if(!other->left || rb_is_black(other->left))
                {
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }

                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if(node)
        rb_set_black(node);
}


//-------------------------------------------------------------------------------rotation
/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *	       px							  px
 *	      /							     /
 *	     x							    y
 *     /   \	  --(左旋)-->		   / \
 *    lx    y						  x   ry
 *	 /  \					         / \
 *	ly  ry					        lx  ly
 *
 *
 */

template <class T>
void RBTree<T>::leftRotate(RBTNode<T> *&root, RBTNode<T> *x)
{
    RBTNode<T>* y = x->right;

    x->right = y->left;

    if(y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;

    if(x->parent == nullptr)
        root = y;
    else
    {
        if(x->parent->left == x)
            x->parent->left = y;    //如果x是它父节点的左孩子，将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

template <class T>
void RBTree<T>::rightRotate(RBTNode<T> *&root, RBTNode<T> *y)
{
    RBTNode<T>* x = y->left;

    y->left = x->right;

    if(x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;

    if(y->parent == nullptr)
        root = x;
    else
    {
        if(y->parent->left == y)
            y->parent->left = x;
        else
            y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}


//------------------------------------------------------------search

template <class T>
RBTNode<T>* RBTree<T>::search(RBTNode<T> *x, T key) const
{
    if(x == nullptr || x->key == key)
        return x;
    if(key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template <class T>
RBTNode<T>* RBTree<T>::search(T key)
{
    search(mRoot, key);
}



//非递归

template <class T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T> *x, T key) const
{
    while((x != nullptr) && (x->key != key))
    {
        if(key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

template <class T>
RBTNode<T>* RBTree<T>::iterativeSearch(T key)
{
    iterativeSearch(mRoot, key);
}




//查找最小节点

template <class T>
RBTNode<T>* RBTree<T>::minimum(RBTNode<T> *tree)
{
    if(tree == nullptr)
        return nullptr;
    while(tree->left != nullptr)
        tree = tree->left;
    return tree;
}

template <class T>
T RBTree<T>::minimum()
{
    RBTNode<T>* p = minimum(mRoot);
    if(p != nullptr)
        return p->key;
    return (T)nullptr;
}


//查找最大结点

template <class T>
RBTNode<T>* RBTree<T>::maximum(RBTNode<T>* tree)
{
    if(tree == nullptr)
        return nullptr;

    while(tree->right != nullptr)
        tree = tree->right;
    return tree;
}

template <class T>
T RBTree<T>::maximum()
{
    RBTNode<T> *p = maximum(mRoot);
    if(p != nullptr)
        return p->key;

    return (T)nullptr;
}


//--------------------------------------------------------------successor_predecessor

template <class T>
RBTNode<T>* RBTree<T>::successor(RBTNode<T> *x)
{
    if(x->right != nullptr)
        return minimum(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    RBTNode<T>* y = x->parent;
    while((y != nullptr) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

template <class T>
RBTNode<T>* RBTree<T>::predecessor(RBTNode<T> *x)
{
    if(x->left != nullptr)
        return maximum(x->left);

    RBTNode<T>* y = x->parent;
    while((y != nullptr) && (x == y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}



#endif // RBTREEDEFINE

