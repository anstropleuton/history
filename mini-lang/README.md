Mini statically-typed interpreted language - modular refactor.

Build:
  mkdir build && cd build
  cmake ..
  cmake --build .

Run:
  ./mini_lang <file.mg>

Tests:
  ctest -V
