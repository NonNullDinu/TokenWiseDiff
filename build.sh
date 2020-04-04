#!/usr/bin/env bash
mkdir build && cd build && cmake .. && cmake --build . --target twdiff && cp twdiff ../ && cd .. && rm -rf build || (x=$? && cd .. && rm -rf build && exit $x) || exit $?