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

    notice->tree_traversal(notice->get_root());
    notice->final_url();


    class content_parsing* menu_all = new menu("https://www.skku.edu/skku/campus/support/welfare_11_1.do?mode=info&srDt=2023-12-04&srCategory=L&conspaceCd=20201104&srResId=3&srShowTime=W");

    menu_all->tree_traversal(menu_all->get_root());

    std::string signal;
    for (int i = 0 ; i < menu_all->get_content().size(); i++){
        signal += menu_all->get_content()[i];
    }

    openai::start("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");
    std::string input;

    while (1) {
        std::cout << "원하시는 메뉴의 숫자를 입력해주세요.\n1. 공지 요약\n2. 메뉴 정보\n입력: ";
        std::cin >> input;
        int check = 0;
        if (!strcmp(input.c_str(), "1")) {
            while (1) {
                if (!check) {
                    std::cout << "오늘의 공지사항은 다음과 같습니다." << std::endl;

                    for (int i = 0; i < notice->get_link().size(); i++) {
                        std::cout << i + 1 << ' ' << notice->get_content()[i] << std::endl;
                    }
                    check = 1;
                }
                int num;
                std::cout << "요약을 원하시는 공지 번호를 입력해주세요 (exit:0) : ";
                std::cin >> num;
                if (num == 0) break;

                class content_parsing* summary = new summarize(notice->get_link()[num-1]);
                summary->tree_traversal(summary->get_root());

                std::string c = summary->get_content()[0];
                while(c.find('\u000A', 0) != -1){
                    c.replace(c.find('\u000A'), 1, "\\n");
                }
                print_gpt p1("{ \"model\": \"gpt-4-1106-preview\", \"messages\":[{\"role\":\"user\", \"content\": \"" + c + "위 내용을 요약해줘" + "\"}], \"max_tokens\": 600, \"temperature\": 0 }");
                p1.resquest_to_gpt();
                p1.parsing_response();
                std::cout << p1.get_response() << std::endl;

                delete summary;         // free memory
            }
        } else if (!strcmp(input.c_str(), "2")) {
            std::cin.ignore(1);
            while(1) {
                std::cout << "요일과 코너이름을 입력해주세요(just input 'exit' if you want break)\n";
                getline(std::cin, input);
                if(!strcmp(input.c_str(), "exit")) break;
                print_gpt p1("{ \"model\": \"gpt-4-1106-preview\", \"messages\":[{\"role\":\"user\", \"content\": \"" + signal + input + "\"}], \"max_tokens\": 200, \"temperature\": 0 }");
                p1.resquest_to_gpt();
                p1.parsing_response();
                std::cout << p1.get_response() << std::endl;
            }
        } else {
            break;
        }
    }

    delete notice;
    delete menu_all;
}
