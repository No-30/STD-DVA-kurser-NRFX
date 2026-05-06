#include <assert.h>
#include <stdlib.h>
#include "BSTree.h"
#include <math.h>
#include "stdHeader.h"

/*Det är helt tillåtet att lägga till egna hjälpfunktioner men de befintliga funktionerna ska implementeras enligt instruktionerna*/

/* Statiska hjälpfunktioner används av andra funktioner i trädet och ska inte ligga i interfacet (användaren behöver inte känna till dessa) */

/* Skapar en trädnod med det givna datat. Denna funktion bör vara den enda som använder malloc för att skapa en ny treeNode*/
static struct treeNode* create_tree_node(int data)
{
    struct treeNode* node = malloc(sizeof(struct treeNode));
    assert(node != NULL);
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void write_sorted_to_array_helper(const BSTree tree, int arr[], int* index)
{
    if (tree == NULL)
    {
        return;
    }

    write_sorted_to_array_helper(tree->left, arr, index);
    arr[(*index)++] = tree->data;
    write_sorted_to_array_helper(tree->right, arr, index);
}

/* Returnerar en dynamiskt allokerad array som innehåller trädets data sorterat */
static int* write_sorted_to_array(const BSTree tree)
{
    int count = number_of_nodes(tree);
    int* arr = malloc(sizeof(int) * count);
    assert(arr != NULL || count == 0);

    int index = 0;
    write_sorted_to_array_helper(tree, arr, &index);
    return arr;
}

/* Bygger upp ett sorterat, balanserat träd från en sorterad array */
static void build_tree_sorted_from_array(BSTree* tree, const int arr[], int size)
{
    if (size <= 0)
    {
        *tree = NULL;
        return;
    }

    int mid = size / 2;
    *tree = create_tree_node(arr[mid]);
    build_tree_sorted_from_array(&(*tree)->left, arr, mid);
    build_tree_sorted_from_array(&(*tree)->right, arr + mid + 1, size - mid - 1);
}

/* Implementation av trädet, funktionerna i interfacet */

/* Skapar ett tomt träd - denna funktion ar färdig */
BSTree create_empty_tree(void)
{
    return NULL;
}

/* Returnerar 1 ifall trädet är tomt, 0 annars */
int is_empty_BST(const BSTree tree)
{
    if (tree == NULL)
        return 1;

    return 0;
}

static void insert_sorted_helper(BSTree* tree, int data)
{
    if (*tree == NULL)
    {
        *tree = create_tree_node(data);
        return;
    }

    if (data < (*tree)->data)
    {
        insert_sorted_helper(&(*tree)->left, data);
    }
    else if (data > (*tree)->data)
    {
        insert_sorted_helper(&(*tree)->right, data);
    }
    else
    {
        /* Dubletter ska inte tillåtas */
    }
}

/* Sätter in 'data' sorterat i *tree */
void insert_sorted_BST(BSTree* tree, int data)
{
    assert(tree != NULL);
    insert_sorted_helper(tree, data);
}

/* Utskriftsfunktioner
Det räcker att ni implementerar LR-ordningarna*/
void print_preorder(const BSTree tree)
{
    if (tree == NULL)
    {
        return;
    }
    sprintf(sprintfBuffer, "\n\rBesöker nod:");
    //uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\r%d", tree->data);
    uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\rVänster:");
    //uarte_write(sprintfBuffer);
    print_preorder(tree->left);
    sprintf(sprintfBuffer, "\n\rHöger:");
    //uarte_write(sprintfBuffer);
    print_preorder(tree->right);
}

void print_inorder(const BSTree tree)
{
    if (tree == NULL)
    {
        return;
    }

    sprintf(sprintfBuffer, "\n\rVänster:");
    //uarte_write(sprintfBuffer);
    print_inorder(tree->left);
    sprintf(sprintfBuffer, "\n\rBesöker nod:");
    //uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\r%d", tree->data);
    uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\rHöger:");
    //uarte_write(sprintfBuffer);
    print_inorder(tree->right);
}

void print_postorder(const BSTree tree)
{
    if (tree == NULL)
    {
        return;
    }

    sprintf(sprintfBuffer, "\n\rVänster:");
    //uarte_write(sprintfBuffer);
    print_postorder(tree->left);
    sprintf(sprintfBuffer, "\n\rHöger:");
    //uarte_write(sprintfBuffer);
    print_postorder(tree->right);
    sprintf(sprintfBuffer, "\n\rBesöker nod:");
    //uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\r%d", tree->data);
    uarte_write(sprintfBuffer);
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find_BST(const BSTree tree, int data)
{
    if (tree == NULL)
    {
        //sprintf(sprintfBuffer, "\n\rtomt");
        //uarte_write(sprintfBuffer);
        return 0;
    }

    if (data == tree->data)
    {
        //sprintf(sprintfBuffer, "\n\rfinns");
        //uarte_write(sprintfBuffer);
        return 1;
    }

    if (data < tree->data)
    {
        //sprintf(sprintfBuffer, "\n\rsöker vänster");
        //uarte_write(sprintfBuffer);
        return find_BST(tree->left, data);
    }

    //sprintf(sprintfBuffer, "\n\rsöker höger");
    //uarte_write(sprintfBuffer);
    return find_BST(tree->right, data);
}

static struct treeNode* find_min_node(BSTree tree)
{
    assert(tree != NULL);
    while (tree->left != NULL)
    {
        tree = tree->left;
    }
    return tree;
}

/* Tar bort 'data' från trädet om det finns */
void remove_element_BST(BSTree* tree, int data)
{
    if (tree == NULL || *tree == NULL)
    {
        return;
    }

    if (data < (*tree)->data)
    {
        remove_element_BST(&(*tree)->left, data);
    }
    else if (data > (*tree)->data)
    {
        remove_element_BST(&(*tree)->right, data);
    }
    else
    {
        struct treeNode* node = *tree;

        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            *tree = NULL;
        }
        else if (node->left == NULL)
        {
            *tree = node->right;
            free(node);
        }
        else if (node->right == NULL)
        {
            *tree = node->left;
            free(node);
        }
        else
        {
            struct treeNode* successor = find_min_node(node->right);
            node->data = successor->data;
            remove_element_BST(&node->right, successor->data);
        }
    }
}

/* Returnerar hur många noder som totalt finns i trädet */
int number_of_nodes_BST(const BSTree tree)
{
    if (tree == NULL)
    {
        return 0;
    }

    return 1 + number_of_nodes_BST(tree->left) + number_of_nodes_BST(tree->right);
}

/* Returnerar hur djupt trädet är */
int depth(const BSTree tree)
{
    if (tree == NULL)
    {
        return 0;
    }

    int left_depth = depth(tree->left);
    int right_depth = depth(tree->right);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

/* Returnerar minimidjupet för trädet
Se math.h för användbara funktioner*/
int min_depth(const BSTree tree)
{
    if (tree == NULL)
    {
        return 0;
    }

    if (tree->left == NULL && tree->right == NULL)
    {
        return 1;
    }

    if (tree->left == NULL)
    {
        return 1 + min_depth(tree->right);
    }

    if (tree->right == NULL)
    {
        return 1 + min_depth(tree->left);
    }

    int left_min = min_depth(tree->left);
    int right_min = min_depth(tree->right);
    return 1 + (left_min < right_min ? left_min : right_min);
}

/* Balansera trädet så att depth(tree) == minDepth(tree) */
void balance_tree(BSTree* tree)
{
    if (tree == NULL || *tree == NULL)
    {
        return;
    }

    int* arr = write_sorted_to_array(*tree);
    int count = number_of_nodes_BST(*tree);
    free_tree(tree);
    build_tree_sorted_from_array(tree, arr, count);
    free(arr);
}

/* Töm trädet och frigör minnet för de olika noderna */
void free_tree(BSTree* tree)
{
    if (tree == NULL || *tree == NULL)
    {
        return;
    }

    free_tree(&(*tree)->left);
    free_tree(&(*tree)->right);
    free(*tree);
    *tree = NULL;
}


