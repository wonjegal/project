#include <iostream>
#include <string>
#include <curl/curl.h>

// html class
class html {
public:
    html(std::string url) {
        get_html(url);
    };

    std::string get_response() const{
        return response;
    }

    void get_html(std::string url);

    virtual ~html(){}               // default destructor

protected:
    std::string response;
};


