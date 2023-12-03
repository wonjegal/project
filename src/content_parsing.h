#include "make_tree.h"
#include <cstring>
#include <vector>
#include <regex>

// html -> html_tree -> content_parsing

class content_parsing : public html_tree {
public:
    content_parsing(std::string url);

    // parsing 후 앞 뒤 공백을 제거
    std::string& ltrim(std::string& str) {
        str.erase(0, str.find_first_not_of(" \r\n\t\v\f"));
        return str;
    }
    std::string& rtrim(std::string& str) {
        str.erase(str.find_last_not_of(" \r\n\t\v\f") + 1);
        return str;
    }
    std::string& trim(std::string& str) {
        return ltrim(rtrim(str));
    }

    // url = host url + sub url
    void virtual final_url(){}

    // tree 순회 하면서 적절한 attr 찾기
    void virtual tree_traversal(myhtml_tree_node_t* node){}
    void virtual check_node_attr(myhtml_tree_node_t* node){}
    std::vector<std::string> virtual get_link(){
        std::vector<std::string> link(1);
        return link;
    }
    std::vector<std::string> virtual get_content(){
        std::vector<std::string> content(1);
        return content;
    }

    virtual ~content_parsing(){}

protected:
    int start;
    int end;
    int ur;
};

// html -> html_tree -> content_parsing -> artice, , ,  

class article : public content_parsing {
public:
    article(std::string url);

    std::vector<std::string> get_link();        // article에 맞게 재정의
    std::vector<std::string> get_content();

    void tree_traversal(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);

    void final_url(){
        for(int i = 0 ; i < link.size();i++) link[i] = ur + link[i];
    }

    ~article(){}

private:
    char attr_value[100];                                      // attr value
    char attr_key[100];                                        // attr key
    std::vector<std::string> link, content;
    std::string ur;
};

// polymorphism
class menu : public content_parsing{
public:
    menu(std::string url);

    std::vector<std::string> get_content();      // menu에 맞게 재정의

    void tree_traversal(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);

    ~menu(){}

private:
    char attr_value_week[100];
    char attr_value_cont[100];
    char end_attr[100];
    std::vector<std::string> content;
};

// polymorphism
class summarize : public content_parsing{
public:
    summarize(std::string url);

    std::vector<std::string> get_content();

    void tree_traversal(myhtml_tree_node_t* node);

    ~summarize(){}

private:
    char attr_value[100];
    std::vector<std::string> content;
};