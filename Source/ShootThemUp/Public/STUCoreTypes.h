#pragma once
#include "STUCoreTypes.generated.h"

// forward declaration
class ASTUBaseWeapon;

//��������� ������� ����������� WeaponComponent ��� ������� �����������
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

//��������� ��������� ��� ����
USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    //���������� ��� ���������� ������ � ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    //���������� ��� ���������� ���������. �������� meta - ������ ���� ������� �������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"));
    int32 CLips;

    //���������� ��� ����������� ���������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

//��������� �������� ���������� �� ������ � �������� ����������� � ����
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    //��������� ����� ������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    //�������� �����������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

//������� ����������� ��������� � ��� ���� �� ��� ���
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

//������� ���������� ����� �������� ����� ���������
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);