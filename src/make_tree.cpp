#include "make_tree.h"

void html_tree::initialize() {
    // basic init
    myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // init tree
    tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
}

void html_tree::make_tree() {
    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, response.c_str(), response.size());
    root = myhtml_tree_get_document(tree);
}