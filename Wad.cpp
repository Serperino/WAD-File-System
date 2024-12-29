#include <iostream>
#include <fstream>
#include "Wad.h"
#include <sstream>
#include<ostream>
#include <stack>
#include <string.h>
Wad::Node* currentreeRoot = nullptr;
std::string currentfileName;
Wad* Wad::loadWad(const std::string &path) {
    currentfileName = path;
    Wad* newWad = new Wad();
    std::ifstream file(path, std::ios_base::binary);
    if (file.is_open()) {
       // std::cout << "how many times do i hit this?" << std::endl;
        file.read((char*)&newWad->magic, 4);
        newWad->magic[4]= '\0';
        file.read((char*)&newWad->numDescriptors, 4);
        file.read((char*)&newWad->descriptorOffset, 4); //move forward this
        file.seekg(newWad->descriptorOffset, std::ios::beg);
        //std::cout << newWad->descriptorOffset << std::endl;
        std::vector<descriptor> wadDescriptors;
        std::vector<char*> lumps;
        for(int i =0; i < newWad->numDescriptors; i++){
        descriptor newDescriptor;
        std::string test;

            file.read((char*)&newDescriptor.elementOffset, 4);
            file.read((char*)&newDescriptor.elementLength, 4);
           file.read((char*)&newDescriptor.name, 8);
            // file.seekg(newDescriptor.elementOffset, std::ios_base::beg);
            //  file.seekg(wadDescriptors[i].elementOffset, std::ios_base::beg);
            // char* lumpData = new char[wadDescriptors[i].elementLength];
            // file.read(lumpData, wadDescriptors[i].elementLength);
            //  file.seekg(newDescriptor.name, std::ios_base::beg);
            //  file.read((char*)&newDescriptor.name, 8);
           // file.read(lumpData, newDescriptor.elementLength);
           // lumps.push_back(lumpData);
           // file.read((char*)&test, newDescriptor.elementLength);
           //std::cout << test << std::endl;
           // std::cout << newDescriptor.name << newDescriptor.elementLength <
            if(newDescriptor.elementLength == 0){
                newDescriptor.isDirectory = true;
            }
           newDescriptor.name[8] = '\0';
         // 
          //std::cout << "how many times do i get here" << std::endl;
          
            newWad->wadsDescriptors.push_back(newDescriptor);
        }


       // for(int i =0; i < newWad->wadsDescriptors.size(); i++) {
       //     std::cout << "element offset:" << newWad->wadsDescriptors[i].elementOffset;
       //    std::cout << " element length:" << newWad->wadsDescriptors[i].elementLength;
       //    std::cout << " name:" << newWad->wadsDescriptors[i].name << std::endl;


       // }


    } else {
        std::cout << "Failed to open the file." << std::endl;
    }
    file.close();
//RANDOM STUFF I HOPE WORKS ZONE
    // std::ifstream otherFile(path, std::ios_base::binary);
    //       if (otherFile.is_open()) {
    //         for(int i =0; i < newWad->wadsDescriptors.size(); i++){
    //              if( newWad->wadsDescriptors[i].elementLength != 0){
    //                 otherFile.seekg( newWad->wadsDescriptors[i].elementOffset, std::ios_base::beg);
    //                 std::vector<char> buffer( newWad->wadsDescriptors[i].elementLength);
    //                 otherFile.read(buffer.data(),  newWad->wadsDescriptors[i].elementLength);
    //                   for(int j =0; j < buffer.size(); j++){
    //                     std::cout << buffer[j];
    //                      }
    //              }
    //         }

    //       }
    

  //  newWad->getLumps(path, newWad->wadsDescriptors);
    currentreeRoot = createTree(newWad->wadsDescriptors);
   return newWad;
}

void Wad::getLumps(const std::string &path, std::vector<descriptor>&wadDescriptors){
 std::ifstream file(path, std::ios_base::binary);
    if (file.is_open()) {
        std::cout << wadDescriptors.size() << std::endl;
        for(int i =0; i < wadDescriptors.size(); i++){
            if(wadDescriptors[i].elementLength == 0){
                continue;
            }
            else{
            file.seekg(wadDescriptors[i].elementOffset, std::ios_base::beg);
           std::vector<char> buffer(wadDescriptors[i].elementLength);
            file.read(buffer.data(), wadDescriptors[i].elementLength);
            std::cout << wadDescriptors[i].name << std::endl;
            std::cout << "do i die right before the length?" << std::endl;
             std::cout << wadDescriptors[i].elementLength << std::endl;
             std::cout << "do i die right before the offset?" << std::endl;
              std::cout << wadDescriptors[i].elementOffset << std::endl;
            std::cout << "the index:" << i << std::endl;
           
            for(int j =0; j < buffer.size(); j++){
                std::cout << buffer[j];
                 // std::cout << "dont i make it in here???" << std::endl;
            }
          //  delete[] lumpData;
            }
      
      

        //}
    }
}
}

Wad::Node* Wad::createTree(std::vector<descriptor> descriptors) {

    std::stack<Node *> stack;
    Node *root = new Node();
    root = new Node();
    root->name = "themainRoot";
    root->elementLength = 0;
    root->elementOffset = 0;
    currentreeRoot = root;
    stack.push(root);

    for (int i = 0; i < descriptors.size(); i++) {
        std::string nametoCompare = descriptors[i].name;
        if(descriptors[i].elementLength == 0  && nametoCompare.find("_START") != std::string::npos){
            Node* parent = new Node();
            parent->name = descriptors[i].name;
            parent->name.erase(2,6);
            if(parent->name[1] == '_'){
                parent->name.erase(1);
            }
            parent->elementLength = descriptors[i].elementLength;
            parent->elementOffset = descriptors[i].elementOffset;
         //   std::cout << "about to add" << parent->name << std::endl;
            stack.top()->children.push_back(parent);
           // std::cout << descriptors[i].name << std::endl;
            stack.push(parent);
        }
        else if(descriptors[i].elementLength == 0 && descriptors[i].name[0] == 'E' && descriptors[i].name[2] == 'M' && isdigit(int(descriptors[i].name[1])) && isdigit(int(descriptors[i].name[3]))){
            Node* parent = new Node();
            parent->name = descriptors[i].name;
            parent->elementLength = descriptors[i].elementLength;
            parent->elementOffset = descriptors[i].elementOffset;
            stack.top()->children.push_back(parent);
            stack.push(parent);
            for(int j = 0; j < 10; j++){
                i++;
                Node* child = new Node();
                child->name = descriptors[i].name;
                child->elementLength = descriptors[i].elementLength;
                child->elementOffset = descriptors[i].elementOffset;
                stack.top()->children.push_back(child);
                if(j == 9){
                //  std::cout << "children size:" << stack.top()->children.size() << std::endl;
                    stack.pop();
                }
            }
        }
        else if (descriptors[i].elementLength == 0 && nametoCompare.find("_END") != std::string::npos){
           // std::cout << "About to pop: " << stack.top()->name << std::endl;
            stack.pop();
            std::string stringName = descriptors[i].name;
        }
        else {
            Node* child = new Node();
            child->name = descriptors[i].name;
            child->elementLength = descriptors[i].elementLength;
            child->elementOffset = descriptors[i].elementOffset;
            stack.top()->children.push_back(child);
        }
    }
    return root;
}


Wad::Node* Wad::resolvePath(std::string path){
bool found = false;

Node* currdirectoryMarker = currentreeRoot;
std::istringstream  pathing(path);
    std::string dirrPiece;
  
    getline(pathing, dirrPiece, '/');
      if(path == "/"){
        return currdirectoryMarker;
    }
    while (getline(pathing, dirrPiece, '/')) {
        if(currdirectoryMarker->name == dirrPiece && pathing.eof()){
            found = true;
            return currdirectoryMarker;
        }
        for(int i = 0; i < currdirectoryMarker->children.size(); i++){
            if(currdirectoryMarker->children[i]->name == dirrPiece){
                currdirectoryMarker = currdirectoryMarker->children[i];
                if(pathing.eof()){
                    return currdirectoryMarker;
                }
                found = true;
                break;
            }
        }
    }
        if(found){
            return currdirectoryMarker;
        }
        else{
            return nullptr;
        }

    }


bool Wad::isContent(const std::string &path){
    Wad::Node* contentCheck = Wad::resolvePath(path);
    if(contentCheck == nullptr){
        return false;
    }
    if(contentCheck->elementLength != 0){
      //  delete(contentCheck);
        return true;
    }
    else{
      //  delete(contentCheck);
        return false;
    }
}
bool Wad::isDirectory(const std::string &path){
    Wad::Node* contentCheck = Wad::resolvePath(path);
    if(contentCheck == nullptr){
        std::cout << "does it come backj as null ptr" << std::endl;
        return false;
    }
    if(contentCheck->elementLength == 0){
        //  delete(contentCheck);
        return true;
    }
    else{
        //  delete(contentCheck);
        return false;
    }
}
std::string Wad::getMagic(){
    return magic;
}
int Wad::getSize (const std::string &path){
    Wad::Node* sizeGet = Wad::resolvePath(path);
   // std::cout << "do i make it back?" << std::endl;
    if(sizeGet == nullptr){
        return -1;
    }
    int sus = sizeGet->elementLength;
    return sus;

}
int Wad::getDirectory(const std::string &path, std::vector<std::string> *directory){
    Wad::Node* directoryFind = Wad::resolvePath(path);
    if(directoryFind == nullptr || directoryFind->elementLength != 0){
        return -1;
    }
    for(int i =0; i < directoryFind->children.size(); i++){
        directory->push_back(std::string(directoryFind->children[i]->name));
       // std::cout << directoryFind->children[i]->name << std::endl;
       // std::cout << "im  here!" << std::endl;
    }
    
   // std::cout << "the sus?" << directory->size() << std::endl;
    return directory->size();



}

int Wad::getContents(const std::string &path, char *buffer, int length, int offset){
    Wad::Node* directoryFind = Wad::resolvePath(path);
    std::cout << path << std::endl;
    if(directoryFind == nullptr || directoryFind->elementLength == 0){
        return -1;
    }
    std::ifstream file(currentfileName, std::ios_base::binary);
    int newOffset = directoryFind->elementOffset + offset;
    int readLength = length;
    if(length + offset > directoryFind->elementLength){
    readLength = directoryFind->elementLength - offset;
    }

//    memcpy(buffer, newOffset, length);
   
      if (!file.is_open()) {
        std::cout << "am i here" << std::endl;
         return -1;
     }
    else{
    file.seekg(newOffset, std::ios_base::beg);
    file.read(buffer, readLength);
    file.close();
    return readLength;

    }
    

    




}




