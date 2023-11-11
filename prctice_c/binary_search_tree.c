#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
  int key;
  struct node_t *left;
  struct node_t *right;
} node_t;


node_t* bstree_search(node_t *root, int key) {
    if (root == NULL || key == root->key) 
        return root;
    else if (key < root->key) 
        return bstree_search(root->left, key);
    else 
        return bstree_search(root->right, key);
}


node_t* find_minimum(node_t *root) {
    if (root == NULL)
        return NULL;
    else if (root->left != NULL)
        return find_minimum(root->left);
    return root;
}


node_t* new_node(int key) {
    node_t *p;
    p = malloc(sizeof(node_t));
    p->key = key;
    p->left = NULL;
    p->right = NULL;

    return p;
}


node_t* insert(node_t *root, int key) {
    if (root == NULL)
        return new_node(key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else    
        root->left = insert(root->left, key);
    return root;
}


node_t* delete(node_t *root, int key) {
    if (root == NULL) 
        return NULL;
    if (key > root->key)
        root->right = delete(root->right, key);
    else if (key < root->key)
        root->left = delete(root->left, key);
    else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        else if (root->left == NULL || root->right == NULL) {
            node_t* temp;
            if (root->left == NULL)
                temp = root->right;
            else 
                temp = root->left;
            free(root);
            return temp;
        }
        else {
            node_t *temp = find_minimum(root->right);
            root->key = temp->key;
            root->right = delete(root->right, temp->key);
        }
    }
    return root;
}


void inorder(struct node_t *root)
{
    if(root!=NULL)
    {
        inorder(root->left); 
        printf(" %d ", root->key); 
        inorder(root->right);
    }
}


int main() {
    node_t* root;
    root = new_node(20);
    insert(root, 5);
    insert(root, 1);
    insert(root, 15);
    insert(root, 9);
    insert(root, 7);
    insert(root, 12);
    insert(root, 30);
    insert(root, 25);
    insert(root, 40);
    insert(root, 45);
    insert(root, 42);

    inorder(root);
    printf("\n");

    root = delete(root, 45);

    root = delete(root, 9);

    inorder(root);

    return 0;
}

