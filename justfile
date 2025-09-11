alias b := build
alias re:=rebuild

default:
  just --list

install:
  @echo "not implemented"

rebuild: && build
  rm -rf build

configure:
    cmake --preset default

build: configure
    cmake --build --preset default --parallel

run:
  @echo "not implemented"
