#include "get_html.h"

size_t Callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

void html::get_html(std::string url) {

    // libcurl init
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
        return;
    }

    // libcurl setup
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 119.0.0.0 Safari / 537.36");

    // callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);           // output : response

    CURLcode res = curl_easy_perform(curl);                         // error code : res

    // http response error
    if (res != CURLE_OK) {
        std::cerr << "Failed to fetch URL: " << curl_easy_strerror(res) << std::endl;
    }

    // libcurl release
    curl_easy_cleanup(curl);
}