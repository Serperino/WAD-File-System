//
// Created by steve on 4/10/2023.
//
#include <string>
#include <vector>
#ifndef PROJECT3_WAD_H
#define PROJECT3_WAD_H


class Wad {

    struct descriptor {
        char name[9];
        int elementOffset;
        int elementLength;
        bool isDirectory = false;
        std::vector<descriptor> children;

    };










public:
    char magic[5];
    int numDescriptors;
    int descriptorOffset;
    std::vector<descriptor> wadsDescriptors;

    struct Node {
        std::string name;
        int elementOffset;
        int elementLength;
        std::vector<Node*> children;
      //  Node(std::string name){
       //     this->name = name;
      //  }

    };
    struct tree {

    };
public:
    void addChild(Node* node, std::string name);
    void setRoot(Node* root);
    static Wad* loadWad(const std::string &path);
    Node* getRoot();
    static Node* createTree(std::vector<descriptor> descriptors);
    std::string getMagic();
    bool isContent(const std::string &path);
    bool isDirectory(const std::string &path);
    int getSize (const std::string &path);
    int getContents(const std::string &path, char *buffer, int length, int offset = 0);
    int getDirectory(const std::string &path, std::vector<std::string> *directory);
     static Node* resolvePath(std::string path);
    void getLumps(const std::string &path, std::vector<descriptor> &wadDescriptors);


};


#endif //PROJECT3_WAD_H
