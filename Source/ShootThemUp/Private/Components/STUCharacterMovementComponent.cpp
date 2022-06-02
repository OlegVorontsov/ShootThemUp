// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Components/STUCharacterMovementComponent.h"
//подключаем нашего персонажа
#include "Player/STUBaseCharacter.h"

//переопределяем стандартную функцию
float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    //создаем указатель на нашего персонажа
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    //проверяем что: указатель не нулевой и персонаж бежит и: если да увеличиваем MaxSpeed, нет - возвращаем MaxSpeed
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}