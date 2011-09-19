#ifndef COMPLETION_HPP_
#define COMPLETION_HPP_

#include <string>
#include <vector>

using namespace std;

class Completion {
public:
  Completion(string cmd) {root=new CompletionNode(cmd);}
  ~Completion() {if(root) delete root;}
  void insert(vector<string> data) {
    CompletionNode *node = root;
    printf("\r\nInserting\n");
    for(unsigned int i=0;i<data.size();i++) {
      printf("\r\nToken %s\n",data[i].c_str());
      printf("\r\nPointer %d\n",node);
      // if(!node) {
      // 	node=new CompletionNode(data[i]);
      // 	if(!root) root=node;
      // 	printf("\r\nEmpty node, new one created\n");
      // }
      // else {
	printf("\r\nSearching Children\n");
	bool found = false;
	for(unsigned int j=0;j<node->children.size();j++) {
	  printf("\r%s == %s\n",node->children[j]->data.c_str(),data[i].c_str());
	  if(!node->children[j]->data.compare(data[i])) {
	    found = true;
	    printf("\r\nChild found\n");
	    node = node->children[j];
	    break;
	  }
	}
	if(!found) {
	  printf("\r\nChild not found\n");
	  CompletionNode *temp_node = new CompletionNode(data[i]);
	  node->children.push_back(temp_node);
	  node = temp_node;
	}
      // }
    }
  }
  vector<string> search(vector<string> data) {
    vector<string> output;
    CompletionNode *node = root;
    for(unsigned int i=0;i<data.size();i++) {
      bool found = false;
      for(unsigned int j=0;j<node->children.size();j++)
	if(!node->children[j]->data.compare(data[i])) {
	  found = true;
	  node = node->children[j];
	}
      if(!found) {
	break;
      }
    }
    for(unsigned int i=0;i<node->children.size();i++) {
      output.push_back(node->children[i]->data);
    }
    return output;
  }

private:

  class CompletionNode {
  public:
    CompletionNode(string new_data) {data=new_data;}
    ~CompletionNode() {for(unsigned int i=0;i<children.size();i++) delete children[i];}
    string data;
    vector<CompletionNode*> children;
  };

  CompletionNode *root;
    
};

#endif
