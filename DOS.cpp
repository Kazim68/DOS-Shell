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
                    break;
                }
            }
        }
    }

    // dir function
    void dir(){
        for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
            cout << "           DIR         " << it->name << endl;
        }
        cout << endl << endl;
        for (auto it = current->files->begin(); it != current->files->end(); ++it){
            cout << "           File(s)         " << it->name << endl;
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

    // input function
    bool input(){

        cout << current->path << ">";
        string input = getInput();

        if (input == "exit"){
            system("cls");
            return false;
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
            string fileName = input.substr(7, input.length() - 1);
            cout << "Enter file data: ";
            string fileData = getInput();
            current->addFile(fileName, fileData);
        } 
        else if (input.substr(0, 5) == "rmdir"){
            string dirName = input.substr(6, input.length() - 1);
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
                if (it->name == dirName){
                    current->directories->erase(it);
                    break;
                }
            }
        } 
        else if (input.substr(0, 5) == "rmfil"){
            string fileName = input.substr(6, input.length() - 1);
            for (auto it = current->files->begin(); it != current->files->end(); ++it){
                if (it->name == fileName){
                    current->files->erase(it);
                    break;
                }
            }
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