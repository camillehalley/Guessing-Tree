
#include "qtree.h"
#include <exception>
#include <iostream>

const std::string QuestionTree::INTERNAL_HEADER("D:");
const std::string QuestionTree::LEAF_HEADER("FG:");

//Ask the user a yes/no question, and get a true/false answer, respectively
bool QuestionTree::getAnswer(const std::string& question) {
  std::cout << question << std::endl;
  std::string response("?");
  do {
    std::cout << "Enter (y)es or (n)o: ";
    std::getline(std::cin, response);
  } while (!std::cin.fail() && response[0] != 'y' && response[0] != 'n');
  if (response[0] == 'y') {
    return true;
  } else if (response[0] == 'n') {
    return false;
  } else {
    throw new std::exception();
  }
}

//Replace a current child of this node with a new one.
//If orig is a child of this node, then replace it with newChild
//Preconditions:
// * orig should be one of the children of this node
// * a pointer to orig should be held elsewhere so it is not leaked
//Postconditions:
// * if orig was replaced and newChild is not NULL, 
//       newChild's parent will be set to this
//Error cases for which this method will throw an exception
// * orig is not one of this node's children
//Undefined behavior if orig is both the yes and no child of this
void QuestionTree::Distinguish::replaceChild(Node* orig, Node* newChild) {
  //Find which child needs to be replaced, and do the replacing
  //FIXME: only throw an exception if we have a problem, like not finding orig
  if (newChild != NULL){
      newChild ->setParent(orig->getParent());
  }if(yesSubtree == orig){
      newChild ->setParent(this);
      yesSubtree = newChild;
  }else if(noSubtree == orig){
      newChild ->setParent(this);
      noSubtree = newChild;
  }else {
      throw new std::exception();
  }
}

//Play a guessing game, beginning with the question described in this node.
void QuestionTree::Distinguish::play() {
  bool answer = QuestionTree::getAnswer(q);
  //FIXME: well, we have an answer, now what do we do?
  if(answer == true)
		{
    yesSubtree ->play();
  }else{
    noSubtree ->play();
  }
}

//Invoke the user interface to:
//   create another final guess
//   create a new question that will distinguish between the two final guesses
//   make this node a child of the new distinguish node, and in this node's former 
//      parent, replace this node with the new distinguish node subtree.
void QuestionTree::FinalGuess::expand(){
  std::cin.clear();
  std::cout << "What were you thinking of?  "
    << "Include an article (a, an, the) if appropriate:" << std::endl;
  std::string response;
  //Need to use this to read a whole line/phrase instead of one word
  std::getline(std::cin, response);
  std::cout << "What is a yes/no question that would distinguish between " 
    << guess << " and " << response << "?" << std::endl;
  std::string question;
  //Need to use this to read a whole line/phrase instead of one word
  std::getline(std::cin, question);
  bool answerForNew = QuestionTree::getAnswer(std::string("What is the correct answer for ") + response + '?');
    
  Distinguish* newQ;
  FinalGuess* newGuess = new FinalGuess(response, tree);
  //FIXME: create new question node with appropriate yes/no children, and 
  //put the new question node in the tree
  if(answerForNew==true && parent==NULL){ 
      //if this refers to the highest question in the tree, and the answer to the new question is yes
      newQ = new Distinguish(question, tree, NULL, newGuess, this);
      tree.root = newQ;
      newGuess->setParent(newQ);
      this ->setParent(newQ);
  }else if(answerForNew==true && parent!=NULL){
      //if the answer to the new question is yes
      newQ = new Distinguish(question, tree, parent, newGuess, this);
      this ->getParent() ->replaceChild(this, newQ);
      newGuess ->setParent(newQ);
      this ->setParent(newQ);
  }else if(answerForNew == false && parent !=NULL){
      //if the answer to the new question is no
      newQ = new Distinguish(question, tree, parent, this, newGuess);
      this ->getParent()->replaceChild(this, newQ);
      newGuess ->setParent(newQ);
      this ->setParent(newQ);
  }else{
      //if this refers to the highest question in the tree, the answer to the new question is no
      newQ = new Distinguish(question, tree, NULL, this, newGuess);
      tree.root = newQ;
      newGuess ->setParent(newQ);
      this ->setParent(newQ);}
}

//Play a guessing game, beginning with the question described in this node.
void QuestionTree::FinalGuess::play() {
  std::string question;
  question = std::string("Are you thinking of ") + guess + std::string("?");
  if (QuestionTree::getAnswer(question)) {
    std::cout << "Naturally." << std::endl;
  } else {
    std::cout << "Well, drat." << std::endl;
    if (tree.expand()) {
      expand();
    }
  }
}
