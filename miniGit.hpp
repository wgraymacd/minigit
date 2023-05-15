#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// used to store a list of files in the current commit
struct singlyNode
{
    string fileName;    // name of local file
    int fileVersion;    // name of file in .minigit folder
    string newFileName;
    singlyNode *next;
};

// nodes for the DLL, each represent a commit
struct doublyNode
{
    int commitNumber;
    singlyNode *head;
    doublyNode *previous;
    doublyNode *next;
};

class Git
{
public:
    Git()
    {
        
    }
    ~Git()
    {
        // code
        doublyNode *doublyNodeTraverse = currDir;
        doublyNode *toDeleteD;
        singlyNode *singlyNodeTraverse = currDir->head;
        singlyNode *toDeleteS;
        while (doublyNodeTraverse != NULL)
        {
            toDeleteD = doublyNodeTraverse;
            while(singlyNodeTraverse != NULL)
            {
                toDeleteS = singlyNodeTraverse;
                singlyNodeTraverse = singlyNodeTraverse->next;
                delete toDeleteS;                
            }
        
            doublyNodeTraverse = doublyNodeTraverse->previous;
            delete toDeleteD;
        } 
    }
    void initialize(); // create DLL with single head node SLL and commit number 0,
    void addFile();    // add a file to the SLL with version number
    void removeFile(); // delete file if found
    void commit();     // traverse SLL, do some checks, add to .minigit directory, then create new DLL node with copy of previous one
    void checkout();   // check number, overwright current directory files with .minigit ones
    void status();     // check the singly list nodes for the additions and removals
private:
    //currDir is the dir that we are currently on
    doublyNode *currDir;
};
