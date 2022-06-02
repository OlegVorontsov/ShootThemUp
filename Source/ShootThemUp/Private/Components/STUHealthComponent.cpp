// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

//������� ����������� ��������� �����������
DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    //��������� Tick
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    //� ������ ���� ��� ������ ��������� �������� ���
    SetHealth(MaxHealth);

    //�������� ��������� �� ������ ���������
    AActor* CompomemtOwner = GetOwner();
    if (CompomemtOwner)
    {
        //������������� �� ������� OnTakeAnyDamage ���������� � ������� TakeDamage ������
        //� ������ ���� ������� ��������� OnTakeAnyDamage
        CompomemtOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage); //���� �������
    }
}

//������� ��������� �������� ���������
void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    //�������� �������� � ��������� ����� �������
    OnHealthChanged.Broadcast(Health);
}

//������� �������������� ��������
void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    //������������� ������ ���� �������� �������� max ������
    //����� float ��������� � ������� ������� IsNearlyEqual
    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

//������� ��������� ������ ������ ���������
void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy, AActor* DamageCauser)
{
    //������� �� ������� ���� Damage ������������� � ���� ��� ������
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return;
    //�������� �������� ���������
    SetHealth(Health - Damage);

    //������������� ������ ���� �������������� ��������
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    //��������� ���� ��� ���
    if (IsDead())
    {
        //��������� ���� ��� �������� ���� ����� �������
        OnDeath.Broadcast();
    }
    //���� �� ������ ��������� �������� �� AutoHeal
    else if (AutoHeal)
    {
        //�������� ������
        //��������� - ����������, ��������� �� ������ ��� ���������� ������, ������ �� �������, ������� ����������,
        //����� ������� ��������, �������� ����� ������ �������
        GetWorld()->GetTimerManager().SetTimer(
            HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
}
