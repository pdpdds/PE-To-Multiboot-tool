// 
// Copyright 1998-2012 Jeff Bush
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

/// @file ThreadContext.h
#ifndef _THREAD_CONTEXT_H
#define _THREAD_CONTEXT_H

#include "types.h"
#include "x86.h"

class PhysicalMap;

/// ThreadContext contains the architecture dependent state of a thread
class ThreadContext {
public:
	ThreadContext();	// Used for first task.
	ThreadContext(const PhysicalMap*);
	~ThreadContext();
	void Setup(thread_start_t, void *param, unsigned userStack,
		unsigned kernelStack);
	void SwitchTo();
	void PrintStackTrace() const;
	static void SwitchFp();

private:
	static void UserThreadStart(unsigned startAddress, unsigned userStack,
		unsigned param) NORETURN;

	unsigned fStackPointer;
	unsigned fPageDirectory;
	unsigned fKernelStackBottom;
	bool fKernelThread;
	FpState fFpState;
	static ThreadContext *fCurrentTask;

	// Note: this assumes single processor operation.  It should
	// move to Processor.
	static ThreadContext *fFpuOwner;
	static FpState fDefaultFpState;
};

#endif
