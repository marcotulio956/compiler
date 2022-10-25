#include "TreeNode.h"

template <class T>
TreeNode<T>::TreeNode(const T &data):
    m_data(data) {}


template <class T>
void TreeNode<T>::addChild(TreeNode<T>* node){m_children.push_back(node);}
// const T& nodeData = node->getData();

template <class T>
void TreeNode<T>::addChild(T child_name){
    TreeNode<T>* child_ptr = new TreeNode(child_name);
    m_children.push_back(child_ptr);
}

template class TreeNode<std::string>;
