
/*
==================================================
	lib/cstd.cpp

	C++ runtime library routines
==================================================.
*/

#include <stdint.h>

/*
===========================
	Dynamic initializer sections
===========================
*/

// Function pointer typedef for less typing //
typedef void (__cdecl *_PVFV)(void);

// Standard C++ Runtime (STD CRT) __xc_a points to beginning of initializer table
#pragma data_seg(".CRT$XCA")
_PVFV __xc_a[] = { 0 };

// Standard C++ Runtime (STD CRT) __xc_z points to end of initializer table
#pragma data_seg(".CRT$XCZ")
_PVFV __xc_z[] = { 0 };

#pragma data_seg(".CRT$XIA")
_PVFV __xi_a[] = { 0 };

#pragma data_seg(".CRT$XIZ")
_PVFV __xi_z[] = { 0 };

// Select the default data segment again (.data) for the rest of the unit
#pragma data_seg()

// Now, move the CRT data into .data section so we can read/write to it
#pragma comment(linker, "/merge:.CRT=.data")

/*
===========================
	Globals
===========================
*/

// function pointer table to global deinitializer table //
static _PVFV * pf_atexitlist = 0;

// Maximum entries allowed in table //
static unsigned max_atexitlist_entries = 32;

// Current amount of entries in table //
static unsigned cur_atexitlist_entries = 0;

/*
===========================
	Initialize global initializaters (Global constructs, et al)
===========================
*/
static void __cdecl _initterm ( _PVFV * pfbegin,    _PVFV * pfend )
{
	// Go through each initializer
    while ( pfbegin < pfend )
    {
	  // Execute the global initializer
		if (*pfbegin != 0)
		{
			(**pfbegin) ();
		}
	    // Go to next initializer inside the initializer table
        ++pfbegin;
    }
}

/*
===================================
	Initialize the deinitializer function ptr table
===================================
*/
char runtimeTempBuffer[5000];
void __cdecl _atexit_init(void)
{
    max_atexitlist_entries = 32;

	// Warning: Normally, the STDC will dynamically allocate this. Because we have no memory manager, just choose
	// a base address that you will never use for now
 //   pf_atexitlist = (_PVFV *)0x500000;

	pf_atexitlist = (_PVFV *)runtimeTempBuffer;
}

/*
===================================
	Add entry into atexit deinitialzer table. Called by MSVC++ code.
===================================
*/
int __cdecl atexit(_PVFV fn)
{
	// Insure we have enough free space
	if (cur_atexitlist_entries>=max_atexitlist_entries)
		return 1;
	else {

		// Add the exit routine
		*(pf_atexitlist++) = fn;
		cur_atexitlist_entries++;
	}

	return 0;
}

/*
===================================
	Shutdown the CRT, and execute all global dtors.
===================================
*/
void _cdecl Exit () {

	// Go through the list, and execute all global exit routines
	while (cur_atexitlist_entries--) {

			// execute function
			(*(--pf_atexitlist)) ();
	}
}

/*
===================================
	Executes all global dynamic initializers
===================================


*/
void _cdecl InitializeConstructors()
{
   _atexit_init();
   _initterm(__xc_a, __xc_z); 
}

/*
===================================
	MSVC++ calls this routine if a pure virtual function is called
===================================
*/
int __cdecl _purecall_handler()
{
	// for now, halt the system 'til we have a way to display error
	for (;;);

	// print error message here
}
