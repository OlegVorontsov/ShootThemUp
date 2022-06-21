// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STUCoreTypes.h"

#include "STUBaseWeapon.generated.h"

//форвард декларэтион
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    //объявляем делегат оповещающий WeaponComponent что патроны закончились
    FOnClipEmptySignature OnClipEmpty;

    //объявляем функции стрельбы с возможностью переопределения virtual
    //так как разное оружие будет стрелять по-разному
    virtual void StartFire();
    virtual void StopFire();

    //функция смены магазина
    void ChangeClip();

    //функция определяет возможна ли перезарядка
    bool CanReload() const;

    //функция возвращает данные об иконках оружия
    FWeaponUIData GetUIData() const { return UIData; }

    //функция возвращает данные о текущем арсенале оружия
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

    //функция добавления арсенала через пикап
    bool TryToAddAmmo(int32 ClipsAmount);

protected:
    //объявляем скелетал меш для оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    //объевялем переменную для имени сокета который создали в оружии
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    //объявляем переменную max дистанции для выстрела
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    //начальный арсенал оружия
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 5, false};

    //структура иконок оружия из STUCoreTypes
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    //ээфект вспушки из дула
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
        UNiagaraSystem* MuzzleFX;

    virtual void BeginPlay() override;

    //виртуальная функция выстрела для имитации выпущения одной пули
    virtual void MakeShot();

    //виртуальная функция получения начальной и конечной точки стрельбы
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    //функция получения нашего персонажа
    APlayerController* GetPlayerController() const;

    //функция получения доступа к камере персонажа
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    //функция получения расположения сокета в оружии
    FVector GetMuzzleWorldLocation() const;

    //функция выстрела
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    //функция уменьшающая количество патрон при выстреле
    void DecreaseAmmo();

    //функция вернет true когда арсенал пуст
    bool IsAmmoEmpty() const;

    //функция вернет true когда магазин пуст
    bool IsClipEmpty() const;

    //функция вернет true когда арсенал полон
    bool IsAmmoFull() const;

    //функция выведения информации об арсенале
    void LogAmmo();

    //функция спавна компонента эффекта вспышки из дула
    UNiagaraComponent* SpawnMuzzleFX();

private:
    //текущий арсенал оружия
    FAmmoData CurrentAmmo;
};
