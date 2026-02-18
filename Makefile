.PHONY: all

all:
	g++ -O0 \
    -fno-inline \
    -fno-ipa-cp \
    -fno-ipa-sra \
    -fno-ipa-pure-const \
    -fno-tree-dce \
    -fno-tree-sra \
    -fno-tree-vrp \
    -fno-lifetime-dse \
    -fno-aggressive-loop-optimizations \
    -fno-strict-aliasing \
    -fno-optimize-sibling-calls \
    -fno-delete-null-pointer-checks \
    -fno-omit-frame-pointer \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -fno-rtti \
	src/main.cpp src/fake_va.cpp -o main