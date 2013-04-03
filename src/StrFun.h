#ifndef _STR_FUN_H_
#define _STR_FUN_H_
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iconv.h>
#include <string.h>
#include <map>

using namespace std;

class CStrFun
{
public:
        CStrFun();
        virtual ~CStrFun();

        static void Str2Lower(string &sSource, int nLen);
        static string itos(long long i){
                stringstream s;
                s << i;
                return s.str();
        }

        /* Locate a substring ignoring case
         * The  function  returns a value equal or lager than zero,
         * or -1 if the substring is not found.
         */

        static string::size_type FindCase(string haystack, string needle);
        static string::size_type FindCaseFrom(string haystack, string needle, int From);

        static void EmptyStr(string &str);
        string ReplaceStr(string str, string srstr, string dsstr);
        void replaceString(string &str,string srstr,string dsstr);
        string EraseStr(string str, string substr);
        void splitQuery(string query, string &keywords, string &district, string &address);

        static void Translate(char* SourceStr);
        int Utf8BytePos(char* c, int step);
        int Utf8AbsPos(char* c, int step);
        string GetStr(string s,std::vector<string> &keywords, int step);
        string getThumbPicturePath(string picturePath);

        int WordOccurrenceCount( std::string str, std::string word );

        string base64_encode ( const string& sInput );
        string base64_decode ( const string& sInput );
        //string URLEncode(string s);
        //string URLDecode(string s);
        string Encode(string str);
        string Decode(string str);
        string URLDecodeString(string s);
        string URLEncodeString(string s);
        void stringReplaceAll(string *s, string before, string after);
        string stringFill(string s,int num); //字符串填充
        string trim(string s);  //去除字符串两边的空格
        int distance(const std::string source, const std::string target);
        string D64_to_String(float value);
        int findOccurrence(char *input, char *chrSet);
        string replaceAllByChr(string inputString, string characters) ;
        string extractField(string fullDocText, string fieldName);
        vector<string> splitString(string keyword,string sToken);
        string stringFromVector(vector<string> &vString);

        std::string replaceByMap(std::string,const std::map<std::string,std::string>&);
	string normalize(string str);
};

//code converter class by kun
class CodeConverter {
        private:
        iconv_t cd;
        public:
        // 构造
        CodeConverter(const char *from_charset,const char *to_charset) {
                cd = iconv_open(to_charset,from_charset);
        }

        // 析构
        ~CodeConverter() {
                iconv_close(cd);
        }

        // 转换输出
        int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
                char **pin = &inbuf;
                char **pout = &outbuf;

                memset(outbuf,0,outlen);
                return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
        }
};
#endif
