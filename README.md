# WAD File System

## Overview
The goal of this project was to implement a library for reading WAD files and creating a file system capable of opening and reading files. Additionally, a daemon was developed to access the system once mounted.

## Implementation

### Classes and Data Structures
- **Wad Class**: Contains the following:
  - A `descriptor` struct with:
    - `char[] name`
    - `int offset`
    - `int length`
  - Additional members:
    - `char[] magic` (WAD file identifier)
    - `int numDescriptors` (number of descriptors)
    - `int descriptorOffset` (offset to descriptor data)
    - `std::vector<descriptor>` (list of descriptors)
- **File System Structure**:
  - Organized as an **n-ary tree**:
    - Each node represents a file or directory.
    - Parent nodes are directories; children can be files or subdirectories.
    - Node contents:
      - Element name
      - Offset and length
      - Vector of child nodes (`std::vector<Node*>`).

### Key Functions

#### Wad.cpp/Wad.h
- `Wad* loadWad(const std::string &path)`
  - Opens a WAD file and initializes a `Wad` object.
  - Reads header information, descriptor data, and constructs the file tree.
- `std::string getMagic()`
  - Returns the magic identifier of the WAD file.
- `bool isContent(const std::string &path)`
  - Determines if a given path points to a file (not a directory).
- `bool isDirectory(const std::string &path)`
  - Determines if a given path points to a directory.
- `int getSize(const std::string &path)`
  - Returns the size of the element at the specified path.
- `int getDirectory(const std::string &path, std::vector<std::string> *directory)`
  - Retrieves the children of a directory and populates the provided vector.
- `int getContents(const std::string &path, char *buffer, int length, int offset = 0)`
  - Reads file content into a buffer.
- `Node* createTree(std::vector<descriptor> descriptors)`
  - Builds the n-ary tree structure from descriptor data.
- `Node* resolvePath(const std::string &path)`
  - Resolves a file path to its corresponding node in the tree.

#### Wadfs.cpp
- `int getattr()`
  - Identifies whether a path points to a file or directory.
- `int read() / readdir()`
  - `read`: Reads file contents.
  - `readdir`: Retrieves directory contents.
- `int open() / opendir()`
  - Marks files and directories as open.

## Testing
- Utilized `wad_dump.cpp` with provided WAD files.
- Verified functionality using specified commands from the project rubric.

## Bugs
- No known bugs.

## Link
- [Project Demo Video](https://youtu.be/0xs6u8yGhyU)

## References
- [FUSE: Writing a Simple Filesystem](https://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-asimple-filesystem-using-fuse/)
- [Facile Engineering Blog on FUSE](https://engineering.facile.it/blog/eng/write-filesystem-fuse/)

## Author
John Mone
