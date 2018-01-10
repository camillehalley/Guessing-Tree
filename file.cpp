
#include "qtree.h"

//(Intended to be) recursive function to build a tree from the input file
QuestionTree::Node* QuestionTree::buildTreeFromStream(std::istream& in) {
  //Read the header
  std::string kind;
  in >> kind;
  //Read the space character
  in.get();
  //Read the rest of the line
  std::string restOfLine;
  std::getline(in, restOfLine);
  //Now, do something with the stuff we read!
  //FIXME: don't just return NULL!
  Node* myRoot;
    
  if(kind == "FG:"){
      FinalGuess* newGuess = new FinalGuess(restOfLine, *this, NULL);
      myRoot = newGuess;}
	else{
      Distinguish* newQ = new Distinguish(restOfLine, *this, NULL, buildTreeFromStream(in), buildTreeFromStream(in));
      newQ -> yesSubtree -> setParent(newQ);
      newQ -> noSubtree -> setParent(newQ);
      myRoot = newQ;}
      
  return myRoot;
		}

void QuestionTree::readFromStream(std::istream& in) {
  root = buildTreeFromStream(in);}

void QuestionTree::writeToStream(std::ostream& out) {
  root->printSubtree(out);}
