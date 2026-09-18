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

# git-scripts provides git-clone-related, which clones the Daikon branch that
# corresponds to this repository's branch.
GIT_SCRIPTS="/tmp/${USER:-$(id -un)}/git-scripts"
clone_git_scripts() {
  rm -rf "$GIT_SCRIPTS"
  mkdir -p "$(dirname "$GIT_SCRIPTS")"
  # Retry once, in case of a transient network failure.
  git clone --depth 1 -q https://github.com/plume-lib/git-scripts.git "$GIT_SCRIPTS" \
    || (sleep 1m && git clone --depth 1 -q https://github.com/plume-lib/git-scripts.git "$GIT_SCRIPTS")
}
if git -C "$GIT_SCRIPTS" rev-parse --git-dir > /dev/null 2>&1 ; then
  # An update failure is not fatal; the existing clone is good enough.
  git -C "$GIT_SCRIPTS" pull -q || echo "Warning: cannot update $GIT_SCRIPTS; using it as is." >&2
else
  # The directory does not exist, or a previous clone was interrupted.
  clone_git_scripts
fi

"$GIT_SCRIPTS/git-clone-related" codespecs daikon "$DAIKONDIR"

# Daikon builds Kvasir from $DAIKONDIR/fjalar, so pointing that at this
# repository is what makes Daikon test this code.  (Daikon's test-kvasir.sh
# clones Fjalar as a sibling of $DAIKONDIR if no such sibling exists, but that
# clone is not what gets built.)
FJALAR_IN_DAIKON="${DAIKONDIR}/fjalar"
if [ -L "$FJALAR_IN_DAIKON" ] ; then
  # Restore the previous target on exit, rather than leaving a developer's
  # Daikon checkout pointing at this repository.
  PREVIOUS_FJALAR_LINK="$(readlink "$FJALAR_IN_DAIKON")"
  trap 'ln -nsf "$PREVIOUS_FJALAR_LINK" "$FJALAR_IN_DAIKON"' EXIT
elif [ -e "$FJALAR_IN_DAIKON" ] ; then
  # A real directory here is the usual layout for a Daikon developer, so do not
  # touch it; test a different Daikon checkout instead.
  echo "$FJALAR_IN_DAIKON exists and is not a symbolic link; not overwriting it." >&2
  echo "To run this script, set DAIKONDIR to a different (possibly nonexistent)" >&2
  echo "directory, into which Daikon will be cloned.  For example:" >&2
  echo "  DAIKONDIR=/tmp/${USER:-$(id -un)}/daikon-test-kvasir $0" >&2
  exit 1
fi
ln -nsf "$FJALARDIR" "$FJALAR_IN_DAIKON"

cd "$DAIKONDIR"
./scripts/test-kvasir.sh
