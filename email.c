#include <stdio.h>
#include <string.h>
#include "curl/curl.h"
#include "fetch.h"
#include "b64-decode.h"
#include "ui.h"
#include "uim.h"


// Callback function to write attachment data to file
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* file = (FILE*)userp;
    size_t written = fwrite(contents, size, nmemb, file);
    return written;
}

int email() {
    CURL* curl;
    CURLcode res;
    FILE* file;
    // Set up libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        int flag = 0;
        // Set IMAP server and account details
        char email[] = "photoeditorcproject@gmail.com"; // Replace with your Gmail address
        char password[] = "xtgcggxpnrmtuulf"; // Replace with your Gmail password
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\project\\cacert-2023-05-30.pem");

        // Set the mailbox name and UID
        char mailbox[] = "INBOX";
        int uid = ui();

        // Create the URL with the UID
        char url[100];
        sprintf(url, "imaps://imap.gmail.com/%s;UID=%d", mailbox, uid);

        // Connect to the IMAP server and log in
        curl_easy_setopt(curl, CURLOPT_USERNAME, email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the fetch request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            flag = 1;
        }

        // Open a file to save the fetched email
        file = fopen("C:\\project\\fetch\\image.txt", "wb");
        if (!file) {
            printf("Error opening file for writing.");
            return 1;
        }

        // Set the write callback function to write data into the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the fetch request and save the email content to the file
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            flag = 1;
        }

        if(flag){
            uim();
            return 1;
        }

        // Clean up and close the file
        fclose(file);

        // Clean up and close the IMAP session
        curl_easy_cleanup(curl);
    } else {
        printf("Error initializing libcurl.\n");
        return 1;
    }

    // Clean up libcurl
    curl_global_cleanup();
    fetch("C:\\project\\fetch\\image.txt","C:\\project\\fetch\\fetch.txt");
    b64decode("C:\\project\\fetch\\fetch.txt","C:\\project\\fetch\\fetch.txt");

    return 0;
}