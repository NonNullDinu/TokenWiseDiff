#!/usr/bin/env bash
mkdir -p build || exit $?
pushd build || exit $?
cmake .. && cmake --build . --target twdiff || exit $?
cp twdiff ../ || exit $?
popd || exit $?