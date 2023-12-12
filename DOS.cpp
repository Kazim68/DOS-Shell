#include <iostream>
#include <list>
#include <iterator>
#include <string>
using namespace std;

// files class
class Files{
public:
    string name;
    string data;

    Files(string _name, string _data){
        name = _name;
        data = _data;
    }

    void print(){
        cout << data << endl;
    }
};


// directory class
class Directory{
public:
    string name;
    string path;
    list<Directory>* directories;
    list<Files>* files;
    Directory* parent;

    Directory(string _name, Directory* _parent){
        name = _name;
        parent = _parent;
        path = setPath();

        directories = new list<Directory>();
        files = new list<Files>();
    }

    void addFile(string _name, string _data){
        files->push_back(Files(_name, _data));
    }

    void addDirectory(string _name){
        directories->push_back(Directory(_name, this));
    }

    void printPath(){
        if (path == "V"){
            cout << "V:\\" << endl;
        } else {
            cout << path << endl;
        }
    }

    string setPath(){
        if (parent == nullptr){
            return "V:\\";
        }
        return parent->path + name + "\\";
    }

    // checking if directory already exists
    bool checkDirectory(string _name){
        for (auto it = directories->begin(); it != directories->end(); it++){
            if (it->name == _name){
                return true;
            }
        }
        return false;
    }

    // checking if file already exists
    bool checkFile(string _name){
        for (auto it = files->begin(); it != files->end(); it++){
            if (it->name == _name){
                return true;
            }
        }
        return false;
    }
};


// tree class
class Tree{
public:
    Directory* root;
    Directory* current;

    Tree(){
        current = root = new Directory("V", nullptr);
    }

    void addFile(string _name, string _data){
        root->files->push_back(Files(_name, _data));
    }

    void addDirectory(string _name){
        root->directories->push_back(Directory(_name, root));
    }

    void printPath(){
        cout << root->path  << endl;
    }

    // cd function
    void cd(string _name){
        if (_name == ".."){
            if (current->parent != nullptr){
                current = current->parent;
            }
        } else {
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
                if (it->name == _name){
                    current = &(*it);
                    return;
                }
            }
            cout << "Directory not found" << endl;
        }
    }

    // dir function
    void dir(){
       if(current->directories->size()==0){
            cout<<"no files"<<endl;
       }
       else{
         for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
            cout << "           DIR         " << it->name << endl;
        }
        cout << endl << endl;
        for (auto it = current->files->begin(); it != current->files->end(); ++it){
            cout << "           File(s)         " << it->name << endl;
        }
       }
    }

    // mkdir function
    void mkdir(string _name){

        // checking if directory already exists
        if (!current->checkDirectory(_name)){
            current->directories->push_back(Directory(_name, current));
        } 
        else {
            cout << "Directory already exists" << endl;
        }
    }




    // rmdir function
    void rmdir(string _name){
        for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
            if (it->name == _name){
                current->directories->erase(it);
                break;
            }
        }
    }

    // mkfile function
    void mkfile(string _name){

        // checking for duplicate file names
        if (!current->checkFile(_name)){
            cout << "Enter file data: ";
            string fileData = getInput();
            current->addFile(_name, fileData);
        } 
        else {
            cout << "File already exists" << endl;
        }
    }

    // rmfile function
    void rmfile(string _name){
        for (auto it = current->files->begin(); it != current->files->end(); ++it){
            if (it->name == _name){
                current->files->erase(it);
                break;
            }
        }
    }
    //rename function
    void rename(string oldname){
        string currentnName = oldname.substr(0,oldname.find(" "));
        cout<<"current name is "<<currentnName<<endl;
        string newName = oldname.substr(oldname.find(" ")+1,oldname.length()-1);
        cout<<"new name is "<<newName<<endl;
        if(current->checkFile(currentnName)){
            for (auto it = current->files->begin(); it != current->files->end(); ++it){
                if (it->name == currentnName){
                    it->name=newName;
                    cout<<it->name<<endl;
                    return;
                }
            }
        }
        else if(current->checkDirectory(currentnName)){
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
                if (it->name == currentnName){
                    // do update the path of the directory as well
                    it->path=it->parent->path+newName+"\\";
                    it->name=newName;
                    cout<<it->name<<endl;
                    return;
                }
            }
        }
        else{
            cout<<"file or directory not found"<<endl;
        }
    }

    void attrib(string att){
       bool isExist = current->checkFile(att);
         if(isExist){
            for (auto it = current->files->begin(); it != current->files->end(); ++it){
                if (it->name == att){
                    cout<<"file name is "<<it->name<<endl;
                    cout<<"file data is "<<it->data<<endl;
                    return;
                }
            }
         }
         else{
             cout<<"file not found"<<endl;
         }
        
    }

    // input function
    bool input(){
        
        cout << current->path << ">";
        string input = getInput();

        if (input == "exit"){
            system("cls");
            return false;
        }
        else if(input.substr(0,7)=="attrib "){
            attrib(input.substr(7,input.length()-1));
        }
        
        else if (input == "dir"){
            dir();
        } 
        else if (input == "cd.."){
            cd("..");
        } 
        else if (input == "cd/"){
            current = root;
        }
        else if (input == "cd."){
            current->printPath();
        }
        else if (input == "help"){
            help();
        }
        else if (input == "cls"){
            system("cls");
        }
        else if (input.substr(0, 3) == "cd "){
            cd(input.substr(3, input.length() - 1));
        }
        else if (input.substr(0, 5) == "mkdir"){
            mkdir(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 6) == "mkfile"){
            mkfile(input.substr(7, input.length() - 1));
        } 
        else if (input.substr(0, 5) == "rmdir"){
            rmdir(input.substr(6, input.length() - 1));
        } 
        else if(input=="pwd"){
            current->printPath();
        }
        else if(input.substr(0,6)=="rename"){
            string oldname=input.substr(7,input.length()-1);
            rename(oldname);
        }
        else if (input.substr(0, 5) == "rmfil"){
            rmfile(input.substr(6, input.length() - 1));
        } 
        else {
            cout << "Invalid command" << endl;
        }
        return true;
    
    }

    // get input
    string getInput(){
        string input;
        getline(cin, input);
        return input;
    }

    // run function
    void run(){
        while (input());
    }

    // help function
    void help(){
        cout << "dir - list directory" << endl;
        cout << "cd.. - go to parent directory" << endl;
        cout << "cd <directory name> - go to directory" << endl;
        cout << "mkdir <directory name> - create directory" << endl;
        cout << "mkfile <file name> - create file" << endl;
        cout << "rmdir <directory name> - remove directory" << endl;
        cout << "rmfile <file name> - remove file" << endl;
        cout << "pwd see the current working directory" << endl;
        cout << "rename <old name> <new name> rename file or directory" << endl;
        cout << "exit - exit program" << endl;
    }
};

// split function
string split(string input, char splitter){
    string output = "";
    int j = 0;
    for (int i = 0; i < input.length(); i++){
        if (input[i] == splitter){
            j = i + 1;
            break;
        }
    }
    for (int i = j; i < input.length(); i++){
        output += input[i];
    }
    return output;
}

int main(){
    Tree tree;

    tree.run();
    
    return 0;
}