# llvm-lab

一个用于学习 LLVM New Pass Manager 的实验项目，目前包含两个独立的 Pass 插件：

- `AddToMul`：将整数 `add` 和浮点数 `fadd` 指令替换为对应的乘法指令。
- `CFG`：输出函数控制流图中基本块之间的边。

> `AddToMul` 是学习性质的变换，会改变程序语义，不应作为实际优化使用。

## 环境要求

- LLVM 14（`clang`、`opt` 和用于构建插件的 LLVM 必须为同一版本）
- CMake 3.20+
- 支持 C++17 的编译器

如果 CMake 无法自动找到 LLVM，可通过 `-DLLVM_DIR=$(llvm-config --cmakedir)` 指定其位置。

## 构建与运行 AddToMul

```bash
cmake -S AddToMul -B AddToMul/build \
  -DLLVM_DIR=$(llvm-config --cmakedir)
cmake --build AddToMul/build

opt-14 -load-pass-plugin=AddToMul/build/AddToMulPlugin.so \
  -passes=add-to-mul -S AddToMul/test.ll -o AddToMul/output.ll
```

## 构建与运行 CFG

```bash
cmake -S CFG -B CFG/build \
  -DLLVM_DIR=$(llvm-config --cmakedir)
cmake --build CFG/build

clang-14 -S -emit-llvm -O0 \
  -Xclang -disable-O0-optnone CFG/cfg_test.c -o CFG/cfg_test.ll
opt-14 -load-pass-plugin=CFG/build/CFGPlugin.so \
  -passes=prt-cfg -disable-output CFG/cfg_test.ll
```

## 项目结构

```text
llvm-lab/
├── AddToMul/   # 加法转乘法 Pass 及测试 IR
├── CFG/        # CFG 输出 Pass 及测试程序
└── test.ll     # 额外的 LLVM IR 示例
```
