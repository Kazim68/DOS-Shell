#include <iostream>
#include <list>
#include <iterator>
#include <string>
#include <queue>
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

    bool operator<(const Files &other) const
    {
        return size < other.size;
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

    Files *findFile(string _name)
    {
        if (_name.find("V:\\") != string::npos || _name.find("V:") != string::npos)
        {
            return findFile(_name.substr(_name.find("\\") + 1, _name.length() - 1));
        }
        else if (_name.find("\\") != string::npos)
        {
            string name = _name.substr(0, _name.find("\\"));

            for (auto it = directories->begin(); it != directories->end(); ++it)
            {
                if (it->name == name)
                {
                    return it->findFile(_name.substr(_name.find("\\") + 1, _name.length() - 1));
                }
            }
        }
        else
        {
            for (auto it = files->begin(); it != files->end(); ++it)
            {
                if (it->name == _name)
                {
                    return &(*it);
                }
            }
        }
        return nullptr;
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
    Directory *getDirectoryFromPath(string _path)
    {
        if (_path == "V:\\" || _path == "V:")
        {
            return this;
        }
        else if (_path.find("\\") != string::npos)
        {
            string name = _path.substr(0, _path.find("\\"));

            if (name == "V:\\" || name == "V:")
            {
                return this->getDirectoryFromPath(_path.substr(_path.find("\\") + 1, _path.length() - 1));
            }

            for (auto it = directories->begin(); it != directories->end(); ++it)
            {
                if (it->name == name)
                {
                    return it->getDirectoryFromPath(_path.substr(_path.find("\\") + 1, _path.length() - 1));
                }
            }
        }
        else
        {
            for (auto it = directories->begin(); it != directories->end(); ++it)
            {
                if (it->name == _path)
                {
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
    string prompt = "> ";
    queue<Files *> *printFiles;
    priority_queue<Files *> *printFilesPriority;
    int timer;

    Tree()
    {
        current = root = new Directory("V", nullptr);
        printFiles = new queue<Files *>();
        printFilesPriority = new priority_queue<Files *>();
        timer = 0;
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
        if (current->directories->size() == 0 && current->files->size() == 0)
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
                cout << "           File(s)         "
                     << "(" << it->size << ") bytes " << it->name << endl;
            }

            cout << endl
                 << "\t\t(" << current->directories->size() << ") Dir(s)" << endl;
            cout << "\t\t(" << current->files->size() << ") File(s)" << endl;
        }
    }

    // mkdir function
    void mkdir(string _name)
    {

        if (_name.empty())
        {
            cout << "Invalid syntax" << endl;
            return;
        }

        if (_name.find("\\") != string::npos || _name == "V:\\" || _name == "V:")
        {
            cout << "Invalid syntax" << endl;
            return;
        }

        // checking if directory already exists
        if (!current->checkDirectory(_name) && !current->checkFile(_name))
        {
            current->directories->push_back(Directory(_name, current));
        }
        else
        {
            cout << "Directory or name already exists" << endl;
        }
    }

    // rmdir function
    void rmdir(string _name)
    {
        if (!current->checkDirectory(_name))
        {
            cout << "Directory not found" << endl;
            return;
        }
        for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
        {
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

        if (_name.empty())
        {
            cout << "Invalid syntax" << endl;
            return;
        }
        string text = _name.substr(_name.find(".") + 1, _name.length() - 1);
        if (text != "txt")
        {
            _name = _name.substr(0, _name.find(".")) + ".txt";
        }
        // checking for duplicate file names
        if (!current->checkFile(_name) && !current->checkDirectory(_name))
        {
            // add txt extension in the file name if it is not provieded

            cout << "Enter file data: ";
            string fileData = getInput();
            current->addFile(_name, fileData);
        }
        else
        {
            cout << "File or name already exists" << endl;
        }
    }

    // rmfile function
    void rmfile(string _name)
    {
        if (!current->checkFile(_name))
        {
            cout << "File not found" << endl;
            return;
        }
        for (auto it = current->files->begin(); it != current->files->end(); ++it)
        {
            if (it->name == _name)
            {
                current->files->erase(it);
                break;
            }
        }
    }

    string getNameWithExt(string name){
        if (name.find(".") != string::npos){
            if (name.substr(name.find("."), name.size() - 1) != "txt"){
                name = name.substr(0, name.find(".")) + ".txt";
            }
        }
        else{
            name += ".txt";
        }
        return name;
    }
    // rename function
    void rename(string oldname)
    {
        string currentnName = oldname.substr(0, oldname.find(" "));
        currentnName = getNameWithExt(currentnName);
        cout<<currentnName<<endl;
        string newName = oldname.substr(oldname.find(" ")+1, oldname.length() - 1);
        newName = getNameWithExt(newName);
        cout << newName << endl;
        if (current->checkFile(currentnName))
        {
            for (auto it = current->files->begin(); it != current->files->end(); ++it)
            {
                if (it->name == currentnName)
                {
                    it->name = newName;
                    return;
                }
            }
        }
        else if (current->checkDirectory(currentnName))
        {
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
            {
                if (it->name == currentnName)
                {
                    // do update the path of the directory as well
                    it->path = it->parent->path + newName + "\\";
                    it->name = newName;
                    return;
                }
            }
        }
        else
        {
            cout << "file or directory not found" << endl;
        }
    }

    void attrib(string att)
    {
        Files *file = current->findFile(att);
        if (file == nullptr)
        {
            cout << "file not found" << endl;
            return;
        }
        cout << "file name is " << file->name << endl;
        cout << "file data is " << file->data << endl;
    }
    void find(string filename)
    {

        Files *file = current->findFile(filename);
        if (file == nullptr)
        {
            cout << "file not found" << endl;
            return;
        }
        cout << "file name is " << file->name << endl;
        cout << "file data is " << file->data << endl;
    }

    // findf function
    void findf(string input)
    {

        string name = input.substr(0, input.find(" "));
        string text = input.substr(input.find(" ") + 1, input.length() - 1);

        Files *file = current->findFile(name);
        if (file == nullptr)
        {
            cout << "file not found" << endl;
            return;
        }
        string fileData = file->data;
        if (fileData.find(text) != string::npos)
        {
            cout << "text found" << endl;
            cout << "file name is " << file->name << endl;
            cout << "file data is " << file->data << endl;
        }
        else
        {
            cout << "text not found" << endl;
        }
    }

    // findStr function
    void findStr(string text)
    {
        for (auto it = current->files->begin(); it != current->files->end(); ++it)
        {
            if (it->data.find(text) != string::npos)
            {
                cout << "file name is " << it->name << endl;
                cout << "file data is " << it->data << endl;
            }
        }
    }

    // format function
    void format()
    {
        root = current = new Directory("V", nullptr);
    }

    // utility function to delete a file given specified a path
    void deleteFile(string src)
    {
        if (src.find("V:\\") != string::npos || src.find("V:") != string::npos)
        {
            return deleteFile(src.substr(src.find("\\") + 1, src.length() - 1));
        }
        else if (src.find("\\") != string::npos)
        {
            string name = src.substr(0, src.find("\\"));

            for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
            {
                if (it->name == name)
                {
                    return deleteFile(src.substr(src.find("\\") + 1, src.length() - 1));
                }
            }
        }
        else
        {
            for (auto it = current->files->begin(); it != current->files->end(); ++it)
            {
                if (it->name == src)
                {
                    current->files->erase(it);
                    return;
                }
            }
        }
    }

    // utility function to delete a directory given specified a path
    void deleteDirectory(string src)
    {
        if (src.find("V:\\") != string::npos || src.find("V:") != string::npos)
        {
            return deleteDirectory(src.substr(src.find("\\") + 1, src.length() - 1));
        }
        else if (src.find("\\") != string::npos)
        {
            string name = src.substr(0, src.find("\\"));

            for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
            {
                if (it->name == name)
                {
                    return deleteDirectory(src.substr(src.find("\\") + 1, src.length() - 1));
                }
            }
        }
        else
        {
            for (auto it = current->directories->begin(); it != current->directories->end(); ++it)
            {
                if (it->name == src)
                {
                    current->directories->erase(it);
                    return;
                }
            }
        }
    }

    // utility function to set path after moving directory
    void setPathAfterMove(Directory *src)
    {
        for (auto it = src->directories->begin(); it != src->directories->end(); ++it)
        {
            it->path = src->path + it->name + "\\";
            setPathAfterMove(&(*it));
        }
    }

    // move directory form source to destination or file
    void move(string input)
    {
        string src = input.substr(0, input.find(" "));
        string des = input.substr(input.find(" ") + 1, input.length() - 1);

        if (src.empty() || des.empty())
        {
            cout << "Invalid syntax" << endl;
            return;
        }

        // check if source exists
        Directory *srcDir = current->getDirectoryFromPath(src);
        Files *srcFile = current->findFile(src);
        if (srcDir == nullptr && srcFile == nullptr)
        {
            cout << "Source not found" << endl;
            return;
        }

        // check if destination exists
        Directory *desDir = current->getDirectoryFromPath(des);
        Files *desFile = current->findFile(des);
        if (desFile != nullptr)
        {
            // simply copy content of source file to destination file and remove source file
            if (srcFile != nullptr)
            {
                desFile->data = srcFile->data;
                desFile->size = srcFile->size;
                deleteFile(src);
            }
            else if (srcDir != nullptr)
            {
                cout << "Cannot move directory to file" << endl;
            }
            return;
        }
        else if (desDir == nullptr)
        {
            cout << "Destination not found" << endl;
            return;
        }

        // check if destination is not a child of source
        if (srcDir != nullptr)
        {
            Directory *temp = desDir;
            while (temp != nullptr)
            {
                if (temp == srcDir)
                {
                    cout << "Cannot move directory to its child" << endl;
                    return;
                }
                temp = temp->parent;
            }
        }

        // move directory to destination
        if (srcDir != nullptr)
        {
            srcDir->parent = desDir;
            srcDir->path = desDir->path + srcDir->name + "\\";
            desDir->directories->push_back(*srcDir);
            setPathAfterMove(srcDir);
            deleteDirectory(src);
        }
        else if (srcFile != nullptr)
        {
            // move file to destination
            desDir->files->push_back(*srcFile);
            deleteFile(src);
        }
    }

    // copy directory form source to destination or file
    void copy(string input)
    {

        // folder to folder -> it will copy all the files inside this directory to destination
        // file to folder-> it will copy the file to destination if there is no file of it's name
        // file to file -> it will copy the data
        // folder to file -> it will copy and add the data of all the files in the directory

        int count = 0;

        string src = input.substr(0, input.find(" "));
        string des = input.substr(input.find(" ") + 1, input.length() - 1);

        if (src.empty() || des.empty())
        {
            cout << "Invalid syntax" << endl;
            return;
        }

        // check if source exists
        Directory *srcDir = current->getDirectoryFromPath(src);
        Files *srcFile = current->findFile(src);

        if (srcDir == nullptr && srcFile == nullptr)
        {
            cout << "Source not found" << endl;
            return;
        }

        // check if destination exists
        Directory *desDir = current->getDirectoryFromPath(des);
        Files *desFile = current->findFile(des);

        if (desFile == nullptr && desDir == nullptr)
        {
            cout << "Destination not found" << endl;
            return;
        }

        // now 4 cases

        // folder to folder
        if (srcDir != nullptr && desDir != nullptr)
        {

            // copy all the files in the directory to destination
            for (auto it = srcDir->files->begin(); it != srcDir->files->end(); ++it)
            {
                if (desDir->checkFile(it->name))
                {
                    Files *temp = desDir->findFile(it->name);
                    temp->data = it->data;
                    temp->size = it->size;
                }
                else
                {
                    desDir->files->push_back(copyFile(&(*it)));
                }
                count++;
            }

            cout << count << " file(s) copied" << endl;
        }
        // file to folder
        else if (srcFile != nullptr && desDir != nullptr)
        {

            // check if file of same name exists
            if (desDir->checkFile(srcFile->name))
            {
                Files *temp = desDir->findFile(srcFile->name);
                temp->data = srcFile->data;
                temp->size = srcFile->size;
                cout << "1 file(s) copied" << endl;
                return;
            }

            desDir->files->push_back(copyFile(srcFile));
            cout << "1 file(s) copied" << endl;
        }
        // file to file
        else if (srcFile != nullptr && desFile != nullptr)
        {
            desFile->data = srcFile->data;
            desFile->size = srcFile->size;
            cout << "1 file(s) copied" << endl;
        }
        // folder to file
        else if (srcDir != nullptr && desFile != nullptr)
        {

            // copy all the files in the directory to destination
            desFile->data = "";
            desFile->size = 0;
            for (auto it = srcDir->files->begin(); it != srcDir->files->end(); ++it)
            {
                desFile->data += it->data;
                desFile->size += it->size;
                count++;
            }

            cout << count << " file(s) copied" << endl;
        }
    }

    // make a new file and copy the data of the file in it
    Files copyFile(Files *src)
    {
        return Files(src->name, src->data);
    }

    // add file to queue
    void print(string input)
    {
        Files *file = current->findFile(input);

        if (file == nullptr)
        {
            cout << "File not found" << endl;
            return;
        }

        printFiles->push(file);
    }

    // add file to priority queue
    void pprint(string input)
    {
        Files *file = current->findFile(input);

        if (file == nullptr)
        {
            cout << "File not found" << endl;
            return;
        }

        printFilesPriority->push(file);
    }

    // print files in queue
    void Queue()
    {
        if (printFiles->empty())
        {
            cout << "No files in queue" << endl;
            return;
        }

        for (int i = 0; i < printFiles->size(); i++)
        {
            Files *temp = printFiles->front();
            printFiles->pop();
            cout << temp->name << endl;
            printFiles->push(temp);
        }
    }

    // print files in priority queue
    void pqueue()
    {
        if (printFilesPriority->empty())
        {
            cout << "No files in queue" << endl;
            return;
        }

        priority_queue<Files *> *temp = new priority_queue<Files *>();

        while (!printFilesPriority->empty())
        {
            Files *file = printFilesPriority->top();
            printFilesPriority->pop();
            cout << file->name << endl;
            temp->push(file);
        }

        printFilesPriority = temp;
    }

    void tree(Directory *temp, int space = 0)
    {
        for (int i = space - 5; i > 0; i--)
        {
            if (i == space - 5)
            {
                cout << "|";
            }
            else
            {
                cout << " ";
            }
        }

        if (space >= 5)
        {
            cout << "|_____";
        }
        cout << temp->name << endl;

        for (auto it = temp->directories->begin(); it != temp->directories->end(); ++it)
        {
            tree(&(*it), space + 5);
        }
    }

    // input function
    bool input()
    {
        cout << current->path << prompt;
        string input = getInput();

        if (input == "exit")
        {
            system("cls");
            return false;
        }
        else if (input == "prompt")
        {
            prompt = prompt == "> " ? "$ " : "> ";
        }
        else if (input == "format")
        {
            format();
        }
        else if (input == "dir")
        {
            dir();
        }
        else if (input == "tree")
        {
            tree(current);
        }
        else if (input == "cd.." || input == "cd ..")
        {
            if (current->parent != nullptr)
            {
                current = current->parent;
            }
        }
        else if (input == "cd/")
        {
            current = root;
        }
        else if (input == "cd.")
        {
            current->printPath();
        }
        else if (input == "help")
        {
            help();
        }
        else if (input == "cls")
        {
            system("cls");
            header();
        }
        else if (input.substr(0, 3) == "cd ")
        {

            Directory *temp = current->getDirectoryFromPath(input.substr(3, input.length() - 1));
            if (temp != nullptr)
            {
                current = temp;
            }
            else
            {
                cout << "Directory not found" << endl;
            }
        }
        else if (input.substr(0, 8) == "findstr ")
        {
            findStr(input.substr(8, input.length() - 1));
        }
        else if (input.substr(0, 6) == "findf ")
        {
            findf(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 5) == "copy ")
        {
            copy(input.substr(5, input.length() - 1));
        }
        else if (input.substr(0, 5) == "move ")
        {
            move(input.substr(5, input.length() - 1));
        }
        else if (input.substr(0, 7) == "attrib ")
        {
            attrib(input.substr(7, input.length() - 1));
        }
        else if (input.substr(0, 5) == "mkdir")
        {
            mkdir(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 6) == "mkfile")
        {
            mkfile(input.substr(7, input.length() - 1));
        }
        else if (input.substr(0, 5) == "rmdir")
        {
            rmdir(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 6) == "print ")
        {
            print(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 7) == "pprint ")
        {
            pprint(input.substr(7, input.length() - 1));
        }
        else if (input.substr(0, 5) == "queue")
        {
            Queue();
        }
        else if (input.substr(0, 6) == "pqueue")
        {
            pqueue();
        }
        else if (input == "pwd")
        {
            current->printPath();
        }
        else if (input.substr(0, 6) == "rename")
        {
            string oldname = input.substr(7, input.length() - 1);
            rename(oldname);
        }
        else if (input.substr(0, 5) == "rmfil")
        {
            rmfile(input.substr(6, input.length() - 1));
        }
        else if (input.substr(0, 5) == "find ")
        {
            string filename = input.substr(5, input.length() - 1);
            find(filename);
        }
        else
        {
            cout << "Invalid command" << endl;
        }

        timer++;

        if (timer == 3)
        {
            timer = 0;
            if (!printFiles->empty())
            {
                printFiles->pop();
            }
            if (!printFilesPriority->empty())
            {
                printFilesPriority->pop();
            }
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
        cout << "findf <file name> <text> - find file with text" << endl;
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
        cout << "move <source> <destination> move file or directory" << endl;
        cout << "copy <source> <destination> copy file or directory" << endl;
        cout << "findstr <text> find text in files" << endl;
        cout << "prompt change prompt" << endl;
        cout << "tree prints all directories in tree structure" << endl;
        cout << "print <file name> add file to queue" << endl;
        cout << "pprint <file name> add file to priority queue" << endl;
        cout << "queue print files in queue" << endl;
        cout << "pqueue print files in priority queue" << endl;
    }

    void header()
    {
        cout << "DOS Shell\tDSA Project\t" << endl;
        cout << "2022-CS-115 && 2022-CS-123" << endl;
        cout << "==========================" << endl;
    }
};

// text editor
class TextEditor
{
public:
    // TO-DO
};

int main()
{
    Tree tree;

    tree.run();

    return 0;
}