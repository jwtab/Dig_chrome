
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

   cJSON * browser_json = cJSON_GetObjectItem(root,"browser");
   if(browser_json)
   {
      cJSON * show_home_button_json = cJSON_GetObjectItem(browser_json,"show_home_button");
      printf("[browser.show_home_button] %d\r\n",show_home_button_json->valueint);
   }

   cJSON * hp_json = cJSON_GetObjectItem(root,"homepage");
   if(hp_json)
   {
      printf("[homepage] %s\r\n",hp_json->valuestring);
   }

   cJSON *homepage_is_newtabpage_json = cJSON_GetObjectItem(root,"homepage_is_newtabpage");
   if(homepage_is_newtabpage_json)
   {
      printf("[homepage_is_newtabpage] %d\r\n",homepage_is_newtabpage_json->valueint);
   }

   cJSON * session_json = cJSON_GetObjectItem(root,"session");
   if(session_json)
   {
      cJSON * type_json = cJSON_GetObjectItem(session_json,"restore_on_startup");
      printf("[session.restore_on_startup] %d\r\n",type_json->valueint);

      cJSON * startup_urls = cJSON_GetObjectItem(session_json,"startup_urls");
      if(startup_urls)
      {
         for(int index = 0; index < cJSON_GetArraySize(startup_urls); index++)
         {
            cJSON * url_json = cJSON_GetArrayItem(startup_urls,index);
            printf("[session.startup_urls %d] %s\r\n",index + 1,url_json->valuestring);
         }
      }
   }

   printf("\r\nhmac数据\r\n");

   cJSON * macs_json = cJSON_GetObjectItem(cJSON_GetObjectItem(root,"protection"),"macs");

   cJSON * macs_browser_show_home_button = cJSON_GetObjectItem(cJSON_GetObjectItem(macs_json,"browser"),"show_home_button");
   printf("[protection.macs.browser.show_home_button] %s\r\n",macs_browser_show_home_button->valuestring);

   cJSON * macs_homepage_json = cJSON_GetObjectItem(macs_json,"homepage");
   if(macs_homepage_json)
   {
      printf("[protection.macs.homepage] %s\r\n",macs_homepage_json->valuestring);
   }

   cJSON * macs_homepage_is_newtabpage = cJSON_GetObjectItem(macs_json,"homepage_is_newtabpage");
   if(macs_homepage_is_newtabpage)
   {
      printf("[protection.macs.homepage_is_newtabpage] %s\r\n",macs_homepage_is_newtabpage->valuestring);
   }

   cJSON * macs_session_restore_on_startup = cJSON_GetObjectItem(cJSON_GetObjectItem(macs_json,"session"),"restore_on_startup");
   if(macs_session_restore_on_startup)
   {
      printf("[protection.macs.session.restore_on_startup] %s\r\n",macs_session_restore_on_startup->valuestring);
   }

   cJSON * macs_session_startup_urls = cJSON_GetObjectItem(cJSON_GetObjectItem(macs_json,"session"),"startup_urls");
   if(macs_session_startup_urls)
   {
      printf("[protection.macs.startup_urls] %s\r\n",macs_session_startup_urls->valuestring);
   }

   cJSON * super_mac_json = cJSON_GetObjectItem(cJSON_GetObjectItem(root,"protection"),"super_mac");
   if(super_mac_json)
   {
      printf("[protection.super_mac] %s\r\n",super_mac_json->valuestring);
   }
}
