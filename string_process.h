
#pragma once
#include <string>
#include <vector>
#include <sys/time.h>
//这个文件可以采用C++11替代且代码简便编写
static int64_t gtm() {
	struct timeval tm;
	gettimeofday(&tm, 0);
	int64_t re = ((int64_t)tm.tv_sec) * 1000 * 1000 + tm.tv_usec;
	return re;
}

static void splitStr(const std::string& inputStr, const std::string &key, std::vector<std::string>& outStrVec) {
	if (inputStr == "") {
		return;
	}
	int pos = inputStr.find(key);
	int oldpos = 0;
	if (pos > 0) {
		std::string tmp = inputStr.substr(0, pos);
		outStrVec.push_back(tmp);
	}
	while (1) {
		if (pos < 0) {
			break;
		}
		oldpos = pos;
		int newpos = inputStr.find(key, pos + key.length());
		std::string tmp = inputStr.substr(pos + key.length(), newpos - pos - key.length());
		outStrVec.push_back(tmp);
		pos = newpos;
	}
	int tmplen = 0;
	if (outStrVec.size() > 0) {
		tmplen = outStrVec.at(outStrVec.size() - 1).length();
	}
	if (oldpos + tmplen < inputStr.length() - 1) {
		std::string tmp = inputStr.substr(oldpos + key.length());
		outStrVec.push_back(tmp);
	}
}

static std::string trim(std::string &s) {
	if (s.empty()) {
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

static int toInt(const std::string &in){
	int re = 0;
	sscanf(in.c_str(), "%d", &re);
	return re;
}
static float toFloat(const std::string &in) {
	float re = 0;
	sscanf(in.c_str(), "%f", &re);
	return re;
}
static std::string toStr(float in) {
	char chr[20] = { 0 };
	sprintf(chr, "%f", in);
	std::string re(chr);
	return re;
}
static std::string toStr(int in){
	char chr[20] = {0};
	sprintf(chr, "%d", in);
	std::string re(chr);
	return re;
}
static std::string to4dStr(int in){
	char chr[20] = {0};
	sprintf(chr, "%04d", in);
	std::string re(chr);
	return re;
}
static std::string to5dStr(int in){
	char chr[20] = {0};
	sprintf(chr, "%05d", in);
	std::string re(chr);
	return re;
}
static std::string to6dStr(int in){
	char chr[20] = {0};
	sprintf(chr, "%06d", in);
	std::string re(chr);
	return re;
}

