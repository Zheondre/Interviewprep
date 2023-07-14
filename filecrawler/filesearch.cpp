#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <deque> 
#include <unordered_map>
#include <algorithm>

using namespace std;

// compile with g++ ./filesearch

enum{ 
    FILECRAWLER, 
    ADDFILE, 
    RMFILE, 
    ADDIR, 
    RMDIR, 
    LOAD, 
    SAVE, 
    PURGE, 
    OPTIONS,
    EXIT, 
};

enum ntype{ 
    DRV,
    FOLDER, 
    FLE, 
};

struct nfile{  
    ntype node_type;
    nfile* parent;
    string name;
    vector<nfile*> children; 
    nfile(){};
    ~nfile(){
        cout << "Removing " << this->name << endl; 
        this->parent = NULL; 
        this->name.clear(); 
        this->children.clear();
    }
    nfile(string name){ 
        node_type = ntype::FLE; 
        this->name = name;
    } 
};

struct nfolder : nfile {  
    nfolder(){
        node_type = ntype::DRV; 
        this->parent = NULL;
        this->name = "DRIVE";
    }
     nfolder(string name) {
        node_type = ntype::FOLDER; 
        this->name = name;
     }

};

class flemgr{ 

    string newcontent; 
    // key: full path to file or folder. Val: file pointer.
    unordered_map<string, nfile*> filePath; 
    unordered_map<nfile*, string> filename; 
    int filecount;
    //vector<nfile*> drive;

    public:
    
    nfolder drive; 

    int mode; 
    flemgr(){
        mode = 0;
        filecount =0;
    };
    flemgr(int mode){ 
        this->mode = mode; 
        filecount =0;
    };

    deque<nfile*> fileseach(const string& filename);
    bool addContent(const string& filename);
    void setcontentname(string name);

   void buildDirectoryStructure(ifstream&, nfolder*, string);
};

deque<nfile*> flemgr::fileseach(const string& filename){

    unordered_map<nfile*, string> visitedNodes;
    deque<nfile*> filepath; 
    queue<nfile*> bfs; 

    if(drive.children.size() == 0){ 
        return filepath; 
    }

    bfs.push(&drive); 

    while(!bfs.empty()) { 

       nfile* currentNode = bfs.front(); 
       bfs.pop();

       if(visitedNodes.find(currentNode) != visitedNodes.end()){
            continue; 
        } 

        visitedNodes[currentNode] = currentNode->name;

        if(currentNode->node_type != ntype::FLE){ 
            for(int i =0; i < currentNode->children.size(); i++){ 
                bfs.push(currentNode->children[i]);
            }
        } else { 

            nfile *temp = currentNode;

            switch(mode){
                case FILECRAWLER:
                    
                    if(currentNode->name != filename){ 
                        continue;
                    }
                    cout << endl<< "Printing File Path.\n" ;
                    while(!bfs.empty()){ 
                        bfs.pop();
                    }

                    while(temp && (temp->node_type != ntype::DRV)){ 
                        filepath.push_back(temp);             
                        cout << temp->name << endl;
                        temp = temp->parent;
                    }
                break;
                case ADDFILE: 
                    nfolder *fr = new nfolder(this->newcontent); 
                    fr->parent = temp;
                break;
                case RMFILE: 
                    cout << "Removing " << temp->name << endl; 
                    delete(temp);
                    temp = NULL;
                break;
                case ADDIR: 
                    //addir(filename, &drive);
                break; 
                
                case RMDIR: 
                break; 

                case PURGE: 
                    delete(temp); 
                    temp = NULL;
                    //filecount--;
                break;
            }
        }
    }
    cout << endl;
    return filepath;
}

//#define DEBUG_FILELOAD
void flemgr::buildDirectoryStructure(ifstream& file, nfolder* currentFolder, string pwd){ 
    string line; 
    //cout << endl << "In BuildS" <<endl;

    while(getline(file,line)){
        if(line.find("*") != string::npos)
        { //represents the end of the current folder
            return; 
        }
        int pos = line.find('-');
        if(pos >= 0)
        {   // lines with - are files
            nfile *fle = new nfile(line.substr(pos+2, line.length()-1)); 
#ifdef DEBUG_FILELOAD
            //cout << line << endl;
            cout << currentFolder->name << " added " << fle->name << endl;
#endif
            fle->parent = currentFolder; 
            currentFolder->children.push_back(fle);
        } else 
        {  // Line indicates a folder
            line.erase(
                remove_if
                (line.begin(), line.end(), 
                []
                (char c)
                {
                    return ( c == ' ' || c == '\t');
                }), 
                line.end()
            );

            nfolder *fr = new nfolder(line); 
            fr->parent = currentFolder;
            currentFolder->children.push_back(fr); 
#ifdef DEBUG_FILELOAD
            cout << currentFolder->name << " added " << fr->name << endl;
#endif
            buildDirectoryStructure(file, fr,  pwd += " -> " + currentFolder->name );
        }
    }

}

//void PathFinder
void report(){ 

}

int main(int argc, char *argv[]) {

    const char *options =  R""""(
        0 - Find shortest path between two files.\n 
        1 - Add file.\n
        2 - Remove file.\n
        3 - Add dir.\n
        4 - Remove dir.\n
        5 - Load drive structure.\n
        6 - Save drive structure to file.\n
        7 - Options.\n
        8 - Exit.\n)"""";

    std::ifstream inputFile("testcase2.txt");
    if(!inputFile){ 
        cerr << "Failed to open the input file." << endl; 
        return 1; 
    }

    // Read the source and target file names from input
    std::string sourceFileName, targetFileName;
    
    //sourceFileName = "File2";
    //targetFileName = "File6";

    sourceFileName = "File5";
    targetFileName = "File9";

    /*
    std::cout << "Enter the source file name: ";
    std::cin >> sourceFileName;
    std::cout << "Enter the target file name: ";
    std::cin >> targetFileName;
    */ 
    
    //nfolder drive; 

    int mode = 0;

    deque<nfile*> target; 
    deque<nfile*> source; 

    flemgr mgr ;

    switch(mode){
        case FILECRAWLER: 
            mgr.buildDirectoryStructure(inputFile, &mgr.drive, "");
            target = mgr.fileseach(targetFileName); 
            source = mgr.fileseach(sourceFileName); 
        break;
        case ADDFILE: 
            // // FolderA -> SubfolderA -> File2 -> FolderB -> SubfolderD -> File6 
            std::cout << "Enter the path + the source file name: ";
            std::cin >> sourceFileName;

        break;
        case RMFILE: 
            if(mgr.drive.children.size() < 1){ 
                cout << "nothing to delete" <<endl;
                break;
            }
            std::cout << "Enter the path + source file name: ";
            std::cin >> sourceFileName;
            mgr.setcontentname(sourceFileName);
 
            // get path 
            //rm(filename, &drive);
        break;
        case ADDIR: 
            std::cout << "Enter the path + dir name: ";
            std::cin >> sourceFileName;

        //addir(filename, &drive);
        break; 
        case RMDIR: 
            if(mgr.drive.children.size() < 1){ 
                cout << "nothing to delete" <<endl;
                break;
            }
            std::cout << "Enter the path + dir name: ";
            std::cin >> sourceFileName;

            mgr.setcontentname(sourceFileName);

        break;
        case LOAD:

        //if files are loaded file an error.
        buildDirectoryStructure(inputFile, &mgr.drive);

        break;
        case SAVE:
        break; 
        case PURGE: 
            mgr.fileseach(""); 
        break;
        case OPTIONS: 
        cout << options << endl;
        break; 
        case EXIT:
            return 0;
        break; 

        default: 
        cout << "Option doesn't exist."<<endl;
    };

    vector<string> fpath;

    while(
        (!target.empty()) && 
        (!source.empty()) && 
        (target.back() == source.back())
        )
    {
        target.pop_back(); 
        source.pop_back();
    }

    if(target.empty() && source.empty()){ 
        cout << "files are mssing." <<endl; 
        return 1;
    }
    
    if(target.empty() || source.empty()){ 
        cout << "A file is missing." <<endl; 
        return 1;
    }

    if((target.size() == 1) && (source.size() == 1)){ 
        if(target.front()->name == source.front()->name){ 
            fpath.push_back(target.front()->parent->name); 
            fpath.push_back(source.front()->name);  
            fpath.push_back(target.front()->name); 

            cout << target.front()->parent->name << " -> "; 
            cout << target.front()->name << " -> " ; 
            cout << source.front()->name;
            return 0;       
        }
    }

    while(!source.empty()){ 
        fpath.push_back(source.back()->name);
        source.pop_back();
    }

    while(!target.empty()){ 
        fpath.push_back(target.back()->name);
        target.pop_back();
    }

    cout << "The shortest path is" <<endl;
    for(int i = 0 ; i < fpath.size() -1 ; i++ ){ 
        cout << fpath[i] << " -> " ; 
    }
    cout << fpath[fpath.size()-1];
}
//target                            //source
// FolderA -> SubfolderA -> File2 -> FolderB -> SubfolderD -> File6  