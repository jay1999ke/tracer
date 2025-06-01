remake() {
    git clean -f -d -x
    cmake . -DCMAKE_BUILD_TYPE=Debug
    make
}