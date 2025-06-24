// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Subject_RemCharacter.h"

#ifdef SUBJECT_REM_Subject_RemCharacter_generated_h
#error "Subject_RemCharacter.generated.h already included, missing '#pragma once' in Subject_RemCharacter.h"
#endif
#define SUBJECT_REM_Subject_RemCharacter_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class ASubject_RemCharacter ****************************************************
#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execDoJumpEnd); \
	DECLARE_FUNCTION(execDoJumpStart); \
	DECLARE_FUNCTION(execDoLook); \
	DECLARE_FUNCTION(execDoMove);


SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemCharacter_NoRegister();

#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASubject_RemCharacter(); \
	friend struct Z_Construct_UClass_ASubject_RemCharacter_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemCharacter_NoRegister(); \
public: \
	DECLARE_CLASS2(ASubject_RemCharacter, ACharacter, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/Subject_Rem"), Z_Construct_UClass_ASubject_RemCharacter_NoRegister) \
	DECLARE_SERIALIZER(ASubject_RemCharacter)


#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	ASubject_RemCharacter(ASubject_RemCharacter&&) = delete; \
	ASubject_RemCharacter(const ASubject_RemCharacter&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASubject_RemCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASubject_RemCharacter); \
	DEFINE_ABSTRACT_DEFAULT_CONSTRUCTOR_CALL(ASubject_RemCharacter) \
	NO_API virtual ~ASubject_RemCharacter();


#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_21_PROLOG
#define FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_INCLASS_NO_PURE_DECLS \
	FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ASubject_RemCharacter;

// ********** End Class ASubject_RemCharacter ******************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemCharacter_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
