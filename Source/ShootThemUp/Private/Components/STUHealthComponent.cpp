// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

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

    check(MaxHealth > 0);

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
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    //�������� �������� � ��������� ����� ������� � CoreTypes
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

//������� �������������� ��������
void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    //������������� ������ ���� �������� �������� max ������
    //����� float ��������� � ������� ������� IsNearlyEqual
    if (IsHealthFull() && GetWorld())
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
    PlayCameraShake();
}

//������� ������ true ����� �������� max
bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

//������� ���������� �������� ����� �����
bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    //������� ������ �� �������: �������� ����� ��� �������� max ��� ���-�� �����. �������� �����.
    if (IsDead() || IsHealthFull() || HealthAmount <= 0)
        return false;

    SetHealth(Health + HealthAmount);

    return true;
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead())
        return;

    //�������� ��������� �� ��������
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player)
        return;

    //�������� ��������� �� ����������
    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
        return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
