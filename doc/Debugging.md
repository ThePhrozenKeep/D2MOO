

# Debug a function by comparing results

Here is an ad-hoc example of how to compare results of two functions at runtime.
Ideally we should be able to use a test framework for those kind of things. Here is a quick and dirty way to do it in the meantime.

Example using D2Common_11263

D2Collision.h 

```cpp
// Add this after the declaration of D2Common_11263
using D2Common_11263Type = int (__stdcall*)(D2RoomStrc* pRoom, D2CoordStrc* pBeginCoord, D2CoordStrc* pEndCoord, uint16_t nMask);
extern D2Common_11263Type D2Common_11263OrgPtr;
```

D2Collision.cpp

```cpp
// Define the original function pointer
D2Common_11263Type D2Common_11263OrgPtr;
// Rename the real function to D2Common_11263_Impl
BOOL __stdcall D2Common_11263_Impl(D2RoomStrc* pRoom, D2CoordStrc* pBeginCoord, D2CoordStrc* pEndCoord, uint16_t nCollisionType);
// Our detour
BOOL __stdcall D2Common_11263(D2RoomStrc* pRoom, D2CoordStrc* pBeginCoord, D2CoordStrc* pEndCoord, uint16_t nCollisionType)
{
	static bool useOriginal = true;
	static bool debugging = false;
    
    // Keep copies of the returned / modified values
	BOOL retValOrg;
	BOOL retValImpl;
    D2CoordStrc pEndCoordCopyOrg;
    D2CoordStrc pEndCoordCopyImpl;
	do {
        // Reset params to original values
        pEndCoordCopyOrg = *pEndCoord;
        pEndCoordCopyImpl = *pEndCoord;
        
        // Call both functions (assumes there's no side-effects)
		retValOrg = D2Common_11263OrgPtr(pRoom, pBeginCoord, &pEndCoordCopyOrg, nCollisionType);
		retValImpl = D2Common_11263_Impl(pRoom, pBeginCoord, &pEndCoordCopyImpl, nCollisionType);

        // Compare results
		if (retValOrg != retValImpl
			|| pEndCoordCopyOrg.nX != pEndCoordCopyImpl.nX
			|| pEndCoordCopyOrg.nY != pEndCoordCopyImpl.nY
			)
		{
            // Trigger breakpoint and loop if something does not match
			if (!debugging)
			{
				DebugBreak();
				debugging = true;
			}
		}

	} while (debugging);

    // Return the value you want
	if (useOriginal)
	{
		*pEndCoord = pEndCoordCopyOrg;
		return retValOrg;
	}
	else
	{
		*pEndCoord = pEndCoordCopyImpl;
		return retValImpl;
	}
}


```

D2Common.patch.cpp

```diff
diff --git a/D2.Detours.patches/1.10f/D2Common.patch.cpp b/D2.Detours.patches/1.10f/D2Common.patch.cpp
index 248c662..9c447ac 100644
--- a/D2.Detours.patches/1.10f/D2Common.patch.cpp
+++ b/D2.Detours.patches/1.10f/D2Common.patch.cpp
@@ -8,6 +8,7 @@
 #include <Drlg/D2DrlgOutdoors.h>
 #include <Drlg/D2DrlgTileSub.h>
 #include <Drlg/D2DrlgOutPlace.h>
+#include <D2Collision.h>

 //#define DISABLE_ALL_PATCHES

@@ -1310,7 +1311,7 @@ static PatchAction patchActions[GetOrdinalCount()] = {
     PatchAction::FunctionReplacePatchByOriginal,       //   DATATBLS_GetMonSeqTableRecord                                       @11262
-    PatchAction::FunctionReplacePatchByOriginal,       //   D2Common_11263                                                      @11263
+    PatchAction::Ignore,       //   D2Common_11263                                                      @11263
     PatchAction::FunctionReplacePatchByOriginal,       //   STATLIST_SetSkillId                                                 @11264
@@ -1419,7 +1420,10 @@ static ExtraPatchAction extraPatchActions[] = {

+
+
+    { 0x6FD45210 - D2CommonImageBase, &D2Common_11263, PatchAction::FunctionReplaceOriginalByPatch, &D2Common_11263OrgPtr},
+
     { 0, 0, PatchAction::Ignore}, // Here because we need at least one element in the array
 };
```