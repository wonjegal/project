#include "content_parsing.h"

content_parsing::content_parsing(std::string url) {
    int start = 0;
    int end = 0;
    get_html(url);
    initialize();
    make_tree();
}

article::article(std::string url):content_parsing(url){
    strcpy(attr_value, "board-list-content-wrap ");
    strcpy(attr_key, "href");
}

menu::menu(std::string url):content_parsing(url){
    strcpy(attr_value_week, "weeListTit");
    strcpy(attr_value_cont, "weeListCont");
    strcpy(end_attr, "mealImg");
}

void article::check_node_attr(myhtml_tree_node_t* node)
{
    myhtml_tree_attr_t* attr = myhtml_node_attribute_first(node);

    while (attr) {
        const char* name = myhtml_attribute_key(attr, NULL);

        if (name) {
            const char* value = myhtml_attribute_value(attr, NULL);

            if (value) {
                if (!strcmp(value, attr_value)) {
                    start = 1;
                }
            }

            if (start == 1 && !strcmp(name, attr_key)) {
                link.push_back(value);
                end = 1;
            }
        }

        attr = myhtml_attribute_next(attr);
    }
}

void article::print_article(myhtml_tree_node_t* node) {

	while (node) {
		const char* tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);
	
		check_node_attr(node);

        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        if (tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {
            const char* node_text = myhtml_node_text(node, NULL);
            if (start == 1 && end == 1) {
                content.push_back(node_text);
                start = 0;
                end = 0;
            }
        }
        else {
        }

        // check children
        print_article(myhtml_node_child(node));
        node = myhtml_node_next(node);
	}
}

void menu::check_node_attr(myhtml_tree_node_t *node) {
    myhtml_tree_attr_t* attr = myhtml_node_attribute_first(node);

    while (attr) {
        const char* name = myhtml_attribute_key(attr, NULL);

        if (name) {
            const char* value = myhtml_attribute_value(attr, NULL);

            if (value) {
                if (!strcmp(value, attr_value_week) || !strcmp(value, attr_value_cont)) {
                    start = 1;
                    end = 1;
                }
                if (!strcmp(value, end_attr)){
                    start = 0;
                    end = 0;
                }
            }

        }
        attr = myhtml_attribute_next(attr);
    }
}

void menu::print_menu(myhtml_tree_node_t *node) {
    while (node) {
        const char* tag_name = myhtml_tag_name_by_id(tree, myhtml_node_tag_id(node), NULL);

        check_node_attr(node);

        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        if (tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {
            const char* node_text = myhtml_node_text(node, NULL);
            if ((start == 1 && end == 1)) {
                content.push_back(node_text);
            }
        }
        else {
        }

        // check children
        print_menu(myhtml_node_child(node));
        node = myhtml_node_next(node);
    }
}
