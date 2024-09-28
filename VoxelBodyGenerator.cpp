// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelBodyGenerator.h"
#include "PlanetVoxel.h"

// Sets default values
AVoxelBodyGenerator::AVoxelBodyGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AVoxelBodyGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	FVector chunks = { FMath::CeilToFloat(((float)PlanetRadius / ChunkSize.X)),
	FMath::CeilToFloat(((float)PlanetRadius / ChunkSize.Y)),
	FMath::CeilToFloat(((float)PlanetRadius / ChunkSize.Z)) };
	//+1 incase chunk size line up with planetradius (still can build up w/ +1)

	for (int x = 0 - chunks.X; x < chunks.X; x++)
	{
		for (int y = 0 - chunks.Y; y < chunks.Y; y++)
		{
			for(int z = 0 - chunks.Z; z < chunks.Z; z++)

			{
				FVector local = FVector(x * VoxelSize * ChunkSize.X, y * VoxelSize * ChunkSize.Y, z * VoxelSize * ChunkSize.Z);
				FTransform transform = FTransform(FRotator::ZeroRotator, local + GetActorLocation(), FVector::OneVector);
				APlanetVoxel* Chunk = GetWorld()->SpawnActorDeferred<APlanetVoxel>(APlanetVoxel::StaticClass(), transform, this);
				Chunk->ChunkSize = ChunkSize;
				Chunk->ChunkLocation = local;
				Chunk->Material = Material;
				Chunk->Material2 = Material2;
				Chunk->Material3 = Material3;
				Chunk->PlanetRadius = PlanetRadius;
				Chunk->VoxelSize = VoxelSize;
				Chunk->FinishSpawning(transform);
			}
		}
	}
}

