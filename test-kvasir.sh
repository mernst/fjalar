#!/bin/bash

# Clone Daikon, then run Daikon's scripts/test-kvasir.sh using the Fjalar and
# Kvasir in this repository (rather than a fresh clone of Fjalar).
#
# This complements ./test.sh.  Daikon's test-kvasir.sh runs the DynComp
# regression tests, which ./test.sh does not.  It does not build the
# documentation nor check that command-line options are documented.

# Fail the whole script if any command fails
set -e

FJALARDIR="$(cd "$(dirname "$0")" && pwd)"
cd "$FJALARDIR"

if [[ "$OSTYPE" == "darwin"* ]]; then
  JAVA_HOME="${JAVA_HOME:-$(/usr/libexec/java_home)}"
else
  JAVA_HOME="${JAVA_HOME:-$(dirname "$(dirname "$(readlink -f "$(which javac)")")")}"
fi
export JAVA_HOME

DAIKONDIR="${DAIKONDIR:-$(cd .. && pwd)/daikon}"
export DAIKONDIR
echo "DAIKONDIR=$DAIKONDIR"

# Daikon's test-kvasir.sh clones git-scripts into this same directory, so
# cloning it here means it is cloned only once.
GIT_SCRIPTS="/tmp/${USER:-$(id -un)}/git-scripts"
if [ -d "$GIT_SCRIPTS" ] ; then
  git -C "$GIT_SCRIPTS" pull -q || true
else
  mkdir -p "$(dirname "$GIT_SCRIPTS")"
  git clone --depth 1 -q https://github.com/plume-lib/git-scripts.git "$GIT_SCRIPTS"
fi

"$GIT_SCRIPTS/git-clone-related" codespecs daikon "$DAIKONDIR"

# Daikon builds Kvasir from $DAIKONDIR/fjalar, so pointing that at this
# repository is what makes Daikon test this code.  (Daikon's test-kvasir.sh
# clones Fjalar as a sibling of $DAIKONDIR if no such sibling exists, but that
# clone is not what gets built.)
if [ -d "${DAIKONDIR}/fjalar" ] && [ ! -L "${DAIKONDIR}/fjalar" ] ; then
  echo "${DAIKONDIR}/fjalar exists and is not a symbolic link; not overwriting it." >&2
  exit 1
fi
ln -nsf "$FJALARDIR" "${DAIKONDIR}/fjalar"

cd "$DAIKONDIR"
./scripts/test-kvasir.sh
