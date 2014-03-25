#include <string.h>
#include <curl/curl.h>
#include "control.h"

bool move(int dir, int degree)
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(!curl) return false;
	
	char url[75];
	sprintf((char*)&url, "http://nidq.no-ip.org/decoder_control.cgi?command=%i&onestep=1&degree=%i", dir, degree);
	sprintf((char*)&url, "http://192.168.1.200/decoder_control.cgi?command=%i&onestep=1&degree=%i", dir, degree);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:31415LAS");

	/* Perform the request, res will get the return code */ 
	res = curl_easy_perform(curl);
	
	curl_easy_cleanup(curl);

	return (res == CURLE_OK);
}
