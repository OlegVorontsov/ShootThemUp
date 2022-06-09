#pragma once
#include "STUCoreTypes.generated.h"

// forward declaration
class ASTUBaseWeapon;

//объявляем делегат оповещающий WeaponComponent что патроны закончились
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

//объявляем структуру для пуль
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    //переменная для количества патрон в магазине
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    //переменная для количества магазинов. параметр meta - только если арсенал конечен
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"));
    int32 CLips;

    //переменная для определения завершения арсенала
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

//структура хранящая информацию об оружии и анимации перезарядки к нему
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    //объявляем класс оружия
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    //анимация перезарядки
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

//делегат оповещающий персонажа о том умер он или нет
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

//делегат вызывается когда меняются жизни персонажа
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);
