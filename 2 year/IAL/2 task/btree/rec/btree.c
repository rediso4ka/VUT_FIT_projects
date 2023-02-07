/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL) {
        return false;
    }
    if (tree->key == key) {
        *value = tree->value;
        return true;
    }
    if (key < tree->key) {
        return bst_search(tree->left, key, value);
    }
    return bst_search(tree->right, key, value);
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (tree == NULL) {
        return;
    }
    if ((*tree) == NULL) {
        bst_node_t *new = (bst_node_t *) malloc(sizeof(bst_node_t));
        if (new == NULL) {
            return;
        }
        new->key = key;
        new->value = value;
        new->left = NULL;
        new->right = NULL;
        (*tree) = new;
    } else {
        if (key < (*tree)->key) {
            bst_insert(&((*tree)->left), key, value);
        } else if (key > (*tree)->key) {
            bst_insert(&((*tree)->right), key, value);
        } else {
            (*tree)->value = value;
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if ((*tree) == NULL) {
        return;
    }
    if ((*tree)->right == NULL) {   // we found the most right one in 1 step
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        target->left = (*tree)->left;
        free((*tree));
        return;
    }
    if ((*tree)->right->right == NULL) {    // we found the most right one in more steps
        bst_node_t *tmp = (*tree)->right;
        target->key = (*tree)->right->key;
        target->value = (*tree)->right->value;
        (*tree)->right = (*tree)->right->left;
        free(tmp);
        return;
    }
    bst_replace_by_rightmost(target, &((*tree)->right));
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (tree == NULL) {
        return;
    }
    if ((*tree) == NULL) {
        return;
    }
    if (key < (*tree)->key) {
        /// Node to delete is left son
        if ((*tree)->left != NULL) {
            if ((*tree)->left->key == key) {
                if ((*tree)->left->left == NULL && (*tree)->left->right == NULL) {              // no sons
                    free((*tree)->left);
                    (*tree)->left = NULL;
                    return;
                } else if ((*tree)->left->left != NULL && (*tree)->left->right != NULL) {       // 2 sons
                    bst_replace_by_rightmost((*tree)->left, &((*tree)->left->left));
                    return;
                } else if ((*tree)->left->left == NULL) {                                       // right son
                    bst_node_t *tmp = (*tree)->left;
                    (*tree)->left = (*tree)->left->right;
                    free(tmp);
                    return;
                } else {                                                                        // left son
                    bst_node_t *tmp = (*tree)->left;
                    (*tree)->left = (*tree)->left->left;
                    free(tmp);
                    return;
                }
            }
        }
        /// Bad luck, going further
        bst_delete(&((*tree)->left), key);
        return;
    } else if (key > (*tree)->key) {
        /// Node to delete is right son
        if ((*tree)->right != NULL) {
            if (key == (*tree)->right->key) {
                if ((*tree)->right->left == NULL && (*tree)->right->right == NULL) {            // no sons
                    free((*tree)->right);
                    (*tree)->right = NULL;
                    return;
                } else if ((*tree)->right->left != NULL && (*tree)->right->right != NULL) {     // 2 sons
                    bst_replace_by_rightmost((*tree)->right, &((*tree)->right->left));
                    return;
                } else if ((*tree)->right->left == NULL) {                                      // right son
                    bst_node_t *tmp = (*tree)->right;
                    (*tree)->right = (*tree)->right->right;
                    free(tmp);
                    return;
                } else {                                                                        // left son
                    bst_node_t *tmp = (*tree)->right;
                    (*tree)->right = (*tree)->right->left;
                    free(tmp);
                    return;
                }
            }
        }
        /// Bad luck, going further
        bst_delete(&((*tree)->right), key);
        return;
    } else {                                // node to delete is the root
        bst_node_t *to_delete = (*tree);
        if ((*tree)->left == NULL && (*tree)->right == NULL) {              // no sons
            (*tree) = NULL;
            free(to_delete);
            return;
        } else if ((*tree)->left != NULL && (*tree)->right != NULL) {       // 2 sons
            bst_replace_by_rightmost((*tree), &((*tree)->left));
        } else if ((*tree)->left == NULL) {                                 // right son
            (*tree) = (*tree)->right;
            free(to_delete);
        } else {                                                            // left son
            (*tree) = (*tree)->left;
            free(to_delete);
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (tree == NULL) {
        return;
    }
    if ((*tree) == NULL) {
        return;
    }
    bst_dispose(&((*tree)->left));
    bst_dispose(&((*tree)->right));
    free((*tree));
    (*tree) = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_print_node(tree);
        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_inorder(tree->left);
        bst_print_node(tree);
        bst_inorder(tree->right);
    }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_postorder(tree->left);
        bst_postorder(tree->right);
        bst_print_node(tree);
    }
}
