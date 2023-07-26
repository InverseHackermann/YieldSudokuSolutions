#include <array>
#include <bitset>
#include <functional>
#include <iostream>

int main() {
  std::array<std::array<int, 9>, 9> board = {};
  std::array<std::bitset<9>, 9> rowset = {}, colset = {}, squareset = {};

  const std::function<bool(int, int, int)> set_square =
    [&](int r, int c, int v) -> bool{
      int u = v - 1;
      if ((rowset[r] | colset[c] | squareset[r / 3 * 3 + c / 3])[u]) return false;
      board[r][c] = v;
      return rowset[r][u] = colset[c][u] = squareset[r / 3 * 3 + c / 3][u] = true;
    };

  const std::function<void(int, int)> clear_square =
    [&](int r, int c){
      int v = board[r][c] - 1;
      rowset[r][v] = colset[c][v] = squareset[r / 3 * 3 + c / 3][v] = false;
      board[r][c] = 0;
    };

  for (int i = 0; i < 9; ++i) {
    std::string line;
    std::getline(std::cin, line);
    for (int j = 0; j < 9 && j < line.size(); ++j) {
      if (std::isdigit(line[j]) && line[j] != '0') {
        if (!set_square(i, j, line[j] - '0')) {
          std::cerr << "bad board at " << i << "," << j << std::endl;
          return 1;
        }
      }
    }
  }

  const std::function<std::function<bool(int r, int c)>()> make_solution_generator =
    [&](){
      int ip = 0;
      int nr = 0;
      int nc = 0;
      int v = 0;
      std::function<bool(int, int)> generate_solution;
      return [
        &, ip, nr, nc, v, generate_solution
      ](int r, int c) mutable -> bool {switch(ip){case 0:;
        if (r >= 9) {
          ip = 1; return true; case 1:;
          return false;
        }
        if ((nc = c + 1) >= 9) {
          nc = 0;
          nr = r + 1;
        } else {
          nr = r;
        }
        if (board[r][c] > 0) {
          generate_solution = make_solution_generator();
          while (generate_solution(nr, nc)) {
            ip = 2; return true; case 2:;
          }
          return false;
        }
        for (v = 1; v <= 9; ++v) {
          if (set_square(r, c, v)) {
            generate_solution = make_solution_generator();
            while (generate_solution(nr, nc)) {
              ip = 3; return true; case 3:;
            }
            clear_square(r, c);
          }
        }
        default: return false;
      }};
    };

  const auto generate_solution = make_solution_generator();
  while (generate_solution(0, 0)) {
    for (const auto& row : board) {
      for (const auto& cell : row) {
        std::cout << cell;
      }
      std::cout << '\n';
    }
    std::cout << std::endl;
  }
}
