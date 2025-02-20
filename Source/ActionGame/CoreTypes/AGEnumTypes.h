#pragma once

UENUM()
enum class EAGConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EAGValidType : uint8
{
	Valid,
	InValid
};

UENUM()
enum class EAGSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EAGCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EAGCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Canceled
};