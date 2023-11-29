#include "make_tree.h"
#include <string.h>
#include <vector>

// html -> html_tree -> content_parsing

class content_parsing : public html_tree {
public:
    content_parsing(std::string url);

    // string 관련 함수들 (parsing 후 content와 link가 많은 공백들로 이루어져있어 필요)
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
    //std::string& remove_space(std::string& str);

    // tree순환하면서 적절한 attr 찾기
    void virtual print_article(myhtml_tree_node_t* node){}
    void virtual check_node_attr(myhtml_tree_node_t* node){}

    // 결과물 content, link 반환
    std::vector<std::string> virtual get_content(){
        std::vector<std::string> trash(0);
        return trash;
    }
    std::vector<std::string> virtual get_link(){
        std::vector<std::string> trash(0);
        return trash;
    }

    virtual ~content_parsing(){}

protected:
    int start;
    int end;
};

// html -> html_tree -> content_parsing -> artice, , ,  

class article : public content_parsing {
public:
    article(std::string url);

    std::vector<std::string> get_link();

    std::vector<std::string> get_content();

    void print_article(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);     // article에 맞게 재정의

    ~article(){}

private:
    char attr_value[100];                                      // attr value
    char attr_key[100];                                        // attr key
    std::vector<std::string> link, content;
};

// polymorphism
class menu : public content_parsing{
public:
    menu(std::string url);

    std::vector<std::string> get_content();

    void print_article(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);    // menu에 맞게 재정의

    ~menu(){}

private:
    char attr_value_week[100];
    char attr_value_cont[100];
    char end_attr[100];
    std::vector<std::string> content;
};