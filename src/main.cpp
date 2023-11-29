#include <algorithm>
#include "content_parsing.h"
#include "openai.hpp"

class print_gpt{
private:
    std::string request;
    std::string response;
    nlohmann::json j1;
public:
    print_gpt(std::string str) : request(str) {
        j1 = nlohmann::json::parse(str);
    }
    void resquest_to_gpt(){
        auto chat = openai::chat().create(j1);
        response = chat.dump(2);
    }
    void parsing_response(){
        auto idx1 = response.find("content", 0), idx2 = response.find("role", 0);
        response = response.substr(idx1 + 10, idx2-idx1 -21);
        while(response.find("\\n-") != -1){
            response.erase(response.find("\\n-"), 3);
        }
        while(response.find("\\n") != -1){
            response.erase(response.find("\\n"), 2);
        }
    }
    std::string get_response() const{
        return response;
    }
};

int main(void) {

    class content_parsing* notice =  new article("https://www.skku.edu/skku/campus/skk_comm/notice01.do");

    notice->print_article(notice->get_root());

    for (int i = 0; i < notice->get_link().size(); i++) {
        std::cout << notice->get_content()[i] << " , " << "https://www.skku.edu/skku/campus/skk_comm/notice01.do" + notice->get_link()[i] << std::endl;
    }


    std::cout << "\n\n";

    class content_parsing* menu_all = new menu("https://www.skku.edu/skku/campus/support/welfare_11_1.do?mode=info&srDt=2023-11-23&srCategory=L&conspaceCd=20201104&srResId=3&srShowTime=W");

    menu_all->print_article(menu_all->get_root());

    std::string signal;
    for (int i = 0 ; i < menu_all->get_content().size(); i++){
        if(menu_all->get_content()[i].find("1F") != -1) signal += "    ";
        else if(menu_all->get_content()[i].find("2F") != -1) signal += "      ";
        signal += menu_all->get_content()[i];
    }

    openai::start("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");
    std::string input;
    while(1) {
        std::cout << "요일과 코너이름을 입력해주세요\n";
        getline(std::cin, input);
        if(!strcmp(input.c_str(), "exit")) break;
        print_gpt p1("{ \"model\": \"gpt-3.5-turbo\", \"messages\":[{\"role\":\"user\", \"content\": \"" + signal + input + "\"}], \"max_tokens\": 200, \"temperature\": 0 }");
        p1.resquest_to_gpt();
        p1.parsing_response();
        std::cout << p1.get_response() << std::endl;
    }
}
