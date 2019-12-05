//
// Created by Vineet on 12/3/2019.
//
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#ifndef CHEATERS_HASHTABLE_H
#define CHEATERS_HASHTABLE_H
//
// Created by Vineet on 12/3/2019.
//

using namespace std;

class HashTable {
public:

    const static int size = 13093;//49613;
    vector<int> hashTable[size];

    void hashFunc(int index, string chunk) {

        chunk.c_str();
        int hashNum = 1; int i = 0;
        //hash function
        while(chunk[i] != '\0') {  //add, then multiply, then add, then multiply...
            hashNum = hashNum + hashNum * 31^ (76963 * chunk[i]);
            i++;
            if(chunk[i] == '\0')
                break;
            hashNum = hashNum - chunk[i];
            i++;
        }

        cout << chunk << endl;
        if(hashNum < 0)
            hashNum = hashNum * -1;
        cout << hashNum << endl;
        //place index into array of vectors
        if(hashTable[hashNum % size].empty())
            hashTable[hashNum % size].push_back(index);
        else {
            bool flag = true;
            //iterate through vector
            for(int i = 0; i < hashTable[hashNum % size].size(); i++) {
                if (hashTable[hashNum % size].at(i) == index) {
                    flag = false;
                    break;
                }
            }
            if(flag) {
                hashTable[hashNum % size].push_back(index);
            }

        }
    }

};


#endif //CHEATERS_HASHTABLE_H
