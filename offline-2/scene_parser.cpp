#include <bits/stdc++.h>
using namespace std;

vector<string> split(string line, char delim) {
  vector<string> tokens;
  stringstream ss(line);
  string token;
  while (getline(ss, token, delim)) {
    tokens.push_back(token);
  }
  return tokens;
}

void gen_init(ofstream &_output_file) {
  _output_file << "#include<bits/stdc++.h>" << endl;
  _output_file << "using namespace std;" << endl;
  _output_file << "\n#include \"libgg.hpp\"" << endl;
  _output_file << "\nint main()" << endl;
  _output_file << "{" << endl;
  _output_file << "  ofstream out1(\"stage1.txt\");" << endl;
  _output_file << "  ofstream out2(\"stage2.txt\");" << endl;
  _output_file << "  ofstream out3(\"stage3.txt\");" << endl;
  _output_file << "  ggInit(1000, 1000);" << endl;
}

void gen_end(ofstream &_output_file) {
  _output_file << "  ggEnd();" << endl;
  _output_file << "  ggPrintTriangles(0, out1);" << endl;
  _output_file << "  ggPrintTriangles(1, out2);" << endl;
  _output_file << "  ggPrintTriangles(2, out3);" << endl;
  _output_file << "  return 0;" << endl;
  _output_file << "}" << endl;
}

void gen_camera(ifstream &_input_file, ofstream &_output_file) {
  vector<vector<string>> tokens(3, vector<string>(3));
  string line;
  for(int i = 0; i< 3; i++) {
    getline(_input_file, line);
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    tokens[i] = split(line, ' ');
  }
  _output_file << "  ggLookAt(" << tokens[0][0] << ", " << tokens[0][1] << ", "
               << tokens[0][2] << ", " << tokens[1][0] << ", " << tokens[1][1]
               << ", " << tokens[1][2] << ", " << tokens[2][0] << ", "
               << tokens[2][1] << ", " << tokens[2][2] << ");" << endl;
}

void gen_perspective(ifstream &_input_file, ofstream &_output_file) {
  string line;
  getline(_input_file, line);
  line.erase(line.find_last_not_of(" \n\r\t") + 1);

  vector<string> tokens = split(line, ' ');
  _output_file << "  ggPerspective(" << tokens[0] << ", " << tokens[1] << ", "
               << tokens[2] << ", " << tokens[3] << ");" << endl;
}

void gen_triangle(ifstream &_input_file, ofstream &_output_file) {
  string line;
  string vertex[3];
  for (int i = 0; i < 3; i++) {
    getline(_input_file, line);
    line.erase(line.find_last_not_of(" \n\r\t") + 1);

    vertex[i] = "Point(";
    vector<string> tokens = split(line, ' ');
    for (int j = 0; j < 3; j++) {
      vertex[i] += tokens[j];
      if (j != 2) {
        vertex[i] += ", ";
      }
    }
    vertex[i] += ")";
  }
  _output_file << "  ggDrawTriangle(" << vertex[0] << ", " << vertex[1] << ", "
               << vertex[2] << ");" << endl;
}

void gen_translate(ifstream &_input_file, ofstream &_output_file) {
  string line;
  getline(_input_file, line);
  line.erase(line.find_last_not_of(" \n\r\t") + 1);

  vector<string> tokens = split(line, ' ');
  _output_file << "  ggTranslate(" << tokens[0] << ", " << tokens[1] << ", "
               << tokens[2] << ");" << endl;
}

void gen_scale(ifstream &_input_file, ofstream &_output_file) {
  string line;
  getline(_input_file, line);
  line.erase(line.find_last_not_of(" \n\r\t") + 1);
  vector<string> tokens = split(line, ' ');
  _output_file << "  ggScale(" << tokens[0] << ", " << tokens[1] << ", "
               << tokens[2] << ");" << endl;
}

void gen_rotate(ifstream &_input_file, ofstream &_output_file) {
  string line;
  getline(_input_file, line);
  line.erase(line.find_last_not_of(" \n\r\t") + 1);
  vector<string> tokens = split(line, ' ');
  _output_file << "  ggRotate(" << tokens[0] << ", " << tokens[1] << ", "
               << tokens[2] << ", " << tokens[3] << ");" << endl;
}

void gen_push(ofstream &_output_file) {
  _output_file << "  ggPushMatrix();" << endl;
}

void gen_pop(ofstream &_output_file) {
  _output_file << "  ggPopMatrix();" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: ./scene_parser <input_file_name> <output_file_name>"
         << endl;
    return 1;
  }
  string input_file_name = argv[1];
  string output_file_name = argv[2];
  ifstream input_file(input_file_name);
  ofstream output_file(output_file_name);

  gen_init(output_file);
  gen_camera(input_file, output_file);
  gen_perspective(input_file, output_file);

  string line;
  while (getline(input_file, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    // todo: parse the first 4 lines
    if (line == "triangle") {
      gen_triangle(input_file, output_file);
    } else if (line == "translate") {
      gen_translate(input_file, output_file);
    } else if (line == "scale") {
      gen_scale(input_file, output_file);
    } else if (line == "rotate") {
      gen_rotate(input_file, output_file);
    } else if (line == "push") {
      gen_push(output_file);
    } else if (line == "pop") {
      gen_pop(output_file);
    } else if (line == "end") {
      gen_end(output_file);
      break;
    }
  }
  return 0;
}
