SCENE_DIR=scenes
BIN_NAME=RT
SPPM_BIN_NAME=RT_sppm
RELEASE_BUILD_DIR=cmake-build-release
DEBUG_BUILD_DIR=cmake-build-debug

RELEASE_BUILD=$(RELEASE_BUILD_DIR)/$(BIN_NAME)
DEBUG_BUILD=$(DEBUG_BUILD_DIR)/$(BIN_NAME)

SPPM_RELEASE_BUILD=$(RELEASE_BUILD_DIR)/$(SPPM_BIN_NAME)
SPPM_DEBUG_BUILD=$(DEBUG_BUILD_DIR)/$(SPPM_BIN_NAME)

# for final results
cornell-2ball: $(RELEASE_BUILD)
	$^ -i scenes/cornell-2ball.yml -o output/final/cornell-2ball.bmp -p4 -s1024

cornell-bezier: $(RELEASE_BUILD)
	$^ -i scenes/cornell-bezier.yml -o output/final/cornell-bezier.bmp -p4 -s1024

big-mesh: $(RELEASE_BUILD)
	$^ -i scenes/big-mesh.yml -o output/final/big-mesh.bmp -p4 -s512

cornell-depth: $(RELEASE_BUILD)
	$^ -i scenes/cornell-depth.yml -o output/final/cornell-depth.bmp -p4 -s1024

cornell-2ball-ppm: $(SPPM_RELEASE_BUILD)
	$^ -i scenes/cornell-2ball.yml -o output/final/cornell-2ball-sppm.bmp -p 10000000 -n 100 -r 0.008

final: cornell-2ball cornell-2ball-ppm cornell-depth cornell-bezier big-mesh


# for debug
cornell: $(RELEASE_BUILD)
	$^ -i scenes/cornell.yml -o output/cornell.bmp $(args)

cornell-sppm: $(SPPM_RELEASE_BUILD)
	$^ -i scenes/cornell.yml -o output/cornell-sppm.bmp $(args)

#sphere: $(RELEASE_BUILD)
#	$^ -i scenes/sphere/sphere-cylcoords-1k.obj -I scenes/sphere -o output/sphere.bmp $(args)

#sibenik: $(DEBUG_BUILD)
#	$^ -i scenes/sibenik/sibenik.obj -I scenes/sibenik -o output/sibenik.bmp $(args)

$(DEBUG_BUILD):
	cmake --build cmake-build-debug --target $(BIN_NAME) -j`nproc`

$(RELEASE_BUILD):
	cmake --build cmake-build-release --target $(BIN_NAME) -j`nproc`

$(SPPM_DEBUG_BUILD):
	cmake --build cmake-build-debug --target $(SPPM_BIN_NAME) -j`nproc`

$(SPPM_RELEASE_BUILD):
	cmake --build cmake-build-release --target $(SPPM_BIN_NAME) -j`nproc`

.PHONY: $(RELEASE_BUILD) $(DEBUG_BUILD) $(SPPM_RELEASE_BUILD) $(SPPM_DEBUG_BUILD) # always build, cmake handles the cache

reload-debug: clean-debug
	cd $(DEBUG_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ..

reload-release: clean-release
	cd $(RELEASE_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release ..

clean-debug:
	rm -rf $(DEBUG_BUILD_DIR)/*

clean-release:
	rm -rf $(RELEASE_BUILD_DIR)/*

.PHONY: reload-debug reload-release clean-debug clean-release sphere cornell sibenik
