// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUCharacterMovementComponent.h"
//���������� ������ ���������
#include "Player/STUBaseCharacter.h"

//�������������� ����������� �������
float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    //������� ��������� �� ������ ���������
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    //��������� ���: ��������� �� ������� � �������� ����� �: ���� �� ����������� MaxSpeed, ��� - ���������� MaxSpeed
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}