#!/bin/sh

FILE=./tagger_firmware/git_info.h

echo '/*' > $FILE 
echo ' * This file is updated by update_git_hash.sh' >> $FILE
echo ' * Type make or make git hook post-commit using this script' >> $FILE
echo ' */' >> $FILE

echo  -n '#ifndef GIT_COMMIT\n#define GIT_COMMIT "' >> $FILE
git rev-parse HEAD | tr -d "\n" >> $FILE 
echo '"\n#endif' >> $FILE
echo -n '#ifndef GIT_TAG\n#define GIT_TAG "' >> $FILE
git describe --tags | tr -d "\n" >> $FILE
echo '"\n#endif' >> $FILE
