#include "StrFun.h"

CStrFun::CStrFun()
{

}

CStrFun::~CStrFun()
{

}

string CStrFun::normalize(string str)
{
    string normalizedStr=trim(str);
    Str2Lower(normalizedStr,normalizedStr.length());
    normalizedStr=ReplaceStr(normalizedStr," ","_");
    return normalizedStr;
}

void CStrFun::Str2Lower(string& str, int nLen)
{
        char distance='A'-'a' ;

        for( int i=0 ; i<nLen; i++ )
                if  ( str[i]>='A' && str[i]<='Z' )
                        str[i] -= distance ;
}

bool _nocase_compare(char c1, char c2)
{
        return toupper(c1) == toupper(c2);
}

string::size_type CStrFun::FindCase(string haystack, string needle)
{
        if (haystack.empty()) return string::npos;
        if (needle.empty()) return string::npos;

        string::iterator pos;
        pos = search(haystack.begin(), haystack.end(),
                needle.begin(),needle.end(), _nocase_compare);

        if( pos == haystack.end()){
                return string::npos;
        }else{
                return (pos - haystack.begin());
        }
}

string::size_type CStrFun::FindCaseFrom(string haystack, string needle, int nFrom)
{
        //assert( haystack.empty() == false );
        //assert( needle.empty() == false );
        if (haystack.empty()) return string::npos;
        if (needle.empty()) return string::npos;

        string::iterator pos;
        pos = search(haystack.begin()+nFrom, haystack.end(),
                needle.begin(),needle.end(), _nocase_compare);

        if( pos == haystack.end()){
                return string::npos;
        }else{
                return (pos - haystack.begin());
        }
}

string CStrFun::EraseStr(string str , string substr)
{
        if( str.size() == 0 || substr.size() == 0 )
                return "";

        string::size_type idx = 0;
        string::size_type sub_length = substr.length();
        idx = str.find(substr,idx);
        while( idx != string::npos ){
                str.erase(idx,sub_length);
                idx = str.find(substr,idx);
        }
        return str;
}

//替换函数，str-模板，srstr-要被替换的内容，dsstr-需要替换成的内容
string CStrFun::ReplaceStr(string str,string srstr,string dsstr)
{
        if( str.size() ==0 || srstr.size() == 0 )
                return "";

        string::size_type idx = 0;
        string::size_type sub_length = srstr.length();
        idx = str.find(srstr,idx);
        while( idx != string::npos ){
                str.replace(idx,sub_length,dsstr);

                if( idx+dsstr.size() > str.size() ) break;

                idx = str.find(srstr,idx+dsstr.size());
        }
        return str;
}
//
void CStrFun::replaceString(string &str,string srstr,string dsstr){
        if(str.size()==0 || srstr.size()==0){
                return ;
        }
        string::size_type idx=0;
        string::size_type sub_length=srstr.length();
        idx=str.find(srstr,idx);
        if(idx!=string::npos){
                str.replace(idx,sub_length,dsstr);
        }
}
void CStrFun::EmptyStr(string &str)
{
        const string delims(" \t\r\n");
        string::size_type idx=0, pre_idx;

        while( (idx = str.find_first_of(delims, idx)) != string::npos ){
                pre_idx = idx;
                str.replace(idx,1,1,' ');
                idx++;
                while( (idx = str.find_first_of(delims, idx)) != string::npos ) {
                        if( idx-pre_idx == 1 ){
                                str.erase(idx, 1);
                        } else {
                                break;
                        }
                }

                idx--;
        }

}

// translate the encoded URL(%xx) to actual chars
void CStrFun::Translate(char* SourceStr)
{
        int i=0;
        int j=0;
        char *tempstr,tempchar1,tempchar2;

        tempstr = (char*)malloc(strlen(SourceStr) + 1);
        if(tempstr == NULL){
                return;
        }

        while (SourceStr[j]) {
                if ((tempstr[i]=SourceStr[j])=='%'){
                        if (SourceStr[j+1]>='A')
                                tempchar1=((SourceStr[j+1]&0xdf)-'A')+10;
                        else
                                tempchar1=(SourceStr[j+1]-'0');
                        if (SourceStr[j+2]>='A')
                                tempchar2=((SourceStr[j+2]&0xdf)-'A')+10;
                        else
                                tempchar2=(SourceStr[j+2]-'0');

                        tempstr[i]=tempchar1*16+tempchar2;
                        j=j+2;
                }
                i++;
                j++;

        }
        tempstr[i]='\0';
        strcpy(SourceStr,tempstr);

        if(tempstr) free(tempstr);
}


//add by kun
void CStrFun::splitQuery(string query, string &keywords, string &district, string &address){
        string queryCopy = query;
        if(query.size() == 0)
                return;
        int pos = queryCopy.find("$$");
        if(pos != string::npos){
                keywords = queryCopy.substr(0, pos);
                queryCopy = queryCopy.substr(pos + 2);
        }
        else
                return;
        if(queryCopy.size() == 0)
                return;
        pos = queryCopy.find("**");
        if(pos != string::npos){
                district = queryCopy.substr(0, pos);
                queryCopy = queryCopy.substr(pos + 2);
        }
        else
                return;
        if(queryCopy.size() == 0)
                return;
        address = queryCopy;
        return;

}

/////for abstract generation///////////
int CStrFun::Utf8BytePos(char* c, int step)
{
        int pos =       0;
        while(pos       <       step)
        {
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10)   &&      (c[pos]&0x08)       &&      (c[pos]&0x04))
                {
                        step    =       step    +       5;
                        pos     =       pos     +       6;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10)   &&      (c[pos]&0x08))
                {
                        step    =       step    +       4;
                        pos     =       pos     +       5;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10))
                {
                        step    =       step    +       3;
                        pos     =       pos     +       4;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20))
                {
                        step    =       step    +       2;
                        pos     =       pos     +       3;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40))
                {
                        step    =       step    +       1;
                        pos     =       pos     +       2;
                        continue;
                }
                pos     =       pos     +       1;
        }
        return pos;
}
//获得utf8的字���串截取的开始实际位置的函数
int CStrFun::Utf8AbsPos(char* c, int step)
{
        int pos =       0;
        int absPos=0;
        while(pos       <       step)
        {
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10)   &&      (c[pos]&0x08)       &&      (c[pos]&0x04))
                {
                        pos     =       pos     +       6;
                        absPos++;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10)   &&      (c[pos]&0x08))
                {
                        pos     =       pos     +       5;
                        absPos++;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20)   &&      (c[pos]&0x10))
                {
                        pos     =       pos     +       4;
                        absPos++;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40)   &&      (c[pos]&0x20))
                {
                        pos     =       pos     +       3;
                        absPos++;
                        continue;
                }
                if((c[pos]&0x80)        &&      (c[pos]&0x40))
                {
                        pos     =       pos     +       2;
                        absPos++;
                        continue;
                }
                pos++;
                absPos++;
        }
        return absPos;
}
//返回截取的字符串
string CStrFun::GetStr(string s,std::vector<string> &keywords, int step)
{
        int i,j,k,m,start,end,start1,end1;
        int wordsNum = keywords.size();
        int d[2][255];
        string str = "";
        char* pchr = strdup(s.c_str());
        k = 0;
        for(j=0;j<wordsNum;j++)
        {
                m       =       0;
                for(i=0;i<s.length();i++)
                {
                        if(m==keywords[j].length() && keywords[j].length()!=0)
                        {
                                d[0][k] =       i-keywords[j].length();
                                d[1][k] =       i;
                                k++;
                                m       =       0;
                        }
                        if(s[i]==keywords[j][m])
                                m++;
                        else
                                m       =       0;
                        if(k>=255)
                                break;
                }
        }
        for(j=0;j<k-1;j++)
                for(m=j+1;m<k;m++)
                {
                        if(d[0][j]>d[0][m])
                        {
                                i                       =       d[0][j];
                                d[0][j] =       d[0][m];
                                d[0][m] =       i;
                                i                       =       d[1][j];
                                d[1][j] =       d[1][m];
                                d[1][m] =       i;
                        }
                }
        i=0;
        while(i<k)
        {
                start   =       Utf8AbsPos(pchr,d[0][i]);
                end     =       Utf8AbsPos(pchr,d[1][i]);
                start   =       start   -       step;
                end     =       end     + step;
                if(start>0 && i==0)
                        str     =       "...";
                if(start<0)
                        start   =       0;
                if(end>s.length())
                        end     =       s.length();
                j=i+1;
                while(j<k)
                {
                        start1  =       Utf8AbsPos(pchr,d[0][j]);
                        end1            =       Utf8AbsPos(pchr,d[1][j]);
                        start1  =       start1-step;
                        end1            =       end1+step;
                        if(start1<0)
                                start1  =       0;
                        if(end1>s.length())
                                end1    =       s.length();
                        if(start1>end)
                        {
                                break;
                        }
                        if(end1>end)
                                end     =       end1;
                        i++;
                        j++;
                }
                if(end==s.length())
                {
                        start   =       Utf8BytePos(pchr,start);
                        end     =       Utf8BytePos(pchr,end);
                        str     =       str+s.substr(start,end-start);
                }
                else
                {
                        start   =       Utf8BytePos(pchr,start);
                        end     =       Utf8BytePos(pchr,end);
                        str     =       str+s.substr(start,end-start)+"...";
                }
                i++;
        }
        return str;
}

string CStrFun::getThumbPicturePath(string picturePath) {
        string::size_type pos;
        string thumbPicturePath = "";
        if((pos = picturePath.find_last_of("/")) != string::npos){
                thumbPicturePath = picturePath.substr(0,pos) + "/thumb" + picturePath.substr(pos);
        }
        return thumbPicturePath;
}

//add by kun for word count in a string
int CStrFun::WordOccurrenceCount( std::string str, std::string word )
{
        int count(0);
        std::string::size_type word_pos( 0 );
        while ( word_pos!=std::string::npos )
        {
                word_pos = str.find(word, word_pos );
                if ( word_pos != std::string::npos )
                {
                        ++count;
                        // start next search after this word
                        word_pos += word.length();
                }
        }
        return count;
}

static const char* alfabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string CStrFun::base64_encode ( const string& sInput )
{
        unsigned char x=0, topbit=0;
        int v=0;
        string sOutput;
        do
        {
                if ( topbit < 6 )
                {
                        x++;
                        v <<= 8;
                        if ( x <= sInput.length() ) v += sInput[x-1];
                        topbit += 8;
                }
                topbit -= 6;
                if ( x > sInput.length() && !v )
                        break;
                sOutput += alfabet[(v >> topbit) & 63];
                v &= (1 << topbit) - 1;
        } while ( x < sInput.length() || v );
        int eq = (8 - (sOutput.length() % 4)) % 4;
        while ( eq-- )
                sOutput += '=';
        sOutput = ReplaceStr(sOutput,"===","A==");
        return sOutput;
}

string CStrFun::base64_decode ( const string& sInput )
{
        unsigned char x=0, topbit=0;
        int v=0, inlen = sInput.length();
        string sOutput;
        while ( inlen && sInput[inlen-1] == '=' )
                inlen--;
        do
        {
                while ( topbit < 8 )
                {
                        x++;
                        v <<= 6;
                        if ( x <= inlen ) v += (strchr(alfabet, sInput[x-1]) - alfabet);
                        topbit += 6;
                }
                topbit -= 8;
                if ( x > inlen && !v )
                        break;
                sOutput += (char)((v >> topbit) & 255);
                v &= ((1 << topbit) - 1);
        } while ( x <= inlen || v );
        return sOutput;
}


string CStrFun::Encode(string str)
{
                for(int i=0;i<str.length();i++)
                {
                str[i]=char(int(str[i])+17);
                }
      return base64_encode(str);
}

string CStrFun::Decode(string str)
{
                str=base64_decode(str);
      for(int i=0;i<str.length();i++)
                {
                str[i]=char(int(str[i])-17);
                }
      return str;
}

string CStrFun::URLEncodeString(string s) {
  string out;
  for (unsigned int x=0;x<s.length();x++) {
    if (s[x] == ' ')
      out += '+';
    else if ((s[x] >= 'a' && s[x] <= 'z')
             ||  (s[x] >= 'A' && s[x] <= 'Z') || s[x]=='.'
             ||  (s[x] >= '0' && s[x] <= '9')) {
      out += s[x];
    } else {
      out += '%';
      out += "0123456789ABCDEF"[15&(s[x]>>4)];
      out += "0123456789ABCDEF"[s[x]&15];
    }
  }
  return out;
}

//URL解码
string CStrFun::URLDecodeString(string s){
        string buffer = "";
        int len = s.length();

        for (int i = 0; i < len; i++) {
                int j = i ;
                char ch = s.at(j);
                if ((ch == '%') && ((j+2)<len)){
                        char tmpstr[] = "0x0__";
                        int chnum;
                        tmpstr[3] = s.at(j+1);
                        tmpstr[4] = s.at(j+2);
                        chnum = strtol(tmpstr, NULL, 16);
                        buffer += chnum;
                        i += 2;
                }else if (ch == '+') {
                        buffer += " ";
                } else {
                        buffer += ch;
                }
        }
        return buffer;
}

void CStrFun::stringReplaceAll(string *s, string before, string after) {
        size_t i;

        i = s->find (before);
        while (i != string::npos) {
                s->replace (i, before.length(), after);
                i = s->find (before, i+after.length());
        };
}

//add by guan:填充字符串
string CStrFun::stringFill(string s,int num){
        string buffer=s;
        int len=buffer.length();
        if(len<num){
                int icount=num-len;
                while(icount>0){
                        buffer+=" ";
                        icount--;
                }
        }
        return  buffer;
}
//add by guan:去除字符串两边的空格
string CStrFun::trim(string s){
//      s.erase(s.begin(),s.begin()+s.find_first_not_of(" "));
//      s.erase(s.begin()+s.find_last_not_of(" ")+1,s.end());

        if (s.length()==0) return "";

        int p=0;
        while (s[p]==' ') p++;
        int q=s.length()-1;
        while (s[q]==' ') q--;

        return s.substr(p,q-p+1);
}
int CStrFun::distance(const std::string source, const std::string target) {

  // Step 1

  const int n = source.length();
  const int m = target.length();
  if (n == 0) {
    return m;
  }
  if (m == 0) {
    return n;
  }

  // Good form to declare a TYPEDEF

  typedef std::vector< std::vector<int> > Tmatrix;

  Tmatrix matrix(n+1);

  // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
  // allow for allocation on declaration of 2.nd dimension of vec of vec

  for (int i = 0; i <= n; i++) {
    matrix[i].resize(m+1);
  }

  // Step 2

  for (int i = 0; i <= n; i++) {
    matrix[i][0]=i;
  }

  for (int j = 0; j <= m; j++) {
    matrix[0][j]=j;
  }

  // Step 3

  for (int i = 1; i <= n; i++) {

    const char s_i = source[i-1];

    // Step 4

    for (int j = 1; j <= m; j++) {

      const char t_j = target[j-1];

      // Step 5

      int cost;
      if (s_i == t_j) {
        cost = 0;
      }
      else {
        cost = 1;
      }

      // Step 6

      const int above = matrix[i-1][j];
      const int left = matrix[i][j-1];
      const int diag = matrix[i-1][j-1];
       int cell = min( above + 1, min(left + 1, diag + cost));

      // Step 6A: Cover transposition, in addition to deletion,
      // insertion and substitution. This step is taken from:
      // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
      // Enhanced Dynamic Programming ASM Algorithm"
      // (http://www.acm.org/~hlb/publications/asm/asm.html)

      if (i>2 && j>2) {
        int trans=matrix[i-2][j-2]+1;
        if (source[i-2]!=t_j) trans++;
        if (s_i!=target[j-2]) trans++;
        if (cell>trans) cell=trans;
      }

      matrix[i][j]=cell;
    }
  }

  // Step 7

  return matrix[n][m];
}
string CStrFun::D64_to_String(float value){
        std::stringstream number;
         if( value > 1000000000 ) {
           // number << (value/1000000000) << std::setw(9) << std::setfill('0') << (value%1000000000);
          } else {
            number << value;
         }
          return number.str();
}
int CStrFun::findOccurrence(char *input, char *chrSet) {//查找指定字符串在字符串的位置
        if ((!input) || (!chrSet))
                return -1;
        char *tmp=input;
        int cCounter=0;
        while (*tmp) {
                char *cTmp=chrSet;
                while (*cTmp) {
                        if (*cTmp==*tmp) {
                                return cCounter;
                        }
                        cTmp++;
                }
                cCounter++;
                tmp++;
        }
        return -1;
}
string CStrFun::replaceAllByChr(string inputString, string characters) {//去除指定字符串
        char *inputCopy=strdup(inputString.c_str());
        char *setCopy=strdup(characters.c_str());

        char blank = ' ';

        int thisOccurence=findOccurrence(inputCopy, setCopy);
        while (thisOccurence > -1) {
                inputCopy[thisOccurence] = blank;
                //strcpy(inputCopy+thisOccurence, inputCopy+thisOccurence+1);
                thisOccurence=findOccurrence(inputCopy, setCopy);
        }

        string outputString;
        outputString=inputCopy;
        free(inputCopy);
        free(setCopy); //add by renyongquan for debug memory leak
        return outputString;
}

string CStrFun::extractField(string fullDocText, string fieldName) {
        string startTag = "<" + fieldName + ">";
        string endTag = "</" + fieldName + ">";
        int len = strlen(fieldName.c_str());
        size_t startPlace=fullDocText.find(startTag);
        size_t endPlace=fullDocText.find(endTag);

        if (startPlace==std::string::npos||endPlace==std::string::npos||(startPlace
                        == endPlace-len-2)) {
                // mismatched ending tag - just exit.
                return "";
        }

        // get the variable...
        string field=fullDocText.substr(startPlace+len+2, (endPlace-startPlace-len
                        -2));
        if (field.size() != 0)
                return field;
        else
                return "";
}
//按照一种分割符把字符串分割开
vector<string> CStrFun::splitString(string keyword,string sToken){
/*      vector<string> vTemp;
        string strTemp;
        strTemp=trim(keyword);
        char *ckeyword=strdup(strTemp.c_str());
        char *cToken=strdup(sToken.c_str());
        char *token=strtok(ckeyword,cToken);
        while(token){
                if(strlen(trim(token).c_str())){
                        char *cstr=strdup(token);
                        if(cstr){
                                string s=cstr;
                                vTemp.push_back(s);
                        }
                }
                token=strtok(NULL,cToken);
        }
        free(ckeyword);
        free(cToken);
*/
    vector<string> vTemp;
    int p=0,q=0;
    while ((p=keyword.find(sToken,q))!=-1)
    {
        if (keyword.substr(q,p-q)!="")
           vTemp.push_back(keyword.substr(q,p-q));
        q=p+sToken.size();
    }
    if (keyword.substr(q)!="")
        vTemp.push_back(keyword.substr(q));

    return vTemp;
}
//从vector
string CStrFun::stringFromVector(vector<string> &vString){
        string strTemp="";
        if(vString.size()==0){
                return  "";
        }else{
                for(int i=0;i<vString.size();i++){
                        if(i==0){
                                strTemp+=vString[i];
                        }else{
                                strTemp+=" "+vString[i];
                        }
                }
                return strTemp;
        }
}

string CStrFun::replaceByMap(string nString,const map<string,string>& Map)
{
        size_t pos = 0;
        size_t pos_max_end = 0;
        string temp;
        map<string,string>::const_iterator itr = Map.end();
        while(pos < nString.size())
        {
                if((nString[pos] & 0xc0) == 0x80)
                {
                        ++ pos;
                        continue;
                }
                pos_max_end = (21 > (nString.size() - pos)) ? 21 : (nString.size() - pos);
                for(size_t j = pos_max_end; j != 0; --j)
                {
                        temp = nString.substr(pos,j);
                        if((itr = Map.find(temp)) != Map.end())
                        {
                                nString.replace(pos,j,itr->second);
                                pos += itr->second.size() - 1;
                                break;
                        }
                }
                ++ pos;
        }
        return nString;
}
