#include "make_tree.h"
#include <string.h>
#include <vector>

// html -> html_tree -> content_parsing

class content_parsing : public html_tree {
public:
    content_parsing(std::string url);

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

protected:
    int start;
    int end;
};

// html -> html_tree -> content_parsing -> artice, , ,  

class article : public content_parsing {
public:
    article(std::string url);

    std::vector<std::string> get_link() const{
        return link;
    }

    std::vector<std::string> get_content(){
        for (int i = 0; i < content.size(); i++) {
            content[i] = trim(content[i]);
        }
        return content;
    }

    void print_article(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);     // print_article���� ȣ���ϴ� �Լ�, list start end �ɹ������鿡 ����

private:
    char attr_value[100];                                      // attr value
    char attr_key[100];                                        // attr key
    std::vector<std::string> link, content;
};

// polymorphism
class menu : public content_parsing{
public:
    menu(std::string url);

    std::vector<std::string> get_content(){
        for (int i = 0; i < content.size(); i++) {
            content[i] = trim(content[i]);
        }
        return content;
    }

    void print_menu(myhtml_tree_node_t* node);
    void check_node_attr(myhtml_tree_node_t* node);     // print_article���� ȣ���ϴ� �Լ�, list start end �ɹ������鿡 ����

private:
    char attr_value_week[100];
    char attr_value_cont[100];
    char end_attr[100];
    std::vector<std::string> content;
};