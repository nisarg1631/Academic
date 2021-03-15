#ifndef BOOK_HPP
#define BOOK_HPP

#include "utils.hpp"

class book{
    protected:
        string path, filename, title, author;

        void get_book_info(){
            const string title_prefix = "Title: ";
            const string author_prefix = "Author: ";
            ifstream in((path+filename).c_str());
            string line;
            while(getline(in,line) && (title.empty() || author.empty())){
                if(!line.compare(0,title_prefix.size(),title_prefix))
                    title=line.substr(title_prefix.length());
                if(!line.compare(0,author_prefix.size(),author_prefix))
                    author=line.substr(author_prefix.length());
            }
            if(title[title.size()-1]=='\r')
                title.erase(title.size()-1);
            if(author[author.size()-1]=='\r')
                author.erase(author.size()-1);
            in.close();
        }

    public:
        enum Type { novel, play };

        book() {}
        book(string path, string filename) : path(path), filename(filename){
            get_book_info();
        }
        ~book() {}
        
        void print_info(){
            cout<<"\tFilename: "<<filename<<endl;
            cout<<"\tTitle: "<<title<<endl;
            cout<<"\tAuthor: "<<author<<endl;
            cout<<endl;
        }
        
        //returns true if search is a substring of title (case insensitive)
        bool title_search(string search){
            return is_substring(title,search);
        }

        //returns true if search is a substring of author (case insensitive)
        bool author_search(string search){
            return is_substring(author,search);
        }

        void read_book(){
            ifstream in((path+filename).c_str());
            string line, separator("#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#");
            vector<string> page(40);
            int cnt=0;
            while(getline(in,line)){
                page[cnt++]=line;
                if(cnt==40){
                    cout<<'\t'<<separator<<endl;
                    for(int i=0; i<cnt; i++)
                        cout<<page[i]<<endl;
                    cout<<'\t'<<separator<<'\n'<<endl;
                    cnt=0;
                    char op;
                    cout<<"To continue reading input 'c', to return to previous menu input 'q': "; cin>>op;
                    cout<<endl;
                    if(op!='c')
                        break;
                }
            }
            if(cnt){
                cout<<'\t'<<separator<<endl;
                for(int i=0; i<cnt; i++)
                    cout<<page[i]<<endl;
                cout<<"\nEND OF BOOK."<<endl;
                cout<<'\t'<<separator<<'\n'<<endl;
            }
            in.close();
        }

        virtual Type get_type() = 0;
        virtual void book_actions() = 0;
};

class novel : public book{

    vector<chapter> chapters;

    void parse(){
        ifstream in((path+filename).c_str());
        enum State { none, chapter_change, in_chapter, in_para };
        State state = none;
        string line;
        const string chapter_prefix="CHAPTER";
        int chap_num=0, para_num=0;
        chapter curr_chapter;
        paragraph curr_paragraph;
        while(getline(in,line)){
            if(line[line.size()-1]=='\r')
                line.erase(line.size()-1);
            if(line.empty()){
                if(state==chapter_change){
                    state=in_chapter;
                }
                if(state==in_para){
                    curr_chapter.paragraphs.push_back(curr_paragraph);
                    state=in_chapter;
                }
            }
            else if(!line.compare(0,chapter_prefix.size(),chapter_prefix)){
                if(state!=none){
                    chapters.push_back(curr_chapter);
                }
                state=chapter_change;
                para_num=0;
                chap_num++;
                curr_chapter.paragraphs.clear();
                curr_chapter.chapter_num=chap_num;
                curr_chapter.chapter_name=line;
            }
            else{
                if(state==in_chapter){
                    para_num++;
                    state=in_para;
                    curr_paragraph.sentences.clear();
                    curr_paragraph.paragraph_num=para_num;
                    curr_paragraph.sentences.push_back(sentence(format_string(line)));
                }
                else if(state==in_para){
                    curr_paragraph.sentences.push_back(sentence(format_string(line)));
                }
                else if(state==chapter_change){
                    curr_chapter.chapter_name.append(" "+line);
                }
            }
        }
        if(state!=none){
            chapters.push_back(curr_chapter);
        }
        in.close();
    }

    void word_counter(){
        string word;
        cout<<"Enter a word: "; cin>>word;

        vector< pair<int, int> > chapter_freq;
        vector< pair<int, pair<int,int> > > para_freq;
        for(int i=0; i<chapters.size(); i++){
            chapter_freq.push_back(make_pair(chapters[i].get_occurence(word),chapters[i].chapter_num));
            for(int j=0; j<chapters[i].paragraphs.size(); j++){
                pair<int, int> para_info=make_pair(chapters[i].chapter_num, chapters[i].paragraphs[j].paragraph_num);
                para_freq.push_back(make_pair(chapters[i].paragraphs[j].get_occurence(word), para_info));
            }
        }
        sort(chapter_freq.begin(),chapter_freq.end(),greater< pair<int,int> > ());
        sort(para_freq.begin(),para_freq.end(),greater< pair<int, pair<int,int> > > ());

        size_t sz;
        cout<<"Enter number of top chapters and paragraphs to display: "; cin>>sz;
        cout<<"\n\t Top "<<min(chapter_freq.size(), sz)<<" occurences of \""<<word<<"\" in chapters:\n";
        for(int i=0; i<min(chapter_freq.size(), sz); i++)
            cout<<"\t> "<<chapter_freq[i].first<<" occurences in CHAPTER "<<chapter_freq[i].second<<endl;
        cout<<endl;
        cout<<"\n\t Top "<<min(para_freq.size(), sz)<<" occurences of \""<<word<<"\" in paragraphs:\n";
        for(int i=0; i<min(para_freq.size(), sz); i++)
            cout<<"\t> "<<para_freq[i].first<<" occurences in CHAPTER "<<para_freq[i].second.first<<" PARAGRAPH "<<para_freq[i].second.second<<endl;
        cout<<endl;
    }

    public:
        novel() {}
        novel(string path, string filename) : book(path, filename){}
        ~novel() {}

        Type get_type(){
            return book::novel;
        }

        void book_actions(){
            if(chapters.empty())
                parse();
            cout<<"\n\tNovel "<<title<<" selected.\n"<<endl;
            int op=1;
            while(op==1||op==2){
                cout<<"Book actions:"<<endl;
                cout<<"1> Read the novel."<<endl;
                cout<<"2> Find top chapters and paragraphs based on word count."<<endl;
                cout<<"3> Return to main menu."<<endl;
                cout<<"Select option: "; cin>>op;
                cout<<endl;
                if(op==1)
                    read_book();
                else if(op==2)
                    word_counter();
            }
        }
};

class play : public book{

    vector<act> acts;
    vector<string> characters;

    void parse(){
        ifstream in((path+filename).c_str());
        enum State { none, in_act, in_scene };
        State state = none;
        string line;
        const string act_prefix="ACT";
        const string scene_prefix="SCENE";
        int act_num=0, scene_num=0;
        scene curr_scene;
        act curr_act;
        //set to store all characters in the play
        set<string> character_set;
        while(getline(in,line)){
            if(line[line.size()-1]=='\r')
                line.erase(line.size()-1);
            if(!line.compare(0,act_prefix.size(),act_prefix)){
                if(state!=none){
                    character_set.insert(curr_scene.characters.begin(), curr_scene.characters.end());
                    curr_act.scenes.push_back(curr_scene);
                    acts.push_back(curr_act);
                }
                state=in_act;
                scene_num=0;
                act_num++;
                curr_act.scenes.clear();
                curr_act.act_num=act_num;
            }
            else if(!line.compare(0,scene_prefix.size(),scene_prefix)){
                if(state==in_act)
                    state=in_scene;
                else if(state==in_scene){
                    character_set.insert(curr_scene.characters.begin(), curr_scene.characters.end());
                    curr_act.scenes.push_back(curr_scene);
                }
                
                scene_num++;
                curr_scene.characters.clear();
                curr_scene.scene_num=scene_num;
            }
            else if(is_play_character(line)){
                if(state==in_scene)
                    curr_scene.add_character(line);
            }
        }
        if(state!=none){
            character_set.insert(curr_scene.characters.begin(), curr_scene.characters.end());
            curr_act.scenes.push_back(curr_scene);
            acts.push_back(curr_act);
        }
        characters.assign(character_set.begin(), character_set.end());
        in.close();
    }

    void character_relations(){
        int op;
        cout<<"List of characters:"<<endl;
        for(int i=0; i<characters.size(); i++)
            cout<<i+1<<"> "<<characters[i]<<endl;
        cout<<"Choose character (integer from 1 to "<<characters.size()<<"): "; cin>>op;
        if(op>=1 && op<=characters.size()){
            string curr=characters[op-1];
            cout<<"\n\t Characters appearing with "<<curr<<"\n"<<endl;
            set<string> related;
            for(int i=0; i<acts.size(); i++){
                for(int j=0; j<acts[i].scenes.size(); j++){
                    if(acts[i].scenes[j].characters.count(curr))
                        related.insert(acts[i].scenes[j].characters.begin(), acts[i].scenes[j].characters.end());
                }
            }
            related.erase(curr);
            for(set<string>::iterator it=related.begin(); it!=related.end(); it++)
                cout<<"\t> "<<*it<<endl;
            cout<<endl;
        }
        else
            cout<<"\n\tPlease enter an integer in the given range.\n"<<endl;
    }

    public:
        play() {}
        play(string path, string filename) : book(path, filename){}
        ~play() {}

        Type get_type(){
            return book::play;
        }

        void book_actions(){
            if(acts.empty())
                parse();
            cout<<"\n\tPlay "<<title<<" selected.\n"<<endl;
            int op=1;
            while(op==1||op==2){
                cout<<"Book actions:"<<endl;
                cout<<"1> Read the play."<<endl;
                cout<<"2> Characters appearing in a scene with a specific character."<<endl;
                cout<<"3> Return to main menu."<<endl;
                cout<<"Select option: "; cin>>op;
                cout<<endl;
                if(op==1)
                    read_book();
                else if(op==2)
                    character_relations();
            }
        }
};

#endif