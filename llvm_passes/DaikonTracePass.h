/***************
DaikonTracePass.h
Authors: Arijit Chattopadhyay (https://github.com/arijitvt/RTool), Abraham Chan (Integration with LLFI)
  This llvm pass acts as the front end instrumenter of the Daikon tool
  
  Run the pass with the opt -daikontracepass option after loading LLFI.so
  
  This pass injects a function call at the entry and exit points of 
  every function that prints variable type and value 
  to a file specified during the pass.
***************/

#ifndef DAIKONTRACE_PASS_H
#define DAIKONTRACE_PASS_H


#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/IR/DataLayout.h"

#include <cmath>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>

using namespace llvm;
using namespace std;


namespace llfi {

class DaikonTrace : public FunctionPass {

  public:
	static char ID;
	static bool isInit;

  private:
	vector<Value*> globalList;
	vector<string> doNotInstrumentFunctions;
	vector<string> programPoints;
	Type *voidType;
	IntegerType *int8Type; 	
	IntegerType *int32Type;
	IntegerType *int64Type;
	PointerType *ptr8Type; 
	PointerType *ptr32Type;
	PointerType *ptr64Type;

	PointerType *ptrPtr32Type;
	PointerType *ptrPtr64Type;

	PointerType *ptrFloatType;
	PointerType *ptrDoubleType;
	PointerType *ptrPtrFloatType;
	PointerType *ptrPtrDoubleType;

	StructType *structType;
	PointerType *ptrStructType;
	PointerType *ptrPtrStructType;

	FunctionType *functionType;
	Value *clapDummyVar;

  public:
	DaikonTrace() : FunctionPass(ID) {}
	virtual bool runOnFunction(Function &F);
	virtual void getAnalysisUsage(AnalysisUsage &AU) const {
	    AU.addRequired<DataLayout>();
	}

  private:
	//Main helpers
	void hookAtFunctionStart(Function *);
	void hookAtFunctionEnd(Function *);

  	//Setup
	void doInit(Module *module);
	void populateGlobals(Module *module);

	//Program Point Generation
	void generateProgramPoints(Module *);
	void loadProgramPoints(Module *);
	void dumpDeclFileAtEntryAndExit(Function*, string);

	//Utilities
	bool isGlobal(Value *value);
	bool isSupportedType(StringRef typeName);
	bool doNotInstrument(StringRef funcName);
	void putTabInFile(fstream &stream, int tabCount);
	bool isFileEmpty(fstream &inputFile);
	Value* getValueForString(StringRef variableName,Module *module);
	string find_and_replace(string source, const char find, const string replace);
	string demangle(const char* name);
	string getDemangledFunctionName(const char* name);
	string getTypeString(Value *value);
	string getTypeString(Type *type);

};//Class DaikonTrace

//Initialize the pass
char DaikonTrace::ID = 0;
bool DaikonTrace::isInit = false;

}//namespace llfi
#endif
