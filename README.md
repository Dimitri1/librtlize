cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True        -DLLVM_DIR=/home/dgerin/llvmOfficial/build/lib/cmake/llvm/ ../lib-rtlize/

../llvmOfficial/build/bin/clang -fsyntax-only -fplugin=lib/librtlize-plugin.so -Xclang -analyze -Xclang -analyzer-checker=demo.streamchecker -c  ../lib-rtlize/test/minimal.cpp  -I/home/dgerin/systemc-2.3/src/  -I. -I/home/dgerin/systemc-2.3/include/
