CXX = clang++
CXX_FLAGS = -g -std=c++17 

BUILD_DIR = ./build
PLIST_DIR = ./plist

SOURCE = main.cpp
TARGET = ${BUILD_DIR}/main.exe


${TARGET}: ${SOURCE}| ${BUILD_DIR} ${PLIST_DIR}
	${CXX} ${CXX_FLAGS} $< --analyze -o ${PLIST_DIR}/main.plist
	${CXX} ${CXX_FLAGS} $< -o $@

${BUILD_DIR}:
	@mkdir -p ./build

${PLIST_DIR}:
	@mkdir -p ./plist

run:
	@${TARGET}

clean:
	@rm -rf ./build
	@rm -rf ./plist


.PHONY: run clean
