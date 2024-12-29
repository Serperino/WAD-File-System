#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "Wad.h"
#include "Wad.cpp"

static int do_getattr(const char* path, struct stat* stbuf);
static int do_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi);
static int do_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi);
static int do_open (const char *, struct fuse_file_info *);
static int do_opendir (const char *, struct fuse_file_info *);




static struct fuse_operations wadfs_operations = {
    .getattr = do_getattr,
   .open = do_open,
   .read = do_read,
   .opendir = do_opendir,
   .readdir = do_readdir,
  


};

static int do_open (const char *, struct fuse_file_info *){
    return 0;
}
static int do_opendir (const char *, struct fuse_file_info *){
    return 0;
}
static int do_getattr( const char *path, struct stat *st ){
    st->st_uid = getuid();
	st->st_gid = getgid();
	st->st_atime = time( NULL );
	st->st_mtime = time( NULL );
    Wad newWad;
     Wad::Node* getAttributes = newWad.resolvePath(path);
  

    if(getAttributes->elementLength == 0){
        st->st_mode = S_IFDIR | 0555;
		st->st_nlink = 2;
    }
    else {
        st->st_mode = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size = getAttributes->elementLength;

    }
    return 0;
}



static int do_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi ){
 //Wad::Node* getAttributes = Wad::resolvePath(path);
 //Wad wad;
int bytesRead = ((Wad*)(fuse_get_context()->private_data))->getContents(path, buffer, size, offset);
 //std::ifstream file(currentfileName, std::ios_base::binary);
//int bytesRead = getContents(path, buffer, size, offset);
return bytesRead;
// if(bytesRead == -1){
//     delete[] filledBuff;
//     return 0;
// }
// memcpy(buffer, filledBuff, bytesRead);
//   delete[] filledBuff;
// return bytesRead;


}

static int do_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ){
    filler( buffer, ".", NULL, 0 );
	filler( buffer, "..", NULL, 0 ); 
    Wad::Node* getAttributes = Wad::resolvePath(path);
    if(getAttributes->elementLength == 0){
        for(int i =0; i < getAttributes->children.size(); i++){
            filler(buffer, getAttributes->children[i]->name.c_str(), NULL, 0);
        }
    }

    return 0;






}






int main( int argc, char *argv[] ){
    std::string workingDirectory = get_current_dir_name(); 
    std::string wadPath = argv[1];
   
    if(wadPath.at(0) != '/'){
        if(wadPath.at(0) == '.'){
            wadPath = wadPath.substr(1, wadPath.length() - 1);
        }
        wadPath = workingDirectory + "/" + wadPath;
    }
     Wad *myWad = Wad::loadWad(wadPath);
   // argv[1] = argv[2];
   // argv[2] = NULL;
   char** ptr = new char*[2];
   ptr[0] = argv[0];
   ptr[1] = argv[2];
	return fuse_main(argc-1, ptr, &wadfs_operations, myWad);
    //argv[1] = "-f";
    //return fuse_main(argc, argv, &wadfs_operations, myWad);
}
