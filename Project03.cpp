/* Name       : Srujana Endreddy 
 * Date       : November 9, 2022 
 * File Name  : Project03.cpp
 * Description: In this Project03.cpp, a Huffman Coding tree is assembled. To build a Huffman encoding, first a Huffman Tree must be constructed 
 * from the character frequency map and then extract the binary codes for each character from the tree. In this file, Huffman Tree
 * Node class is given that includes the character in the Huffman Tree (ch), the character's frequency (freq), left and right child 
 * node pointers, and a unique id. Along with this, three constuctors are also given which advances the Node's internal ID when 
 * required. The algorithm to build a Huffman Tree from the priority queue is done from the function buildHuffmanTree that includes
 * two parameters. After the tree is built, it is used to get a string code for each character and then store all char-code pairs 
 * in the map. Each char in this map is a capital letter and the correspnding code is a string of 1s and 0s (binary code). To 
 * extract the Huffman codes into a map, the function getHuffmanCodes is used. This algorithm that is implemented here is doubly 
 * recursive. The function calls itself twice after adding 0 or 1 to code. At last, a main function is provided that is used for 
 * testing and printing the code to the terminal. */

#include <iostream>
#include <sstream>
#include <queue>
#include <map>
using namespace std;

// A Huffman Tree Node
class Node {
public:
    static int count;
    char   ch;               // Character represented by this node
    double freq;             // Frequency with which the character appears in the string
    Node*  left  = nullptr;  // Pointer to left Node, if there is one
    Node*  right = nullptr;  // Pointer to right Node, if there is one
    int    id;               // Unique ID of Node

    // Default constructor
    Node() {
        this->ch    = '\0';
        this->freq  = 0.0;
        this->left  = nullptr;
        this->right = nullptr;
        this->id    = 0;
    }

    // Constructor with no children
    Node(char ch, double freq) {
        this->ch    = ch;
        this->freq  = freq;
        this->left  = nullptr;
        this->right = nullptr;
        this->id    = ++count;
    }

    // Constructor with child Nodes
    Node(char ch, double freq, Node* left, Node* right) {
        this->ch    = ch;
        this->freq  = freq;
        this->left  = left;
        this->right = right;
        this->id    = ++count;
    }

    // Overload the function call operator (operator()) for priority queue ordering
    // Causes this class to become a FunctionObject type
    bool operator() (const Node* n1, const Node* n2) {
        return n1->freq > n2->freq;
    }

    // Utility to check if Node is leaf
    bool isLeaf() {
        return this->left == nullptr && this->right == nullptr;
    }
};
int Node::count = 0;        // Initialize static member of class

// Encode a string using the given Huffman Code table
string encodeString(string msg, map<char, string>& huffmanCodes) {
    string tstr;
    for (char c: msg) {
        tstr.append(huffmanCodes[c]);
    }
    return tstr;
}

// Decode the next symbol on the encoded string
int decodeNext(const string& encoded, Node* n, int idx, ostringstream& ss) {
    
    // No Node. No action.
    if (n == nullptr) { return idx; }

    // Found a leaf node. Add char to decoded string
    if (n->isLeaf()) {
        ss << n->ch;
        return idx;
    }

    // Advance to the next character
    idx++;

    // Decode bits represented as chars '0' and '1'
    n = (encoded.at(idx) == '0') ? n->left : n->right;
    idx = decodeNext(encoded, n, idx, ss);

    return idx;
}

// Decode the encoded string starting at the Huffman tree root node
string decodeString(string& encoded, Node* root) {
    ostringstream ss;
    int idx = -1;
    int max = encoded.length() - 1;

    // Traverse the Huffman Tree and decode the encoded string
    while (idx < max) { idx = decodeNext(encoded, root, idx, ss); }
    return ss.str();
}

// Builds Huffman Tree
Node* buildHuffmanTree(map<char, double> lfreq) {

    // Your code here
    //create priority queue pq
    priority_queue<Node*, vector<Node*>, Node> pq;
    

    //loop over pair<char, double> objects in the lfreq character frequency map data structure 
    for (pair<char, double>item:lfreq){ 
        
        //construct a new node and push it on the priority queue. The constructor takes two arguments ch and freq
        //These arguments correspond to the each pair object's .first and .second fields
        Node* nodes = new Node(item.first, item.second); 
        pq.push(nodes);
    }
    

    //Loop over the priority queue while it contains more than one Node. If there's more than 1 Node, enter the while loop
    while (pq.size()>1){ 
        Node* left = pq.top();   //declare a new Node* called left and assign it to the top() of the priority queue
        pq.pop();                //pop the element off the top of the priority queue

        Node* right = pq.top();  //declare another Node* called right and assign it to the top() of the priority queue 
        pq.pop();                //pop() the element off the top of the priority queue


        //create a new Node called tmp and to this node constructor four arguments are passed 
        Node* tmp = new Node('\0', left->freq + right->freq, left, right);  
        pq.push(tmp);  //push tmp Node back on the priority queue 
    }
return pq.top();
}


// Get Huffman codes from a tree
//This algorithm implemented here is doubly recursive where the void getHuffmanCodes function calls itself twice after it adds 0 or 1 to the code
void getHuffmanCodes(Node* n, map<char, string>& huffmanCodes, string code="") {

    // Your code here
    //if the current node is null, meaning there are no more nodes in the tree to iterate over then it returns as there is nothing else to do
    if(n == nullptr){
        return;
    }

    //if the current node is a leaf, then the huffmanCode assigns [n->ch] to the string code in the huffmanCodes map and then returns
    //When a leaf node is reached then it means that the path to the char in the Huffman tree is completed, hence the char-pair will be saved to the map then
    else if(n->isLeaf()){
        huffmanCodes[n->ch] = code;
    }

    /*This else branch now performs two recursive calls to getHuffmanCodes. When HuffmanCodes are called on a leaf node's left child
    then it returns string 0 which means that the encoded char can be found in the left edge of a Node. When HuffmanCodes are 
    called on a leaf node's right child it reutrns string 1 which means that the encoded char can be found down the right child edge of the Node
    After completion, the huffmanCodes map will contain all the mappins between each char and its string coding of 1s and 0s */
    else{
        getHuffmanCodes(n->left, huffmanCodes, (code+ "0"));
        getHuffmanCodes(n->right, huffmanCodes, (code+ "1"));
    }
}

// Test Huffman Encoding
int main() {

    map<char, double> lfreq 
       {{' ',12.0000}, {'E',11.1607}, {'A', 8.4966}, {'R', 7.5809}, {'I', 7.5448}, {'O', 7.1635},
        {'T', 6.9509}, {'N', 6.6544}, {'S', 5.7351}, {'L', 5.4893}, {'C', 4.5388}, {'U', 3.6308}, 
        {'D', 3.3844}, {'P', 3.1671}, {'M', 3.0129}, {'H', 3.0034}, {'G', 2.4705}, {'B', 2.0720},
        {'F', 1.8121}, {'Y', 1.7779}, {'W', 1.2899}, {'K', 1.1016}, {'V', 1.0074}, {'X', 0.2902}, 
        {'Z', 0.2722}, {'J', 0.1965}, {'Q', 0.1962}};

    Node* root = buildHuffmanTree(lfreq);

    // Traverse the Huffman tree and store Huffman codes in a map
    map<char, string> huffmanCodes;
    getHuffmanCodes(root, huffmanCodes);

    cout << "Huffman Codes:" << endl;
    for ( pair<char, string> pair: huffmanCodes ) {
        cout << pair.first << ": " << pair.second << endl;
    }

    string msg = "THE COLLEGE OF NEW JERSEY IS A HIGHLY SELECTIVE INSTITUTION THAT IS CONSISTENTLY RECOGNIZED AS ONE OF THE TOP COMPREHENSIVE COLLEGES IN THE NATION";

    // Encode string and print
    string encoded = encodeString(msg, huffmanCodes);
    cout << "Encoded string: " << encoded << endl;

    // Decode string and print
    string decoded = decodeString(encoded, root);
    cout << "Decoded string: " << decoded << endl;

}