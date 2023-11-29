#include <myhtml/api.h>
#include "get_html.h"

// html -> html_tree
class html_tree : public html {		
public:
    html_tree(){}

    html_tree(std::string url) : html(url){
        initialize();
        make_tree(response);
    }

	myhtml_tree_node_t* get_root()const {
		return root;
	}

	void initialize();
	void make_tree(const std::string in_html);

    virtual ~html_tree(){}

protected:
	myhtml_t* myhtml;
	myhtml_tree_t* tree;
	myhtml_tree_node_t* root;

};