#ifndef __Config_INI_H__
#define __Config_INI_H__
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>

using namespace std;

namespace config
{

typedef map<string, string, less<string> > strMap;
typedef strMap::iterator strMapIt;

const char*const MIDDLESTRING = "_____|||||_______";
inline std::string& ltrim(std::string &ss, int (*pf)(int)=std::isspace)
{
    std::string::iterator p=std::find_if(ss.begin(),ss.end(),std::not1(std::ptr_fun(pf)));
    ss.erase(ss.begin(),p);
    return ss;
}
inline std::string& rtrim(std::string &ss, int (*pf)(int)=std::isspace)
{
    std::string::reverse_iterator p=std::find_if(ss.rbegin(),ss.rend(),std::not1(std::ptr_fun(pf)));
    ss.erase(p.base(),ss.end());
    return ss;
}
inline std::string& trim(std::string &st)
{
    ltrim(rtrim(st));
    return st;
}

struct parseini{
	string strsect;
	strMap *pmap;
	parseini(strMap & strmap):pmap(&strmap){}
	void operator()( const string & strini)
	{
		size_t first =strini.find("[");
		size_t last = strini.rfind("]");
		if( first != string::npos && last != string::npos && first != last+1)
		{
			strsect = strini.substr(first+1,last-first-1);
			return ;
		}
		if(strsect.empty())
				return ;

		first =strini.find("=");
		if(first== string::npos)
			return ;

		string strtmp1= strini.substr(0,first);
		string strtmp2=strini.substr(first+1, string::npos);
		first= strtmp1.find_first_not_of(" \t");
		last = strtmp1.find_last_not_of(" \t");
		if(first == string::npos || last == string::npos)
			return ;
		string strkey = strtmp1.substr(first, last-first+1);
		first = strtmp2.find_first_not_of(" \t");
		//配置注释
        if(((last = strtmp2.find("\t#", first )) != -1) ||
                ((last = strtmp2.find(" #", first )) != -1) ||
                ((last = strtmp2.find("\t//", first )) != -1)||
                ((last = strtmp2.find(" //", first )) != -1))
        {
                strtmp2 = strtmp2.substr(0, last-first);
        }
        last = strtmp2.find_last_not_of(" \t");
        if(first == string::npos || last == string::npos)
            return ;
        string value = strtmp2.substr(first, last-first+1);
        string mapkey = strsect + MIDDLESTRING;
        mapkey += strkey;
        (*pmap)[mapkey]=trim(value);
        return ;
	}
};

class INI
{
private:
    static INI *instance ;

public:
	INI();
    ~INI();

    static INI *GetInstance();

    bool Open(const char* path);

    string Read(const char*psect, const char*pkey);
	
protected:
    bool do_open(const char* pinipath);
	
    strMap    c_inimap;
};

}

#endif
