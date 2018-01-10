#include "qtree.h"
#include <fstream>

int main(int argc, const char* argv[]) {
  QuestionTree qt(true);
  if (argc > 1) {
    std::ifstream treefile(argv[1]);
    qt.readFromStream(treefile);
  }
  do {
    qt.play();
  } while (QuestionTree::getAnswer("Do you want to play again?"));
  if (QuestionTree::getAnswer("Do you want to save the tree?")) {
    std::cout << "Enter a file name: ";
    std::string file;
    std::cin >> file;
    std::ofstream outfile(file.c_str());
    qt.writeToStream(outfile);
  }
  return 0;
}
