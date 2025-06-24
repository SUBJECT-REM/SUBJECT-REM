// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Subject_Rem/Subject_RemGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeSubject_RemGameMode() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemGameMode();
SUBJECT_REM_API UClass* Z_Construct_UClass_ASubject_RemGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_Subject_Rem();
// ********** End Cross Module References **********************************************************

// ********** Begin Class ASubject_RemGameMode *****************************************************
void ASubject_RemGameMode::StaticRegisterNativesASubject_RemGameMode()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_ASubject_RemGameMode;
UClass* ASubject_RemGameMode::GetPrivateStaticClass()
{
	using TClass = ASubject_RemGameMode;
	if (!Z_Registration_Info_UClass_ASubject_RemGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("Subject_RemGameMode"),
			Z_Registration_Info_UClass_ASubject_RemGameMode.InnerSingleton,
			StaticRegisterNativesASubject_RemGameMode,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_ASubject_RemGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_ASubject_RemGameMode_NoRegister()
{
	return ASubject_RemGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_ASubject_RemGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *  Simple GameMode for a third person game\n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Subject_RemGameMode.h" },
		{ "ModuleRelativePath", "Subject_RemGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Simple GameMode for a third person game" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASubject_RemGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ASubject_RemGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_Subject_Rem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ASubject_RemGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ASubject_RemGameMode_Statics::ClassParams = {
	&ASubject_RemGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008003ADu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ASubject_RemGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ASubject_RemGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ASubject_RemGameMode()
{
	if (!Z_Registration_Info_UClass_ASubject_RemGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASubject_RemGameMode.OuterSingleton, Z_Construct_UClass_ASubject_RemGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ASubject_RemGameMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ASubject_RemGameMode);
ASubject_RemGameMode::~ASubject_RemGameMode() {}
// ********** End Class ASubject_RemGameMode *******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h__Script_Subject_Rem_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ASubject_RemGameMode, ASubject_RemGameMode::StaticClass, TEXT("ASubject_RemGameMode"), &Z_Registration_Info_UClass_ASubject_RemGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASubject_RemGameMode), 147359042U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h__Script_Subject_Rem_4036124933(TEXT("/Script/Subject_Rem"),
	Z_CompiledInDeferFile_FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h__Script_Subject_Rem_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_SUBJECT_REM_Subject_Rem_Source_Subject_Rem_Subject_RemGameMode_h__Script_Subject_Rem_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
