#include <iostream>
#include <list>
#include <iterator>
#include <string>
using namespace std;

// files class
class Files
{
public:
    string name;
    string data;
    int size;

    Files(string _name, string _data)
    {
        name = _name;
        data = _data;
        size = data.length();
    }

    void print()
    {
        cout << data << endl;
    }
};

// directory class
class Directory
{
public:
    string name;
    string path;
    list<Directory> *directories;
    list<Files> *files;
    Directory *parent;

    Directory(string _name, Directory *_parent)
    {
        name = _name;
        parent = _parent;
        path = setPath();

        directories = new list<Directory>();
        files = new list<Files>();
    }

    void addFile(string _name, string _data)
    {
        files->push_back(Files(_name, _data));
    }

    void addDirectory(string _name)
    {
        directories->push_back(Directory(_name, this));
    }

    void printPath()
    {
        if (path == "V")
        {
            cout << "V:\\" << endl;
        }
        else
        {
            cout << path << endl;
        }
    }

    string setPath()
    {
        if (parent == nullptr)
        {
            return "V:\\";
        }
        return parent->path + name + "\\";
    }

    // checking if directory already exists
    bool checkDirectory(string _name)
    {
        for (auto it = directories->begin(); it != directories->end(); it++)
        {
            if (it->name == _name)
            {
                return true;
            }
        }
        return false;
    }

    // checking if file already exists
    bool checkFile(string _name)
    {
        for (auto it = files->begin(); it != files->end(); it++)
        {
            if (it->name == _name)
            {
                return true;
            }
        }
        return false;
    }

    // get directory through path
    Directory *getDirectoryFromPath(string _path){
        if (_path == "V:\\" || _path == "V:"){
            return this;
        }
        else if (_path.find("\\") != string::npos) {
            string name = _path.substr(0, _path.find("\\"));

            if (name == "V:\\" || name == "V:"){
                return this->getDirectoryFromPath(_path.substr(_path.find("\\") + 1, _path.length() - 1));
            }

            for (auto it = directories->begin(); it != directories->end(); ++it){
                if (it->name == name){
                    return it->getDirectoryFromPath(_path.substr(_path.find("\\") + 1, _path.length() - 1));
                }
            }
        }
        else{
            for (auto it = directories->begin(); it != directories->end(); ++it){
                if (it->name == _path){
                    return &(*it);
                }
            }
        }
        return nullptr;
    }
};

// tree class
class Tree
{
public:
    Directory *root;
    Directory *current;

    Tree()
    {
        current = root = new Directory("V", nullptr);
    }

    void addFile(string _name, string _data)
    {
        root->files->push_back(Files(_name, _data));
    }

    void addDirectory(string _name)
    {
        root->directories->push_back(Directory(_name, root));
    }

    void printPath()
    {
        cout << root->path << endl;
    }

    // dir function
    void dir()
    {
        if (current->directories->size() == 0)
        {
            cout << "no files" << endl;
        }
        else
        {
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
            {
                cout << "           DIR         " << it->name << endl;
            }
            cout << endl
                 << endl;
            for (auto it = current->files->begin(); it != current->files->end(); ++it)
            {
                cout << "           File(s)         " << "(" << it->size << ") bytes " << it->name << endl;
            }

            cout << endl << "\t\t(" << current->directories->size() << ") Dir(s)" << endl;
            cout << "\t\t(" << current->files->size() << ") File(s)" << endl;
        }
    }

    // mkdir function
    void mkdir(string _name){

        if (_name.find("\\") != string::npos){
            cout << "Invalid syntax" << endl;
            return;
        }

        // checking if directory already exists
        if (!current->checkDirectory(_name))
        {
            current->directories->push_back(Directory(_name, current));
        }
        else
        {
            cout << "Directory already exists" << endl;
        }
    }

    // rmdir function
    void rmdir(string _name){
        if (!current->checkDirectory(_name)){
            cout << "Directory not found" << endl;
            return;
        }
        for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
            if (it->name == _name)
            {
                current->directories->erase(it);
                break;
            }
        }
    }

    // mkfile function
    void mkfile(string _name)
    {

        // checking for duplicate file names
        if (!current->checkFile(_name))
        {
            cout << "Enter file data: ";
            string fileData = getInput();
            current->addFile(_name, fileData);
        }
        else
        {
            cout << "File already exists" << endl;
        }
    }

    // rmfile function
    void rmfile(string _name)
    {
        if (!current->checkFile(_name)){
            cout << "File not found" << endl;
            return;
        }
        for (auto it = current->files->begin(); it != current->files->end(); ++it){
            if (it->name == _name){
                current->files->erase(it);
                break;
            }
        }
    }
    // rename function
    void rename(string oldname)
    {
        string currentnName = oldname.substr(0, oldname.find(" "));
        cout << "current name is " << currentnName << endl;
        string newName = oldname.substr(oldname.find(" ") + 1, oldname.length() - 1);
        cout << "new name is " << newName << endl;
        if (current->checkFile(currentnName)){
            for (auto it = current->files->begin(); it != current->files->end(); ++it){
                if (it->name == currentnName){
                    it->name = newName;
                    cout << it->name << endl;
                    return;
                }
            }
        }
        else if (current->checkDirectory(currentnName)) {
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it){
                if (it->name == currentnName){
                    // do update the path of the directory as well
                    it->path = it->parent->path + newName + "\\";
                    it->name = newName;
                    cout << it->name << endl;
                    return;
                }
            }
        }
        else{
            cout << "file or directory not found" << endl;
        }
    }

    void attrib(string att)
    {
        bool isExist = current->checkFile(att);
        if (isExist)
        {
            for (auto it = current->files->begin(); it != current->files->end(); ++it)
            {
                if (it->name == att)
                {
                    cout << "file name is " << it->name << endl;
                    cout << "file data is " << it->data << endl;
                    return;
                }
            }
        }
        else
        {
            cout << "file not found" << endl;
        }
    }
    void find(string filename)
    {
        bool isExist = current->checkFile(filename);
        if (isExist)
        {
            for (auto it = current->files->begin(); it != current->files->end(); ++it)
            {
                if (it->name == filename)
                {
                    cout << "file name is " << it->name << endl;
                    cout << "file data is " << it->data << endl;
                    return;
                }
            }
        }
        else
        {
            cout << "file not found" << endl;
        }
    }

    // findf function
    void findf(string name, string text){

    }

    // findStr function
    void findStr(string text){
        for (auto it = current->files->begin(); it != current->files->end(); ++it){
            if (it->data.find(text) != string::npos){
                cout << "file name is " << it->name << endl;
                cout << "file data is " << it->data << endl;
            }
        }
    }

    // format function
    void format(){
        root = current = new Directory("V", nullptr);
    }

    // move directory form source to destination or file
    void move(string input){

    }


    // copy directory form source to destination or file
    void copy(string input){

    }

    // input function
    bool input()
    {
        cout << current->path << ">";
        string input = getInput();

        if (input == "exit"){
            system("cls");
            return false;
        }
        else if (input == "format"){
            format();
        }
        else if (input == "dir"){
            dir();
        }
        else if (input == "cd.." || input == "cd .."){
            if (current->parent != nullptr) {
                current = current->parent;
            }
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
            header();
        }
        else if (input.substr(0, 3) == "cd "){

            Directory* temp = current->getDirectoryFromPath(input.substr(3, input.length() - 1));
            if (temp != nullptr){
                current = temp;
            }
            else{
                cout << "Directory not found" << endl;
            }

        }
        else if (input.substr(0, 8) == "findstr "){
            findStr(input.substr(8, input.length() - 1));
        }
        else if (input.substr(0, 7) == "attrib "){
            attrib(input.substr(7, input.length() - 1));
        }
        else if (input.substr(0, 5) == "move "){
            string sourceAndDes = input.substr(5, input.length() - 1);
            move(sourceAndDes);
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
        else if (input == "pwd"){
            current->printPath();
        }
        else if (input.substr(0, 6) == "rename"){
            string oldname = input.substr(7, input.length() - 1);
            rename(oldname);
        }
        else if (input.substr(0, 5) == "rmfil"){
            rmfile(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 5) == "find "){
            string filename = input.substr(5, input.length() - 1);
            find(filename);
        }
        else{
            cout << "Invalid command" << endl;
        }
        return true;
    }

    // get input
    string getInput()
    {
        string input;
        getline(cin, input);
        return input;
    }

    // run function
    void run()
    {
        system("cls");
        header();
        while (input())
            ;
    }

    // help function
    void help()
    {
        cout << "attrib - see the file data" << endl;
        cout << "find <file name> - find file" << endl;
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
        cout << "cls - clear screen" << endl;
        cout << "format for format the disk" << endl;
    }

    void header(){
        cout << "DOS Shell\tDSA Project\t" << endl;
        cout << "2022-CS-115 && 2022-CS-123" << endl;
        cout << "==========================" << endl;
    
    }
};

int main()
{
    Tree tree;

    tree.run();

    return 0;
}