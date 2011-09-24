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
    // printf("\r\nInserting\n");
    for(unsigned int i=0;i<data.size();i++) {
      // printf("\r\nToken %s\n",data[i].c_str());
      // 	printf("\r\nSearching Children\n");
	bool found = false;
	for(unsigned int j=0;j<node->children.size();j++) {
	  // printf("\r%s == %s\n",node->children[j]->data.c_str(),data[i].c_str());
	  if(!node->children[j]->data.compare(data[i])) {
	    found = true;
	    // printf("\r\nChild found\n");
	    node = node->children[j];
	    break;
	  }
	}
	if(!found) {
	  // printf("\r\nChild not found\n");
	  CompletionNode *temp_node = new CompletionNode(data[i]);
	  node->children.push_back(temp_node);
	  node = temp_node;
	}
      // }
    }
  }
  void insert(string data) {
    vector<string> input;
    input.push_back(data);
    insert(input);
  }
  vector<string> all () {
    vector<string> output;
    for(unsigned int i=0;i<root->children.size();i++)
      output.push_back(root->children[i]->data);
    return output;
  }
  vector<string> search(vector<string> &data) {
    // printf("\r\nCompletion Searching\n");
    vector<string> output;
    CompletionNode *node = root;
    bool error = false;
    for(unsigned int i=0;i<data.size();i++) {
      // printf("\r\nToken %s\n",data[i].c_str());
      bool found = false;
      for(unsigned int j=0;j<node->children.size();j++) {
	// printf("\r%s == %s\n",node->children[j]->data.c_str(),data[i].c_str());
	if(!node->children[j]->data.compare(data[i])) {
	  // if(!node->children[j]->children.size()) break;
	  found = true;
	  node = node->children[j];
	  // printf("\r\nChild found\n");
	  break;
	}
      }
      if(!found) {
	error=true;
	// printf("\r\nChild not found\n");
	break;
      }
    }
    if(!error)
      for(unsigned int i=0;i<node->children.size();i++)
	output.push_back(node->children[i]->data);
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
