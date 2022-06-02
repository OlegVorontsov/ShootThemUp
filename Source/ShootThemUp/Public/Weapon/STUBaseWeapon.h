// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

//форвард декларэтион
class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //объ€вл€ем функцию стрельбы с возможностью переопределени€ virtual
    //так как разное оружие будет стрел€ть поразному
    virtual void Fire();

protected:
    //объ€вл€ем скелетал меш дл€ оружи€
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    virtual void BeginPlay() override;
};
