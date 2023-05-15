#include "git.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
using namespace std;

void Git::initialize()
{
    int userOption = 0;
    cout << "Are you sure you want to create an empty repository?\n1. Yes\n2. No" << endl;
    cin >> userOption;
    if(userOption != 1)
    {
        return;
    }
    if (mkdir(".minigit", 0777) == -1)
    {
        cerr << "Existing directory has been detected. Using existing directory." << endl;

    }
    else
    {
        cout << "Directory Created \n";
    }
    doublyNode *dn = new doublyNode;
    dn->commitNumber = 0;
    dn->head = NULL;
    dn->next = NULL;
    dn->previous = NULL;
    currDir = dn;
    return;
}

bool findFile_Helper(string name)
{
    ifstream file;
    bool find = false;
    while (!find)
    {
        file.open(name);
        //if the file is not open, that means that the file does not exist and the file need to be created
        if (file.fail())
        {
            cout << "File does not exist. Please enter a valid file name." << endl;
            find = false;
            return find;
        }
        //if the file is found, continue on
        else
        {
            file.close();
            find = true;
            return find;
        }
    }
    return find;
}

void Git::addFile()
{
    string fName;
    //check file existance in the directory
    bool findFile = false;
    while (!findFile)
    {
        cout << "Enter a file name: ";
        cin >> fName;
        findFile = findFile_Helper(fName);
    }
    //nodes created to traverse SLL
    singlyNode *searcher;
    singlyNode *searcherPrev;
    searcher = currDir->head;
    //check previous existance in SLL
    while (searcher != NULL)
    {
        searcherPrev = searcher;
        if (searcher->fileName == fName)
        {
            cout << "File has already been added" << endl;
            return;
        }
        searcher = searcher->next;
    }
    //creating new temp node to store the new node
    singlyNode *tempSinglyNode = new singlyNode;
    tempSinglyNode->fileName = fName;
    tempSinglyNode->fileVersion = 0;
    tempSinglyNode->next = NULL;
    tempSinglyNode->newFileName = "";
    //if the new node to be added is the head
    if (searcher == currDir->head)
    {
        currDir->head = tempSinglyNode;
    }
    //if the new node isn't the head
    else
    {
        searcherPrev->next = tempSinglyNode;
    }
    cout << "File added to current commit." << endl;
    return;
}

void Git::removeFile()
{
    string fName;
    //check file existance in the directory
    bool findFile = false;
    while (!findFile)
    {
        cout << "Enter a file name: ";
        cin >> fName;
        findFile = findFile_Helper(fName);
    }
    //nodes created to traverse SLL
    singlyNode *searcher;
    singlyNode *searcherPrev;
    searcher = currDir->head;
    //check previous existance in SLL
    bool canDelte = false;
    while (searcher != NULL)
    {
        searcherPrev = searcher;
        if (searcher->fileName == fName)
        {
            canDelte = true;
        }
        searcher = searcher->next;
    }
    if (canDelte)
    {
        //delte the SLL node
        searcher = currDir->head;
        searcherPrev = NULL;
        while (searcher != NULL)
        {
            //if the searcher file name is found
            if (searcher->fileName == fName)
            {
                //if the searcher is the head of the SLL
                if (searcher == currDir->head)
                {
                    currDir->head = searcher->next;
                    delete searcher;
                }
                //if the searcher if the tail of the SLL
                else if (searcher->next == NULL)
                {
                    searcherPrev->next = NULL;
                    delete searcher;
                }
                else
                {
                    searcherPrev->next = searcher->next;
                    delete searcher;
                }
            }
            searcherPrev = searcher;
            searcher = searcher->next;
        }
        cout << "File successfuly deleted." << endl;
    }
    else
    {
        cout << "This file cannot be deleted" << endl;
    }
    return;
}

int getIndex(singlyNode *temp)
{
    for (int i = 0; i < temp->fileName.length(); i++)
    {
        if (temp->fileName[i] == '.')
        {
            return i;
        }
    }
    return 0;
}

//helper function to copy a SLL
singlyNode *copyList(singlyNode *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        //make a new node and set its values to the node we copy from
        singlyNode *newNode = new singlyNode;
        newNode->fileName = head->fileName;
        newNode->fileVersion = head->fileVersion;
        newNode->newFileName = head->newFileName;
        //recursively call to make the next pointer equal to the copied lists next
        newNode->next = copyList(head->next);
        return newNode;
    }
}

void Git::commit()
{
    //new file to be added
    singlyNode *temp = currDir->head;
    // ifstream to find the file in the miniGit directory
    ifstream findInMiniGit;
    ifstream toCopy;
    ifstream toCompare;
    ofstream copy;
    string line;
    string lineToCompare;
    bool changesMade = false;
    int period = 0;
    bool isEqual = true;
    while (temp != NULL)
    {
        temp->newFileName = temp->fileName;
        period = getIndex(temp);
        // set the var fileName to be filename + version + .<ending>
        temp->newFileName = temp->newFileName.substr(0, period) + to_string(temp->fileVersion) + temp->newFileName.substr(period);
        findInMiniGit.open("./.minigit/" + temp->newFileName);
        // if the file is not open create a new file
        if (findInMiniGit.fail())
        {
            toCopy.open(temp->fileName);
            copy.open("./.minigit/" + temp->newFileName);
            while (getline(toCopy, line))
            {
                copy << line << endl;
            }
            toCopy.close();
            copy.close();
            changesMade = true;
        }
        // if the fileVersion does exist
        else
        {
            // compare the file in the curr repository
            isEqual = true;
            toCompare.open(temp->fileName);
            while (getline(findInMiniGit, line) && getline(toCompare, lineToCompare))
            {
                if (lineToCompare != line)
                {
                    isEqual = false;
                    break;
                }
            }
            toCompare.close();
            // if the file is equal to the old one, dont do anything
            if (isEqual)
            {
                //do nothing
                cout << "No changes to " << temp->fileName << " have been detected." << endl;
            }
            else
            {
                cout << "Changes have been detected in " << temp->fileName << ". Files have been updated." << endl;
                // increment the version
                temp->fileVersion++;
                // reset the file name
                temp->newFileName = temp->fileName;
                // update the file name
                temp->newFileName = temp->newFileName.substr(0, period) + to_string(temp->fileVersion) + temp->newFileName.substr(period);
                // add the new file version to the .minigit
                toCopy.open(temp->fileName);
                copy.open("./.minigit/" + temp->newFileName);
                while (getline(toCopy, line))
                {
                    copy << line << endl;
                }
                toCopy.close();
                copy.close();
                changesMade = true;
            }
        }
        temp = temp->next;
        findInMiniGit.close();
    }

    if (changesMade)
    {
        // set commitNumber, previous, next, and head
        doublyNode *committed = new doublyNode;
        committed->commitNumber = currDir->commitNumber + 1;
        committed->previous = currDir;
        committed->next = NULL;
        committed->head = copyList(currDir->head);

        currDir = committed;
        cout << "Staged files have been committed." << endl;
    }
    else
    {
        cout << "No files have been modified. Currently on commit #" << currDir->commitNumber << endl;
    }
    return;
}

void Git::checkout()
{
    int userCommit = 0;
    int currentDirCommit = currDir->commitNumber;
    cout << "Enter a version you would like to check out: ";
    cin >> userCommit;
    if (userCommit > currentDirCommit || userCommit < 0)
    {
        cout << "Invalid Version." << endl;
        return;
    }
    //cout "Are you sure you want to do this? You will lose local changes if you have not commit yet?"
    //cin user option to procceed or not.
    int userOption = 0;
    cout << "Are you sure you want to do this? You will lose local changes if you have not commit yet.\n1. Yes\n2. No" << endl;

    bool done = false;
    while (!done)
    {
        cin >> userOption;
        if (userOption != 2 && userOption != 1)
        {
            cout << "Enter a valid option." << endl;
        }
        else if (userOption == 2)
        {
            return;
        }
        else // it was 1
        {
            done = true;
        }
    }
    doublyNode *temp = currDir;
    while (temp->commitNumber >= userCommit)
    {
        temp = temp->previous;
    }
    // now in previous commit
    ifstream toCopy;
    ofstream copy;
    singlyNode *tempSLL = temp->head;
    singlyNode *currDirSLL;
    //changing the files from temp commit to the curr directory
    while (tempSLL != NULL)
    {
        currDirSLL = currDir->head;
        while (currDirSLL != NULL)
        {
            //we found the SLL with the same name
            if (currDirSLL->fileName == tempSLL->fileName)
            {
                break;
            }
            currDirSLL = currDirSLL->next;
        }
        if (currDirSLL != NULL)
        {
            toCopy.open("./.minigit/" + tempSLL->newFileName);
            copy.open(currDirSLL->fileName);
            string line;
            while (getline(toCopy, line))
            {
                copy << line << endl;
            }
            toCopy.close();
            copy.close();
        }
        tempSLL = tempSLL->next;
    }

    done = false;
    cout << "Press 3 to return to current commit." << endl;
    while (!done)
    {
        cin >> userOption;
        if (userOption != 3)
        {
            cout << "Enter a valid option." << endl;
        }
        else // it was 3
        {
            done = true;
        }
    }
    currDirSLL = currDir->head;
    while (currDirSLL != NULL)
    {
        toCopy.open("./.minigit/" + currDirSLL->newFileName);
        copy.open(currDirSLL->fileName);
        string line2 = "";
        while (getline(toCopy, line2))
        {
            copy << line2 << endl;
        }
        toCopy.close();
        copy.close();
        currDirSLL = currDirSLL->next;
    }
    return;

    // test other version
    /*
    int userCommit = 0;
    cout << "Enter a version you would like to check out: ";
    cin >> userCommit;
    
    singlyNode *node = currDir->head;
    doublyNode *temp = currDir;
    ofstream oFile;
    string nodeName;

    while (node != NULL) // clears current directory
    {
        nodeName = node->fileName;
        remove(nodeName);
        node = node->next;
    }

    while (temp->commitNumber >= userCommit) // get into previous version
    {
        temp = temp->previous;
    }
    */
}

void Git::status()
{
    singlyNode *temp;
    temp = currDir->head;
    cout << "Current Git Status (Commit #" << currDir->commitNumber << ")" << endl;
    cout << "-------------------------------------" << endl;
    while (temp != NULL)
    {
        cout << "Added " << temp->fileName << " ----> ";
        temp = temp->next;
    }
    cout << "\n";
    return;
}
