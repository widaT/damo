#include "ini.h"
#include "common.h"
namespace config {
	INI *INI::instance = nullptr;

	INI::INI(){
        string path = cwd();
        #ifdef DEBUG
            string config_path = "/home/wida/cppworkspace/damo/etc/conf.conf"; //本地开发调试时使用
        #else
            string config_path = path+"/etc/conf.conf";
        #endif
        if (!(Open(config_path.c_str()))){
             cerr << "open config file(" << config_path << ") error!"  <<endl;
             exit(-1);
        }
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


