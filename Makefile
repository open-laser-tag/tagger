.DEFAULT_GOAL := build

update_hash:
	./update_git_hash.sh

.PHONY: clean
clean:
	rm ./src/git_info.h
	platformio run --target clean

build: update_hash
	platformio run

upload: build
	platformio run -t upload
