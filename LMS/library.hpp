#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "utils.hpp"
#include "book.hpp"

class library{
    string path;
    map<string, book*> books;

    public:
        library() {}
        library(string path) : path(path){
            vector<string> content = get_directory_content(path);
            map<string, int> data = read_index(path);

            for(int i=0; i<content.size(); i++){
                int op;
                if(data.count(content[i]))
                    op=data[content[i]]+1;
                else{
                    cout<<"New book "<<content[i]<<" found. Select type of book:"<<endl;
                    cout<<"1. Novel"<<endl;
                    cout<<"2. Play"<<endl;
                    cout<<"Select option [1, 2] (integer other than these will select play by default): "; cin>>op;
                    cout<<endl;
                }
                if(op==1)
                    books[content[i]]=new novel(path, content[i]);
                else
                    books[content[i]]=new play(path, content[i]);
            }
            ofstream out((path+"index.txt").c_str());
            for(map<string, book*>::iterator it=books.begin(); it!=books.end(); it++)
                out<<it->first<<" "<<it->second->get_type()<<endl;
            out.close();

            cout<<"\tLIBRARY INITIALISED"<<endl;
        }

        void print_list(vector<book*> booklist){
            for(int i=0; i<booklist.size(); i++){
                cout<<"\t<"<<i+1<<">"<<endl;
                booklist[i]->print_info();
            }
        }

        void list_all(){
            if(books.empty()){
                cout<<"\tNo books present in library."<<endl;
                return;
            }
            vector<book*> booklist;
            for(map<string, book*>::iterator it=books.begin(); it!=books.end(); it++)
                booklist.push_back(it->second);
            print_list(booklist);  
            book_select(booklist);          
        }

        void list_by_title(){
            string search;
            cout<<"Enter title to search for: "; cin>>search;
            vector<book*> booklist;
            for(map<string, book*>::iterator it=books.begin(); it!=books.end(); it++){
                if(it->second->title_search(search))
                    booklist.push_back(it->second);
            }
            if(booklist.empty()){
                cout<<"\n\tNo books match the given search criteria."<<endl;
                return;
            }
            cout<<"\n\tSEARCH RESULTS\n"<<endl;
            print_list(booklist);
            book_select(booklist);
        }

        void list_by_author(){
            string search;
            cout<<"Enter author to search for: "; cin>>search;
            vector<book*> booklist;
            for(map<string, book*>::iterator it=books.begin(); it!=books.end(); it++){
                if(it->second->author_search(search))
                    booklist.push_back(it->second);
            }
            if(booklist.empty()){
                cout<<"\n\tNo books match the given search criteria."<<endl;
                return;
            }
            cout<<"\n\tSEARCH RESULTS\n"<<endl;
            print_list(booklist);
            book_select(booklist);
        }

        void book_select(vector<book*> booklist){
            int op;
            cout<<"Select a book from above list (integer from 1 to "<<booklist.size()<<")"<<endl;
            cout<<"or enter 0 to return to main menu: "; cin>>op;
            if(op>=1 && op<=booklist.size())
                booklist[op-1]->book_actions();
        }
};

#endif