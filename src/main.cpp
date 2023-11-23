#include <algorithm>
#include "content_parsing.h"
#include "openai.hpp"

int main(void) {
    class article homepage_noti("https://www.skku.edu/skku/campus/skk_comm/notice01.do");

    homepage_noti.print_article(homepage_noti.get_root());

    for (int i = 0; i < homepage_noti.get_link().size(); i++) {
        std::cout << homepage_noti.get_content()[i] << " , " << "https://www.skku.edu/skku/campus/skk_comm/notice01.do" + homepage_noti.get_link()[i] << std::endl;
    }


    std::cout << "\n\n";
    class menu homepage_menu("https://www.skku.edu/skku/campus/support/welfare_11_1.do?mode=info&srDt=2023-11-23&srCategory=L&conspaceCd=20201104&srResId=3&srShowTime=W");

    homepage_menu.print_menu(homepage_menu.get_root());

    for (int i = 0 ; i < homepage_menu.get_content().size(); i++){
        std::string str = homepage_menu.get_content()[i];
        if (str == "") continue;
        if (str.find("\n\t\t\t") != std::string::npos) {
            int i;
            for (i = 0 ;  i < str.size() ; i++){
                if (str[i] == '\n') break;
            }
            str.erase(i, str.find_last_not_of(" \r\n\t\v\f") - 1);
            std::cout << std::endl << str << std::endl;
        }
        else if ((str[0] == '1') || (str[0] == '2')) std::cout << std::endl << str << std::endl;
        else if (!strcmp(str.c_str(), "메뉴")) std::cout << "\n";
        else if (!strcmp(str.c_str(), "코너")) std::cout <<  " " << str;
        else{
            std::cout << str;
        }
    }

    openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    // openai::start("your_API_key", "optional_organization"); // Or you can handle it yourself

    auto completion = openai::completion().create(R"(
    {
        "model": "text-davinci-003",
        "prompt": "Say this is a test",
        "max_tokens": 7,
        "temperature": 0
    }
    )"_json); // Using user-defined (raw) string literals
    std::cout << "Response is:\n" << completion.dump(2) << '\n';

    auto image = openai::image().create({
                                                { "prompt", "A logo with a cello in a heart"},
                                                { "n", 1 },
                                                { "size", "512x512" }
                                        }); // Using initializer lists
    std::cout << "Image URL is: " << image["data"][0]["url"] << '\n';
    return 0;
}
