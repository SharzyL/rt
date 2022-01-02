SCENE_DIR=scenes
BIN_NAME=RT
RELEASE_BUILD_DIR=cmake-build-release
DEBUG_BUILD_DIR=cmake-build-debug
RELEASE_BUILD=$(RELEASE_BUILD_DIR)/$(BIN_NAME)
DEBUG_BUILD=$(DEBUG_BUILD_DIR)/$(BIN_NAME)

cornell: $(RELEASE_BUILD)
	$^ -i scenes/CornellBox-Original.obj -o output/cornell.bmp --cam-pos 0,1.3,3 --cam-dir 0,-0.13,-1 $(args)

sphere: $(RELEASE_BUILD)
	$^ -i scenes/sphere/sphere-cylcoords-1k.obj -I scenes/sphere -o output/sphere.bmp $(args)

sibenik: $(DEBUG_BUILD)
	$^ -i scenes/sibenik/sibenik.obj -I scenes/sibenik -o output/sibenik.bmp $(args)

$(DEBUG_BUILD):
	cmake --build cmake-build-debug --target RT -j`nproc`

$(RELEASE_BUILD):
	cmake --build cmake-build-release --target RT -j`nproc`

.PHONY: $(RELEASE_BUILD) $(DEBUG_BUILD)  # always build, cmake handles the cache

reload-debug: clean-debug
	cd $(DEBUG_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..

reload-release: clean-release
	cd $(RELEASE_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release ..

clean-debug:
	rm -rf $(DEBUG_BUILD_DIR)/*

clean-release:
	rm -rf $(RELEASE_BUILD_DIR)/*

.PHONY: reload-debug reload-release clean-debug clean-release sphere cornell sibenik
