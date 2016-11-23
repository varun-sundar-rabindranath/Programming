CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
LLVMCONFIG := /home/varun/llvm_and_clang/build/Debug+Asserts/bin/llvm-config

CXXFLAGS := -I$(shell $(LLVMCONFIG) --src-root)/tools/clang/include -I$(shell $(LLVMCONFIG) --obj-root)/tools/clang/include $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG) -I /usr/local/include -I /home/varun/llvm_and_clang/libs/clang/3.5.0/include -I /usr/include/x86_64-linux-gnu -I/include -I/usr/include 
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))

SOURCES = print_function_name.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
	    -lclangTooling\
	    -lclangFrontendTool\
	    -lclangFrontend\
	    -lclangDriver\
	    -lclangSerialization\
	    -lclangCodeGen\
	    -lclangParse\
	    -lclangSema\
	    -lclangStaticAnalyzerFrontend\
	    -lclangStaticAnalyzerCheckers\
	    -lclangStaticAnalyzerCore\
	    -lclangAnalysis\
	    -lclangARCMigrate\
	    -lclangRewriteFrontend\
	    -lclangRewriteCore\
	    -lclangEdit\
	    -lclangAST\
	    -lclangLex\
	    -lclangBasic\
	    $(shell $(LLVMCONFIG) --libs)\
	    $(shell $(LLVMCONFIG) --system-libs)\
#            -lcurses

all: $(OBJECTS) $(EXES)

%: %.o
	$(CXX) -o $@ $< $(CLANGLIBS) $(LLVMLDFLAGS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~
