#include "ini.h"
#include "common.h"
namespace config {
	INI *INI::instance = new INI();

	INI::INI(){
#ifdef COMMON_CONFIG
		Open(COMMON_CONFIG);
#endif
	}

	INI::~INI(){
		delete INI::instance;
	}

	INI* INI::GetInstance(){
		if(nullptr == INI::instance){
            INI::instance=new INI();
		}
		return INI::instance;
	}

	bool INI::Open(const char* path)
	{
		return do_open(path);
	}

	string INI::Read(const char*psect, const char*pkey)
	{
		string mapkey = psect;
		mapkey += MIDDLESTRING;
		mapkey += pkey;
		auto it = c_inimap.find(mapkey);
		if(it == c_inimap.end())
			return "";
		else
			return it->second;
	}

	bool INI::do_open(const char* path)
	{
		ifstream fin(path);
		if(!fin.is_open())
			return false;
		vector<string> strvect;
		while(!fin.eof())
		{
			string inbuf;
			getline(fin, inbuf,'\n');
			trim(inbuf);
			if (inbuf=="" || inbuf[0]=='#')
				continue;
			strvect.push_back(inbuf);
		}
		if(strvect.empty())
			return false;
		for_each(strvect.begin(), strvect.end(), parseini(c_inimap));
		return !c_inimap.empty();
	}
}


