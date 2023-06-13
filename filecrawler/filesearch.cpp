#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <deque> 
#include <unordered_map>
#include <algorithm>

using namespace std;

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

deque<nfile*> fileseach(nfolder *drive, string filename){

    unordered_map<nfile*, string> visitedNodes;
    deque<nfile*> filepath; 
    queue<nfile*> bfs; 

    if(drive->children.size() == 0){ 
        return filepath; 
    }

    bfs.push(drive); 

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
            if(currentNode->name != filename){ 
                continue;
            }
            cout << endl<< "Printing File Path.\n" ;
            while(!bfs.empty()){ 
                bfs.pop();
            }

            nfile *temp = currentNode;

            while(temp && (temp->node_type != ntype::DRV)){ 
                filepath.push_back(temp);             
                cout << temp->name << endl;
                temp = temp->parent;
            }
        }
    }
    cout << endl;
    return filepath;
}

//#define DEBUG_FILELOAD
void buildDirectoryStructure(ifstream& file, nfolder* currentFolder){ 
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
            buildDirectoryStructure(file, fr);
        }
    }

}

int main(int argc, char *argv[]) {

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
    nfolder drive; 

    buildDirectoryStructure(inputFile, &drive);

    deque<nfile*> target; 
    deque<nfile*> source; 

    //cout << endl << "Calling FileSearch" << endl << endl;
    target = fileseach(&drive, targetFileName); 

    //cout << endl << "Calling FileSearch" << endl << endl;
    source = fileseach(&drive, sourceFileName); 

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