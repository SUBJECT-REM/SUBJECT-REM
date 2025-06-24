// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Subject_RemGameMode.h"

#ifdef SUBJECT_REM_Subject_RemGameMode_generated_h
#error "Subject_RemGameMode.generated.h already included, missing '#pragma once' in Subject_RemGameMode.h"
#endif
#define SUBJECT_REM_Subject_RemGameMode_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class ASubject_RemGameMode *****************************************************
SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemGameMode_NoRegister();

#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASubject_RemGameMode(); \
	friend struct Z_Construct_UClass_ASubject_RemGameMode_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemGameMode_NoRegister(); \
public: \
	DECLARE_CLASS2(ASubject_RemGameMode, AGameModeBase, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/Subject_Rem"), Z_Construct_UClass_ASubject_RemGameMode_NoRegister) \
	DECLARE_SERIALIZER(ASubject_RemGameMode)


#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_15_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ASubject_RemGameMode(ASubject_RemGameMode&&) = delete; \
	ASubject_RemGameMode(const ASubject_RemGameMode&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASubject_RemGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASubject_RemGameMode); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(ASubject_RemGameMode) \
	NO_API virtual ~ASubject_RemGameMode();


#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_12_PROLOG
#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_15_INCLASS_NO_PURE_DECLS \
	FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ASubject_RemGameMode;

// ********** End Class ASubject_RemGameMode *******************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
