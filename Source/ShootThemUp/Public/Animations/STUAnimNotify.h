// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

//объ€вл€ем делегат
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

UCLASS()
class SHOOTTHEMUP_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	//переопредел€ем стандартную фцнкцию
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	//создаем делегат
    FOnNotifiedSignature OnNotified;
};
