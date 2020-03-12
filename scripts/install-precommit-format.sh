#!/bin/sh
# from https://github.com/STORM-IRIT/Radium-Engine/blob/master/scripts/install-scripts-linux.sh
ROOT=`git rev-parse --show-toplevel`

file_or_link() {
  if [ ! -e "$1" ] && [ ! -L "$1" ]; then
    return 0
  else
    return 1
  fi
}

if file_or_link "${ROOT}/.git/hooks/pre-commit"; then
  echo "Create symlink for pre-commit hook";
  ln -s "${ROOT}/scripts/pre-commit-clang-format.sh" "${ROOT}/.git/hooks/pre-commit"
  chmod +x "${ROOT}/scripts/pre-commit-clang-format.sh"
else
  echo "pre-commit hook already present, please remove .git/hooks/pre-commit";
fi

if file_or_link "${ROOT}/.clang-format"; then
  echo "create synlink for clang-format style file";
  ln -rs "${ROOT}/scripts/clang-format" "${ROOT}/.clang-format"
else
  echo "clang-format link already present, please remove .clang-format"
fi