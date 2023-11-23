#include <myhtml/api.h>
#include "get_html.h"

// html -> html_tree
class html_tree : public html {		
public:
	myhtml_tree_node_t* get_root()const {
		return root;
	}

protected:
	void initialize();
	void make_tree();

	myhtml_t* myhtml;
	myhtml_tree_t* tree;
	myhtml_tree_node_t* root;
};