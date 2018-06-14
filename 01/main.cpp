/********************************************************************
*                   (贴吧)图片爬取
* 实现将单个网页内的所有图片， 下载到本地。
* 具体实现：
* 使用正则表达式，进行检索关键内容
* 使用curl 实现下载功能
******************************************************************/
#include <iostream>
#include <curl/curl.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>

/*贴吧， 图片地址长度都是固定的。  把测得的长度保存到len中*/
static int len = strlen("https://imgsa.baidu.com/forum/w%3D580/sign=de9b539822dda3cc0be4b82831e83905/45f09d315c6034a8cb727286c71349540823767f.jpg");
static int flag = 0;    //标志位，非0表示已检索完所有图片链接
static int num = 0;     //图片数目计算， 默认从零开始

/*将下载的数据写入文件*/
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);

    return written;
}

/*
进行实际的下载请求的处理
url -- 下载链接
filename -- 保存文件时用的文件名
*/
void Download(char *url, char *filename)
{
    CURL *curl_handle;
    FILE *file;

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    file = fopen(filename, "wb");
    if(file)
    {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, file);

        curl_easy_perform(curl_handle);

        fclose(file);
    }

    curl_easy_cleanup(curl_handle);
}

/*
贴吧图片下载
image_url -- 图片的链接地址
*/
void Download_image(char *image_url)
{
    char filename[10] = { };

    sprintf(filename, "%d.%c%c%c", num++, image_url[len-3], image_url[len-2], image_url[len-1]);


    std::cout << filename << std::endl;
    Download(image_url, filename);

    free(image_url);
}

/*
进行正则查询，并开始下载
buf -- 整个网页的代码
*/
char *Retrieval(char *buf)
{
    int status;
    int cflags = REG_EXTENDED;
    regmatch_t pmatch[1];
    regex_t reg;
    const char *patten = "https://imgsa.baidu.com/forum/w%3D580/sign=(.*)";
    char *image_url = (char *)malloc(sizeof(char) * (len + 1));

    regcomp(&reg, patten, cflags);

    status = regexec(&reg, buf, 1, pmatch, 0);
    if(status == REG_NOMATCH)
    {
        flag = 1;
        return NULL;
    }

    if(status == 0)
    {
        buf = &buf[pmatch[0].rm_so];
        for(int i = 0; i < len; i++)
           image_url[i] = buf[i];
        image_url[len] = '\0';

        Download_image(image_url);

        buf = &buf[len];
    }

    return buf;
}

int main(int argc, char *argv[])
{
    char *url;
    FILE *file_html;
    char *buf = (char *)malloc(sizeof(char) * 1024 * 1024);
    char *buf_start = buf;
    int i = 0;
    char ch;

    if(argc != 2)
        url = (char *)"https://tieba.baidu.com/p/5610496176";
    else
        url = argv[1];

    /*全局libcurl初始化*/
    curl_global_init(CURL_GLOBAL_ALL);

    /*下载网页代码， 并保存为temp.html的文件*/
    Download(url, (char *)"temp.html");

    /*将temp.html的内容， 读取到buf里*/
    file_html = fopen("temp.html", "r");
    while((ch = getc(file_html)) != EOF)
    {
        buf[i++] = ch;
    }
    buf[i] = '\0';


    /*进行实际的下载处理*/
    while(!flag)
    {
        buf = Retrieval(buf);
    }

    /*程序退出的处理*/
    free(buf_start);
    curl_global_cleanup();

    return 0;
}
