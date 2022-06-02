// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

//делегат оповещающий персонажа о том умер он или нет
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

//делегат вызываетс€ когда мен€ютс€ жизни персонажа
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    //функци€ геттер здоровь€ персонажа
    float GetHealth() const { return Health; }

    //функци€ определ€юща€ умер персонаж или нет
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    //делегат оповещающий персонажа о том умер он или нет
    FOnDeathSignature OnDeath;

    //делегат оповещающий персонажа о том что уменьшились жизни
    FOnHealthChangedSignature OnHealthChanged;

protected:
    virtual void BeginPlay() override;

    //переменна€ дл€ максимального здоровь€ персонажа
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    //переменна€ вкл/выкл авто восстановлени€ здоровь€
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    //частота с которой восстанавливаетс€ здоровье
    //пееменные доступны только если AutoHeal = true
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    //зедержка перед вызовом авто восстановлени€ здоровь€
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    //шаг восстанавливаютс€ здоровь€
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

private:
    //переменна€ здоровь€ персонажа
    float Health = 0.0f;

    //дискриптор таймера восстановлени€ здоровь€
    FTimerHandle HealTimerHandle;

    //функци€ восстановлени€ здоровь€
    void HealUpdate();

    //функци€ нанесени€ любого ущерба. параметры: актор которому нанесен ущерб, размер ущерба
    //тип ущерба, контроллер отвечающий за ущерб, актор нанесший ущерб
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
        class AController* InstigatedBy, AActor* DamageCauser);

    //функци€ установки здоровь€ персонажу
    void SetHealth(float NewHealth);
};
