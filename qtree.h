#ifndef CS270_QUESTION_TREE
#define CS270_QUESTION_TREE
#include <string>
#include <iostream>

//A decision tree to play a game of X questions, where X is yet to 
//be determined.
class QuestionTree {
 public:
  //In file I/O, this is the string beginning a line for an internal node
  static const std::string INTERNAL_HEADER;
  //In file I/O, this is the string beginning a line for a leaf
  static const std::string LEAF_HEADER;
 private:
  //Forward declarations: we promise these classes will exist
  class FinalGuess;
  class Distinguish;

  //Abstract base node class
  class Node {
   public:
    //Althrough this class is abstract, it does have a field, so we make a 
    // constructor for subclasses to call to set it if they choose to.
    Node(QuestionTree& _tree, Distinguish* _parent = NULL) : tree(_tree), parent(_parent) {}

    //Play a guessing game, beginning with the question described in this node.
    virtual void play() = 0;
    // Get the parent of the current Node, or NULL if it is the root
    // Requires that nodes have parents, but no promises about 
    // children.  Clearly identifies all internal nodes as Distinguish nodes.
    Distinguish* getParent() { return parent; }
    // Set the parent of the current Node
    void setParent(Distinguish* newParent) { parent = newParent; }
    // Delete the subtree rooted at this node : default behavior is to do nothing
    virtual void deleteSubtree() { };
    // Print the subtree rooted at this node to the output stream specified
    virtual void printSubtree(std::ostream& out) = 0;
    // In case any derived classes need a destructor that can be called 
    // when a Node* is being deleted.
    virtual ~Node() { };

    //Normally fields would be made private, but since this is already a private class within 
    //  QuestionTree, only code inside the QuestionTree class can access them anyway.

    //The tree object to which this node belongs
    QuestionTree& tree;
    //This node's parent (may be NULL)
    Distinguish* parent;
  };

  class Distinguish : public Node {
    friend class FinalGuess;
   public:

    //Construct a distinguish node
    //Note that the last arguments are optional and default to NULL
    //This constructor does not modify any pointers outside of the node being constructed.
    //If _parent, yes, or no are non-NULL, they will need to have field modified for the 
    //  tree to be consistent.
    Distinguish(const std::string& q, QuestionTree& t, Distinguish* parent = NULL, Node* yes = NULL, Node* no = NULL)  : 
      Node(t, parent), q(q), yesSubtree(yes), noSubtree(no) {};

    //Nothing to destroy in this node specifically
    virtual ~Distinguish() { }

    //Inherited methods: see base class for descriptions
    virtual void play();

    //Recursively delete the subtree connected to this node, but not the node itself
    virtual void deleteSubtree() { 
      yesSubtree->deleteSubtree();
      delete yesSubtree;
      yesSubtree = NULL;
      noSubtree->deleteSubtree();
      delete noSubtree;
      noSubtree = NULL;
    }

    //Recursively print the tree
    virtual void printSubtree(std::ostream& out) {
      out << INTERNAL_HEADER << ' ' << q << std::endl;
      yesSubtree->printSubtree(out);
      noSubtree->printSubtree(out);
    };

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
    void replaceChild(Node* orig, Node* newChild);

    //Normally fields would be made private, but since this is already a private class within 
    //  QuestionTree, only code inside the QuestionTree class can access them anyway.

    //The question that divides the two subtrees
    std::string q;
    //The root of the subtree for items whose answer to the current question is "yes"
    Node* yesSubtree;
    //The root of the subtree for items whose answer to the current question is "no"
    Node* noSubtree;
  };

  class FinalGuess : public Node {
    friend class Distinguish;
   public:
    //Initialize a FinalGuess.  Parent is set to NULL if not provided.
    FinalGuess(const std::string& g, QuestionTree& t, Distinguish* _parent = NULL) : 
      Node(t, _parent), guess(g) {};
    //Nothing to destroy in this node specifically
    virtual ~FinalGuess() { }
    //See base description
    virtual void play();
    //Invoke the user interface to:
    //   create another final guess
    //   create a new question that will distinguish between the two final guesses
    //   make this node a child of the new distinguish node, and in this node's former 
    //      parent, replace this node with the new distinguish node subtree.
    void expand();
    //Print this node to the output stream
    virtual void printSubtree(std::ostream& out) {
      out << LEAF_HEADER << ' ' << guess << std::endl;
    };
   private:
    //The final item/object
    std::string guess;
  };

  //A recursive function that will read a tree or subtree from the input stream, 
  //  returning a pointer to the subtree created.  Will return when a subtree is 
  //  finished being read from the stream, leaving any remaining stream contents.
  Node* buildTreeFromStream(std::istream& in);

  //The root of the decision tree
  Node* root;

  //A boolean option (false by default) controlling whether or not to invoke the 
  //  user interface to expand the decision tree if the decision tree fails to 
  //  categorize what the user was considering.
  bool expandOnWrongGuess;

 public:
  QuestionTree(bool expanding = false) : 
    root(new FinalGuess("a polar bear", *this)), 
    expandOnWrongGuess(expanding) { };
  ~QuestionTree() { clear(); }
  void clear() { root->deleteSubtree(); delete root; root = NULL; }
  bool expand() { return expandOnWrongGuess; }

  //Play the game 
  void play() { root->play(); }

  //Construct the tree from the provided stream: 
  void readFromStream(std::istream& in);
  
  //Write the tree to the provided stream
  void writeToStream(std::ostream& out);

  //Ask the user a true-false question, and get a yes/no answer back
  static bool getAnswer(const std::string& question);

};
#endif // CS270_QUESTION_TREE
