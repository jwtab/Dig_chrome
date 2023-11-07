
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>

#include <string>
using namespace std;

#include <cJSON.h>
#include <sha2.h>
#include <Base64.h>

#include <sqlite3.h>

/*
   homepage相关.
*/
void parse_homepage_Preferences(const char * pre_path);

int
main(int argc,char **argv)
{
   printf("======  测试Chrome-Hp_MacOS版本   ======\r\n");
   parse_homepage_Preferences("./test/Secure_Preferences.macOS.chrome");

   printf("======  测试Edge-Hp_MacOS版本   ======\r\n");
   parse_homepage_Preferences("./test/Secure_Preferences.macOS.edge");

   return 0;
}

cJSON * _chrome_get_json(const char * json_path)
{
	char * json_buf = nullptr;
	long json_len = 0;
	FILE * f = NULL;

   cJSON * root = NULL;

	f = fopen(json_path,"rb");
	if (NULL == f)
	{
		return root;
	}

	fseek(f, 0, SEEK_END);

	json_len = ftell(f);
	json_len = json_len + 1;

	fseek(f, 0, SEEK_SET);

	json_buf = (char*)malloc(json_len);
	if (NULL != json_buf)
	{
		memset(json_buf, 0, json_len);

		fread(json_buf, json_len, 1, f);

		root = cJSON_Parse(json_buf);
	}
	
	if (NULL != json_buf)
	{
		free(json_buf);
		json_buf = NULL;
	}

	fclose(f);
	f = NULL;

	return root;
}

void parse_homepage_Preferences(const char * pre_path)
{
   cJSON * root = _chrome_get_json(pre_path);

   cJSON * hp_json = cJSON_GetObjectItem(root,"homepage");
   if(hp_json)
   {
      printf("\r\n[Hp] %s\r\n",hp_json->valuestring);
   }

   cJSON * session_json = cJSON_GetObjectItem(root,"session");
   if(session_json)
   {
      cJSON * type_json = cJSON_GetObjectItem(session_json,"restore_on_startup");
      printf("\r\n[Restore_on_startup] %d\r\n",type_json->valueint);

      cJSON * startup_urls = cJSON_GetObjectItem(session_json,"startup_urls");
      if(startup_urls)
      {
         for(int index = 0; index < cJSON_GetArraySize(startup_urls); index++)
         {
            cJSON * url_json = cJSON_GetArrayItem(startup_urls,index);
            printf("[startup_url_%d] %s\r\n",index + 1,url_json->valuestring);
         }
      }
   }
}
