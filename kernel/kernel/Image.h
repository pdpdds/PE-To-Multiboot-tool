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

#ifndef _IMAGE_H
#define _IMAGE_H

#include "types.h"

class Team;

class Image {
public:
	Image();
	virtual ~Image();
	int Open(const char path[]);
	int Load(Team&);
	unsigned int GetEntryAddress() const;
	const char* GetPath() const;

private:
	struct Elf32_Shdr* FindSection(const char name[]) const;
	int ReadHeader();
	void PrintSections() const;
	void PrintSymbols() const;
	
	int fFileHandle;
	void *fBaseAddress;
	char *fPath;
	struct Elf32_Ehdr *fHeader;
	struct Elf32_Shdr *fSectionTable;
	struct Elf32_Sym *fSymbolTable;
	struct Elf32_Phdr *fProgramHeaderTable;
	char *fStringTable;
	int fStringTableSize;
	char *fSectionStringTable;
	int fSectionStringTableSize;	
};

#endif
