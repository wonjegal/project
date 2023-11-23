#include <iostream>
#include <string>
#include <curl/curl.h>

// html class
class html {
protected:
	void  get_html(std::string url);
	
	std::string response;
};


