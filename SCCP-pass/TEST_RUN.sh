# Build llvm
# cmake -G "Ninja" \
# -DCMAKE_C_COMPILER=$(which gcc) -DCMAKE_CXX_COMPILER=$(which g++) \
# -DLLVM_ENABLE_PROJECTS='clang;lld' -DLLVM_TARGETS_TO_BUILD='AArch64' \
# -DLLVM_DEFAULT_TARGET_TRIPLE=aarch64 \
# -DLLVM_PARALLEL_COMPILE_JOBS=8 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
# ../llvm

# export LLVM_DIR=./build/bin/llvm
# cd ./llvm-my-pass/build
# cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR 
# make
# cd ../..

# ./build/bin/clang -S -O0 -emit-llvm ./llvm-my-pass/tests/input.c -o input.ll
# ./build/bin/opt -load-pass-plugin ./llvm-my-pass/build/libMyPass.so -passes=MyPass -disable-output input.ll
# ./build/bin/opt -passes=dot-cfg -cfg-func-name="req1" input.ll -cfg-dot-filename-prefix=cfg_output -o /dev/null
clang -S -O0 -emit-llvm tests/input.c -o input.ll
opt -load-pass-plugin build/libMyPass.so -passes=MySCCP -disable-output input.ll
# my ignore -cfg-func-name because of older version of opt
# opt -passes=dot-cfg -cfg-func-name="req1" input.ll -cfg-dot-filename-prefix=cfg_output -o /dev/null
# dot -Tpng cfg_output.req1.dot -o cfg_output.req1.png
