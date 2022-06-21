// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    //переопределяем функции стрельбы
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    //переменная времени между выстрелами
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    //переменная половины ширину конуса разброса при стрельбе
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    //переменная размера ущерба
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    //переменная визуального эффекта выстрела
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    //переменная трейса пули
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    //переменная конечной точки эффекта
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

    virtual void BeginPlay() override;

    //переопределяем функцию выстрела для имитации выпущения одной пули
    virtual void MakeShot() override;

    //переопределяем функцию получения начальной и конечной точки стрельбы
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    //объявляем таймер
    FTimerHandle ShotTimerHandle;

    //переменная для указателя на эфект вспышки из дула
    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    //функция нанесения ущерба
    void MakeDamage(FHitResult& HitResult);

    //функция спавна эффета
    void InitMuzzleFX();

    //вкл видимости
    void SetMuzzleFXVisibility(bool Visible);

    //функция спавна трейса пули
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
