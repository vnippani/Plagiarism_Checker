#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
const int n = 6;

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files);


int main()
    {
    string dir = string("sm_doc_set");
    vector<string> files = vector<string>();

    getdir(dir,files);

    for (unsigned int i = 2;i < files.size();i++) {
        cout << i << files[i] << endl;
    }

    ifstream myfile;

    vector<string> chunkVec;
    for (unsigned int i = 0; i < files.size();i++) {
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
                for(int j = 0; j < strlen(readString); j++) {
                    if((readString[j] < 123 && readString[j] > 96) || (readString[j] < 58 && readString[j] > 47)) {
                        chunk[chunkptr] = readString[j];
                        chunkptr++;
                    }
                    else if(readString[j] < 91 && readString[j] > 64) {
                        chunk[chunkptr] = readString[j] + 32;
                        chunkptr++;
                    }
                }
                chunkWords += 1;

                //check if chunk full
                if(chunkWords >= n) {
                    chunk[chunkptr] = '\0';
                    chunkVec.push_back(string(chunk));
                    cout << string(chunk) << endl;
                    chunkptr = 0;
                    chunkWords = 0;
                }
                //97-122 = valid characters, 65 - 90 = capitals, 48-57 = numbers
            }
        } else {
            cout << "fail" << endl;
        }
        //line.c_str();
        myfile.close();
    }
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


