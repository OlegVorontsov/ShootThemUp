// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

// forward declaration
class ASTUBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    //функции стрельбы
    void StartFire();
    void StopFire();

protected:
    //объ€вл€ем класс дл€ спавна оружи€
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    //объ€вл€ем переменную с имененм сокета присоединнеи€ к персонажу
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponSocket";

    virtual void BeginPlay() override;

private:

    //об€вл€ем указатель на оружие
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    //функци€ спавна оружи€
    void SpawnWeapon();
};
