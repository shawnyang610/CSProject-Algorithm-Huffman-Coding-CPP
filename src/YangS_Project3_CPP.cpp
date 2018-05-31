/*
CSCI 700: Proj.3 Huffman Coding (C++)
Prof. Tsaiyun Phillips
Coder: Shawn Yang
*/

//please include(uncomment) the following line for use with visual studio
//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class listBinTreeNode {
	public:
		string chStr;
		int prob;
		listBinTreeNode *next, *left, *right;
		listBinTreeNode() {
			chStr = "dummy";
			prob = 0;
			next = NULL;
			left = NULL;
			right = NULL;
		};
		listBinTreeNode(string str, int prb) {
			chStr = str;
			prob = prb;
			next = NULL;
			left = NULL;
			right = NULL;
		}
		static void printNode(listBinTreeNode* inNode, ofstream *outFile) {
			(*outFile) << "Node: (chStr:" + (*inNode).chStr + ", prob:" << (*inNode).prob << ",";
			if ((*inNode).next == NULL) {
				(*outFile) << " next:null,";
			}
			else {
				(*outFile) << " next: " + (*(*inNode).next).chStr + ",";
			}
			if ((*inNode).left == NULL) {
				(*outFile) << " left:null,";
			}
			else {
				(*outFile) << " left:" + (*(*inNode).left).chStr + ",";
			}
			if ((*inNode).right == NULL) {
				(*outFile) << " right:null)"<<endl;
			} else {
				(*outFile) << " right:" + (*(*inNode).right).chStr + ")"<<endl;
			}
			
		};
};

class HuffmanLinkedList {
	public:
		listBinTreeNode *listHead;
		listBinTreeNode *oldListHead;
		HuffmanLinkedList() {
			listHead = (new listBinTreeNode());
		}
		void constructHuffmanList(string str, int prb);
		listBinTreeNode* findSpot(int prb, listBinTreeNode* newListHead);
		void listInsert(listBinTreeNode& spot, listBinTreeNode& newNode);
		bool isEmpty() { return ((*listHead).next == NULL); };
		void printList(ofstream& outFile, listBinTreeNode* newListHead);
};
	void HuffmanLinkedList::constructHuffmanList(string str, int prb) {
	}
	listBinTreeNode* HuffmanLinkedList::findSpot(int prb, listBinTreeNode* newListHead) {
		listBinTreeNode *walker = newListHead;
		while (((*walker).next!=NULL) && (prb > (*((*walker).next)).prob)) {
			walker = (*walker).next;
		}
		return walker;
	}
	void HuffmanLinkedList :: listInsert(listBinTreeNode& spot, listBinTreeNode& newNode) {
		newNode.next = (spot).next;
		(spot).next = &newNode;
}
	void HuffmanLinkedList::printList(ofstream& outFile, listBinTreeNode* newListHead) {
		outFile << "ListHead -> (" + ((*newListHead).chStr) + ", " << (*newListHead).prob << ", ";
		listBinTreeNode *walker = newListHead;
		while ((*walker).next != NULL) {
			walker = (*walker).next;
			outFile << (*walker).chStr + ") --> (" + (*walker).chStr + ", " << (*walker).prob << ", ";
		}
		outFile << "null)" << endl;
	};

class HuffmanBinaryTree {
public:
	listBinTreeNode* listHeadPtr;
	listBinTreeNode* root;
	listBinTreeNode* spot;
	listBinTreeNode* nNode;
	HuffmanLinkedList* inputList;
	HuffmanBinaryTree(HuffmanLinkedList* oldList);
	void constructHuffmanBinTree(ofstream &outDebugFile);
	static void preOrderTraversal(listBinTreeNode* root, ofstream* outFile) {
		if (root==NULL){}
		else {
			listBinTreeNode::printNode(root, outFile);
			preOrderTraversal((*root).left, outFile);
			preOrderTraversal((*root).right, outFile);
		}
	};
	static void inOrderTraversal(listBinTreeNode* root, ofstream* outFile) {
		if (root == NULL) {}
		else {
			inOrderTraversal((*root).left, outFile);
			listBinTreeNode::printNode(root, outFile);
			inOrderTraversal((*root).right, outFile);
		}
	};
	static void postOrderTraversal(listBinTreeNode* root, ofstream* outFile) {
		if (root == NULL) {}
		else {
			postOrderTraversal((*root).left, outFile);
			postOrderTraversal((*root).right, outFile);
			listBinTreeNode::printNode(root, outFile);
		}
	};
	static bool isLeaf(listBinTreeNode* inNode) {
		if (((*inNode).left == NULL) && ((*inNode).right == NULL)) {
			return true;
		} else {
			return false;
		};
	}
	void constructCharCode(listBinTreeNode* root, string code, ofstream* outFile);
	
};
	HuffmanBinaryTree::HuffmanBinaryTree(HuffmanLinkedList* oldList) {
		listBinTreeNode*root = NULL;
		inputList = oldList;
		//Step 3.0 create a dummy listBinTreeNode and let oldListHead point to the dummy node
		// save the original linked list.

		(*oldList).oldListHead = new listBinTreeNode();
		listHeadPtr = ((*oldList).listHead);

		(*(*oldList).oldListHead).next = (*(*oldList).listHead).next;
	}
	void HuffmanBinaryTree::constructHuffmanBinTree(ofstream &outDebugFile) {
		if ((*listHeadPtr).next == NULL)
			cout << "empty list" << endl;		
		else {
			while ((*((*listHeadPtr).next)).next != NULL) { //step 3.3 repeat 3.1-2
				//step 3.1
				nNode = new listBinTreeNode();
				(*nNode).chStr = (*((*listHeadPtr).next)).chStr + (*((*((*listHeadPtr).next)).next)).chStr;
				(*nNode).prob = (*((*listHeadPtr).next)).prob + (*((*((*listHeadPtr).next)).next)).prob;
				(*nNode).left = (*listHeadPtr).next;
				(*nNode).right = (*(*listHeadPtr).next).next;
				(*listHeadPtr).next = (*(*(*listHeadPtr).next).next).next;
				listBinTreeNode::printNode(nNode, &outDebugFile);
				//step 3.2
				spot = (*inputList).findSpot((*nNode).prob, listHeadPtr);
				(*inputList).listInsert(*spot, *nNode);
				(*inputList).printList(outDebugFile, listHeadPtr);
			}
			//step 3.4
			root = nNode;
		}
	}
	void HuffmanBinaryTree::constructCharCode(listBinTreeNode* root, string code, ofstream* outFile) {
		if (root == NULL) {
			cout << "This is an empty tree!" << endl;
			return;
		} else if (isLeaf(root)) {
			(*outFile) << (*root).chStr << " " << code << endl;
		}
		else {
			constructCharCode((*root).left, code + "0", outFile);
			constructCharCode((*root).right, code + "1", outFile);
		}
	}

int main(int argc, char *argv[]){
	ifstream inFile;
	ofstream outFile1, outFile2, outFile3, outFile4, outFile5;
	//Step 2.0 open inFile and all outFiles:
	try {
		if (argc < 6) {
			cout << "Please specific 1 input and 5 output files in form of arguments." << endl;
			return 0;
		}
		inFile.open(argv[1]);
		outFile1.open(argv[2]);
		outFile2.open(argv[3]);
		outFile3.open(argv[4]);
		outFile4.open(argv[5]);
		outFile5.open(argv[6]);
	} catch (const fstream* e) {
		cout << e << endl;
	}
	cout << "files openned successfully." << endl;
	//2.1 listHead <-- create a dummy listBinTreeNode for listHead to point to
	HuffmanLinkedList* huffList = new HuffmanLinkedList();
	string chStr;
	listBinTreeNode* spot;
	char ch;
	int prob;
	while (inFile >> ch >> prob) { //2.2 get pairs from inFile
		spot = (*huffList).findSpot(prob, ((*huffList).listHead)); //2.3 insertion spot
		chStr = ch;
		listBinTreeNode* nNode = new listBinTreeNode(chStr, prob); //2.4
		(*huffList).listInsert(*spot, (*nNode)); //2.5 insert NN between spot and spot.next
		(*huffList).printList(outFile5, (*huffList).listHead); //2.6
		//2.7 repeat which inFile not empty
	}
	inFile.close(); //2.8
	cout << "HuffmanLinkedList construction finished." << endl;
	//Step 3 Construct Huffman binary tree
	HuffmanBinaryTree* huffTree = new HuffmanBinaryTree(huffList);
	(*huffTree).constructHuffmanBinTree(outFile5);
	cout << "HuffmanBinaryTree construction finished." << endl;
	//Step 4 Construct Huffman char-code pairs in recursion
	(*huffTree).constructCharCode((*huffTree).root, "", &outFile1);
	cout << "CharCode construction finished." << endl;
	HuffmanBinaryTree::preOrderTraversal((*huffTree).root, &outFile2);
	HuffmanBinaryTree::inOrderTraversal((*huffTree).root, &outFile3);
	HuffmanBinaryTree::postOrderTraversal((*huffTree).root, &outFile4);
	cout << "3 types of traversals finished." << endl;
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	outFile5.close();
	cout << "All output finished, program exits." << endl;
	return 0;
};