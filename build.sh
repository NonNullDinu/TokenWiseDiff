#!/usr/bin/env bash
mkdir -p build || exit $?
pushd build || exit $?
cmake .. && make -j "$(nproc)" || exit $?
cp twdiff ../ || exit $?
popd || exit $?