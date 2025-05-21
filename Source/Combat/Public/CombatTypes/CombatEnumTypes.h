#pragma once

UENUM()
enum class ECombatConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class ECombatValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class ECombatSuccessType : uint8
{
	Successful,
	Failed
};

UENUM(BlueprintType)
enum class ECombatCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM(BlueprintType)
enum class ECombatCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class ECombatGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	Insane
};

UENUM(BlueprintType)
enum class ECombatGameInputMode : uint8
{
	GameOnly,
	UIOnly
};