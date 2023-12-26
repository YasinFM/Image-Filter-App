#include <stdio.h>
#include "curl/curl.h"

int main(){
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, "photoeditorcproject@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "xtgcggxpnrmtuulf");
        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.example.com/INBOX;UIDVALIDITY=1;UIDNEXT=1");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH FLAGGED UNSEEN");
        res = curl_easy_perform(curl);
        printf("%d",res);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}
