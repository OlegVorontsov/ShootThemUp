// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

//объ€вл€ем делегат оповещающий WeaponComponent что патроны закончились
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

//форвард декларэтион
class USkeletalMeshComponent;

//объ€вл€ем структуру дл€ пуль
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    //переменна€ дл€ количества патрон в магазине
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    //переменна€ дл€ количества магазинов. параметр meta - только если арсенал конечен
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"));
    int32 CLips;

    //переменна€ дл€ определени€ завершени€ арсенала
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //объ€вл€ем делегат оповещающий WeaponComponent что патроны закончились
    FOnClipEmptySignature OnClipEmpty;

    //объ€вл€ем функции стрельбы с возможностью переопределени€ virtual
    //так как разное оружие будет стрел€ть по-разному
    virtual void StartFire();
    virtual void StopFire();

    //функци€ смены магазина
    void ChangeClip();

    //функци€ определ€ет возможна ли перезар€дка
    bool CanReload() const;

protected:
    //объ€вл€ем скелетал меш дл€ оружи€
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //объев€лем переменную дл€ имени сокета который создали в оружии
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    //объ€вл€ем переменную max дистанции дл€ выстрела
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    //начальный арсенал оружи€
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 5, false};

    virtual void BeginPlay() override;

    //виртуальна€ функци€ выстрела дл€ имитации выпущени€ одной пули
    virtual void MakeShot();

    //виртуальна€ функци€ получени€ начальной и конечной точки стрельбы
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    //функци€ получени€ нашего персонажа
    APlayerController* GetPlayerController() const;

    //функци€ получени€ доступа к камере персонажа
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    //функци€ получени€ расположени€ сокета в оружии
    FVector GetMuzzleWorldLocation() const;

    //функци€ выстрела
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    //функци€ уменьшающа€ количество патрон при выстреле
    void DecreaseAmmo();

    //функци€ вернет true когда арсенал пуст
    bool IsAmmoEmpty() const;

    //функци€ вернет true когда магазин пуст
    bool IsClipEmpty() const;

    //функци€ выведени€ информации об арсенале
    void LogAmmo();

private:
    //текущий арсенал оружи€
    FAmmoData CurrentAmmo;
};
