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

    //������� ������ �������� ���������
    float GetHealth() const { return Health; }

    //������� ������������ ���� �������� ��� ���
    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    //������� ����������� ��������� � ��� ���� �� ��� ���
    FOnDeathSignature OnDeath;

    //������� ����������� ��������� � ��� ��� ����������� �����
    FOnHealthChangedSignature OnHealthChanged;

protected:
    virtual void BeginPlay() override;

    //���������� ��� ������������� �������� ���������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    //���������� ���/���� ���� �������������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    //������� � ������� ����������������� ��������
    //��������� �������� ������ ���� AutoHeal = true
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    //�������� ����� ������� ���� �������������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    //��� ����������������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

private:
    //���������� �������� ���������
    float Health = 0.0f;

    //���������� ������� �������������� ��������
    FTimerHandle HealTimerHandle;

    //������� �������������� ��������
    void HealUpdate();

    //������� ��������� ������ ������. ���������: ����� �������� ������� �����, ������ ������
    //��� ������, ���������� ���������� �� �����, ����� �������� �����
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
        class AController* InstigatedBy, AActor* DamageCauser);

    //������� ��������� �������� ���������
    void SetHealth(float NewHealth);
};
