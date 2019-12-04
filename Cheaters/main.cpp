#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include "HashTable.h"

using namespace std;
const int n = 4;
// change to scanf or cin

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files);

int hashFunc(char chunk[]) {
    int hashNum = 0;
    for(int i = 0; i < strlen(chunk) - 1; i++) {
        if(i + 1 < strlen(chunk) - 1) {
            hashNum += chunk[i] * (chunk[i+1] - 30);
        } else {
            hashNum += chunk[i];
        }
    }
    if(hashNum < 0)
        hashNum = hashNum * -1;
    return hashNum;
}

int main() {
    string dir = string("sm_doc_set");
    vector<string> files = vector<string>();
    getdir(dir,files);
    for (unsigned int i = 2;i < files.size();i++) {
        cout << i << files[i] << endl;
    }

    ifstream myfile;
    HashTable table;             //hash table that will be used

    for (unsigned int i = 2; i < files.size();i++) {
        myfile.open("sm_doc_set/" + files[i]);
        string line;

        if(myfile.is_open()){

            char chunk[100];
            int chunkptr = 0;
            int chunkWords = 0;

            while(myfile >> line) {
                //need to add way to know chunk is done
               // cout << line << endl;
                const char *readString = line.c_str();

                //filter string for useless info
                //97-122 = valid characters, 65 - 90 = capitals, 48-57 = numbers
                for(int j = 0; j < strlen(readString); j++) {
                    if((readString[j] < 123 && readString[j] > 96) || (readString[j] < 58 && readString[j] > 47)) {
                        chunk[chunkptr] = readString[j];
                        chunkptr++;
                    }
                    else if(readString[j] < 91 && readString[j] > 64) {
                        chunk[chunkptr] = readString[j] + 32;
                        chunkptr++;
                    } else if(readString[j] == '<') {//filter <br, words:, </br, possible bibliography filter
                        if(j != strlen(readString) - 1 && (readString[j+1] == 'b' || readString[j+1] == '/'))
                            j = j + 2;
                    }

                }
                chunkWords += 1;

                //check if chunk full, if so hash and add index to table
                if(chunkWords >= n) {
                    chunk[chunkptr] = '\0'; //ends chunk
                    table.hashFunc(i,chunk);

                    cout << string(chunk) << endl;
                    chunkptr = 0;
                    chunkWords = 0;
                }

            }
        } else {
            cout << "fail" << endl;
        }
        //line.c_str();
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
           cout << collisionDet[i][j] << " ";
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
        cout << endl;
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
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }

    closedir(dp);
    return 0;
}


