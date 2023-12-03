#include "content_parsing.h"

content_parsing::content_parsing(std::string url) : html_tree(url) {
    start = 0;
    end = 0;
}

article::article(std::string url) : content_parsing(url){
    strcpy(attr_value, "board-list-content-wrap ");
    strcpy(attr_key, "href");
    ur = url;
}

menu::menu(std::string url) : content_parsing(url){
    strcpy(attr_value_week, "weeListTit");
    strcpy(attr_value_cont, "weeListCont");
    strcpy(end_attr, "mealImg");
}

summarize::summarize(std::string url) : content_parsing(url){
    strcpy(attr_value, "pre");
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

void article::tree_traversal(myhtml_tree_node_t* node) {
	while (node) {
		check_node_attr(node);
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        if (tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {          // MyHtml_TAG__TEXT = 0x001, MyHTML_TAG__COMMENT = 0x002
            const char* node_text = myhtml_node_text(node, NULL);
            if (start == 1 && end == 1) {
                content.push_back(node_text);
                start = 0;
                end = 0;
            }
        }

        // check children
        tree_traversal(myhtml_node_child(node));
        node = myhtml_node_next(node);
	}
}


std::vector<std::string> article::get_link(){
    return link;
}

std::vector<std::string> article::get_content(){
    for (int i = 0; i < content.size(); i++) {
        content[i] = trim(content[i]);
    }
    return content;
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

void menu::tree_traversal(myhtml_tree_node_t *node) {
    while (node) {
        check_node_attr(node);

        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        if (tag_id == MyHTML_TAG__TEXT || tag_id == MyHTML_TAG__COMMENT) {
            const char* node_text = myhtml_node_text(node, NULL);
            if ((start == 1 && end == 1)) {
                if(strcmp(node_text, "")) content.push_back(node_text);
            }
        }

        // check children
        tree_traversal(myhtml_node_child(node));
        node = myhtml_node_next(node);
    }
}

std::vector<std::string> menu::get_content(){
    auto iter = content.begin();
    while(iter != content.end()) {
        // 양 끝 공백 제거
        *iter = trim(*iter);
        if (!strcmp((*iter).c_str(), "")) {
            iter = content.erase(iter);
            continue;
        }
        // 중간 공백 제거
        std::string temp = *iter;
        int j = -1, k = -1;
        for (int i = 0 ; i < temp.size(); i++){
            if ((j == -1) && (temp[i] == '\t' || temp[i] == '\n')) j = i;
            if ((j != -1) && (k == -1) && ((temp[i] != '\t') && (temp[i] != '\n'))) k = i;
        }
        if((j != -1) && (k != -1)) *iter = temp.erase(j, k-j);

        *iter = ' ' + *iter + ' ';
        // 확실히 구분 짓기 위해 요일 앞에 markdown 표시
        std::regex r("(월요일|화|수|목|금)\\(\\d+\\.\\d+\\)");
        std::smatch m;
        if(std::regex_match(*iter, m, r)){
            *iter = '#' + *iter;
        }
        iter++;
    }

    return content;
}


void summarize::tree_traversal(myhtml_tree_node_t* node) {

    while (node) {
        myhtml_tag_id_t tag_id = myhtml_node_tag_id(node);

        mycore_string_t *test;
        if (tag_id == MyHTML_TAG_PRE) {
            test = myhtml_token_node_string(myhtml_node_token(myhtml_node_child(node)));
            content.push_back(test->data);
        }

        // check children
        tree_traversal(myhtml_node_child(node));
        node = myhtml_node_next(node);
    }
}

std::vector<std::string> summarize::get_content(){
    return content;
}