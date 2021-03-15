#include "library.hpp"

void exec_lib(string path){
    library lib(path);
    int op=0;
    while(op!=4){
        cout<<endl;
        cout<<"\t#### LMS ####\n"<<endl;
        cout<<"1> List all books"<<endl;
        cout<<"2> Search for book by title"<<endl;
        cout<<"3> Search for book by author"<<endl;
        cout<<"4> Exit"<<endl;
        cout<<endl;
        cout<<"Enter option: "; cin>>op;
        cout<<endl;
        if(op==1)
            lib.list_all();
        else if(op==2)
            lib.list_by_title();
        else if(op==3)
            lib.list_by_author();
        else if(op==4)
            cout<<"\tBYE!"<<endl;
        else
            cout<<"\tInvalid option."<<endl;
    }
}

int main(int argc, char **argv){
    try{
        if(argc>1){
            string path = argv[1];
            if((*(path.end()))!='/')
                path.append("/");
            exec_lib(path);
        }
        else
            throw string("Please pass directory as command line argument.");
    }
    catch(const string& errormsg){
        cerr<<errormsg<<endl;
    }
    return 0;
}