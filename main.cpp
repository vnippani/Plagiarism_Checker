#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include "HashTable.h"

using namespace std;
//const int chunkSize = 200;
// change to scanf or cin


/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files);

string filter(string line);

int main(int argc, char* argv[]) {
    //input in order file, n
    int n = atoi(argv[2]);

     //make chunk array to fit different n values, doesn't use too much memory

    string dir = argv[1];
    vector<string> files = vector<string>();
    getdir(dir,files);

    ifstream myfile;
    HashTable table;             //hash table that will be used

    for (unsigned int i = 2; i < files.size();i++) {
        myfile.open(dir + "/" + files[i]);
        string line;

        if(myfile.is_open()) {

           // char chunk[chunkSize];
            string chunk = "";
            int chunkWords = 0;

            while(myfile >> line) {
                chunk += filter(line);
                chunkWords += 1;
                //check if chunk full, if so hash and add index to table
                if(chunkWords >= n) {
                    table.hashFunc(i,chunk);
                    chunk = "";
                    chunkWords = 0;
                }
            }

        } else {
            cout << "fail" << endl;
        }

        myfile.close();
    }

    int collisionDet[files.size()][files.size()];
    for(int i = 0; i < files.size(); i++) {
        for(int j = 0; j < files.size(); j++) {
            collisionDet[i][j] = 0;
        }
    }

    for(int i = 0; i < table.size; i++) {
        if(table.hashTable[i].size() > 1) {
            for(int j = 0; j < table.hashTable[i].size(); j++) {
                for(int k = j + 1; k < table.hashTable[i].size(); k++) {
                    collisionDet[table.hashTable[i].at(j)][table.hashTable[i].at(k)] += 1;
                }
            }
        }
    }

    //find and report on highest numbers
    int highest[15] = {0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}; //holds the 5 highest values

    for(int i = 0; i < files.size(); i++) {
        for(int j = 0; j < files.size(); j++) {
           if(collisionDet[i][j] > highest[2] ) {
               highest[14] = highest[11];
               highest[13] = highest[10];
               highest[12] = highest[9];

               highest[11] = highest[8];
               highest[10] = highest[7];
               highest[9] = highest[6];

               highest[8] = highest[5];
               highest[7] = highest[4];
               highest[6] = highest[3];

               highest[5] = highest[2];
               highest[4] = highest[1];
               highest[3] = highest[0];

               highest[2] = collisionDet[i][j];
               highest[1] = j;
               highest[0] = i;

           } else if(collisionDet[i][j] > highest[5]) {
               highest[14] = highest[11];
               highest[13] = highest[10];
               highest[12] = highest[9];

               highest[11] = highest[8];
               highest[10] = highest[7];
               highest[9] = highest[6];

               highest[8] = highest[5];
               highest[7] = highest[4];
               highest[6] = highest[3];

               highest[5] = collisionDet[i][j];
               highest[4] = j;
               highest[3] = i;
           } else if(collisionDet[i][j] > highest[8]) {
               highest[14] = highest[11];
               highest[13] = highest[10];
               highest[12] = highest[9];

               highest[11] = highest[8];
               highest[10] = highest[7];
               highest[9] = highest[6];

               highest[8] = collisionDet[i][j];
               highest[7] = j;
               highest[6] = i;
           } else if(collisionDet[i][j] > highest[11]) {
               highest[14] = highest[11];
               highest[13] = highest[10];
               highest[12] = highest[9];

               highest[11] = collisionDet[i][j];
               highest[10] = j;
               highest[9] = i;
           } else if(collisionDet[i][j] > highest[14]) {
               highest[14] = collisionDet[i][j];
               highest[13] = j;
               highest[12] = i;
           }
        }
    }

    cout << highest[2] << ": files " << files[highest[1]] << " and " << files[highest[0]] << endl;
    cout << highest[5] << ": files " << files[highest[4]] << " and " << files[highest[3]] << endl;
    cout << highest[8] << ": files " << files[highest[7]] << " and " << files[highest[6]] << endl;
    cout << highest[11] << ": files " << files[highest[10]] << " and " << files[highest[9]] << endl;
    cout << highest[14] << ": files " << files[highest[13]] << " and " << files[highest[12]] << endl;
    return 0;
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        //cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }

    closedir(dp);
    return 0;
}

string filter(string line)
{
    line.c_str();
    string word;
    for(int j = 0; line[j] != '\0'; j++) {
        if((line[j] < 123 && line[j] > 96) || (line[j] < 58 && line[j] > 47)) {
            word += line[j];
        } else if(line[j] < 91 && line[j] > 64) {
            line[j] = line[j] + 32;
            word += line[j];
        } else if(line[j] == '<') {//filter <br, words:, </br, possible bibliography filter
            if(line[j+1] != '\0' && (line[j+1] == 'b' || line[j+1] == '/'))
                j = j + 2;
        }

    }
    return word;
}
