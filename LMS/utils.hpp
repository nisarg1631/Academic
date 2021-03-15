#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <dirent.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

vector<string> get_directory_content(string path){
    vector<string> content;
    dirent *file;
    DIR *directory = opendir(path.c_str());
    if(directory!=NULL){
        while(file=readdir(directory))
            if((file->d_name)[0]!='.' && string(file->d_name).compare("index.txt"))
                content.push_back(file->d_name);
    }
    else
        throw string("Invalid directory");
    return content;
}

map<string, int> read_index(string path){
    ifstream in((path+"index.txt").c_str());
    map<string, int> data;
    string line;
    while(getline(in,line)){
        stringstream ss(line);
        string name; int type;
        ss>>name>>type;
        data[name]=type;
    }
    in.close();
    return data;
}

//converts string to lowercase
string to_lower(string str){
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

//does a case insensitive check to determine if substr is a substring of str
inline bool is_substring(string str, string substr){
    return (to_lower(str).find(to_lower(substr))!=string::npos);
}

//utilty function for format_string
inline char remove_special(char c){
    if((c>='0' && c<='9') || (c>='a' && c<='z') || (c>='A' && c<='Z'))
        return c;
    else
        return ' ';
}

//replaces all special characters in input string with a blank space
string& format_string(string& str){
    transform(str.begin(), str.end(), str.begin(), remove_special);
    return str;
}

//returns true if all characters encountered before a '.' are capital english letters
bool is_play_character(string line){
    for(int i=0; i<line.size(); i++){
        if(line[i]=='.')
            return true;
        if((line[i]<'A' || line[i]>'Z') && line[i]!=' ')
            return false;
    }
    return true;
}

//classes which define the structure of a novel
class sentence{
    vector<string> words;
    public:
        sentence() {}
        sentence(string line){
            stringstream ss(line);
            string word;
            while(ss>>word)
                words.push_back(word);
        }
        int get_occurence(string str){
            int cnt=0;
            for(int i=0; i<words.size(); i++)
                if(to_lower(str)==to_lower(words[i]))
                    cnt++;
            return cnt;
        }
    friend class novel;
};

class paragraph{
    int paragraph_num;
    vector<sentence> sentences;
    public:
        int get_occurence(string str){
            int cnt=0;
            for(int i=0; i<sentences.size(); i++)
                cnt+=sentences[i].get_occurence(str);
            return cnt;
        }
    friend class novel;
};

class chapter{
    int chapter_num;
    string chapter_name;
    vector<paragraph> paragraphs;
    public:
        int get_occurence(string str){
            int cnt=0;
            for(int i=0; i<paragraphs.size(); i++)
                cnt+=paragraphs[i].get_occurence(str);
            return cnt;
        }
    friend class novel;
};

//classes which define the structure of a play
class scene{
    int scene_num;
    set<string> characters;
    public:
        void add_character(string line){
            string name;
            for(int i=0; i<line.size() && line[i]!='.'; i++)
                name.push_back(line[i]);
            if(!name.empty())
                characters.insert(name);
        }
    friend class play;
};

class act{
    int act_num;
    vector<scene> scenes;
    friend class play;
};

#endif