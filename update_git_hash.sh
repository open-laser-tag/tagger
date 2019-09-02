#!/bin/sh

FILE="./tagger_firmware/git_info.h"

COMMIT=`git rev-parse HEAD`
TAG=`git describe --tags`

rm -f $FILE

cat <<EOT >> $FILE
/*
* This file is updated by update_git_hash.sh
*/
#ifndef GIT_COMMIT
  #define GIT_COMMIT "$COMMIT"
#endif

#ifndef GIT_TAG
  #define GIT_TAG "$TAG"
#endif
EOT
