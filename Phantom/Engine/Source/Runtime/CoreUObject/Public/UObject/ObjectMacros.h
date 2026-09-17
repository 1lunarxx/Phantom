#pragma once
#include "pch.h"

/** Objects flags for internal use (GC, low level UObject code) */
enum class EInternalObjectFlags : int32
{
	None = 0,
	//~ All the other bits are reserved, DO NOT ADD NEW FLAGS HERE!
	ReachableInCluster = 1 << 23, ///< External reference to object in cluster exists
	ClusterRoot = 1 << 24, ///< Root of a cluster
	Native = 1 << 25, ///< Native (UClass only). 
	Async = 1 << 26, ///< Object exists only on a different thread than the game thread.
	AsyncLoading = 1 << 27, ///< Object is being asynchronously loaded.
	Unreachable = 1 << 28, ///< Object is not reachable on the object graph.
	PendingKill = 1 << 29, ///< Objects that are pending destruction (invalid for gameplay but valid objects)
	RootSet = 1 << 30, ///< Object will not be garbage collected, even if unreferenced.
	HadReferenceKilled = 1 << 31, ///< Object had a reference null'd out by markpendingkill

	GarbageCollectionKeepFlags = Native | Async | AsyncLoading,
	//~ Make sure this is up to date!
	AllFlags = ReachableInCluster | ClusterRoot | Native | Async | AsyncLoading | Unreachable | PendingKill | RootSet | HadReferenceKilled
};