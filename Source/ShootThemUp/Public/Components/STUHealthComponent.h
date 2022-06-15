// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "STUCoreTypes.h"

#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    //функция геттер здоровья персонажа
    float GetHealth() const { return Health; }

    //функция определяющая умер персонаж или нет
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    //функция возвращает кол-во % оставшегося здоровья
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    //делегат оповещающий персонажа о том умер он или нет
    FOnDeathSignature OnDeath;

    //делегат оповещающий персонажа о том что уменьшились жизни
    FOnHealthChangedSignature OnHealthChanged;

protected:
    virtual void BeginPlay() override;

    //переменная для максимального здоровья персонажа
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    //переменная вкл/выкл авто восстановления здоровья
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    //частота с которой восстанавливается здоровье
    //пееменные доступны только если AutoHeal = true
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    //зедержка перед вызовом авто восстановления здоровья
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    //шаг восстанавливаются здоровья
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

private:
    //переменная здоровья персонажа
    float Health = 0.0f;

    //дискриптор таймера восстановления здоровья
    FTimerHandle HealTimerHandle;

    //функция восстановления здоровья
    void HealUpdate();

    //функция нанесения любого ущерба. параметры: актор которому нанесен ущерб, размер ущерба
    //тип ущерба, контроллер отвечающий за ущерб, актор нанесший ущерб
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
        class AController* InstigatedBy, AActor* DamageCauser);

    //функция установки здоровья персонажу
    void SetHealth(float NewHealth);
};
