// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

//������� ����������� ��������� �����������
DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;

    //������� ��� ���������� �� AnimNotify
    InitAnimations();

    //�������� ������� ������ ������
    SpawnWeapons();

    //������� ������������ ������� ������ � �������������� ��� � ���� ���������
    EquipWeapon(CurrentWeaponIndex);
}

//������� ��������� ������ � ����� ����
void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    //���������� �� ������� Weapons
    for (auto Weapon : Weapons)
    {
        //����������� ������ �� ���������
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        //���������� ������
        Weapon->Destroy();
    }
    //������� ������ ������
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

//������� ������ ������
void USTUWeaponComponent::SpawnWeapons()
{
    //������� ���������� ���� ���������� ������ ��������� - ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //��������� �� ���������� ���� � ��������� �� ���������. ���� �� ������� �� �������
    if (!GetWorld() || !Character)
        return;

    //������� ������
    for (auto WeaponClass : WeaponClasses)
    {
        //������� ������ ���������� ������ � ���������� � ����������
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon)
            continue;

        //��� ������� ������������ ������ ��������� ownera - ������ ���������
        Weapon->SetOwner(Character);

        //��������� ������ � ������ ����������
        Weapons.Add(Weapon);

        //������� ������ � ������ �� ����� ��������� � ������� �������
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

//������� ������������� ������ � ������ ��������� �� ����� ������
void USTUWeaponComponent::AttachWeaponToSocket(
    ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
        return;

    //������� ���������� ������������ ������� ������������� ��� �������������
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    //������� � ���� ��������� � ������� ���������� ������������� � ���������� ���������� ����� �������������
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

//������� ������������ ������� ������ � �������������� ��� � ���� ���������
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //������� ������� ������ �� �����
    if (CurrentWeapon)
    {
        //������������� ��������
        CurrentWeapon->StopFire();

        //������� ������� ������ � ����� ���������
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    //������ ��������� �� ������ ������
    CurrentWeapon = Weapons[WeaponIndex];

    //������� ������ � ���� ���������
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    //�������� ������� ����� ������
    EquipAnimInProgress = true;

    //�������� ������� ��������������� ��������
    PlayAnimMontage(EquipAnimMontage);
}

//������� ����� ������
void USTUWeaponComponent::NextWeapon()
{
    //��������� ����� �� ������ ������
    if (!CanEquip())
        return;

    //����������� ������ � ��������� ������� Weapons
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

//������� ������������ �������� ����� ������/�����������
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character)
        return;

    //������������� ��������
    Character->PlayAnimMontage(Animation);
}

//������� ��� ���������� �� AnimNotify
void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
        return;

    //������� ������ �������
    const auto NotifyEvents = EquipAnimMontage->Notifies;

    //���������� ������
    for (auto NotifyEvent : NotifyEvents)
    {
        //������� ������ notify � ���������� � ����������
        auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);

        //���� ����� ������ notify ������ ������� ����� �������
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

//������� �������� �� �������
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    //������� ���������� ���� ���������� ��������� ����� ����������
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    //������ ��� ������ ���������
    if (!Character || MeshComponent != Character->GetMesh())
        return;

    //����������� ������� ����� ������
    EquipAnimInProgress = false;
}

//������� ����������� ��������/������ ������
bool USTUWeaponComponent::CanFire() const
{
    //��������� ���� �� ������ � �� ���� �� ����� ������
    return CurrentWeapon && !EquipAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress;
}

//������� ��������
void USTUWeaponComponent::StartFire()
{
    //���������� ����� �� ��������
    if (!CanFire())
        return;

    //�������� ������� StartFire � STUBaseWeapon
    CurrentWeapon->StartFire();
}
void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    //�������� ������� StopFire � STUBaseWeapon
    CurrentWeapon->StopFire();
}
